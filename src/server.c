#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "shared.h"

void *client(void *data)
{
    int cd = *(int *)data;
    free(data);

    printf("New client [%d]\n", cd);
    // todo: main loop for client operations

    pthread_exit(NULL);
}

int main()
{
    int sd;
    socklen_t inet_len;
    struct sockaddr_in saddr, caddr;

    int rc;
    pthread_attr_t attr;
    pthread_t thread_id;

    rc = pthread_attr_init(&attr);
    if (rc == -1)
    {
        printf("Error on pthread attribute init\n");
        return 1;
    }

    rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (rc == -1)
    {
        printf("Error on pthread detached state\n");
        return 1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        printf("Error on socket creation\n");
        return 1;
    }

    if (bind(sd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        printf("Error on socket bind\n");
        return 1;
    }

    if (listen(sd, 5) == -1)
    {
        printf("Error on socket listen\n");
        return 1;
    }

    printf("Listening on port %d...\n", PORT);

    while (1)
    {
        inet_len = sizeof(caddr);
        int *cd = malloc(sizeof(int));

        *cd = accept(sd, (struct sockaddr *)&caddr, &inet_len);
        if (*cd == -1)
        {
            printf("Error on client accept\n");
            close(sd);
            return 1;
        }

        rc = pthread_create(&thread_id, &attr, client, (void *)cd);
        if (rc == -1)
        {
            printf("Error on thread create\n");
            return 1;
        }
    }
}
