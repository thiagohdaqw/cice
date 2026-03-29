#include "stun.h"
#include "udp_server.h"
#include <stdio.h>

#define PORT 3478

void on_message_received(UDPServer *server, Client client, void *this) {
    // StunMessage message = {0};

    // if (!stun_parse_message(&message, client.message, client.message_len)) {
    //     printf("Failed to parse client messsage\n");
    //     return;
    // }

    char client_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client.addr.sin_addr, client_addr, sizeof(client_addr));

    int client_port = ntohs(client.addr.sin_port);

    char buffer[1024] = {0};
    snprintf(buffer, 1023, "%s %d", client_addr, client_port);

    sendto(server->fd, buffer, strlen(buffer), 0, (struct sockaddr*)&client.addr, client.addr_len);
}

int main(int argc, char *argv[]) {

    UDPServer server = udp_server_create(PORT);
    server.on_message_received = &on_message_received;

    return !udp_server_listen(&server);
}