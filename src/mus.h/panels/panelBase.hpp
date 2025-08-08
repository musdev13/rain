#pragma once
#ifdef __WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef DOUBLE
#endif
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
using namespace ftxui;

class panelBase {
protected:
    ftxui::Component layout;

public:
    virtual ~panelBase() = default;

    virtual ftxui::Element getElement() = 0;

    ftxui::Component getLayout() {
        return layout;
    }
};