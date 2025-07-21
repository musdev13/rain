#include "../mus.h/ytm/ytm.hpp"

std::string clearYTMURL(const std::string& url) {
    std::size_t v_pos = url.find("v=");
    if (v_pos == std::string::npos)
        return "";

    std::size_t id_start = v_pos + 2;
    std::size_t id_end = url.find('&', id_start);

    if (id_end == std::string::npos)
        return url.substr(id_start);
    else
        return url.substr(id_start, id_end - id_start);
}

void getYTMTrackInfo(const std::string& videoID, std::string& title, std::string& artist) {
    std::string url = "https://music.youtube.com/watch?v=" + videoID;
    std::string cmd_info = "yt-dlp -j \"" + url + "\"";

    FILE* pipe = popen(cmd_info.c_str(), "r");

    std::stringstream ss;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        ss << buffer;
    }
    pclose(pipe);

    json j = json::parse(ss.str());
    title = j.value("title", "No title");
    artist = j.value("uploader", "No artist");
}

void getYTMTrack(const std::string& videoID, const std::string& outputFile) {
    std::string url = "https://music.youtube.com/watch?v=" + videoID;

    std::string cmd = "yt-dlp -x --audio-format mp3 -o \"" + outputFile + "\" \"" + url + "\"";

    int ret = system(cmd.c_str());
}

void addYTMTrack(std::string ytmURL){
    std::string trackID = clearYTMURL(ytmURL);
    std::string rainCommand = "@rain:ytm\\"+trackID;
    insertFirstLine(pathToFolder+"/playlist", rainCommand);
}
