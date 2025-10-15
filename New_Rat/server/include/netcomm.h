#pragma once

#include <stdlib.h>
#include <stdint.h>

int recv_expected(int sock, size_t expected_len, uint8_t *dest, size_t dest_size);
int send_all(int sock, const uint8_t *data, size_t data_len);