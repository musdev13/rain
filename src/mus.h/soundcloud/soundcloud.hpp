#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "../filefs.hpp"

using json = nlohmann::json;

std::string cleanSCURL(const std::string& url);
void getSCTrackInfo(const std::string& clientID, const std::string& cleanURL, std::string& title, std::string& artist);
void getSCTrack(const std::string& clientID, const std::string& cleanURL, const std::string& outputPath);

void addSoundcloudTrack(std::string soundcloudURL);
