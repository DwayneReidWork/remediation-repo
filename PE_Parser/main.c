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

    // Debuggint
    printf("Argv1 recvd: %s\n", argv[1]);

    // Read the bytes of the executable
    FILE *file = fopen(argv[1], "rb");
    if (NULL == file)
    {
        printf("Failed to open file\n");
        return ERROR;
    }
    // FIRST STRUCT
    IMAGE_DOS_HEADER *dos_header = malloc(sizeof(IMAGE_DOS_HEADER));
    if (NULL == dos_header)
    {
        printf("Malloc failed.\n");
        return ERROR;
    }

    fread(dos_header, sizeof(IMAGE_DOS_HEADER), 1, file);
    printf("Magic #: 0x%X\n", ntohs(dos_header->e_magic));

    // Second struct with ptrs to third and 4th
    IMAGE_NT_HEADERS64 *file_header = malloc(sizeof(IMAGE_NT_HEADERS64));
    if (NULL == file_header)
    {
        printf("Malloc failed.\n");
        return ERROR;
    }
    // Find next position based on e_lfanew (offset) from beginning of file
    fseek(file, dos_header->e_lfanew, SEEK_SET);
    // Read from then on
    fread(file_header, sizeof(IMAGE_NT_HEADERS64), 1, file);

    printf("Hex Signature: 0x%X\n", file_header->Signature);
    printf("Machine Type: 0x%X\n", file_header->FileHeader.Machine);
    printf("Num of Sections: %d\n", file_header->FileHeader.NumberOfSections);
    printf("Section Alignment: %d\n", file_header->OptionalHeader.FileAlignment);
    printf("Image Size: %d\n", file_header->OptionalHeader.SizeOfImage);
    return 0;
}