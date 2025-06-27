#include "../mus.h/panels/3mediaList.hpp"

MediaList::MediaList(const std::vector<std::string>& inputItems, mpv_handle* mpvv, const std::string& pathToFolder){
    // items = {"1","2","3","4","5","6","7","8","9","10"};
    folderPath=pathToFolder;
    items = inputItems;
    mpv=mpvv;
    selected = 0;
    MenuOption opt;
    opt.on_enter = [&]{
        // std::cout << items[selected] << std::endl;
        std::string full_path = folderPath + "/" + items[selected];
        // std::cout << full_path << std::endl;
        std::ofstream logFile("mpv_debug.log", std::ios::app);
        logFile << "Загружаю: " << full_path << std::endl;
        logFile.close();
        const char* cmd[] = {"loadfile", full_path.c_str(), "replace", nullptr};
        mpv_command(mpv, cmd);
        current = selected;
        manual_change = true;
        std::thread([](){
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            manual_change = false;
        }).detach();
    };
    menu = Menu(&items,&selected, opt);

    layout = Container::Horizontal({menu});
}

ftxui::Element MediaList::getElement(){
    return hbox({
        menu->Render()
    }) | yframe | flex;
}