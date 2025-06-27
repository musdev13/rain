#include "../mus.h/panels/2blank.hpp"

Blank::Blank(){text = ftxui::text("");layout=Container::Horizontal({});}
ftxui::Element Blank::getElement() {
    return vbox({
        text,
    });
}