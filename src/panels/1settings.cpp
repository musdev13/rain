#include "../mus.h/panels/1settings.hpp"
#include <ftxui/component/component.hpp>

Settings::Settings(){
    ftxui::InputOption inputOptions;
    inputOptions.placeholder = "Online Track URL...";
    input = Input(&url, inputOptions);
    inputBlocked = CatchEvent(input, [this](Event event) {
        if (event == Event::Return){
            if (url.find("open.spotify.com/track/") != std::string::npos)
                addSpotifyTrack(url);
            else if (url.find("soundcloud.com/") != std::string::npos)
                addSoundcloudTrack(url);
            else if (url.find("music.youtube.com/") != std::string::npos)
                addYTMTrack(url);
            refreshList();
            url = "";
            return true;
        }
        return false;
    });

    ftxui::InputOption searchInputOptions;
    searchInputOptions.placeholder = "Search Online Track...";
    searchInput = Input(&searchInputContent,searchInputOptions);
    searchInputBlocked = CatchEvent(searchInput, [this](Event event) {
        if (event == Event::Return){
            if (osearch.joinable()){
                osearchRunning = false;
                osearch.join();
            }
            std::string formatedName = url_encode(searchInputContent);
            osearchRunning = true;
            osearch = std::thread(osearchd, formatedName);
            return true;
        }
        return false;
    });

    selected = 0;
    menu = Menu(&searchList, &selected, opt);


    layout = Container::Horizontal({
        Container::Vertical({
            inputBlocked,
            searchInputBlocked,
            menu
        })
    });
}

ftxui::Element Settings::getElement() {
    return vbox({
        input->Render(),
        ftxui::separator(),
        searchInput->Render(),
        ftxui::separator(),
        vbox(menu->Render()) | yframe | flex
    }) | flex;
}
