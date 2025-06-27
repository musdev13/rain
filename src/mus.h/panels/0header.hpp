#pragma once
#include "panelBase.hpp"
#include "../menuctl.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include "../buttonOptions/btnHeaderStyle.hpp"


class Header : public panelBase {
private:
    ftxui::Component button1;
    ftxui::Component button2;
    ftxui::Component input;
    Component inputBlocked;
    std::string inputContent;
    int prevMenuID;
public:
    Header(Menuctl* menuctl, ScreenInteractive* screen);

    ftxui::Element getElement() override;
    
    Component getInputComponent();
};