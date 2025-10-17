#include <dirent.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/utils.h"
#include "../include/netcomm.h"
#define BASE_PATH "../"

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

    if (0 > (listen(server_fd, 5))) // 5 = max conns
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

        const char *hello = "Welcome To Server";
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
                free(ping_reply);
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
                free(mulsendbuff);
                free(mul_reply);
                break;
            case ECHO:
                printf("Echo packet recvd.\n");
                // calloc 1 element of x size
                p_echo_t *echo_reply = calloc(1, sizeof(p_echo_t));
                // printf("Data recvd: %s\n", data);
                echo_reply->header.msg_type = ECHO;
                int len = htonl(header.p_len);

                echo_reply->data = calloc(len + 1, sizeof(char));
                echo_reply->header.p_len = htonl(len);
                strncpy((char *)echo_reply->data, (char *)data, len);
                echo_reply->data[len] = '\0';

                // SERIALIZE SERIALIZE SERIALIZE
                // Size of header + P_len
                uint8_t *sendbuff = malloc(sizeof(p_header_t) + htonl(header.p_len));
                // add header to buffer
                memcpy(sendbuff, echo_reply, sizeof(p_header_t));
                // increrment pointer past header and add data
                memcpy(sendbuff + sizeof(p_header_t), data, htonl(header.p_len));

                send_all(new_socket, (uint8_t *)sendbuff, sizeof(p_header_t) + len);
                printf("Echo Reply Sent.\n");
                free(echo_reply->data);
                free(echo_reply);
                free(sendbuff);
                break;
            case SHOW:
                printf("Show packet recvd.\n");
                // create base for reply packet
                p_show_t file_list = {0};
                file_list.header.msg_type = SHOW;
                file_list.header.p_len = 0;
                file_list.file_count = 0;

                // Open directory to get files
                struct dirent *ent;
                DIR *dir = opendir(BASE_PATH);
                if (NULL == dir)
                {
                    perror("Error opening directory");
                    return -1;
                }
                // Loop through directory
                while ((ent = readdir(dir)) != NULL)
                {
                    if (ent->d_type == DT_REG)
                    {
                        // realloc lists
                        char **newNames = realloc(file_list.FileName, (file_list.file_count + 1) * sizeof(char *));
                        uint8_t *newLens = realloc(file_list.FileNameLen, (file_list.file_count + 1) * sizeof(uint8_t));

                        if (!newNames || !newLens)
                        {
                            perror("realloc failed");
                            closedir(dir);
                            return -1;
                        }

                        file_list.FileName = newNames;
                        file_list.FileNameLen = newLens;

                        char *temp = strdup(ent->d_name);
                        if (!temp)
                        {
                            perror("strdup failed");
                            closedir(dir);
                            return -1;
                        }

                        file_list.FileName[file_list.file_count] = temp;
                        file_list.FileNameLen[file_list.file_count] = strlen(temp);

                        // p_len increases by (filename length + 1-byte length field)
                        file_list.header.p_len += (strlen(temp) + sizeof(uint8_t));
                        file_list.file_count++;
                    }
                }

                closedir(dir);

                size_t reply_size = sizeof(p_header_t) + file_list.header.p_len;
                uint8_t *show_buff = malloc(reply_size);
                if (!show_buff)
                {
                    perror("malloc failed");
                    return -1;
                }

                // Copy header into buffer (convert fields to network byte order)
                p_header_t net_header = file_list.header;
                net_header.p_len = htonl(file_list.header.p_len);
                memcpy(show_buff, &net_header, sizeof(p_header_t));

                // Serialize filenames
                uint8_t *ptr = show_buff + sizeof(p_header_t);
                for (int i = 0; i < file_list.file_count; ++i)
                {
                    uint8_t name_len = file_list.FileNameLen[i];
                    memcpy(ptr, &name_len, sizeof(uint8_t));
                    ptr += sizeof(uint8_t);
                    memcpy(ptr, file_list.FileName[i], name_len);
                    ptr += name_len;
                }

                printf("List size: %zu\n", reply_size);
                printf("File Count: %d\n", file_list.file_count);

                send_all(new_socket, show_buff, reply_size);

                // Cleanup
                free(show_buff);
                for (int i = 0; i < file_list.file_count; ++i)
                    free(file_list.FileName[i]);
                free(file_list.FileName);
                free(file_list.FileNameLen);

                printf("List Sent!\n");
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