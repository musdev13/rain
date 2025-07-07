#pragma once
#include <atomic>
#include <iostream>
#include <vector>
#include <string>


extern std::atomic<bool> running;
extern std::atomic<size_t> current;
extern std::atomic<bool> manual_change;
extern std::atomic<int> selected_global;
extern std::vector<std::string> list;
extern std::atomic<bool> infoPanel;
extern std::atomic<bool> updateList;
extern std::vector<std::string> fullPaths;