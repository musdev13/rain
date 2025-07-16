#include "../mus.h/panels/3mediaList.hpp"
#include <ftxui/component/screen_interactive.hpp>

// void logToFile(const std::string& message) {
//     // std::ofstream log("debug_log.txt", std::ios::app);
//     // log << message << std::endl;
// }

MediaList::MediaList(mpv_handle* mpvv) {
    mpv = mpvv;

    selected = 0;
    opt.on_enter = [&] {
        if (selected >= 0 && selected < (int)fullPaths.size()) {
            std::string path = fullPaths[selected];
            if (fs::exists(path)) {
                const char* cmd[] = {"loadfile", path.c_str(), "replace", nullptr};
                mpv_command(mpv, cmd);
                current = selected;
                manual_change = true;
                std::thread([]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    manual_change = false;
                }).detach();
            }
        }
    };
    opt.on_change = [&] {
        selected_global = selected;
    };
    menu = Menu(&list, &selected, opt);
    layout = Container::Horizontal({menu});
}

ftxui::Element MediaList::getElement() {
    selected = selected_global.load();
    return hbox({
        menu->Render()
    }) | yframe | flex;
}
