#include <stdio.h>
#include <stdlib.h>
#include "stuff.h"

void parsechunk(unsigned int chunk, char outfile[255]) {
    printf("Dumping %s\n", outfile);
    buffer = (char*) malloc (sizeof(char) *chunk);
    fread(buffer, 1, chunk, f1);
    f2 = fopen(outfile,"wb");
    fwrite(buffer, 1, chunk, f2);
    free(buffer);
    fclose(f2);


}

int main(int argc, char **argv)
{

    if (argc != 2 ) {
        printf("Usage: XBIGB /path/to/file\n");
        return(1);
    }

    f1 = fopen(argv[1],"rb");

    if (f1 == NULL) {
        printf("Could not open BIGB archive!\n");
        return(1);
    }

    fread(&magic, sizeof(unsigned int), 1, f1);

    if (magic != 0x42474942 ) {
        printf("Missing BIGB header!\n");
        return(1);
    }

    fread(&start, sizeof(unsigned int), 1, f1);

    start += 0x0C;

    fseek(f1, 0x78, SEEK_SET);
    fread(&dumpUC00, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x7c, SEEK_SET);
    fread(&dumpUC01, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x80, SEEK_SET);
    fread(&dump00, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x84, SEEK_SET);
    fread(&dump01, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x88, SEEK_SET);
    fread(&dump02, sizeof(unsigned int), 1, f1);

    fseek(f1, start, SEEK_SET);

    parsechunk(dump00,"dump00.bin");
    parsechunk(dump01,"dump01.bin");
    parsechunk(dump02,"dump02.bin");


    if (dumpUC00 != 0x00) {

        byte = 0x00;

        while (byte == 0x00)
        {
            byte = fgetc(f1);
        }

        fseek(f1, -0x01, SEEK_CUR);
        parsechunk(dumpUC00,"dumpUC00.bin");

    }

    if (dumpUC01 != 0x00) {

        byte = 0x00;

        while (byte == 0x00)
        {
            byte = fgetc(f1);
        }

        fseek(f1, -0x01, SEEK_CUR);
        parsechunk(dumpUC01,"dumpUC01.bin");
    }
    return(0);
}
