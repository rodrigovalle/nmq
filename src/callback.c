#include <string.h>
#include <libwebsockets.h>
#include "callback.h"

int callback_ws(
        struct lws *wsi,
        enum lws_callback_reasons reason,
        void *user,
        void *in,
        size_t len
    )
{
    struct per_session_data *pss = (struct per_session_data *)user;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            pss->writeout = 0;
            break;

        case LWS_CALLBACK_RECEIVE:
            lwsl_notice("rx: %*s\n", len, in);
            //pss->final = lws_is_final_fragment(wsi);
            //pss->binary = lws_frame_is_binary(wsi);
            //memcpy(&pss->buf[LWS_PRE], in, len);
            //pss->len = len;
            pss->writeout = 1;
            lws_rx_flow_control(wsi, 0);
            lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            if (pss->writeout) {
                char buf[LWS_PRE + 5];
                memcpy(&buf[LWS_PRE], "pong!", 5);
                lwsl_notice("tx: %*s\n", 5, "pong!");
                pss->writeout = 0;
                lws_rx_flow_control(wsi, 1);
                lws_write(wsi, &buf[LWS_PRE], 5, LWS_WRITE_TEXT);
            }
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_notice("something bad happened :(\n");
            break;

        case LWS_CALLBACK_CLOSED:
            lwsl_notice("connection closed.\n");
            break;
        
        case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
            lwsl_notice("peer initiated close :(\n");
            break;
    }

    return 0;
}
