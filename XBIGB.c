#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

    unsigned int magic;
    unsigned int dumpUC00;
    unsigned int dumpUC01;
    unsigned int dump00;
    unsigned int dump01;
    unsigned int dumpUCmstr;
    unsigned int start;
    unsigned int zerofill1;
    unsigned int zerofill2;
    char *buffer;
    FILE *f1;
    FILE *f2;

    if (argc != 2 ) {
        printf("Usage: XBIGB /path/to/file\n");
        return(9);
    }

    printf("Input first segment address\n");
    scanf("%x", &start);

    printf("Input first uncompressed segment address\n");
    scanf("%x", &zerofill1);

    printf("Input second uncompressed segment address\n");
    scanf("%x", &zerofill2);

    f1 = fopen(argv[1],"rb");

    if (f1 == NULL) {
        printf("Could not open BIGB archive!\n");
        return(1);
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
    fread(&dump01, sizeof(unsigned int), 1, f1);
    fseek(f1, 0x88, SEEK_SET);
    fread(&dumpUCmstr, sizeof(unsigned int), 1, f1);

    fseek(f1, start, SEEK_SET);

    buffer = (char*) malloc (sizeof(char) *dump00);
    if (buffer == NULL) {
        printf("Copy failed at segment00!\n");
        fclose(f1);
        return(2);

    }
    fread(buffer, 1, dump00, f1);


    f2 = fopen("dump00.bin","wb");
    fwrite(buffer, 1, dump00, f2);
    free(buffer);
    fclose(f2);

    buffer = (char*) malloc (sizeof(char) *dump01);
    if (buffer == NULL) {
        printf("Copy failed at segment01!\n");
        fclose(f1);
        return(4);

    }
    fread(buffer, 1, dump01, f1);


    f2 = fopen("dump01.bin","wb");
    fwrite(buffer, 1, dump01, f2);
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

    fseek(f1, zerofill1, SEEK_SET);

    if (dumpUC00 == 0x00000000 ) {
        printf("Missing segmentUC00 and segmentUC01, should be fine.");
        return(8);
    }

    else    {
        buffer = (char*) malloc (sizeof(char) *dumpUC00);
        if (buffer == NULL) {
            printf("Copy failed at segmentUC00!\n");
            fclose(f1);
            return(5);

        }

        fread(buffer, 1, dumpUC00, f1);


        f2 = fopen("dumpUC00.bin","wb");
        fwrite(buffer, 1, dumpUC00, f2);
        free(buffer);
        fclose(f2);
    }

fseek(f1, zerofill2, SEEK_SET);

    if (dumpUC01 == 0x0000000 ) {
        printf("Missing segmentUC01, should be fine.");
        return(7);
    }

    else    {


        buffer = (char*) malloc (sizeof(char) *dumpUC01);
        if (buffer == NULL) {
            printf("Copy failed at segmentUC01!\n");
            fclose(f1);
            return(6);

        }

        fread(buffer, 1, dumpUC01, f1);


        f2 = fopen("dumpUC01.bin","wb");
        fwrite(buffer, 1, dumpUC01, f2);
        free(buffer);
        fclose(f2);
    }

    return(0);
}
