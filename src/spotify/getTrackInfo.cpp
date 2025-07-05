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

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br, zstd");
    headers = curl_slist_append(headers, "Referer: https://spotmate.online/en");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "X-CSRF-TOKEN: YEDtUGqKz7bp3iexVcUd1kvbi2BHlpOLBuPyXjsF");
    headers = curl_slist_append(headers, "Origin: https://spotmate.online");
    headers = curl_slist_append(headers, "Sec-GPC: 1");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "Cookie: XSRF-TOKEN=eyJpdiI6IjQyRWluM0djeWdTaFd4OUlneG16Y0E9PSIsInZhbHVlIjoiSTJUNFdYU0p6Q1paMDdLL05aejJ0WjI4bVZZYzRSR3lUMGdVUU1oYmUxNEMxMFlCdjFhN09vUndzYndnMnhvVHVhNEVucU5QcXRIYnduR2M4RERDMjVhVUh4M2RtSVpjSUo0aGpMd3dQN3hXK0xSYzl5REJ3dTAzNXBvT1BybGQiLCJtYWMiOiIwZDBmYWRkMDJmNDY2YzRkMjMyOWE3NTBiOTUzMjk0YjhiODg1NDc3NDAwODhjNmY5MWU3Y2RkYzFjZDFmZTkyIiwidGFnIjoiIn0%3D; spotmateonline_session=eyJpdiI6IlRBVDNpS3hUWlBOZEc3L0JWb3NKYXc9PSIsInZhbHVlIjoibkx3MHpQb1o3a0EzSkc5bDFVYkMvYm9lZ0ZaVk9hcjV3bkx2RmpFQTNQbGlPODdzWXpNY0pQM1NVN0Eyb0NSTU9lTVFnSTJJUXFOVENhQ0NJUWxKaENXZGJ5TXVwY0lDa09NdEU2dG9oWnVXM2tFZUdXck9seXAxN0tMR3Y0clYiLCJtYWMiOiIzOTZkZGI0YzgzOTRiZWU1OGI0ZWY4NGE0ZWZhOTA5NzEzODJmNmU5ZjU4MmE5YjcwZGJhOGI0MDU5YTM3NGM2IiwidGFnIjoiIn0%3D");
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