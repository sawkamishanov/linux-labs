#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void signal_handler(int);

int main(int argc, char* argv[])
{
    FILE* file = NULL;
    int read_pipe_fd = *argv[1];          /* номер дескриптора файла для чтения из канала */
    char ch;

    /* Установка сигналов */
    struct sigaction act;
    int sig;

    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGUSR1);

    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    /* Блокировка сигналов */
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);

    if ((file = fopen(argv[2], "a")) == NULL) {
        perror("open file");
    }

    printf("1 процесс готовится читать данные из буфера\n");
    while (read(read_pipe_fd, &ch, 1) > 0) {
        fputc(ch, file);
        kill(0, SIGUSR2);
        sigwait(&act.sa_mask, &sig);
        printf("1 процесс получил сигнал SIGUSR1\n");
    }
    sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL);

    fclose(file);
    exit(EXIT_SUCCESS);
}

void signal_handler(int signum) {}