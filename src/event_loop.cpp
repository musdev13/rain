#include "mus.h/event_loop.hpp"

void event_loop(mpv_handle* mpv, const std::vector<std::string>& playlist) {
    while (running){
        // std::ofstream logFile("mpv_debug.log", std::ios::app);
        mpv_event* event = mpv_wait_event(mpv, 1000);
        if (event->event_id == MPV_EVENT_NONE) continue;

        if (event->event_id == MPV_EVENT_END_FILE) {
            
            // logFile << "MPV EVENT END FILE" << std::endl;
            
            if (manual_change) {
                // logFile << "MANUALLY CHANGED" << std::endl;
                manual_change = false;
                continue;
            }
            // logFile << "CURRENT ++" << std::endl;
            current = (current + 1) % playlist.size();
            std::string path = playlist[current];
            // logFile << "TRYING TO START MPV:" << std::endl << "\t"<<path << std::endl;
            const char* cmd[] = {"loadfile", path.c_str(), "replace", nullptr};
            // logFile << "\t" << cmd << std::endl;
            mpv_command(mpv, cmd);
        }
        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            running = false;
        }
        // logFile.close();
    }
}