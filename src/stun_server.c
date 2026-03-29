#include<stdio.h>
#include"udp_server.h"
#include"stun.h"

#define PORT 3478

void on_message_received(void* this, Client client) {
    printf("Client message: %s", client.message);

    StunMessage message = {0};
    
    if (!stun_parse_message(&message, client.message, client.message_len)) {
        printf("Failed to parse client messsage\n");
        return;
    }
}

int main(int argc, char *argv[]) {

    UDPServer server = udp_server_create(PORT);
    server.on_message_received = &on_message_received;
    
    return !udp_server_listen(&server);
}