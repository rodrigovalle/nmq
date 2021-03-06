#ifndef REQUESTS_H_
#define REQUESTS_H_

#include <curl/curl.h>

struct DynamicBuffer {
    char *data;
    size_t size;
};

void start_curl();
void stop_curl();

CURLcode request_url(const char *url, struct DynamicBuffer *buf);

#endif /* REQUESTS_H_ */
