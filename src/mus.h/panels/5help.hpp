#pragma once
#include "panelBase.hpp"

class HelpPanel:public panelBase {
public:
    HelpPanel();
    ftxui::Element getElement() override;
};