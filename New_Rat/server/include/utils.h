#pragma once

#define SUCCESS 0
#define ERROR -1

typedef enum __attribute__((__packed__))
{
    PING = 1,
    MUL,
    ECHO,
    SHOW,
} PacketType;

// Packet Type (Header for All)
typedef struct __attribute__((__packed__))
{
    // msg_tpe is specific type of PacketType
    PacketType msg_type;
    uint32_t p_len;
} p_header_t;

typedef struct __attribute__((__packed__))
{
    p_header_t header;
} p_ping_t;

typedef struct __attribute__((__packed__))
{
    p_header_t header;
    uint32_t num;
} p_mul_t;

typedef struct __attribute__((__packed__))
{
    p_header_t header;
    char *uInput;
} p_echo_t;

typedef struct __attribute__((__packed__))
{
    p_header_t header;
} p_show_t;