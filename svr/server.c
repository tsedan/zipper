#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 3333

int server, client;
socklen_t inet_len;
struct sockaddr_in saddr, caddr;

int main() {
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    server = socket(PF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        printf("Error on socket creation\n");
        return 0;
    }

    if (bind(server, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
        printf("Error on socket bind\n");
        return 0;
    }

    if (listen(server, 5) == -1) {
        printf("Error on socket listen\n");
        return 0;
    }

    printf("Listening on port %d...\n", PORT);

    while (true) {
        inet_len = sizeof(caddr);
        if ((client = accept(server, (struct sockaddr*)&caddr, &inet_len)) == -1) {
            printf("Error on client accept\n");
            close(server);
            return 0;
        }

        printf("New client connection\n");
    }
}
