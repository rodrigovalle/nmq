#ifndef SONG_H_
#define SONG_H_

#include "list.h"

struct song {
    struct list_head queue;
    char video_id[11];
    char *url;
};

struct song *create_song(const char video_id[11])
{
    struct song *s = malloc(sizeof(struct song));
    memcpy(&s->video_id, video_id, 11);
    s->url = NULL;
}

inline void destroy_song(struct song *s)
{
    if (s->url) free(s->url);
    free(s);
}

#endif /* SONG_H_ */
