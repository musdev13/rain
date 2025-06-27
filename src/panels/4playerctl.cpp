#include "../mus.h/panels/4playerctl.hpp"

playerCtl::playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr) {
    mpv = mpvv;
    screen = &screenr;
    title = "Nothing";
    pp = Button("||", []{}, btnStyle());

    updateThread = std::thread([this]() {
        std::string last_title;
        while (active) {
            const char* t = mpv_get_property_string(mpv, "media-title");
            if (t) {
                std::string new_title = t;
                mpv_free((void*)t);
                if (new_title != last_title) {
                    last_title = new_title;
                    title = new_title;
                    screen->PostEvent(Event::Custom);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    layout = Container::Horizontal({
        pp
    });
}

playerCtl::~playerCtl() {
    active = false;
    if (updateThread.joinable()) updateThread.join();
}


Element playerCtl::getElement(){
    return vbox({
        hbox({
            text(title) | center | flex
        }),
        hbox({
            pp->Render() | center | flex
        })
    });
}

