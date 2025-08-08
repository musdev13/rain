#include "../mus.h/soundcloud/soundcloud.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string cleanSCURL(const std::string& url) {
    std::string base = "soundcloud.com/";
    size_t pos = url.find(base);
    if (pos == std::string::npos) return "";

    pos += base.length();

    size_t end = url.find('?', pos);
    if (end == std::string::npos)
        end = url.length();

    return url.substr(pos, end - pos);
}


std::string httpGET(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        #ifdef __WIN32
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
        #endif
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

void getSCTrackInfo(const std::string& clientID, const std::string& cleanURL, std::string& title, std::string& artist) {
    std::string api = "https://api-v2.soundcloud.com/resolve?url=https://soundcloud.com/" + cleanURL + "&client_id=" + clientID;
    std::string jsonStr = httpGET(api);

    auto data = json::parse(jsonStr);
    title = data["title"].get<std::string>();
    artist = data["user"]["username"].get<std::string>();
}

void getSCTrack(const std::string& clientID, const std::string& cleanURL, const std::string& outputPath) {
    std::string api = "https://api-v2.soundcloud.com/resolve?url=https://soundcloud.com/" + cleanURL + "&client_id=" + clientID;
    std::string jsonStr = httpGET(api);
    auto data = json::parse(jsonStr);

    std::string progressiveURL;
    for (auto& t : data["media"]["transcodings"]) {
        if (t["format"]["protocol"] == "progressive") {
            progressiveURL = t["url"].get<std::string>();
            break;
        }
    }

    std::string streamData = httpGET(progressiveURL + "?client_id=" + clientID);
    std::string streamURL = json::parse(streamData)["url"].get<std::string>();

    CURL* curl = curl_easy_init();
    if (curl) {
        FILE* file = fopen(outputPath.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, streamURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        #ifdef __WIN32
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
        #endif
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(file);
    }
}

void addSoundcloudTrack(std::string soundcloudURL){
    std::string trackID = cleanSCURL(soundcloudURL);
    std::string rainCommand = "@rain:soundcloud\\"+trackID;
    insertFirstLine(pathToFolder+"/playlist", rainCommand);
}
