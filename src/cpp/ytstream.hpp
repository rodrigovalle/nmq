#ifndef YTSTREAM_HPP_
#define YTSTREAM_HPP_

#include <map>
#include <string>
#include "requests.hpp"

class YoutubeStreamEngine
{
    public:
        YoutubeStreamEngine() = default;
        std::string get_url(const std::string& video_id);
    private:
        Requests r; // requests object for issuing url requests
        std::map<std::string, std::string> get_info(const std::string& video_id);
};

#endif /* YTSTREAM_HPP_ */
