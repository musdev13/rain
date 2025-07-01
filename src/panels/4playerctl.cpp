// playerCtl.cpp
#include "../mus.h/panels/4playerctl.hpp"
#include <mpv/client.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <filesystem>

using namespace ftxui;
namespace fs = std::filesystem;

std::string format_time(double seconds) {
    int total = static_cast<int>(seconds);
    int minutes = total / 60;
    int secs = total % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d:%02d", minutes, secs);
    return std::string(buf);
}

double playerCtl::load_volume() {
    std::ifstream in(config_path);
    if (!in) return 50.0;
    double vol = 50.0;
    in >> vol;
    return std::clamp(vol, 0.0, 100.0);
}

void playerCtl::save_volume(double vol) {
    std::ofstream out(config_path);
    if (out) out << std::clamp(vol, 0.0, 100.0);
}

playerCtl::playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr) {
    mpv = mpvv;
    screen = &screenr;
    title = "Nothing";
    artist = "Unknown";
    volume = "n";

    config_path = (fs::path(getenv("HOME")) / ".config/rain/volume.cfg").string();
    fs::create_directories(fs::path(config_path).parent_path());

    double initial_volume = load_volume();
    mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &initial_volume);

    pp = Button(&icon, [this]() {
        int pause_flag = 0;
        if (mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &pause_flag) == 0) {
            pause_flag = !pause_flag;
            icon = pause_flag ? "" : "";
            mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &pause_flag);
            screen->PostEvent(Event::Custom);
        }
    }, btnStyle());

    volLess = Button("", [this]() {
        double vol = 0;
        if (mpv_get_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol) == 0) {
            vol = std::max(0.0, vol - 5);
            mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol);
            save_volume(vol);
        }
    }, btnStyle());

    volMore = Button("", [this]() {
        double vol = 0;
        if (mpv_get_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol) == 0) {
            vol = std::min(100.0, vol + 5);
            mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol);
            save_volume(vol);
        }
    }, btnStyle());

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
            double vol = 0;
            int pause_flag = 0;

            if (mpv_get_property(mpv, "playback-time", MPV_FORMAT_DOUBLE, &time_pos) == 0)
                position = format_time(time_pos);

            if (mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &dur) == 0)
                duration = format_time(dur);

            if (mpv_get_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol) == 0) {
                int vol_int = static_cast<int>(vol + 0.5);
                volume = std::to_string(vol_int) + "%";
            }

            if (mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &pause_flag) == 0)
                icon = pause_flag ? "" : "";

            if (new_title != last_title || new_artist != last_artist) {
                last_title = new_title;
                last_artist = new_artist;
                title = new_title;
                artist = new_artist;
            }

            screen->PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    layout = Container::Horizontal({
        pp,
        volLess,
        volMore
    });
}

playerCtl::~playerCtl() {
    active = false;
    if (updateThread.joinable()) updateThread.join();
}

Element playerCtl::getElement() {
    return hbox({
        text(" "),
        vbox({
            hbox({
                text(title + " - " + artist) | center | flex
            }),
            hbox({
                text(position),
                pp->Render() | center,
                text(duration)
            }) | center | flex
        }) | center | flex,
        vbox({
            text(" "),
            hbox({
                volLess->Render(),
                text(volume),
                volMore->Render()
            })
        })
    });
}
