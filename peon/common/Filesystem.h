#ifndef PEON_FILESYSTEM_H
#define PEON_FILESYSTEM_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "log/Logger.h"

using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

namespace Peon {

void ListDirectory(const string& directory, vector<string>& files);

string ReadFile(const string& file);
}  // namespace Peon

#endif