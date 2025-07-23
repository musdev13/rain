#include "../mus.h/panels/1settings.hpp"
#include <ftxui/component/component.hpp>

void osearchM(std::thread& osearch, std::string& searchInputContent){
    if (searchInputContent.find("https://") != std::string::npos){
        if (searchInputContent.find("open.spotify.com/track/") != std::string::npos)
            addSpotifyTrack(searchInputContent);
        else if (searchInputContent.find("soundcloud.com/") != std::string::npos)
            addSoundcloudTrack(searchInputContent);
        else if (searchInputContent.find("music.youtube.com/") != std::string::npos)
            addYTMTrack(searchInputContent);
        refreshList();
        menuctl.setID(3);
        searchInputContent = "";
    } else {
        if (osearch.joinable()){
            osearchRunning = false;
            osearch.join();
        }
        std::string formatedName = url_encode(searchInputContent);
        osearchRunning = true;
        osearch = std::thread(osearchd, formatedName);
    }
}

Settings::Settings(){
    selected = 0;
    opt.on_enter =[&]{
        // debug(rawSearchList[selected]);
        insertFirstLine(pathToFolder+"/playlist", rawSearchList[selected]);
        refreshList();
        menuctl.setID(3);
        inputContent = "";
    };
    menu = Menu(&searchList, &selected, opt);


    layout = Container::Horizontal({
        menu
    });
}

ftxui::Element Settings::getElement() {
    return vbox({
        vbox(menu->Render()) | yframe | flex
    }) | flex;
}
