#pragma once
#include "panelBase.hpp"
#include <fstream>
#include <thread>
#include "../event_loop.hpp"

class MediaList : public panelBase {
private:
    ftxui::Component menu;
    int selected;
    std::vector<std::string> items;
    mpv_handle* mpv;
    std::string folderPath;
public:
    MediaList(const std::vector<std::string>& inputItems, mpv_handle* mpvv, const std::string& pathToFolder);

    ftxui::Element getElement() override;
};