#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TEXT_PATH "text.txt"
#define OUT_1_TEXT_PATH "thread_1.txt"
#define OUT_2_TEXT_PATH "thread_2.txt"
#define SIZE 256

typedef struct routine_argument{
    FILE* file;
    char* estr;
} routine_arg;

void* start_routine(void* arg);

int main(){
    FILE* text_file = NULL;
    pthread_t thread_1, thread_2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* Параметры потока */
    routine_arg arg_1, arg_2;

    if ((text_file = fopen(TEXT_PATH, "r")) == NULL ||
        (arg_1.file = fopen(OUT_1_TEXT_PATH, "w")) == NULL ||
        (arg_2.file = fopen(OUT_2_TEXT_PATH, "w")) == NULL) {
        perror("open file");
    }

    char str_1[SIZE], str_2[SIZE];
    while(1) {
        if (arg_1.estr = fgets(str_1, sizeof(str_1), text_file)) {
            pthread_create(&thread_1, &attr, &start_routine, (void*)&arg_1);
        }
        if (arg_2.estr = fgets(str_2, sizeof(str_2), text_file)) {
            pthread_create(&thread_2, &attr, &start_routine, (void*)&arg_2);
        }

        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);

        /* Проверка на конец файла или ошибку чтения */
        if (arg_1.estr == NULL || arg_2.estr == NULL) {
            if (feof(text_file) != 0) {
                printf("\nЧтение файла закончено\n");
                break;
            } else {
                printf ("\nОшибка чтения из файла\n");
                break;
            }
        }
    }

    fclose(text_file);
    fclose(arg_1.file);
    fclose(arg_2.file);

    return EXIT_SUCCESS;
}

void* start_routine(void* temp_arg){
    routine_arg* arg = (routine_arg*)temp_arg;
    if (fputs(arg->estr, arg->file) == EOF) {
        perror("write to file");
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}