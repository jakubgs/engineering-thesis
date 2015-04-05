#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//printf("TIME:  %-15s - sec = %ld, nanosec = %ld\n",
#define TEST(FUNC, NAME) \
    clock_gettime(CLOCK_MONOTONIC, &tstart); \
    FUNC; \
    clock_gettime(CLOCK_MONOTONIC, &tend); \
    printf("%ld\n", tend.tv_nsec - tstart.tv_nsec); \


int main(int argc, char *argv[]) {
    struct timespec tstart={0,0}, tend={0,0};
    size_t size = 1000;
    size_t bufflen = 256*size;
    int i = 0, rvalue = 0;
    int fp;
    void *buffer;
    char name[50];
    char *filename;

    if (argc != 2) {
        printf("usage: %s FILENAME\n", argv[0]);
        return 0;
    }
    filename = argv[1];
    buffer = malloc(bufflen);

    printf("opening file: %s\n", filename);
    TEST(fp = open(filename, 0), "open");
    if (fp < 0) {
        perror("open");
        return 1;
    }

    for (i = 1; size <= bufflen; size = 1000*pow(2, i++)) {
        if (size > 200000) {
            size = 200000;
        }
        sprintf(name, "read[%zu]", size);
        TEST(read(fp, buffer, size), name);
    }

    TEST(close(fp), "close");
    printf("-----------------\n");

    free(buffer);
    return 0;
}
