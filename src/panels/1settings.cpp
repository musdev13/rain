#include "../mus.h/panels/1settings.hpp"

Settings::Settings(){
    ftxui::InputOption inputOptions;
    inputOptions.placeholder = "Spotify/Soundcloud Track URL...";
    input = Input(&url, inputOptions);
    inputBlocked = CatchEvent(input, [this](Event event) {
        if (event == Event::Return){
            if (url.find("open.spotify.com/track/") != std::string::npos)
                addSpotifyTrack(url);
            else if (url.find("soundcloud.com/") != std::string::npos)
                addSoundcloudTrack(url);
            refreshList();
            url = "";
            return true;
        }
        return false;
    });


    layout = Container::Horizontal({
        inputBlocked
    });
}

ftxui::Element Settings::getElement() {
    return vbox({
        input->Render()
    });
}
