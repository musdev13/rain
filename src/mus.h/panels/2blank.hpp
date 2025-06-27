#pragma once
#include "panelBase.hpp"

class Blank : public panelBase {
private: 
    Element text;
public:
    Blank();
    ftxui::Element getElement() override;
};