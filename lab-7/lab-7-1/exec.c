#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

/* argv[1] - дескриптор для записи в канал, argv[2] - имя файла */
int main(int argc, char* argv[])
{
    FILE* file = NULL;
    char buf[BUF_SIZE];

    if ((file = fopen(argv[2], "r")) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, BUF_SIZE, file) != NULL) {
        if (write(*argv[1], buf, strlen(buf)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    exit(EXIT_SUCCESS);
}
