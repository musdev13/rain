// filefs.cpp
#include "mus.h/filefs.hpp"
#include "mus.h/atomic_vars.hpp"
#include <string>

void debug(const std::string str){system(("notify-send \""+str+"\"").c_str());}


// void deleteLine(const std::string& pathToFolder, int lineNum) {
//     std::string path = pathToFolder + "/playlist";

//     std::ifstream checkFile(path);
//     checkFile.close();

//     std::ifstream inFile(path);

//     std::vector<std::string> lines;
//     std::string line;
//     while (std::getline(inFile, line)) {
//         lines.push_back(line);
//     }
//     inFile.close();

//     lines.erase(lines.begin() + lineNum);

//     std::ofstream outFile(path);

//     for (const auto& l : lines) {
//         outFile << l << "\n";
//     }
//     outFile.close();
// }



void moveLineUp(const std::string& pathToFolder, int lineNum) {
    std::string path = pathToFolder + "/playlist";
    std::ifstream inFile(path);
    if (!inFile) return;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line))
        lines.push_back(line);
    inFile.close();

    if (lineNum > 0 && lineNum < static_cast<int>(lines.size())) {
        std::swap(lines[lineNum], lines[lineNum - 1]);
    }

    std::ofstream outFile(path);
    for (const auto& l : lines)
        outFile << l << "\n";
}

void moveLineDown(const std::string& pathToFolder, int lineNum) {
    std::string path = pathToFolder + "/playlist";
    std::ifstream inFile(path);
    if (!inFile) return;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line))
        lines.push_back(line);
    inFile.close();

    if (lineNum >= 0 && lineNum < static_cast<int>(lines.size()) - 1) {
        std::swap(lines[lineNum], lines[lineNum + 1]);
    }

    std::ofstream outFile(path);
    for (const auto& l : lines)
        outFile << l << "\n";
}

std::string readFirstLine(const std::string& filename){
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()){
        std::getline(file, line);
    }

    return line;
}

void insertFirstLine(const std::string& path, const std::string& newLine) {
    std::ifstream inFile(path);
    if (!inFile) return;

    std::stringstream buffer;
    buffer << newLine << "\n" << inFile.rdbuf();
    inFile.close();

    std::ofstream outFile(path, std::ios::trunc);
    outFile << buffer.str();
}

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

std::string cacheFolder = [](){
    const char* home = getenv("HOME");
    if (!home) {
        system("notify-send 'HOME not set!'");
        std::exit(1);
    }
    return std::string(home) + "/.cache/rain/";
}();

void refreshList() {
    std::lock_guard<std::mutex> lock(data_mutex);

    files = getFiles(pathToFolder);

    fullPaths.clear();
    formatedItems.clear();

    if (files.empty()) {
        list = formatedItems;
        return;
    }

    fs::create_directories(fs::path(fs::path(getenv("HOME")) / ".cache/rain/test").parent_path());

    fullPaths.reserve(files.size());
    formatedItems.reserve(files.size());

    for (const auto& file : files) {
        if (file.find("@rain:spotify\\") != std::string::npos ||
            file.find("@rain:soundcloud\\") != std::string::npos ||
            file.find("@rain:ytm\\") != std::string::npos) {
            fullPaths.push_back(file);
        } else {
            fullPaths.push_back(std::string(pathToFolder + "/" + file));
        }
    }

    for (size_t i = 0; i < files.size(); ++i) {
        std::string track = files[i];
        std::string title, artist;

        if (track.find("@rain:spotify\\") != std::string::npos ||
            track.find("@rain:soundcloud\\") != std::string::npos ||
            track.find("@rain:ytm\\") != std::string::npos) {

            std::string id = track;
            if (track.find("@rain:spotify\\") != std::string::npos) {
                removeAll(id, "@rain:spotify\\");
            } else if (track.find("@rain:soundcloud\\") != std::string::npos) {
                removeAll(id, "@rain:soundcloud\\");
                for (char& c : id) if (c == '/') c = '.';
            } else if (track.find("@rain:ytm\\") != std::string::npos) {
                removeAll(id, "@rain:ytm\\");
            }

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
                fullPaths[i] = track;
            }
        } else {
            try {
                TagLib::FileRef f((pathToFolder + "/" + track).c_str());
                TagLib::Tag* tag = f.tag();
                title = tag ? tag->title().to8Bit(true) : "unknown";
                artist = tag ? tag->artist().to8Bit(true) : "unknown";
                fullPaths[i] = pathToFolder + "/" + track;
            } catch (...) {
                title = "error";
                artist = "error";
                fullPaths[i] = pathToFolder + "/" + track;
            }
        }

        formatedItems.push_back(title + " - " + artist);
    }

    list = formatedItems;
}
