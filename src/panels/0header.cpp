#include "../mus.h/panels/0header.hpp"

Header::Header(Menuctl* menuctl, ScreenInteractive* screen){
    prevMenuID = 2;
    button1 = Button("", [menuctl, screen, this] {
        int id = menuctl->getID();
        if (id == 1){
            menuctl->setID(prevMenuID);
        }
        else{
            prevMenuID = id;
            menuctl->setID(1);
        }
        screen->PostEvent(Event::Custom);
    }, btnHeaderStyle());
    ftxui::InputOption options;
    options.placeholder = " Search...";
    
    input = Input(&inputContent, options);

    inputBlocked = CatchEvent(input, [](Event event) {
        if (event == Event::Return)
            return true;
        return false;
    });

    layout = Container::Horizontal({
        button1,
        inputBlocked,
    });
}

ftxui::Element Header::getElement() {
    return hbox({
        button1->Render() | center,
        input->Render(),
    });
}

Component Header::getInputComponent(){return input;}