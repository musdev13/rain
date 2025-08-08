#include "../mus.h/osearch/osearch.hpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void osearchSoundcloudTrack(std::string formatedName) {
    CURL* curl = curl_easy_init();

    std::string url = "https://api-v2.soundcloud.com/search/tracks?q="+formatedName+"&client_id="+SC_clientID;
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.88.1");
    #ifdef __WIN32
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
    #endif

    CURLcode res = curl_easy_perform(curl);
    // debug(response);
    json j = json::parse(response);

    // debug(j["collection"][0]["permalink_url"]);
    for (auto el:j["collection"]){
        std::string title = el.value("title", "");
        std::string username = el["user"].value("username", "");
        searchList.push_back(title + " - " + username + " 󰓀");
        rawSearchList.push_back("@rain:soundcloud\\"+cleanSCURL(el.value("permalink_url","")));
    }

    curl_easy_cleanup(curl);
}

void osearchSpotifyTrack(std::string formatedName) {
    CURL* curl = curl_easy_init();

    std::string url = "https://dubolt.com/api/search/?query="+formatedName+"&type=track&limit=20";
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.88.1");
    #ifdef __WIN32
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
    #endif

    CURLcode res = curl_easy_perform(curl);
    json j = json::parse(response);

    for (auto el:j["tracks"]["items"]){
        std::string title = el.value("name","");
        std::string artist;
        for (size_t i=0;i<el["artists"].size();i++){
            if (i>0) artist += "/";
            artist += el["artists"][i]["name"].get<std::string>();
        }
        searchList.push_back(title + " - " + artist + " 󰓇");
        rawSearchList.push_back("@rain:spotify\\"+el.value("id",""));
    }

    curl_easy_cleanup(curl);
}
