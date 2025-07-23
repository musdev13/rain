#pragma once
#include "panelBase.hpp"
#include "../menuctl.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include "../buttonOptions/btnStyle.hpp"
#include <iostream>
#include "../atomic_vars.hpp"
#include "../osearch/osearch.hpp"
// #include <unistd.h>


class Header : public panelBase {
private:
    ftxui::Component button1;
    ftxui::Component button2;
    ftxui::Component input;
    Component inputBlocked;

    int prevMenuID;
    std::thread osearch;
public:
    Header(Menuctl* menuctl, ScreenInteractive* screen);

    ftxui::Element getElement() override;

    Component getInputComponent();
};
