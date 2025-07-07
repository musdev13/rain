#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>
#include "writeCallBack.hpp"

struct Tokens {
    std::string xsrf;    // из куки XSRF-TOKEN
    std::string session; // из куки spotmateonline_session
    std::string csrf;    // из <meta name="csrf-token" content="...">
};

Tokens getTokens();
