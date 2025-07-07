#include "../mus.h/spotify/getTrack.hpp"
using json = nlohmann::json;

#include <string>
#include <curl/curl.h>
#include <fstream>

size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ofstream *outfile = static_cast<std::ofstream *>(userdata);
    size_t totalSize = size * nmemb;
    outfile->write(static_cast<char *>(ptr), totalSize);
    return totalSize;
}

void downloadFile(const std::string &url, const std::string &filename) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return;
    }

    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        curl_easy_cleanup(curl);
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outfile);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

    CURLcode res = curl_easy_perform(curl);

    outfile.close();
    curl_easy_cleanup(curl);
}


void getTrack(const std::string &trackID, std::string filePath) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return;
    }

    std::string response_string;
    std::string spotifyUrl = "https://open.spotify.com/track/"+trackID;
    std::string data = R"({"urls":")" + spotifyUrl + R"("})";
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
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:139.0) Gecko/20100101 Firefox/139.0");

    curl_easy_setopt(curl, CURLOPT_URL, "https://spotmate.online/convert");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

    CURLcode res = curl_easy_perform(curl);
    json j = json::parse(response_string);

    downloadFile(j["url"],filePath);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
