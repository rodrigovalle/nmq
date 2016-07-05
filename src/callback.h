#ifndef CALLBACK_H_
#define CALLBACK_H_

struct per_session_data {
    int writeout;
    //uint8_t buf[LWS_PRE + MAX_PAYLOAD];
    //uint32_t len;
    // these are for fragmented messages
    /*int final;
    int continuation;
    int binary;*/
};

int callback_ws(
        struct lws *wsi,
        enum lws_callback_reasons reason,
        void *user,
        void *in,
        size_t len
    );

#endif /* CALLBACK_H_ */
