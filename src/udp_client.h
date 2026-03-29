#ifndef __UDP_CLIENT_H_INCLUDED__
#define __UDP_CLIENT_H_INCLUDED__

#include <stdbool.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
    int fd;
    int local_port;
    struct sockaddr_in local_addr;
} UDPClient;

bool udp_client_init(UDPClient *client);
struct sockaddr_in get_ip_addr(const char *ip, int port);
#endif

#ifndef __UDP_CLIENT_IMP__
#define __UDP_CLIENT_IMP__

bool udp_client_init(UDPClient *client) {
    client->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->fd < 0) {
        fprintf(stderr, "[CLIENT]: Failed to create socket: %s\n", strerror(errno));
        return false;
    }

    if (client->local_port > 0) {
        memset(&client->local_addr, 0, sizeof(client->local_addr));
        client->local_addr.sin_family = AF_INET;
        client->local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        client->local_addr.sin_port = htons(client->local_port);

        if (bind(client->fd, (struct sockaddr *)&client->local_addr, sizeof(client->local_addr)) < 0) {
            fprintf(stderr, "[CLIENT]: Failed to bind socket to port %d: %s\n", client->local_port, strerror(errno));
            close(client->fd);
            return false;
        }
    }

    return true;
}

struct sockaddr_in get_ip_addr(const char *ip, int port) {
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    return addr;
}

#endif