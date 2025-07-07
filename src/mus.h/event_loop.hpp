#pragma once
#include <vector>
#include <mpv/client.h>
#include <iostream>
// #include <fstream>
#include "atomic_vars.hpp"
#include "filefs.hpp"
#include "spotify/getTrackInfo.hpp"
#include "spotify/getTrack.hpp"


void event_loop(mpv_handle* mpv);