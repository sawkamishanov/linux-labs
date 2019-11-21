#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void out_thread_attr(const char*);
void* routine_start(void*);

int main() {
    FILE* file = NULL;
    pthread_t thread;
    struct sched_param param;
    pthread_attr_t attr;

    if ((file = fopen("test.txt", "r")) == NULL) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    pthread_attr_init(&attr);
    pthread_create(&thread, &attr, &routine_start, (void*)file);
    pthread_join(thread, NULL);
    out_thread_attr("[Параметры главного потока]:");
    if (file->_fileno != -1) {
        printf("Главный поток: созданный поток не закрыл файл. Попытка закрыть...\n");
        if (fclose(file) == 0) {
            printf("Главный поток: файл закрыт\n");
        }
    }

    return EXIT_SUCCESS;
}

void out_thread_attr(const char* msg) {
    int policy;
    struct sched_param param;
    char* spolicy = NULL;

    printf("%s\n", msg);
    pthread_getschedparam(pthread_self(), &policy, &param);

    switch (policy)
    {
    case SCHED_OTHER:
        spolicy = "SCHED_OTHER";
        break;
    case SCHED_FIFO:
        spolicy = "SCHED_FIFO";
        break;
    case SCHED_RR:
        spolicy = "SCHED_RR";
        break;
    default:
        break;
    }

    printf("Класс планирования: %s\n Приоритет: %d\n Мин. приоритет: %d\n Макс. приоритет: %d\n", 
        spolicy, param.sched_priority, sched_get_priority_max(policy), sched_get_priority_min(policy));
}

void* routine_start(void* arg) {
    out_thread_attr("[Параметры нового потока]:");
    FILE* file = (FILE*) arg;
    char buf[256];
    fread(buf, sizeof(char), sizeof(buf), file);
    printf("Содержимое файла:\n%s", buf);
    if (fclose(file) == 0) {
        printf("Созданный поток закрыл файл");
    }
    pthread_exit(0);
}