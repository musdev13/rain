#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "../filefs.hpp"
#include <thread>
#include "../soundcloud/soundcloud.hpp"
#include "../spotify/addSpotifyTrack.hpp"

using json = nlohmann::json;

void osearchM(std::thread& osearch, std::string& searchInputContent);

void osearchSoundcloudTrack(std::string formatedName);
void osearchSpotifyTrack(std::string formatedName);

void osearchd(const std::string formatedName);
