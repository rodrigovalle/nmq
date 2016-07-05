#include <stdio.h>
#include "song_queue.c"

int main()
{
    char step[11] = "_mDxcDjg9P4";
    push_id("song 1");
    push_id("song 2");
    push_id("song 3");
    print_queue();

    struct song *s;
    for (int i = 0; i < 4; i++) {
        s = pop_id();
        printf("%11s\n", s ? s->video_id : "(null)");
    }
}
