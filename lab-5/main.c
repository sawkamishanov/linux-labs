#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int);

int main(int argc, char* argv[]) {
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);

    int in;
    int test_1;
    int* test_2;
    printf("Введите тип ошибки (1 - операция деления, 2 - неверное использование указателей): ");
    scanf("%d", &in);
    switch (in)
    {
    case 1:
        test_1 = 0/0;
        break;
    case 2:
        test_2 = NULL;
        *test_2 = 555;
        break;
    default:
        break;
    }
}

void signal_handler(int sig) {
    switch (sig)
    {
    case SIGFPE:
        printf("Сигнал: SIGFPE (ошибка при делении) \nКод завершения: 1\n");
        exit(1);
        break;
    case SIGSEGV:
        printf("Сигнал: SIGSEGV (ошибка при неверном использовании указателя) \nКод завершения: 2\n");
        exit(2);
        break;
    default:
        break;
    }
}