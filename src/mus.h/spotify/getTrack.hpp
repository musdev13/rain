#pragma once
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "writeCallBack.hpp"
#include "getTokens.hpp"

void getTrack(const std::string &trackID, std::string filePath);
