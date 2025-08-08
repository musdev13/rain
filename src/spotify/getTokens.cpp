#include "../mus.h/spotify/getTokens.hpp"

// Для сбора заголовков (чтобы вытянуть куки)
size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
    size_t totalSize = size * nitems;
    std::string header(buffer, totalSize);
    Tokens* tokens = static_cast<Tokens*>(userdata);

    std::regex xsrfRegex(R"(XSRF-TOKEN=([^;]+))");
    std::regex sessionRegex(R"(spotmateonline_session=([^;]+))");

    std::smatch match;
    if (tokens->xsrf.empty() && std::regex_search(header, match, xsrfRegex)) {
        tokens->xsrf = match[1].str();
    }
    if (tokens->session.empty() && std::regex_search(header, match, sessionRegex)) {
        tokens->session = match[1].str();
    }
    return totalSize;
}



// Основная функция получения всех токенов
Tokens getTokens() {
    CURL* curl = curl_easy_init();
    Tokens tokens;
    std::string htmlContent;
    std::string url = "https://spotmate.online/en";

    if (!curl) {
        std::cerr << "Failed to init curl\n";
        return tokens;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &tokens);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);
    #ifdef __WIN32
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
    #endif

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        return tokens;
    }

    curl_easy_cleanup(curl);

    // Парсим csrf-токен из html (<meta name="csrf-token" content="...">)
    std::regex metaRegex(R"(<meta\s+name=["']csrf-token["']\s+content=["']([^"']+)["'])", std::regex::icase);
    std::smatch match;
    if (std::regex_search(htmlContent, match, metaRegex)) {
        tokens.csrf = match[1].str();
    }

    return tokens;
}


