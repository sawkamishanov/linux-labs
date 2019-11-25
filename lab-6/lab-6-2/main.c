#define _GNU_SOURCE
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

typedef struct init_param {
    int run_counts;         /* Кол-во запусков */
    long tv_sec;            /* Период повторения в сек */
} inparam;

void init_signal(void);
void signal_handler(int);
void init_param(inparam*);
void init_timer(long);

int current_counts = 0;

int main(int argc, char* argv[]) {
    init_signal();

    time_t start, end;
    inparam* param = (inparam*)malloc(sizeof(inparam));
    init_param(param);
    time(&start);
    init_timer(param->tv_sec);
    while(1) {
        pause();
        wait(NULL);
        if (current_counts == param->run_counts) {
            break;
        }
    }
    time(&end);
    printf("Время работы программы: %.2f сек\n", difftime(end, start));

    free(param);
    return EXIT_SUCCESS;
}

/* Установка реакции на сигнал */
void init_signal() {

    /* Установка сигнала на повтор */
    struct sigaction act;
	act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);
}

void init_param(inparam* param) {
    printf("Параметры программы\nКоличество запусков: ");
    scanf("%d", &param->run_counts);
    printf("Период повторения (сек): ");
    scanf("%ld", &param->tv_sec);
}

void init_timer(long tv_sec) {
    struct itimerval value, ovalue;
    value.it_value.tv_sec = tv_sec;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = tv_sec;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);
}

void signal_handler(int signum) {
    if (signum == SIGALRM) {
        if (fork() == 0) {
            time_t start_time = time(NULL);
            printf("\nПроцесс %d\nPID: %d\nДата и время старта: %s\n", current_counts, getpid(), ctime(&start_time));
            exit(EXIT_SUCCESS);
        }
        ++current_counts;
    }
}

