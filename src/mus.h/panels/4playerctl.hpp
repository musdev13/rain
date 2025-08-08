// 4playerctl.hpp
#pragma once
#include "panelBase.hpp"
#include "../event_loop.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include "../buttonOptions/btnStyle.hpp"

std::string format_time(double seconds);

class playerCtl : public panelBase {
private:
    mpv_handle* mpv;
    ScreenInteractive* screen;
    std::thread updateThread;
    std::atomic<bool> active{true};
    std::string config_path = (configDir / "volume.cfg").string();

    bool paused;
    std::string icon = "";

    std::string title;
    std::string artist;
    std::string position = "0:00";
    std::string duration = "0:00";
    Component pp;

    std::string volume;
    Component volLess;
    Component volMore;

public:
    playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr);
    ~playerCtl();

    Element getElement() override;

    double load_volume();
    void save_volume(double vol);

};
