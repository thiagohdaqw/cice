#ifndef __UDP_SERVER_H_INC__
#define __UDP_SERVER_H_INC__

#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MESSAGE_MAX 4 * 1024

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;

    char message[MESSAGE_MAX];
    size_t message_len;
} Client;

typedef struct udp_server_t UDPServer;

typedef struct udp_server_t {
    int port;
    void *this;
    void (*on_message_received)(UDPServer *server, Client client, void* this);

    int fd;
    struct sockaddr_in addr;
} UDPServer;

UDPServer udp_server_create(int port);
bool udp_server_listen(UDPServer *server);

#endif

#ifndef __UDP_SERVER_IMP__
#define __UDP_SERVER_IMP__

UDPServer udp_server_create(int port) {
    UDPServer server = {0};

    server.port = port;
    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server.addr.sin_port = htons(port);

    return server;
}

bool udp_server_listen(UDPServer *server) {
    server->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server->fd < 0) {
        fprintf(stderr, "[SERVER]: failed to open socket");
        return false;
    }

    if (bind(server->fd, (struct sockaddr *)&server->addr,
             sizeof(server->addr)) < 0) {
        fprintf(stderr, "[SERVER]: Failed to bind socket in port %d: %s\n",
                server->port, strerror(errno));
        close(server->fd);
        return false;
    }

    printf("[SERVER]: UDP server listening on port %d\n", server->port);

    Client client = {0};
    char addrstr[INET_ADDRSTRLEN];

    while (true) {
        memset(&client, 0, sizeof(client));
        memset(addrstr, 0, sizeof(addrstr));

        client.addr_len = sizeof(client.addr);
        client.message_len =
            recvfrom(server->fd, client.message, MESSAGE_MAX - 1, 0,
                     (struct sockaddr *)&client.addr, &client.addr_len);

        if (client.message_len < 0) {
            continue;
        }

        inet_ntop(AF_INET, &client.addr.sin_addr, addrstr, sizeof(addrstr));
        printf("[SERVER]: received %d bytes from %s:%d\n", client.message_len,
               addrstr, ntohs(client.addr.sin_port));

        if (server->on_message_received) {
            server->on_message_received(server, client, server->this);
        }
    }

    return true;
}

#endif