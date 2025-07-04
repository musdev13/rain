#include "mus.h/atomic_vars.hpp"

std::atomic<bool> running(true);
std::atomic<size_t> current(0);
std::atomic<bool> manual_change(false);
std::atomic<int> selected_global(0);
std::vector<std::string> list ={};
std::atomic<bool> infoPanel(true);