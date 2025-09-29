#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#define ERROR -1

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (argc != 2)
    {
        printf("Usage: %s <exe_file>\n", argv[0]);
        return ERROR;
    }

    printf("Argv1 recvd: %s\n", argv[1]);
    FILE *file = fopen(argv[1], "rb");
    if (NULL == file)
    {
        printf("Failed to open file\n");
        return ERROR;
    }

    IMAGE_DOS_HEADER *dos_header = malloc(sizeof(IMAGE_DOS_HEADER));
    if (NULL == dos_header)
    {
        printf("Malloc failed.\n");
        return ERROR;
    }

    fread(dos_header, sizeof(IMAGE_DOS_HEADER), 1, file);
    printf("Magic #: %d\n", dos_header->e_magic);
    printf("Hexadecimal: 0x%X\n", dos_header->e_magic);

    IMAGE_NT_HEADERS64 *file_header = malloc(sizeof(IMAGE_NT_HEADERS64));
    if (NULL == file)
    {
        printf("Malloc failed.\n");
        return ERROR;
    }

    fread(file_header, sizeof(IMAGE_FILE_HEADER), 1, file);
    printf("Signature: %ld\n", file_header->Signature);
    printf("Hexadecimal: 0x%lX\n", file_header->Signature);
}

void machine_type_converter(IMAGE_DOS_HEADER dos_header)
{

    // Example: Converting Machine type (WORD) from little-endian to host endian
    WORD machine = dos_header.e_magic;
    machine = ntohs(machine); // Convert from network byte order (big-endian) to host byte order
    printf("Machine Type: 0x%x\n", machine);
}
