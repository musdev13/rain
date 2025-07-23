#pragma once
#include <atomic>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include "menuctl.hpp"


extern std::atomic<bool> running;
extern std::atomic<size_t> current;
extern std::atomic<bool> manual_change;
extern std::atomic<int> selected_global;
extern std::vector<std::string> list;
extern std::atomic<bool> infoPanel;
extern std::atomic<bool> updateList;
extern std::vector<std::string> fullPaths;
extern std::string pathToFolder;
extern std::vector<std::string> files;
extern std::vector<std::string> formatedItems;
extern std::string SC_clientID;
extern std::mutex data_mutex;
extern std::vector<std::string> searchList;
extern std::vector<std::string> rawSearchList;
extern std::atomic<bool> osearchRunning;
extern Menuctl menuctl;
