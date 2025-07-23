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

    CURLcode res = curl_easy_perform(curl);
    json j = json::parse(response);

    // debug(j["collection"][0]["permalink_url"]);
    for (auto el:j["collection"]){
        std::string title = el.value("title", "");
        std::string username = el["user"].value("username", "");
        searchList.push_back(title + " - " + username + " 󰓀");
    }

    curl_easy_cleanup(curl);
}
