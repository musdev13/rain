#include "../mus.h/panels/0header.hpp"

void search(std::string* inputContent){
    // system(("notify-send \"" + list[0] + "\"").c_str());
    for (int i =0; i<list.size();i++){
        // system(("notify-send " + std::to_string(i)).c_str()); 
        if (std::string(list[i]).find(*inputContent) != std::string::npos) { 
            selected_global = i; 
            // system(("notify-send " + std::to_string(i)).c_str());
            *inputContent = "";
            break;
        }
    }
}

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
    button2 = Button("", [this] {
        search(&inputContent);
    }, btnHeaderStyle());
    ftxui::InputOption options;
    options.placeholder = "Search...";
    
    input = Input(&inputContent, options);

    inputBlocked = CatchEvent(input, [this](Event event) {
        if (event == Event::Return){
            search(&inputContent);
            return true;
        }
        return false;
    });

    layout = Container::Horizontal({
        button1,
        inputBlocked,
        button2,
    });
}

ftxui::Element Header::getElement() {
    return hbox({
        button1->Render() | center,
        input->Render(),
        button2->Render() | center,
    });
}

Component Header::getInputComponent(){return input;}