#include "mus.h/atomic_vars.hpp"
#include "mus.h/filefs.hpp"
#include "mus.h/panels/panels.hpp"
#include <ftxui/component/event.hpp>

int main(int argc, char* argv[]){
    pathToFolder = argv[1];
    // std::string artist;
    // std::string title;
    // getTrackInfo("2C1NoGfuZRlp5Cz7AQ6gEt", title, artist);
    // system(("notify-send \""+ title +" - "+artist+"\"").c_str());
    // getTrack("2C1NoGfuZRlp5Cz7AQ6gEt","file.mp3");

    mpv_handle* mpv = mpv_create();
    // mpv_request_log_messages(mpv, "v");
    // mpv_set_option_string(mpv, "no-video", "yes");
    // mpv_set_option_string(mpv, "terminal", "yes");
    mpv_set_option_string(mpv, "vo", "null");
    // mpv_set_property_string(mpv, "keep-open", "yes");
    if (!mpv) {
        std::cerr << "Ошибка создания mpv\n";
        return 1;
    }

    if (mpv_initialize(mpv) < 0) {
        std::cerr << "Ошибка инициализации mpv\n";
        return 1;
    }

    SC_clientID = readFirstLine((fs::path(getenv("HOME")) / ".config/rain/SCCID").string());
    // system(("notify-send \""+SC_clientID+"\"").c_str());

    // for (int i=0;i<10;i++) searchList.push_back("Musya");
    refreshList();
    // for (auto file:fullPaths){
    //     std::cout << file << std::endl;
    // }


    auto screen = ScreenInteractive::TerminalOutput();
    std::vector<std::unique_ptr<panelBase>> panels;
    // panels.push_back(std::unique_ptr<panelBase>(header));
    panels.push_back(std::make_unique<Blank>());
    panels.push_back(std::make_unique<Settings>());
    panels.push_back(std::make_unique<Blank>());
    panels.push_back(std::make_unique<MediaList>(mpv));
    panels.push_back(std::make_unique<playerCtl>(mpv,screen));
    panels.push_back(std::make_unique<HelpPanel>());

    Header* header = new Header(&menuctl, &screen);
    panels[0] = std::unique_ptr<panelBase>(header);

    std::thread eventThread(event_loop, mpv);


    auto layout = Container::Vertical({});


    auto renderer = Renderer(layout, [&]{
        layout->DetachAllChildren();
        layout->Add(panels[0]->getLayout());

        int id = menuctl.getID();
        if (id < 0 || id >= panels.size()) id = 0;

        layout->Add(panels[id]->getLayout());
        layout->Add(panels[4]->getLayout());

        return vbox({
            // здесь хэдер
            panels[0]->getElement(),
            separator(),
            // здесь будет само меню
            hbox({
                panels[menuctl.getID()]->getElement(),
                infoPanel ? (panels[5]->getElement()):(filler())
            }) | flex,
            separator(),
            panels[4]->getElement(),
        }) | border;
    });

    panels[0]->getLayout()->ChildAt(1)->TakeFocus();

    auto app = CatchEvent(renderer, [&](Event event) {
        if (event == Event::CtrlF) {
            header->getInputComponent()->TakeFocus();
            return true;
        }
        if (event == Event::CtrlQ) {
            exit(0);
        }
        if (event == Event::CtrlS){
            panels[4]->getLayout()->ChildAt(0)->TakeFocus();
            return true;
        }
        double time_pos = 0;
        double duration = 0;

        mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &time_pos);
        mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &duration);

        if (event == Event::ArrowRightCtrl && time_pos + 5 < duration) {
            const char* seek_forward[] = {"seek", "5", "relative", nullptr};
            mpv_command(mpv, seek_forward);
            return true;
        }

        if (event == Event::ArrowLeftCtrl && time_pos > 5) {
            const char* seek_backward[] = {"seek", "-5", "relative", nullptr};
            mpv_command(mpv, seek_backward);
            return true;
        }
        if (event == Event::ArrowUpCtrl){
            moveLineUp(pathToFolder, selected_global);
            refreshList();
            if (selected_global -1 >=0) selected_global--;
            return true;
        }
        if (event == Event::CtrlX) {
            if (selected_global >= 0 && selected_global < static_cast<int>(files.size())) {
                deleteLine(pathToFolder, selected_global);
                refreshList();

                if (files.empty()) {
                    selected_global = 0;
                } else if (selected_global >= static_cast<int>(files.size())) {
                    selected_global = static_cast<int>(files.size()) - 1;
                }

                return true;
            } else {
                return false;
            }
        }
        if (event == Event::ArrowDownCtrl){
            moveLineDown(pathToFolder, selected_global);
            refreshList();
            if (selected_global +1 < list.size()) selected_global++;
            return true;
        }
        if (event == Event::CtrlB){
            infoPanel ? infoPanel = false : infoPanel = true;
            return true;
        }
        if (event == Event::CtrlR){
            refreshList();
            return true;
        }
        return false;
    });

    screen.Loop(app);

    running = false;
    eventThread.join();
    mpv_terminate_destroy(mpv);

    return 0;
}
