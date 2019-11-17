#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* argv[1] - первый файл, argv[2] - второй, argv[3] - выходной */
int main(int argc, char* argv[]) {
    FILE* file = NULL;
    int fd[2]; /* fd[0] - для чтения из канала, fd[1] - для записи в канал */
    char ch;

    if (pipe(fd) == -1) {
        perror("open pipe");
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[3], "w")) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    if (!fork()) {
        execl("exec", "exec", &fd[1], argv[1], NULL);
    } else {
        if (!fork()) {
            execl("exec", "exec", &fd[1], argv[2], NULL);
        }
    }

    /* Закрытие канала на запись */
    close(fd[1]);

    /* Цикл чтения символов из канала */
    while (read(fd[0], &ch, 1) > 0) {
        fputc(ch, file);
    }

    /* Ожидание завершения работы потомков */
    //wait(NULL);

    close(fd[0]);
    fclose(file);
}