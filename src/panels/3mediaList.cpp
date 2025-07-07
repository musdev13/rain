#include "../mus.h/panels/3mediaList.hpp"
#include <ftxui/component/screen_interactive.hpp>

// void logToFile(const std::string& message) {
//     // std::ofstream log("debug_log.txt", std::ios::app);
//     // log << message << std::endl;
// }

MediaList::MediaList(const std::vector<std::string>& inputItems, mpv_handle* mpvv, const std::string& pathToFolder) {
    folderPath = pathToFolder;
    items = inputItems;
    mpv = mpvv;

    formatedItems.resize(items.size());
    fullPaths.resize(items.size());

    for (size_t i = 0; i < items.size(); ++i) {
        std::string track = items[i];
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
            TagLib::FileRef f((folderPath + "/" + track).c_str());
            TagLib::Tag* tag = f.tag();
            title = tag ? tag->title().to8Bit(true) : "unknown";
            artist = tag ? tag->artist().to8Bit(true) : "unknown";
            fullPaths[i] = folderPath + "/" + track;
        }
        formatedItems[i] = title + " - " + artist;
    }

    list = formatedItems;
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
