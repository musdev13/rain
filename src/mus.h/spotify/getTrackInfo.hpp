#pragma once

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "writeCallBack.hpp"

void getTrackInfo(const std::string& trackID, std::string& title, std::string& artist);