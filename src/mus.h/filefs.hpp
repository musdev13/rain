// filefs.hpp
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> readInf(const std::string& path);
void writeFile(const std::string& path, const std::string& content);

void removeAll(std::string& str, const std::string& what);

std::vector<std::string> readPlaylist(const fs::path& playlistPath);
void writePlaylist(const fs::path& playlistPath, const std::vector<std::string>& lines);

std::vector<std::string> getFiles(fs::path folderPath);

extern std::string cacheFolder;