#include "../mus.h/spotify/getTrackInfo.hpp"
#include "../mus.h/atomic_vars.hpp"

using json = nlohmann::json;

std::string getSpotifyToken() {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string readBuffer;
    std::string auth = SP_clientID + ":" + SP_clientSecret;
    std::string authBase64;
    {
        BIO* bio, *b64;
        BUF_MEM* bufferPtr;
        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, auth.c_str(), auth.length());
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);
        authBase64.assign(bufferPtr->data, bufferPtr->length);
        BIO_free_all(bio);
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Basic " + authBase64).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) return "";

    try {
        json j = json::parse(readBuffer);
        return j.value("access_token", "");
    } catch (...) {
        return "";
    }
}

bool getTrackInfo(const std::string& trackID, std::string& title, std::string& artist) {
    std::string token = getSpotifyToken();
    if (token.empty()) return false;

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string readBuffer;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, ("https://api.spotify.com/v1/tracks/" + trackID).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) return false;

    try {
        json j = json::parse(readBuffer);
        title = j.value("name", "No title");
        if (j.contains("artists") && !j["artists"].empty())
            artist = j["artists"][0].value("name", "No artist");
        else
            artist = "No artist";
    } catch (...) {
        return false;
    }

    return true;
}
