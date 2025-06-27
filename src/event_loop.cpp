#include "mus.h/event_loop.hpp"

void event_loop(mpv_handle* mpv, const std::vector<std::string>& playlist) {
    while (running){
        mpv_event* event = mpv_wait_event(mpv, 1000);
        if (event->event_id == MPV_EVENT_NONE) continue;

        if (event->event_id == MPV_EVENT_END_FILE) {
            if (manual_change) {
                manual_change = false;
                continue;
            }
            current = (current + 1) % playlist.size();
            std::string path = playlist[current];
            const char* cmd[] = {"loadfile", path.c_str(), "replace", nullptr};
            mpv_command(mpv, cmd);
        }
        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            running = false;
        }
    }
}