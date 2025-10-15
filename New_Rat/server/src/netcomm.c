#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

// Function to send all data, returns total bits on success, -1 on failure
int send_all(int sock, const uint8_t *data, size_t data_len)
{
    size_t total_sent_bits = 0;
    while (total_sent_bits < data_len)
    {
        ssize_t sent_bits = send(sock, data + total_sent_bits, data_len - total_sent_bits, 0);
        if (-1 == sent_bits)
        {
            return -1;
        }
        total_sent_bits += sent_bits;
    }
    return 0;
}

// Function to receive expected data, returns a pointer to the received data buffer
int recv_expected(int sock, size_t expected_len, uint8_t *dest, size_t dest_size)
{
    if (NULL == dest)
    {
        perror("Destination void");
        return -1;
    }

    size_t recvd_len = 0;
    while (recvd_len < expected_len && recvd_len < dest_size)
    {
        ssize_t received = recv(sock, dest + recvd_len, expected_len - recvd_len, 0);
        if (-1 == received)
        {
            return -1;
        }
        recvd_len += received;
    }
    return recvd_len;
}