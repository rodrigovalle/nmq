#include <string>
#include <curl/curl.h>
#include "requests.hpp"
#include "ytstream.hpp"

#include <iostream>

using namespace std;

string YoutubeStreamEngine::get_url(const string& video_id)
{
    YoutubeStreamEngine::get_info(video_id);
    return "sup";
}

map<string, string> YoutubeStreamEngine::get_info(const string& video_id)
{
    string url = 
        "http://www.youtube.com/get_video_info?video_id=" + video_id +
        "&el=detailpage&ps=default&eurl=&gl=US&hl=en";

    cout << "requesting: " << url << endl;
    Requests::parse_qs(r.request_url(url));
    map<string, string> m;
    return m;
}

int main()
{
    // cantina band
    string id = "uJ-NqMHoAa4";
    // step
    //string id = "_mDxcDjg9P4";
    YoutubeStreamEngine y;
    y.get_url(id);
    cout << "done" << endl;
}
