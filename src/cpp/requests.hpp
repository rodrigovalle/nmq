#ifndef REQUESTS_HPP_
#define REQUESTS_HPP_

#include <map>
#include <string>
#include <curl/curl.h>

class Requests
{
    public:
        Requests();
        ~Requests();
        std::string request_url(const std::string& url);

        static std::string parse_qs(const std::string& key, const std::string& qs);
        static std::map<std::string, std::string> parse_qs(const std::string& qs);

    private:
        CURL* curl_handle;
        char errbuf[CURL_ERROR_SIZE];
        static size_t write_string(char *ptr, size_t size, size_t nmemb, void *userdata);
};

#endif /* REQUESTS_HPP_ */
