#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "requests.h"

static CURL *curl;
static char errbuf[CURL_ERROR_SIZE];

static size_t write_buffer(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    struct DynamicBuffer *mem = (struct DynamicBuffer *)userdata;

    mem->data = (char *)realloc(mem->data, mem->size + realsize + 1);
    if (mem->data == NULL) {
        /* out of memory */
        printf("error: realloc() failed\n");
        return 0;
    }

    memcpy(&(mem->data[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    printf("write callback processed: %d bytes\n", realsize);
    return realsize;
}

CURLcode request_url(const char url[], struct DynamicBuffer *buf)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
    CURLcode r = curl_easy_perform(curl);
    return r;
}

void start_curl()
{
    CURLcode r = curl_global_init(CURL_GLOBAL_ALL);
    if (r != 0) {
        printf("curl_global_init() failed\n");
        exit(1);
    }

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buffer);
}

void stop_curl()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

/* EXAMPLE USAGE */
/*
 * int main() {
 *     struct DynamicBuffer buf;
 *     buf.data = malloc(1000);
 *     buf.size = 0;
 * 
 *     start_curl();
 * 
 *     CURLcode r = request_url("http://www.example.com", &buf);
 *     if (r != 0) {
 *         printf("error: %s\n", errbuf);
 *         exit(1);
 *     }
 *     printf("%s\n", buf.data);
 * }
 *
 */
