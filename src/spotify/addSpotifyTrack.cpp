#include "../mus.h/spotify/addSpotifyTrack.hpp"

std::string extractSpotifyID(const std::string& url) {
    const std::string prefix = "https://open.spotify.com/track/";
    size_t start = url.find(prefix);
    if (start == std::string::npos) return "";

    start += prefix.length();
    size_t end = url.find('?', start);

    return url.substr(start, end - start);
}

void addSpotifyTrack(std::string spotifyUrl){
    // system(("notify-send \""+spotifyUrl+"\"").c_str());
    std::string trackID = extractSpotifyID(spotifyUrl);
    std::string rainCommand = "@rain:spotify\\"+trackID;

    insertFirstLine(pathToFolder+"/playlist", rainCommand);
}
