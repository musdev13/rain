#include <string>
#include <nlohmann/json.hpp>
#include "../filefs.hpp"

using json = nlohmann::json;

std::string clearYTMURL(const std::string& url);
void getYTMTrackInfo(const std::string& videoID, std::string& title, std::string& artist);
void getYTMTrack(const std::string& videoID, const std::string& outputFile);
void addYTMTrack(std::string ytmURL);
