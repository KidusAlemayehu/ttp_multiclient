#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RATE_LIMIT 5
#define TIME_WINDOW 60


typedef struct{
    time_t last_time;
    size_t request_count;
} rate_limiter_t;

typedef struct{
    char ip_addr [16];
    rate_limiter_t limit;
} client_data_t;

client_data_t *client_map = NULL;

void get_client_ip_addr(int client_fd, char *ip_addr){
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(client_fd, (struct sockaddr *)&addr, &addr_len);
    inet_ntop(AF_INET, &addr.sin_addr, ip_addr, sizeof(ip_addr));
}

client_data_t* get_client_info(const char* ip_addr){
    client_data_t* client = malloc(sizeof(client_data_t));
    strcpy(client->ip_addr, ip_addr);
    client->limit.last_time = time(NULL);
    client->limit.request_count = 0;
    return client;
}

int check_rate_limit(const char *ip_addr){
    client_data_t *client = get_client_info(ip_addr);
    time_t now = time(NULL);

    if(difftime(now, client->limit.last_time)>=TIME_WINDOW){
        client->limit.request_count = 0;
    }

    if (client->limit.request_count >= RATE_LIMIT){
        return 0;
    }

    client->limit.request_count++;
    client->limit.last_time = now;
    return 1;
}
