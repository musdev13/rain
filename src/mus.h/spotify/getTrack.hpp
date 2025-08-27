#pragma once
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <cstdio>
#include <regex>
#include "writeCallBack.hpp"

void getTrack(const std::string& trackID, const std::string& filePath, const std::string& title,const std::string& artist);
