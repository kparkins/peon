/**
 * Copyright Kyle Parkinson All rights reserved.
 */

#ifndef PEON_UTILITY_H
#define PEON_UTILITY_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "log/Logger.h"

using std::hex;
using std::ifstream;
using std::lock_guard;
using std::mutex;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

namespace Peon {

template <typename Tr, typename Tn, typename Tm>
Tr Mod(Tn n, Tm m) {
  Tr r = n - (n / m) * m;
  if (r < 0) {
    return r + m;
  }
  return r;
};

template <typename T>
string ToHex(T i) {
  stringstream sstream;
  sstream << "0x" << setfill('0') << setw(sizeof(T) * 2) << hex << i;
  return sstream.str();
}

string GmtTimestamp();
vector<string> Split(const string& source, char delimeter);

template <typename... Args>
string StringFormat(const string& format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

string ReadFile(const string& file);

}  // namespace Peon
#endif  // PEON_UTILITY_H
