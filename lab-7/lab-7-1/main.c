#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/* argv[1] - первый файл, argv[2] - второй, argv[3] - выходной */
int main(int argc, char* argv[]) {
    FILE* file = NULL;
    int fd[2]; /* fd[0] - для чтения из канала, fd[1] - для записи в канал */
    pid_t pid_1, pid_2;
    char ch;

    assert(argc == 4);

    if (pipe(fd) == -1) {
        perror("open pipe");
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[3], "w")) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    if (!(pid_1 = fork())) {
        close(fd[0]);
        execl("exec", "exec", &fd[1], argv[1], NULL);
    } else {
        if (!(pid_2 = fork())) {
            close(fd[0]);
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
    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);

    close(fd[0]);
    close(fd[1]);
    fclose(file);
}