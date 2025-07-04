#include "mus.h/filefs.hpp"
#include "mus.h/panels/panels.hpp"

int main(int argc, char* argv[]){
    std::vector<std::string> files = getFiles(argv[1]);
    mpv_handle* mpv = mpv_create();
    // mpv_request_log_messages(mpv, "v");
    // mpv_set_option_string(mpv, "no-video", "yes");
    // mpv_set_option_string(mpv, "terminal", "yes");
    mpv_set_option_string(mpv, "vo", "null");
    if (!mpv) {
        std::cerr << "Ошибка создания mpv\n";
        return 1;
    }

    if (mpv_initialize(mpv) < 0) {
        std::cerr << "Ошибка инициализации mpv\n";
        return 1;
    }

    std::vector<std::string> fullPaths;
    for (const auto& file : files) {
        fullPaths.push_back(std::string(argv[1]) + "/" + file);
    }
    std::thread eventThread(event_loop, mpv, fullPaths);

    Menuctl menuctl;
    auto screen = ScreenInteractive::TerminalOutput();
    std::vector<std::unique_ptr<panelBase>> panels;
    // panels.push_back(std::unique_ptr<panelBase>(header));
    panels.push_back(std::make_unique<Blank>());
    panels.push_back(std::make_unique<Settings>());
    panels.push_back(std::make_unique<Blank>());
    panels.push_back(std::make_unique<MediaList>(files, mpv, argv[1]));
    panels.push_back(std::make_unique<playerCtl>(mpv,screen));

    Header* header = new Header(&menuctl, &screen);
    panels[0] = std::unique_ptr<panelBase>(header);


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
            panels[menuctl.getID()]->getElement(),
            separator(),
            panels[4]->getElement(),
        }) | border;
    });

    panels[0]->getLayout()->ChildAt(1)->TakeFocus();

    auto app = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('/')) {
            header->getInputComponent()->TakeFocus();
            return true;
        }
        if (event == Event::CtrlQ) {
            screen.Exit();
            exit(0);
        }
        if (event == Event::CtrlS){
            panels[4]->getLayout()->ChildAt(0)->TakeFocus();
            return true;
        }
        if (event == Event::ArrowLeftCtrl){
            const char* seek_backward[] = {"seek", "-5", "relative", nullptr};
            mpv_command(mpv, seek_backward);
            return true;
        }
        if (event == Event::ArrowRightCtrl){
            const char* seek_forward[] = {"seek", "5", "relative", nullptr};
            mpv_command(mpv, seek_forward);
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