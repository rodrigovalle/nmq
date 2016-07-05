#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "song.h"

struct song_queue {
    size_t num;
    struct list_head head;
};
struct song_queue q = {0, LIST_HEAD_INIT(q.head)};


void push_id(const char video_id[11])
{
    struct song *s = create_song(video_id);
    list_add(&s->queue, &q.head);
    q.num++;
}

struct song *pop_id()
{
    if (!list_empty(&q.head)) {
        struct song *s = list_entry(q.head.prev, struct song, queue);
        list_del(q.head.prev);
        q.num--;
        return s;
    }
    return NULL;
}

/* debug function (call from debugger), eg:
 * gdb$ call print_queue() */
#ifdef DEBUG
void print_queue()
{
    printf("queue size: %d\n", q.num);

    struct song *iter;
    list_for_each_entry(iter, &q.head, queue) {
        printf("%11s\n", iter->video_id);
    }
}
#endif
