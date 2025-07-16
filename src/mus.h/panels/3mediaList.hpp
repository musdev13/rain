#pragma once
#include "panelBase.hpp"
// #include <fstream>
#include <thread>
#include "../event_loop.hpp"
#include <algorithm>
#include <cctype>

#include "../filefs.hpp"
#include "../spotify/getTrackInfo.hpp"
#include "../spotify/getTrack.hpp"


class MediaList : public panelBase {
private:
    ftxui::Component menu;
    int selected;

    mpv_handle* mpv;
    MenuOption opt;
public:
    MediaList(mpv_handle* mpvv);

    ftxui::Element getElement() override;
};
