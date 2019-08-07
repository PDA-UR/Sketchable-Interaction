
#include <stdlib.h>
#include <stdio.h>


#include "IOManager.h"

int iomanager_read_file_to_buffer(char* buffer, int* size, const char* filepath)
{
    long length;

    FILE* file = fopen(filepath, "rb");

    if(file)
    {
        fseek(file, 0, SEEK_END);
        *size = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = (char *) malloc(*size);

        if(buffer)
            fread(buffer, 1, *size, file);

        fclose(file);

        return 1;
    }

    return 0;
}