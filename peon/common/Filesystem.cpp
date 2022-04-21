#include "Filesystem.h"

void Peon::ListDirectory(const string &directory, vector<string> &files) {
  if (!std::filesystem::exists(directory) ||
      !std::filesystem::is_directory(directory)) {
    LOG_ERROR(directory << " is not a directory.");
    return;
  }
  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (std::filesystem::is_regular_file(entry)) {
      files.push_back(entry.path().filename().string());
    }
  }
}

string Peon::ReadFile(const string &file) {
  stringstream sstream;
  ifstream fileStream(file);
  if (!fileStream.is_open() || !fileStream.good()) {
    return string("");
  }
  sstream << fileStream.rdbuf();
  return sstream.str();
}