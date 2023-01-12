#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 3333

void* client(void* data) {
    int* cd = (int*)data;

    pthread_detach(pthread_self());
    printf("New client %d\n", *cd);
    pthread_exit(NULL);
}

int main() {
    int sd;
    socklen_t inet_len;
    struct sockaddr_in saddr, caddr;

    int rc;
    pthread_t thread_id;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        printf("Error on socket creation\n");
        return 1;
    }

    if (bind(sd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
        printf("Error on socket bind\n");
        return 1;
    }

    if (listen(sd, 5) == -1) {
        printf("Error on socket listen\n");
        return 1;
    }

    printf("Listening on port %d...\n", PORT);

    while (true) {
        inet_len = sizeof(caddr);
        int* cd = malloc(sizeof(int));

        if ((*cd = accept(sd, (struct sockaddr*)&caddr, &inet_len)) == -1) {
            printf("Error on client accept\n");
            close(sd);
            return 1;
        }

        if (rc = pthread_create(&thread_id, NULL, client, (void*)cd)) {
            printf("Error on thread create\n");
            return 1;
        }
    }
}
