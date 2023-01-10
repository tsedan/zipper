#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int server, client;
socklen_t inet_len;
struct sockaddr_in saddr, caddr;

int main() {
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3333);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    server = socket(PF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        printf("Error on socket creation\n");
        return -1;
    }

    if (bind(server, (struct sockaddr*)&caddr, sizeof(caddr)) == -1) {
        printf("Error on socket bind\n");
        return -1;
    }

    if (listen(server, 5) == -1) {
        printf("Error on socket listen\n");
        return -1;
    }

    while (true) {
        printf("Waiting for connection\n");

        inet_len = sizeof(caddr);
        if ((client = accept(server, (struct sockaddr*)&caddr, &inet_len)) == -1) {
            printf("Error on client accept\n");
            close(server);
            return 0;
        }

        printf("New client connection\n");
    }
}
