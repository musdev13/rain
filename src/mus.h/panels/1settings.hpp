#pragma once
#include "panelBase.hpp"
#include "../buttonOptions/btnStyle.hpp"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <thread>
#include "../spotify/addSpotifyTrack.hpp"
#include "../soundcloud/soundcloud.hpp"
#include "../ytm/ytm.hpp"
#include "../url.hpp"
#include "../osearch/osearch.hpp"

void osearchM(std::thread& osearch, std::string& searchInputContent);

class Settings : public panelBase {
private:
    ftxui::Component menu;
    int selected;
    MenuOption opt;
    ftxui::Component searchInput;
    ftxui::Component searchInputBlocked;
    std::string searchInputContent;
    std::thread osearch;
public:
    Settings();

    ftxui::Element getElement() override;
};
