#pragma once
#include "panelBase.hpp"
#include "../buttonOptions/btnStyle.hpp"
#include <ftxui/component/event.hpp>
#include "../spotify/addSpotifyTrack.hpp"

class Settings : public panelBase {
private:
    ftxui::Component spotifyInput;
    ftxui::Component spotifyInputBlocked;
    std::string spotifyUrl;
public:
    Settings();

    ftxui::Element getElement() override;
};
