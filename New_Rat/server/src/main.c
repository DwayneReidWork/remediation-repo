#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

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
        printf("Hello message sent.\n");
    }
}