#include <string.h>
#include <libwebsockets.h>
#include "callback.h"

/* static struct lws_extension extensions[] = {
 *    {"permessage-deflate", deflate-callback, deflate-options}
 * }
 */

#define MAX_PAYLOAD 1024

static struct lws_context_creation_info info;
static struct lws_protocols protocols[] = {
    {
        "default",
        callback_ws,
        sizeof(struct per_session_data),  // per_session_data size
        MAX_PAYLOAD,               // rx_buffer_size
    },
    { NULL, NULL, 0 }
};

static inline struct lws_context *get_context() {
    memset(&info, 0, sizeof(info));
    info.port = 8001;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    //info.max_http_header_data = 8190
    //info.max_http_header_pool = 5
    //info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;
    info.ka_time = 30;
    info.ka_probes = 1;
    info.ka_interval = 1;
    info.server_string = "nmq-wss";
	return lws_create_context(&info);
}

int main()
{
    int n = 0;
	//lws_daemonize("/tmp/.nmqd-lock");
    struct lws_context *ctx = get_context();
    if (ctx == NULL)
        return -1;

    while (n >= 0) {
        n = lws_service(ctx, 10);
    }
    
	lws_context_destroy(ctx);
    return 0;
}
