#pragma once
#include <atomic>
#include <iostream>


extern std::atomic<bool> running;
extern std::atomic<size_t> current;
extern std::atomic<bool> manual_change;
extern std::atomic<int> selected_global;