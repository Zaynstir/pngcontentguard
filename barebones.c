#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE (16 * 1024 * 1024)

int get_big_endian(const char *buf)
{
    return ((unsigned char)buf[0] << 24) |
           ((unsigned char)buf[1] << 16) |
           ((unsigned char)buf[2] << 8) |
           (unsigned char)buf[3];
}

/*
    methods validate() and check_header() exist solely to make sure non-png files don't produce a segfault
*/
void validate(bool val, const char *msg)
{
    if (!val)
    {
        printf("Invalid file: %s\n", msg);
        exit(1);
    }
}

void check_header(const char *buf)
{
    validate((unsigned char)buf[0] == 0x89, "header byte 1");
    validate((unsigned char)buf[1] == 'P', "header byte 2");
    validate((unsigned char)buf[2] == 'N', "header byte 3");
    validate((unsigned char)buf[3] == 'G', "header byte 4");
    validate((unsigned char)buf[4] == 0x0D, "header byte ");
    validate((unsigned char)buf[5] == 0x0A, "header byte ");
    validate((unsigned char)buf[6] == 0x1A, "header byte ");
    validate((unsigned char)buf[7] == 0x0A, "header byte ");
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        char *buf = (char *)malloc(MAX_SIZE);

        //it could make sense to eliminate a lot of this error checking. I can see it both ways
        if (!buf)
        {
            fprintf(stderr, "Couldn't allocate memory\n");
            return 1;
        }

        FILE *f = fopen(argv[1], "r");
        if (!f)
        {
            perror("fopen");
            free(buf);
            return 1;
        }

        int size = fread(buf, 1, MAX_SIZE, f);
        check_header(buf);
        int pos = 8;

        //while could also be pos < size as well I think
        while (buf[pos + 4] != '\0')
        {
            //stores the length of chunk
            //first section in the chunk header
            char lenbuf[4];

            //copies byte by byte
            for (int x = 0; x < 4; x++)
            {
                lenbuf[x] = buf[pos + x];
            }
            //converts the png binary to ints
            int len = get_big_endian(lenbuf);

            char chunkbuf[5];
            for (int x = 0; x < 4; x++)
            {
                chunkbuf[x] = buf[pos + 4 + x];
            }

            //check if first letter is a lowercase (ancillary)
            if (buf[pos + 4] > 96)
            {
                printf("File had ancillary chunks... Dropping file\n");

                //close png file "drop it on the floor"
                fclose(f);
                free(buf);
                return 12;
            }

            pos += 12 + len;
        }
        printf("File OK\n");
        fclose(f);
        free(buf);
        return 0;
    }
    else
    {
        printf("File not provided\n");
        return 1;
    }
}
