#include<stdio.h>
#include"udp_server.h"

#define PORT 3478

void on_message_received(void* this, Client client) {
    printf("Client message: %s", client.message);
}

int main(int argc, char *argv[]) {

    UDPServer server = udp_server_create(PORT);
    server.on_message_received = &on_message_received;
    
    udp_server_listen(&server);
    return 0;
}