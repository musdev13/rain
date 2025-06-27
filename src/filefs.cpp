// filefs.cpp
#include "mus.h/filefs.hpp"
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> getFilesInDirectory(const std::string& path) {
    std::vector<std::string> files;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}
