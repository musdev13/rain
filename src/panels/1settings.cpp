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
    ftxui::InputOption searchInputOptions;
    searchInputOptions.placeholder = "Search Track/Add by URL...";
    searchInput = Input(&searchInputContent,searchInputOptions);
    searchInputBlocked = CatchEvent(searchInput, [this](Event event) {
        if (event == Event::Return){
            osearchM(osearch, searchInputContent);
            return true;
        }
        return false;
    });

    selected = 0;
    opt.on_enter =[&]{
        // debug(rawSearchList[selected]);
        insertFirstLine(pathToFolder+"/playlist", rawSearchList[selected]);
        refreshList();
        menuctl.setID(3);
        searchInputContent = "";
    };
    menu = Menu(&searchList, &selected, opt);


    layout = Container::Horizontal({
        Container::Vertical({
            searchInputBlocked,
            menu
        })
    });
}

ftxui::Element Settings::getElement() {
    return vbox({
        searchInput->Render(),
        ftxui::separator(),
        vbox(menu->Render()) | yframe | flex
    }) | flex;
}
