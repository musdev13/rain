#include "mus.h/event_loop.hpp"
#include "mus.h/atomic_vars.hpp"

#include <thread>
#include <atomic>

void preload_tracks() {
    for (size_t i = 0; i < fullPaths.size(); ++i) {
        if (fullPaths[i].find("@rain:spotify\\") != std::string::npos) {
            std::string id = fullPaths[i];
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

                    list[i] = title + " - " + artist;

                    fullPaths[i] = mp3_path;
                }
            } else {
                fullPaths[i] = mp3_path;
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

            current = (current + 1) % fullPaths.size();
            track = current;
            selected_global = static_cast<int>(current);

            std::string path = fullPaths[current];
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