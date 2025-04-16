#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct UdpPacket {
    char buffer[1024];
};

struct UdpPacket receive_and_respond_udp(int port) {
    int sockfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    struct UdpPacket packet;

    memset(&packet, 0, sizeof(packet));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    recvfrom(sockfd, packet.buffer, sizeof(packet.buffer), 0,
             (struct sockaddr*)&clientaddr, &addrlen);

    printf("C received: %s\n", packet.buffer);

    const char* reply = "Hello from C (UDP)";
    sendto(sockfd, reply, strlen(reply), 0,
           (struct sockaddr*)&clientaddr, addrlen);

    close(sockfd);
    return packet;
}
