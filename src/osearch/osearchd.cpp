#include "../mus.h/osearch/osearch.hpp"

void osearchd(const std::string formatedName){
    searchList.clear();
    rawSearchList.clear();
    if (osearchRunning && SC_clientID != "") {
        osearchSoundcloudTrack(formatedName);
    }
    if(osearchRunning && (SP_clientID != "" && SP_clientSecret != "")){
        osearchSpotifyTrack(formatedName);
    }
    osearchRunning = false;
}
