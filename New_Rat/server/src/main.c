#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/utils.h"
#include "../include/netcomm.h"

int main()
{
    int server_fd, new_socket, status = 0, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (0 == (server_fd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        perror("Socket Failed");
        return ERROR;
    }

    status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    if (-1 == status)
    {
        perror("SetsockOpt Error");
        return ERROR;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (0 > bind(server_fd, (struct sockaddr *)&address, sizeof(address)))
    {
        perror("Bind Failed");
        return ERROR;
    }

    if (0 > listen(server_fd, 5)) // 5 = max conns
    {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        return SUCCESS;
    }

    // Infinite loop to accept multiple connections
    while (1)
    {
        // Accept a new connection
        if (0 > (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
        {
            perror("accept");
            return ERROR;
        }

        printf("Connection accepted.\n");

        const char *hello = "Hello from server";
        status = send_all(new_socket, (uint8_t *)hello, strlen(hello));
        if (-1 == status)
        {
            printf("Send Failed\n");
        }
        printf("Hello message sent.\nWaiting to Recv Packet\n");
        // Buffer for data
        // Null header to recv into and switch on.
        uint8_t *data = NULL;
        p_header_t header = {0};

        while (1)
        {
            switch (receive_packet(new_socket, &header, &data))
            {
            case PING:
                printf("Ping packet recvd.\n");
                p_ping_t *ping_reply = calloc(1, sizeof(ping_reply));
                ping_reply->header.msg_type = PING;

                uint8_t *pingsendbuff = malloc(sizeof(p_header_t));
                memcpy(pingsendbuff, ping_reply, sizeof(p_header_t));
                send_all(new_socket, pingsendbuff, sizeof(p_header_t));
                break;
            case MUL:
                printf("Mul packet recvd.\n");
                p_mul_t *mul_reply = calloc(1, sizeof(p_mul_t));
                mul_reply->header.msg_type = MUL;
                mul_reply->header.p_len = htonl(sizeof(uint32_t));

                uint8_t *mulsendbuff = malloc(sizeof(p_header_t) + sizeof(uint32_t));

                uint32_t received_num = 0, product = 0;
                memcpy(&received_num, data, sizeof(received_num));
                received_num = ntohl(received_num);
                printf("Recvd Num: %d\n", received_num);
                product = received_num * 2;
                printf("Product: %d\n", product);
                mul_reply->num = htonl(product);

                // Serialize header + payload
                memcpy(mulsendbuff, &mul_reply->header, sizeof(p_header_t));
                memcpy(mulsendbuff + sizeof(p_header_t), &mul_reply->num, sizeof(u_int32_t));
                send_all(new_socket, mulsendbuff, sizeof(p_header_t) + htonl(header.p_len));

                break;
            case ECHO:
                printf("Echo packet recvd.\n");
                // calloc 1 element of x size
                p_echo_t *echo_reply = calloc(1, sizeof(echo_reply));
                printf("Data recvd: %s\n", data);
                echo_reply->header.msg_type = ECHO;

                echo_reply->data = malloc(header.p_len);
                echo_reply->header.p_len = header.p_len;
                strcpy((char *)echo_reply->data, (char *)data);

                // SERIALIZE SERIALIZE SERIALIZE
                // Size of header + P_len
                uint8_t *sendbuff = malloc(sizeof(p_header_t) + htonl(header.p_len));
                // add header to buffer
                memcpy(sendbuff, echo_reply, sizeof(p_header_t));
                // increrment pointer past header and add data
                memcpy(sendbuff + sizeof(p_header_t), data, htonl(header.p_len));

                send_all(new_socket, sendbuff, sizeof(p_header_t) + htonl(header.p_len));
                printf("Echo Reply Sent.\n");
                break;
            case SHOW:
                printf("Show packet recvd.\n");
                break;
            default:
                printf("Unknown packet recvd.\n");
                break;
            }
        }

        close(new_socket);
    }
}

int receive_packet(int client_sock, p_header_t *header, uint8_t **dest)
{
    printf("Waiting for Packet.\n");

    if (NULL == header)
    {
        perror("Header Invalid");
        return -1;
    }

    if (NULL == dest)
    {
        perror("Dest Invalid");
        return -1;
    }

    ssize_t bytes = recv_expected(client_sock, sizeof(p_header_t), (uint8_t *)header, sizeof(p_header_t));

    printf("Packet Received.\n");
    if (0 > bytes)
    {
        perror("Receive Failed\n");
        return -1;
    }

    printf("Recvd Packet Type: %d\n", header->msg_type);
    printf("Recvd Packet Len: %d\n", htonl(header->p_len));

    *dest = recv_expected_alloc(client_sock, htonl(header->p_len));

    if (NULL == *dest)
    {
        perror("Malloc Failed");
        return -1;
    }

    return header->msg_type;
}