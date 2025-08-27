#pragma once

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <regex>
#include "writeCallBack.hpp"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

bool getTrackInfo(const std::string& trackID, std::string& title, std::string& artist);
