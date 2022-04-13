
#ifndef GAME_POOL_H
#define GAME_POOL_H

#include <vector>

using std::vector;

class Pool {
 public:
  explicit Pool() {}
  virtual ~Pool() {}
  virtual void* Get(size_t index) = 0;
  virtual void* Allocate(size_t index) = 0;
  virtual void Delete(size_t index) = 0;
  virtual size_t Capacity() const = 0;
  virtual size_t Size() const = 0;
};

const size_t POOL_UNALLOCATED_INDEX = static_cast<size_t>(-1);

template <typename T, size_t ChunkSize = 8192>
class PackedPool : public Pool {
 public:
  PackedPool();

  virtual ~PackedPool();

  void* Get(size_t index);
  void* Allocate(size_t index);
  void Delete(size_t index);

  size_t Capacity() const;
  size_t Size() const;

 protected:
  inline void Expand(size_t n);
  inline void Compact();
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
  if (mPackedIndices.size() >= this->Capacity()) {
    this->Expand(mPackedIndices.size() + 1);
  }
  size_t packedIndex = mPackedIndices.size();
  mPackedIndices.resize(mPackedIndices.size() + 1);
  mSparseIndices[index] = packedIndex;
  mPackedIndices[packedIndex] = index;

  return this->GetData(packedIndex);
}

template <typename T, size_t ChunkSize>
void PackedPool<T, ChunkSize>::Delete(size_t index) {
  assert(index < mSparseIndices.size());

  size_t packedIndex = mSparseIndices[index];
  assert(packedIndex != POOL_UNALLOCATED_INDEX);
  size_t lastPackedIndex = mPackedIndices.size() - 1;

  T* element = static_cast<T*>(this->GetData(packedIndex));
  element->~T();
  T* lastElement = static_cast<T*>(this->GetData(lastPackedIndex));
  memcpy(static_cast<void*>(element), static_cast<void*>(lastElement),
         sizeof(T));

  size_t lastSparseIndex = mPackedIndices[lastPackedIndex];
  mPackedIndices[packedIndex] = lastSparseIndex;
  mSparseIndices[lastSparseIndex] = packedIndex;
  mSparseIndices[index] = POOL_UNALLOCATED_INDEX;
  mPackedIndices.resize(mPackedIndices.size() - 1);
  this->Compact();
}

template <typename T, size_t ChunkSize>
inline void PackedPool<T, ChunkSize>::Expand(size_t n) {
  while (this->Capacity() < n) {
    uint8_t* chunk = new uint8_t[mChunkSize * mElementSize];
    memset(static_cast<void*>(chunk), 0, mChunkSize * mElementSize);
    mData.push_back(chunk);
  }
}

template <typename T, size_t ChunkSize>
inline void PackedPool<T, ChunkSize>::Compact() {
  size_t requiredBytes = mPackedIndices.size() * mElementSize;
  size_t requiredChunks = requiredBytes / mChunkSize;
  if (requiredChunks > 0 && requiredBytes % requiredChunks) {
    requiredChunks += 1;
  }
  size_t numChunks = mData.size() - requiredChunks;
  mData.resize(numChunks);
}

template <typename T, size_t ChunkSize>
inline void* PackedPool<T, ChunkSize>::GetData(size_t packedIndex) {
  size_t chunk = packedIndex / mChunkSize;
  size_t offset = (packedIndex % mChunkSize) * mElementSize;
  return reinterpret_cast<void*>(mData[chunk] + offset);
}

#endif