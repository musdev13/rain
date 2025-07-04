#pragma once
#include <vector>
#include <mpv/client.h>
#include <iostream>
// #include <fstream>
#include "atomic_vars.hpp"


void event_loop(mpv_handle* mpv, const std::vector<std::string>& playlist);