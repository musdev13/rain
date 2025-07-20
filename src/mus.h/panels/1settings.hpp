#pragma once
#include "panelBase.hpp"
#include "../buttonOptions/btnStyle.hpp"
#include <ftxui/component/event.hpp>
#include "../spotify/addSpotifyTrack.hpp"
#include "../soundcloud/soundcloud.hpp"

class Settings : public panelBase {
private:
    ftxui::Component input;
    ftxui::Component inputBlocked;
    std::string url;
public:
    Settings();

    ftxui::Element getElement() override;
};
