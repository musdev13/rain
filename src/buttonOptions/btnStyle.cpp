#include "../mus.h/buttonOptions/btnStyle.hpp"

ButtonOption btnStyle(){
    auto option = ButtonOption();
    option.transform = [](const EntryState& s){
        auto element = text(s.label) | center | bold;
        element = hbox({text(" "),element,text(" ")});
        if (s.focused)
            element = element | color(Color::Black) | bgcolor(Color::White);
        return element;
    };
    return option;
};
