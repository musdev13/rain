#include "mus.h/url.hpp"

std::string url_encode(const std::string& str) {
    std::string encoded = "";
    const char* hex = "0123456789ABCDEF";

    for (unsigned char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else {
            encoded += '%';
            encoded += hex[c >> 4];
            encoded += hex[c & 15];
        }
    }

    return encoded;
}
