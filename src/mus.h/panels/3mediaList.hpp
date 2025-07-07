#pragma once
#include "panelBase.hpp"
// #include <fstream>
#include <thread>
#include "../event_loop.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <algorithm>
#include <cctype>

#include "../filefs.hpp"
#include "../spotify/getTrackInfo.hpp"
#include "../spotify/getTrack.hpp"


class MediaList : public panelBase {
private:
    ftxui::Component menu;
    int selected;
    std::vector<std::string> items;
    std::vector<std::string> formatedItems;
    mpv_handle* mpv;
    std::string folderPath;
    MenuOption opt;
public:
    MediaList(const std::vector<std::string>& inputItems, mpv_handle* mpvv, const std::string& pathToFolder);

    ftxui::Element getElement() override;
};