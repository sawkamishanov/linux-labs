#define _GNU_SOURCE

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#define BUF_SIZE 64

void signal_handler(int);

/* argv[1] - входной файл, argv[2] - второй, argv[3] - третий*/
int main(int argc, char* argv[]) {
    FILE* file = NULL;
    int fd[2]; /* fd[0] - для чтения из канала, fd[1] - для записи в канал */
    pid_t pid_1, pid_2;
    char buf[BUF_SIZE];

    assert(argc == 4);

    /* Установка сигналов */
    struct sigaction act;
    int sig;

    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGUSR1);
    sigaddset(&act.sa_mask, SIGUSR2);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);

    if (pipe2(fd, O_NONBLOCK) == -1) {
        perror("open pipe");
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    if (!(pid_1 = fork())) {
        close(fd[1]);
        execl("exec1", "exec1", &fd[0], argv[2], NULL);
    } else {
        if (!(pid_2 = fork())) {
            close(fd[1]);
            execl("exec2", "exec2", &fd[0], argv[3], NULL);
        }
    }

    while (fgets(buf, BUF_SIZE, file) != NULL) {
        if (write(fd[1], buf, strlen(buf)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    printf("Родительский процесс записал данные в буфер\n");
    kill(0, SIGQUIT);

    /* Ожидание завершения работы потомков */
    waitpid(pid_1, NULL, 0);
    waitpid(pid_2, NULL, 0);

    sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL);
    close(fd[0]);
    close(fd[1]);

    return EXIT_SUCCESS;
}

void signal_handler(int signum) {}