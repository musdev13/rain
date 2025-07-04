#include "../mus.h/panels/1settings.hpp"

Settings::Settings(){
    button = Button("Nothing :3", [] {}, btnStyle());
    layout = Container::Horizontal({ button });
}

ftxui::Element Settings::getElement() {
    return hbox({
        button->Render(),
    });
}