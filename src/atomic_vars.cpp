#include "mus.h/atomic_vars.hpp"
#include <atomic>
#include <vector>

std::atomic<bool> running(true);
std::atomic<size_t> current(0);
std::atomic<bool> manual_change(false);
std::atomic<int> selected_global(0);
std::vector<std::string> list ={};
std::atomic<bool> infoPanel(true);
std::atomic<bool> updateList(false);

std::vector<std::string> fullPaths;
std::string pathToFolder;
std::vector<std::string> files;
std::vector<std::string> formatedItems;

std::string SC_clientID;
std::mutex data_mutex;

std::vector<std::string> searchList;
std::vector<std::string> rawSearchList;
std::atomic<bool> osearchRunning(false);
Menuctl menuctl;
std::string inputContent;
