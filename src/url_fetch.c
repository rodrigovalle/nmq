#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include "url_fetch.h"

const char *script_path = "python/get_video_info.py";


/* Start the url fetcher python script.
 *
 * nmq ships with a python script that's used to calculate the url of the
 * highest quality audio stream given a specific youtube video's id. Feed
 * the script an id over its stdin, and it'll print out the url on stdout.
 * Note that, if execlp() failed, upon writing we'll recieve SIGPIPE, and
 * reading from the socket will result in EOF.
 *
 * @script_path  The relative or complete path to the python script.
 * @return A socket file descriptor used to communicate with the subprocess
 *         over its standard IO streams, or -1 on failure.
 */
int fork_url_fetcher(const char *script_path)
{
    int status;
    int sockpair[2];
    pid_t child_pid;

    status = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
    if (status == -1)
        return -1;

    child_pid = fork();
    switch (child_pid) {
        case -1:
            perror("fork");
            return -1;
            break;

        case 0:   /* child  */
            close(sockpair[1]); // close the parent's socket-end
            dup2(sockpair[0], STDOUT_FILENO);
            dup2(sockpair[0], STDIN_FILENO);

            status = execlp("python", "python", script_path, NULL);
            if (status == -1)
                _exit(1); // signal issue by exiting + closing socket
            break;

        default:  /* parent */
            close(sockpair[0]); // close the child's socket-end
            break;
    }

    return sockpair[1]; // this is the parent's socket-end
}


/* Thread routine that communicates with the fetcher script in the background.
 *
 * @args  
 * @return NULL - never returns
 */
void *url_fetcher_thread(void *args)
{
    int sockfd = fork_url_fetcher(script_path);
}
