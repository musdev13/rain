#include "../mus.h/spotify/getTrackInfo.hpp"

using json = nlohmann::json;

void getTrackInfo(const std::string& trackID, std::string& title, std::string& artist) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return;
    }

    std::string response_string;
    std::string spotifyUrl = "https://open.spotify.com/track/"+trackID;
    std::string data = R"({"spotify_url": ")" + spotifyUrl + R"("})";
    Tokens t = getTokens();
    

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br, zstd");
    headers = curl_slist_append(headers, "Referer: https://spotmate.online/en");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("X-CSRF-TOKEN: "+t.csrf).c_str());
    headers = curl_slist_append(headers, "Origin: https://spotmate.online");
    headers = curl_slist_append(headers, "Sec-GPC: 1");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, ("Cookie: XSRF-TOKEN="+t.xsrf+"; spotmateonline_session="+t.session).c_str());
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Priority: u=0");
    headers = curl_slist_append(headers, "TE: trailers");

    curl_easy_setopt(curl, CURLOPT_URL, "https://spotmate.online/getTrackData");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

    CURLcode res = curl_easy_perform(curl);
    std::cout << response_string << std::endl;
    json j = json::parse(response_string);
    std::string titleTemp = j["name"];

    std::string artistTemp;
    for (size_t i = 0; i < j["artists"].size(); ++i) {
        if (i > 0) artistTemp += "/";
        artistTemp += j["artists"][i]["name"].get<std::string>();
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    title = titleTemp;
    artist=artistTemp;
}