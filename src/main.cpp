// #include "PythonEmbed.hpp"
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <mpv/client.h>
#include <thread>
#include <atomic>

std::atomic<bool> running(true);
std::atomic<size_t> current(0);
std::atomic<bool> manual_change(false);
void event_loop(mpv_handle* mpv, const std::vector<std::string>& playlist) {
    while (running){
        mpv_event* event = mpv_wait_event(mpv, 1000);
        if (event->event_id == MPV_EVENT_NONE) continue;

        if (event->event_id == MPV_EVENT_END_FILE) {
            if (manual_change) {
                // Сбрасываем флаг, не меняем current — это уже сделано вручную
                manual_change = false;
                continue;
            }
            current = (current + 1) % playlist.size();
            std::string path = playlist[current];
            const char* cmd[] = {"loadfile", path.c_str(), "replace", nullptr};
            mpv_command(mpv, cmd);
        }
        if (event->event_id == MPV_EVENT_SHUTDOWN) {
            running = false;
        }
    }
}



using namespace ftxui;
namespace fs = std::filesystem;

std::vector<std::string> getFilesInDirectory(const std::string& path) {
    std::vector<std::string> files;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}

ButtonOption btnStyle(){
    auto option = ButtonOption::Animated();
    option.transform = [](const EntryState& s){
        auto element = text(s.label) | center | bold;
        element = hbox({text(" "),element,text("  ")});
        if (s.focused)
            element = element | color(Color::Black) | bgcolor(Color::White);
        return element;
    };
    return option;
};


class Menuctl{
private:
    int id;
public:
    Menuctl(){id =3;}
    int getID() const {
        return id;
    }
    void setID(int id){
        this->id = id;
    }
};

class panelBase {
protected:
    ftxui::Component layout;

public:
    virtual ~panelBase() = default;

    virtual ftxui::Element getElement() = 0;

    ftxui::Component getLayout() {
        return layout;
    }
};

class Blank : public panelBase {
private: 
    Element text;
public:
    Blank(){text = ftxui::text("");layout=Container::Horizontal({});}
    ftxui::Element getElement() override{
        return vbox({
            text,
        });
    }
};

class Header : public panelBase {
private:
    ftxui::Component button1;
    ftxui::Component button2;
    ftxui::Component input;
    Component inputBlocked;
    std::string inputContent;
    int prevMenuID;
public:
    Header(Menuctl* menuctl, ScreenInteractive* screen) {
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
        }, btnStyle());
        button2 = Button("", [] {}, btnStyle());
        ftxui::InputOption options;
        options.placeholder = "Search...";
        options.on_enter = [&]{
        };
        
        input = Input(&inputContent, options);

        inputBlocked = CatchEvent(input, [](Event event) {
            if (event == Event::Return)
                return true;
            return false;
        });

        layout = Container::Horizontal({
            button1,
            inputBlocked,
            button2,
        });
    }

    ftxui::Element getElement() override {
        return hbox({
            button1->Render(),
            input->Render(),
            button2->Render(),
        });
    }
    
    Component getInputComponent(){
        return input;
    }
};

class Settings : public panelBase {
private:
    ftxui::Component button;

public:
    Settings() {
        button = Button("Some Button", [] {}, btnStyle());
        layout = Container::Horizontal({ button });
    }

    ftxui::Element getElement() override {
        return hbox({
            button->Render(),
        });
    }
};

class MediaList : public panelBase {
private:
    ftxui::Component menu;
    int selected;
    std::vector<std::string> items;
    mpv_handle* mpv;
    std::string folderPath;
public:
    MediaList(const std::vector<std::string>& inputItems, mpv_handle* mpvv, const std::string& pathToFolder){
        // items = {"1","2","3","4","5","6","7","8","9","10"};
        // список треков (уже отсортирован)
        folderPath=pathToFolder;
        items = inputItems;
        mpv=mpvv;
        selected = 0;
        MenuOption opt;
        opt.on_enter = [&]{
            // std::cout << items[selected] << std::endl;
            std::string full_path = folderPath + "/" + items[selected];
            // std::cout << full_path << std::endl;
            std::ofstream logFile("mpv_debug.log", std::ios::app);
            logFile << "Загружаю: " << full_path << std::endl;
            logFile.close();
            const char* cmd[] = {"loadfile", full_path.c_str(), "replace", nullptr};
            mpv_command(mpv, cmd);
            current = selected;
            manual_change = true;
        };
        menu = Menu(&items,&selected, opt);

        layout = Container::Horizontal({menu});
    }

    ftxui::Element getElement() override {
        return hbox({
            menu->Render()
        }) | yframe | flex;
    }
};

class playerCtl : public panelBase {
private:
    std::string title;
    mpv_handle* mpv;
    ScreenInteractive* screen;
    std::thread updateThread;
    std::atomic<bool> active{true};

public:
    playerCtl(mpv_handle* mpvv, ScreenInteractive& screenr) {
        mpv = mpvv;
        screen = &screenr;
        title = "Nothing";

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

        layout = Container::Horizontal({});
    }
    ~playerCtl() {
        active = false;
        if (updateThread.joinable()) updateThread.join();
    }

    Element getElement() override {
    return hbox({
        text(title) | center | flex
    });
}

};


int main(int argc, char* argv[]){
    /*PythonEmbed py;
    if (!py.loadModule("test")){
        return 1;
    }
    std::string pyText = py.callFunc("randomNumOpt",{PythonEmbed::toPyInt(1), PythonEmbed::toPyInt(10)});
    */
    // std::cout << argv[1] << std::endl;
    // std::vector<std::string> items = {"1","2","3","4","5","6","7","8","9","10"};
    std::vector<std::string> files = getFilesInDirectory(argv[1]);
    mpv_handle* mpv = mpv_create();
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
    Header* header = new Header(&menuctl, &screen);
    panels.push_back(std::unique_ptr<panelBase>(header));
    panels.push_back(std::make_unique<Settings>());
    panels.push_back(std::make_unique<Blank>());
    panels.push_back(std::make_unique<MediaList>(files, mpv, argv[1]));
    panels.push_back(std::make_unique<playerCtl>(mpv,screen));

    auto layout = Container::Vertical({});
    


    auto renderer = Renderer(layout, [&]{
        layout->DetachAllChildren();
        layout->Add(panels[0]->getLayout());

        int id = menuctl.getID();
        if (id < 0 || id >= panels.size()) id = 0;

        layout->Add(panels[id]->getLayout());

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
        return false;
    });

    screen.Loop(app);

    // py.~PythonEmbed();
    // eventThread.join();
    
    running = false;
    eventThread.join();
    mpv_terminate_destroy(mpv);

    return 0;
}