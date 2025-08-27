// filefs.hpp
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <algorithm>
#include <filesystem>
#include "atomic_vars.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace fs = std::filesystem;

void debug(const std::string str);
// void deleteLine(const std::string& pathToFolder, int lineNum);
void moveLineUp(const std::string& pathToFolder, int lineNum);
void moveLineDown(const std::string& pathToFolder, int lineNum);
std::string readFirstLine(const std::string& filename);
std::string readLine(const std::string& filename, size_t lineNumber);
void insertFirstLine(const std::string& path, const std::string& newLine);
std::vector<std::string> readInf(const std::string& path);
void writeFile(const std::string& path, const std::string& content);

void removeAll(std::string& str, const std::string& what);

std::vector<std::string> readPlaylist(const fs::path& playlistPath);
void writePlaylist(const fs::path& playlistPath, const std::vector<std::string>& lines);

std::vector<std::string> getFiles(fs::path folderPath);

extern std::string cacheFolder;
void initCacheFolder();

void refreshList();
