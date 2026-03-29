#include "udp_client.h"
#include "utils/string.h"
#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_LEN 1024
#define STUN_SERVER_IP "187.127.2.81"
#define STUN_SERVER_PORT 3478


void stun_request(UDPClient *client) {
    struct sockaddr_in stun_server = get_ip_addr(STUN_SERVER_IP, STUN_SERVER_PORT);

    char message = '.';
    sendto(client->fd, &message, sizeof(message), 0, (struct sockaddr *)&stun_server, sizeof(stun_server));
}

void receive(UDPClient *client) {
    char buffer[BUFFER_LEN] = {0};

    struct sockaddr_in peer;
    socklen_t peer_len;
    char peer_addr[INET_ADDRSTRLEN];
    
    int size = recvfrom(client->fd, buffer, BUFFER_LEN-1, 0, (struct sockaddr*)&peer, &peer_len);

    if (size < 0) {
        fprintf(stderr, "Failed to received message\n");
        return;
    }
    inet_ntop(AF_INET, &peer.sin_addr, peer_addr, sizeof(peer_addr));
    printf("Received %d bytes from peer %s:%d => '%.*s'\n", size, peer_addr, ntohs(peer.sin_port), size, buffer);
}

void *receive_thread(void *arg) {
    UDPClient *client = (UDPClient *)arg;
    while (1) {
        receive(client);
    }
}

void *stun_thread(void *arg) {
    UDPClient *client = (UDPClient *)arg;
    while (1) {
        stun_request(client);
        sleep(1);
    }
    
}

int main() {
    char data[BUFFER_LEN] = {0};
    
    UDPClient client = {0};
    // client.local_port = 9999;

    if (!udp_client_init(&client)) {
        return 1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, (void*)&client);
    pthread_create(&tid, NULL, &stun_thread, (void*)&client);


    stun_request(&client);

    char candidate_ip[BUFFER_LEN] = {0};
    int candidate_port = 0;
    printf("Insert candidate ip:port\n");
    scanf("%s %d", candidate_ip, &candidate_port);

    struct sockaddr_in candidate_addr = get_ip_addr(candidate_ip, candidate_port);

    printf("Trying to communicate with candidate: %s:%d\n", candidate_ip, candidate_port);

    char *message = "hello from client";

    while (1) {
        sendto(client.fd, message, strlen(message), 0, (struct sockaddr*)&candidate_addr, sizeof(candidate_addr));
        usleep(100 * 1000);
    }
    
    return 0;
}