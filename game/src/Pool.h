
#ifndef GAME_POOL_H
#define GAME_POOL_H

#include <cassert>
#include <cstring>
#include <vector>

using std::forward;
using std::move;
using std::swap;
using std::vector;

class Pool {
 public:
  virtual ~Pool() = default;
  virtual void* Get(size_t index) = 0;
  virtual void Free(size_t index) = 0;
  virtual void Compact() = 0;
  virtual size_t Capacity() const = 0;
  virtual size_t Size() const = 0;
  virtual void* Allocate(size_t index) = 0;
  virtual void Destroy(size_t index) = 0;

 protected:
  Pool() = default;
  Pool(const Pool&) = default;
  Pool(Pool&&) = default;
  Pool& operator=(const Pool&) = default;
  Pool& operator=(Pool&&) = default;
};

const size_t POOL_CHUNK_SIZE = 8192;
const size_t POOL_UNALLOCATED_INDEX = static_cast<size_t>(-1);

template <typename T, size_t ChunkSize = POOL_CHUNK_SIZE>
class PackedPool : public Pool {
 public:
  PackedPool();

  virtual ~PackedPool();

  void* Get(size_t index) override;
  void Free(size_t index) override;
  void Compact() override;
  size_t Capacity() const override;
  size_t Size() const override;

  void Destroy(size_t index) override;
  void* Allocate(size_t index) override;

 protected:
  inline void* GetData(size_t packedIndex);

  size_t mChunkSize;
  size_t mElementSize;
  vector<size_t> mPackedIndices;
  vector<size_t> mSparseIndices;
  vector<uint8_t*> mData;
};

template <typename T, size_t ChunkSize>
PackedPool<T, ChunkSize>::PackedPool()
    : mElementSize(sizeof(T)), mChunkSize(ChunkSize) {}

template <typename T, size_t ChunkSize>
PackedPool<T, ChunkSize>::~PackedPool() {}

template <typename T, size_t ChunkSize>
void* PackedPool<T, ChunkSize>::Get(size_t index) {
  if (index >= mSparseIndices.size()) {
    return nullptr;
  }
  size_t packedIndex = mSparseIndices[index];
  if (packedIndex == POOL_UNALLOCATED_INDEX) {
    return nullptr;
  }
  return this->GetData(packedIndex);
}

template <typename T, size_t ChunkSize>
size_t PackedPool<T, ChunkSize>::Capacity() const {
  return (mData.size() * mChunkSize) / mElementSize;
}

template <typename T, size_t ChunkSize>
size_t PackedPool<T, ChunkSize>::Size() const {
  return mPackedIndices.size();
}

template <typename T, size_t ChunkSize>
void* PackedPool<T, ChunkSize>::Allocate(size_t index) {
  if (index >= mSparseIndices.size()) {
    mSparseIndices.resize(index + 1, POOL_UNALLOCATED_INDEX);
  }
  assert(mSparseIndices[index] == POOL_UNALLOCATED_INDEX);
  while (this->Capacity() <= mPackedIndices.size()) {
    uint8_t* chunk = new uint8_t[mChunkSize * mElementSize];
    memset(static_cast<void*>(chunk), 0, mChunkSize * mElementSize);
    mData.push_back(chunk);
  }
  size_t packedIndex = mPackedIndices.size();
  mPackedIndices.resize(mPackedIndices.size() + 1);
  mSparseIndices[index] = packedIndex;
  mPackedIndices[packedIndex] = index;

  return this->GetData(packedIndex);
}

template <typename T, size_t ChunkSize>
void PackedPool<T, ChunkSize>::Free(size_t index) {
  assert(index < mSparseIndices.size());

  size_t packedIndex = mSparseIndices[index];
  assert(packedIndex != POOL_UNALLOCATED_INDEX);
  size_t lastPackedIndex = mPackedIndices.size() - 1;

  T* element = static_cast<T*>(this->GetData(packedIndex));
  T* lastElement = static_cast<T*>(this->GetData(lastPackedIndex));
  if (element != lastElement) {
    new (element) T(move(*lastElement));
    lastElement->~T();
    memset(static_cast<void*>(lastElement), 0, sizeof(T));
  }

  size_t lastSparseIndex = mPackedIndices[lastPackedIndex];
  mPackedIndices[packedIndex] = lastSparseIndex;
  mSparseIndices[lastSparseIndex] = packedIndex;
  mSparseIndices[index] = POOL_UNALLOCATED_INDEX;
  mPackedIndices.resize(mPackedIndices.size() - 1);
}

template <typename T, size_t ChunkSize>
void PackedPool<T, ChunkSize>::Destroy(size_t index) {
  assert(index < mSparseIndices.size());
  size_t packedIndex = mSparseIndices[index];
  assert(packedIndex != POOL_UNALLOCATED_INDEX);
  T* element = static_cast<T*>(this->GetData(packedIndex));
  element->~T();
  memset(static_cast<void*>(element), 0, sizeof(T));
}

template <typename T, size_t ChunkSize>
void PackedPool<T, ChunkSize>::Compact() {
  size_t requiredBytes = mPackedIndices.size() * mElementSize;
  size_t requiredChunks = requiredBytes / (mChunkSize * mElementSize);
  if (requiredChunks > 0 && requiredBytes % requiredChunks) {
    requiredChunks += 1;
  }
  mData.resize(requiredChunks);
  size_t lastSparseIndex = 0;
  for (size_t sparseIndex : mPackedIndices) {
    lastSparseIndex = std::max(lastSparseIndex, sparseIndex);
  }
  mSparseIndices.resize(lastSparseIndex + 1);
}

template <typename T, size_t ChunkSize>
inline void* PackedPool<T, ChunkSize>::GetData(size_t packedIndex) {
  size_t chunk = packedIndex / mChunkSize;
  size_t offset = (packedIndex % mChunkSize) * mElementSize;
  return reinterpret_cast<void*>(mData[chunk] + offset);
}

#endif