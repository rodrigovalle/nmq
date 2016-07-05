#ifndef URL_FETCH_H_
#define URL_FETCH_H_

char *url_request[1000];

int fork_url_fetcher(const char *script_path);
void *url_fetcher_thread(void *args);

#endif /* URL_FETCH_H_ */
