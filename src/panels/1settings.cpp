#include "../mus.h/panels/1settings.hpp"

Settings::Settings(){
    ftxui::InputOption spotifyInputOptions;
    spotifyInputOptions.placeholder = "Spotify Track URL...";
    spotifyInput = Input(&spotifyUrl, spotifyInputOptions);
    spotifyInputBlocked = CatchEvent(spotifyInput, [this](Event event) {
        if (event == Event::Return){
            // Here will be addSpotifyTrack();
            addSpotifyTrack(spotifyUrl);
            refreshList();
            spotifyUrl = "";
            return true;
        }
        return false;
    });


    layout = Container::Horizontal({
        spotifyInputBlocked
    });
}

ftxui::Element Settings::getElement() {
    return vbox({
        spotifyInput->Render()
    });
}
