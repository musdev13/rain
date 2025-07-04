// filefs.cpp
#include "mus.h/filefs.hpp"

std::vector<std::string> readPlaylist(const fs::path& playlistPath) {
    std::vector<std::string> lines;
    std::ifstream file(playlistPath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    return lines;
}

void writePlaylist(const fs::path& playlistPath, const std::vector<std::string>& lines) {
    std::ofstream file(playlistPath, std::ios::trunc);
    for (const auto& line : lines) {
        file << line << "\n";
    }
}

std::vector<std::string> getFiles(fs::path folderPath){
    fs::path playlistPath = folderPath / "playlist";

    std::set<std::string> filesInFolder;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename != "playlist")
                filesInFolder.insert(filename);
        }
    }

    std::vector<std::string> playlist;

    if (!fs::exists(playlistPath)) {
        playlist.assign(filesInFolder.begin(), filesInFolder.end());
        writePlaylist(playlistPath, playlist);
    } else {
        playlist = readPlaylist(playlistPath);
        std::set<std::string> playlistSet(playlist.begin(), playlist.end());

        for (const auto& file : filesInFolder) {
            if (!playlistSet.count(file)) {
                playlist.insert(playlist.begin(), file);
            }
        }

        writePlaylist(playlistPath, playlist);
    }

    return playlist;
}
