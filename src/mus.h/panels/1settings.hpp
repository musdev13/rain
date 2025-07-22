#pragma once
#include "panelBase.hpp"
#include "../buttonOptions/btnStyle.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include "../spotify/addSpotifyTrack.hpp"
#include "../soundcloud/soundcloud.hpp"
#include "../ytm/ytm.hpp"

class Settings : public panelBase {
private:
    ftxui::Component input;
    ftxui::Component inputBlocked;
    std::string url;

    ftxui::Component menu;
    int selected;
    MenuOption opt;
    ftxui::Component searchInput;
    ftxui::Component searchInputBlocked;
    std::string searchInputContent;
public:
    Settings();

    ftxui::Element getElement() override;
};
