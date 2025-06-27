#pragma once
#include "panelBase.hpp"
#include "../event_loop.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include "../buttonOptions/btnStyle.hpp"

class playerCtl : public panelBase {
private:
    std::string title;
    mpv_handle* mpv;
    ScreenInteractive* screen;
    std::thread updateThread;
    std::atomic<bool> active{true};
    Component pp;

public:
    playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr);
    ~playerCtl();

    Element getElement() override;

};