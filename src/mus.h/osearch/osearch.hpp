#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "../filefs.hpp"

using json = nlohmann::json;

void osearchSoundcloudTrack(std::string formatedName);
void osearchd(const std::string formatedName);
