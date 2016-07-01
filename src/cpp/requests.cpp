#include <map>
#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include "requests.hpp"

#include <iostream>

using namespace std;

Requests::Requests()
{
    CURLcode r = curl_global_init(CURL_GLOBAL_ALL);
    if (r != 0) {
        // TODO: report errbuf
        throw runtime_error("curl_global_init() failed\n");
    }

    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errbuf);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_string);
}

Requests::~Requests()
{
    curl_easy_cleanup(curl_handle);
}

string Requests::request_url(const string& url)
{
    string response;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);
    CURLcode r = curl_easy_perform(curl_handle);
    if (r != 0) {
        // TODO: report errbuf
        throw runtime_error("problem issuing request for " + url);
    }
    return response;
}

size_t Requests::write_string(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    string *response = (string *)userdata;
    *response += string(ptr, realsize);
    return realsize;
}

/* static methods */
map<string, string> Requests::parse_qs(const string& qs)
{
    map<string, string> m;
 /* this experiences a bug with the regex implementation in libstdc++
  * works fine with libc++
  *
  * regex match_var("([\\w+%]+)=([^&]*)", regex::optimize);
  * auto words_begin = sregex_iterator(qs.begin(), qs.end(), match_var);
  * auto words_end = sregex_iterator();
  *
  * for (sregex_iterator i = words_begin; i != words_end; i++) {
  *     smatch match = *i;
  *     cout << match[1].str() << "=" << match[2].str() << endl ;
  * }
  */
}

string Requests::parse_qs(const string& key, const string& qs)
{
    return "not implemented yet, lol";
}
