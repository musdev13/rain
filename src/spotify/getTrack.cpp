#include "../mus.h/spotify/getTrack.hpp"

#include <string>
#include <curl/curl.h>
#include <fstream>

size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ofstream *outfile = static_cast<std::ofstream *>(userdata);
    size_t totalSize = size * nmemb;
    outfile->write(static_cast<char *>(ptr), totalSize);
    return totalSize;
}

void getTrack(const std::string& trackID, const std::string& filePath, const std::string& title,const std::string& artist) {

    std::string query = title+" - "+artist;
    // debug(query);
    std::string cmd = "yt-dlp -x --audio-format mp3 --audio-quality 0 -o \"" + filePath + "\" \"ytsearch1:" + query + "\"";

    system(cmd.c_str());
}
