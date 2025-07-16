// filefs.cpp
#include "mus.h/filefs.hpp"
#include "mus.h/atomic_vars.hpp"

std::vector<std::string> readInf(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line) && lines.size() < 2) {
        lines.push_back(line);
    }

    return lines;
}

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::out | std::ios::trunc);

    file << content;
}

void removeAll(std::string& str, const std::string& what) {
  size_t pos;
  while ((pos = str.find(what)) != std::string::npos) {
    str.erase(pos, what.length());
  }
}

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

std::string cacheFolder = std::string(fs::path(getenv("HOME"))) + "/.cache/rain/";

void refreshList(){
    files = getFiles(pathToFolder);

    for (const auto& file : files) {
        fullPaths.clear();
        fs::create_directories(fs::path(fs::path(getenv("HOME"))/".cache/rain/test").parent_path());

        if (file.find("@rain:spotify\\") != std::string::npos){
            // removeAll(ffile,"@rain:spotify\\");
            // system(("notify-send \""+ffile+"\"").c_str());
            fullPaths.push_back(file);

        } else fullPaths.push_back(std::string(pathToFolder + "/" + file));
    }

    formatedItems.clear();
    formatedItems.resize(files.size());
    fullPaths.resize(files.size());

    for (size_t i = 0; i < files.size(); ++i) {
        std::string track = files[i];
        std::string title, artist;

        if (track.find("@rain:spotify\\") != std::string::npos) {
            std::string id = track;
            removeAll(id, "@rain:spotify\\");
            std::string mp3_path = cacheFolder + id + ".mp3";
            std::string info_path = cacheFolder + id + ".infosp";

            if (fs::exists(mp3_path) && fs::exists(info_path)) {
                std::vector<std::string> trackInfo = readInf(info_path);
                title = trackInfo.size() > 0 ? trackInfo[0] : "unknown";
                artist = trackInfo.size() > 1 ? trackInfo[1] : "unknown";
                fullPaths[i] = mp3_path;
            } else {
                title = "loading";
                artist = "loading";
                fullPaths[i] = track; // временно, до загрузки
            }
        } else {
            TagLib::FileRef f((pathToFolder + "/" + track).c_str());
            TagLib::Tag* tag = f.tag();
            title = tag ? tag->title().to8Bit(true) : "unknown";
            artist = tag ? tag->artist().to8Bit(true) : "unknown";
            fullPaths[i] = pathToFolder + "/" + track;
        }
        formatedItems[i] = title + " - " + artist;
    }

    list = formatedItems;
}
