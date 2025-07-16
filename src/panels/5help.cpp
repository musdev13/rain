#include "../mus.h/panels/5help.hpp"

HelpPanel::HelpPanel(){layout=Container::Vertical({});}

ftxui::Element HelpPanel::getElement(){
    return hbox({
        separator(),
        vbox({
            ftxui::text(" Navigation info: "),
            ftxui::text(" Ctrl+F - focus on search bar "),
            ftxui::text(" Ctrl+S - focus on play button "),
            ftxui::text(" Ctrl+←→ - change track pos "),
            ftxui::text(" Ctrl+R - refresh track list "),
            ftxui::text(""),
            ftxui::text(" Ctrl+B - show/hide this panel "),
            ftxui::text(" Ctrl+Q - quit ")
        })
    });
}
