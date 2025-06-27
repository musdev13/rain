#include "../mus.h/panels/4playerctl.hpp"

std::string format_time(double seconds) {
    int total = static_cast<int>(seconds);
    int minutes = total / 60;
    int secs = total % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d:%02d", minutes, secs);
    return std::string(buf);
}

playerCtl::playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr) {
    mpv = mpvv;
    screen = &screenr;
    title = "Nothing";
    artist = "Unknown";
    pp = Button("", []{}, btnStyle());

    updateThread = std::thread([this]() {
    std::string last_title, last_artist;
    while (active) {
        const char* t = mpv_get_property_string(mpv, "media-title");

        const char* raw_artist = mpv_get_property_string(mpv, "metadata/by-key/Artist");
        if (!raw_artist) raw_artist = mpv_get_property_string(mpv, "metadata/by-key/artist");
        if (!raw_artist) raw_artist = mpv_get_property_string(mpv, "metadata/by-key/ARTIST");

        std::string new_title = t ? t : "Unknown";
        std::string new_artist = raw_artist ? raw_artist : "Unknown";

        if (t) mpv_free((void*)t);
        if (raw_artist) mpv_free((void*)raw_artist);

        double time_pos = 0;
        double dur = 0;

        if (mpv_get_property(mpv, "playback-time", MPV_FORMAT_DOUBLE, &time_pos) == 0)
            position = format_time(time_pos);

        if (mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &dur) == 0)
            duration = format_time(dur);

        if (new_title != last_title || new_artist != last_artist) {
            last_title = new_title;
            last_artist = new_artist;
            title = new_title;
            artist = new_artist;
            screen->PostEvent(Event::Custom);
        } else {
            screen->PostEvent(Event::Custom);
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
    return hbox({
        vbox({
            hbox({
                text(title + " — " + artist) | center | flex
            }),
            hbox({
                text(position),
                pp->Render() | center,
                text(duration)
            }) | center | flex
        }) | center | flex
    });
}
