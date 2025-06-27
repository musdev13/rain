#pragma once
#include "panelBase.hpp"
#include "../buttonOptions/btnStyle.hpp"

class Settings : public panelBase {
private:
    ftxui::Component button;
public:
    Settings();

    ftxui::Element getElement() override;
};