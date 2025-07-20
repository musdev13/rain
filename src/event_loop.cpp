#include "mus.h/event_loop.hpp"
#include "mus.h/atomic_vars.hpp"

#include <thread>
#include <atomic>

void preload_tracks() {
    while (running) {
        size_t size;

        {
            std::lock_guard<std::mutex> lock(data_mutex);
            size = fullPaths.size();
        }

        for (size_t i = 0; i < size; ++i) {
            std::string path;

            {
                std::lock_guard<std::mutex> lock(data_mutex);
                path = fullPaths[i];
            }

            if (path.find("@rain:spotify\\") != std::string::npos) {
                std::string id = path;
                removeAll(id, "@rain:spotify\\");
                std::string mp3_path = cacheFolder + id + ".mp3";
                std::string info_path = cacheFolder + id + ".infosp";

                if (!fs::exists(mp3_path) || !fs::exists(info_path)) {
                    std::string title, artist;
                    getTrackInfo(id, title, artist);
                    getTrack(id, cacheFolder + id + ".dwnld");

                    if (fs::exists(cacheFolder + id + ".dwnld")) {
                        fs::rename(cacheFolder + id + ".dwnld", mp3_path);
                        writeFile(info_path, title + "\n" + artist);

                        std::lock_guard<std::mutex> lock(data_mutex);
                        if (i < list.size()) list[i] = title + " - " + artist;
                        if (i < fullPaths.size()) fullPaths[i] = mp3_path;
                    }
                } else {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    if (i < fullPaths.size()) fullPaths[i] = mp3_path;
                }
            }
            else if (path.find("@rain:soundcloud\\") != std::string::npos) {
                std::string id = path;
                removeAll(id, "@rain:soundcloud\\");
                std::string fileID = id;
                for (char& c : fileID) if (c == '/') c = '.';
                std::string mp3_path = cacheFolder + fileID + ".mp3";
                std::string info_path = cacheFolder + fileID + ".infosp";

                if (!fs::exists(mp3_path) || !fs::exists(info_path)) {
                    std::string title, artist;
                    getSCTrackInfo(SC_clientID, id, title, artist);
                    getSCTrack(SC_clientID, id, cacheFolder + fileID + ".dwnld");

                    if (fs::exists(cacheFolder + fileID + ".dwnld")) {
                        fs::rename(cacheFolder + fileID + ".dwnld", mp3_path);
                        writeFile(info_path, title + "\n" + artist);

                        std::lock_guard<std::mutex> lock(data_mutex);
                        if (i < list.size()) list[i] = title + " - " + artist;
                        if (i < fullPaths.size()) fullPaths[i] = mp3_path;
                    }
                } else {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    if (i < fullPaths.size()) fullPaths[i] = mp3_path;
                }
            }
        }
    }
}


void event_loop(mpv_handle* mpv) {
    std::thread preloadThread(preload_tracks);

    int track = 0;
    while (running) {
        mpv_event* event = mpv_wait_event(mpv, 1000);
        if (event->event_id == MPV_EVENT_NONE) continue;

        if (event->event_id == MPV_EVENT_END_FILE) {
            if (manual_change) {
                manual_change = false;
                continue;
            }

            size_t nextTrack = 0;
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                if (!fullPaths.empty())
                    nextTrack = (current + 1) % fullPaths.size();
            }

            current = nextTrack;
            track = current;
            selected_global = static_cast<int>(current);

            std::string path;
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                path = fullPaths[current];
            }

            if (fs::exists(path)) {
                const char* cmd[] = {"loadfile", path.c_str(), "replace", nullptr};
                mpv_command(mpv, cmd);
            }
        }

        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            running = false;
        }
    }

    if (preloadThread.joinable()) {
        preloadThread.join();
    }
}
