#include <stdio.h>
#include <stdlib.h>
#include "stuff.h"

int main(int argc, char **argv)
{
    FILE *f1;
    FILE *f2;

    if (argc != 2 ) {
        printf("Usage: XBIGB /path/to/file\n");
        return(1);
    }

    f1 = fopen(argv[1],"rb");

    if (f1 == NULL) {
        printf("Could not open BIGB archive!\n");
        return(2);
    }

    fseek(f1, 0x00, SEEK_SET);
    fread(&magic, sizeof(unsigned int), 1, f1);

    if (magic != 0x42474942 ) {
        printf("Missing BIGB header!\n");
        return(3);
    }

    fseek(f1, 0x78, SEEK_SET);
    fread(&dumpUC00, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x7c, SEEK_SET);
    fread(&dumpUC01, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x80, SEEK_SET);
    fread(&dump00, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x84, SEEK_SET);
    fread(&dumpUCmstr, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x88, SEEK_SET);
    fread(&dump01, sizeof(unsigned int), 1, f1);

    printf("Input first segment address\n");
    scanf("%x", &start);

    if (dumpUC00 == 0x00000000 ) {
        printf("Missing segmentUC00, should be fine\n");
    }

    else {
        printf("Input first UC segment address (1 to skip)\n");
        scanf("%x", &UCsegment1);
    }

    if (dumpUC01 == 0x0000000 ) {
        printf("Missing segmentUC01, should be fine\n");
    }

    else {
        printf("Input second UC segment address (1 to skip)\n");
        scanf("%x", &UCsegment2);
    }

    fseek(f1, start, SEEK_SET);

    buffer = (char*) malloc (sizeof(char) *dump00);
    if (buffer == NULL) {
        printf("Copy failed at segment00!\n");
        fclose(f1);
        return(4);

    }

    fread(buffer, 1, dump00, f1);


    f2 = fopen("dump00.bin","wb");
    fwrite(buffer, 1, dump00, f2);
    free(buffer);
    fclose(f2);

    buffer = (char*) malloc (sizeof(char) *dumpUCmstr);
    if (buffer == NULL) {
        printf("Copy failed at segmentUCmstr!\n");
        fclose(f1);
        return(5);

    }

    fread(buffer, 1, dumpUCmstr, f1);


    f2 = fopen("dumpUCmstr.bin","wb");
    fwrite(buffer, 1, dumpUCmstr, f2);
    free(buffer);
    fclose(f2);

    buffer = (char*) malloc (sizeof(char) *dump01);
    if (buffer == NULL) {
        printf("Copy failed at segment01!\n");
        fclose(f1);
        return(6);

    }

    fread(buffer, 1, dump01, f1);


    f2 = fopen("dump01.bin","wb");
    fwrite(buffer, 1, dump01, f2);
    free(buffer);
    fclose(f2);

    if (UCsegment1 == 1) {
        printf("Skipping segmentUC00\n");
    }

    else {

        if (dumpUC00 == 0x00000000 ) {
            printf("SegmentUC00 not copied\n");
        }

        else {
            fseek(f1, UCsegment1, SEEK_SET);

            buffer = (char*) malloc (sizeof(char) *dumpUC00);
            if (buffer == NULL) {
                printf("Copy failed at segmentUC00!\n");
                fclose(f1);
                return(7);

            }

            fread(buffer, 1, dumpUC00, f1);


            f2 = fopen("dumpUC00.bin","wb");
            fwrite(buffer, 1, dumpUC00, f2);
            free(buffer);
            fclose(f2);
        }

    }
    if (UCsegment2 == 1) {
        printf("Skipping segmentUC01\n");
    }

    else {

        if (dumpUC01 == 0x00000000 ) {
            printf("SegmentUC01 not copied\n");
        }
        else {
            fseek(f1, UCsegment2, SEEK_SET);

            buffer = (char*) malloc (sizeof(char) *dumpUC01);
            if (buffer == NULL) {
                printf("Copy failed at segmentUC01!\n");
                fclose(f1);
                return(8);

            }

            fread(buffer, 1, dumpUC01, f1);


            f2 = fopen("dumpUC01.bin","wb");
            fwrite(buffer, 1, dumpUC01, f2);
            free(buffer);
            fclose(f2);
        }
    }

    return(0);
}
