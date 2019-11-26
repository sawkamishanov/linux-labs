#include "func.c"

int main(int argc, char* argv[]) {
    void* shmaddr_1 = NULL; /* Указатель на виртуальный адрес */
    void* shmaddr_2 = NULL;
    int run_count = RUN_COUNT;

    if (id_shm_1 == -1) {
        id_shm_1 = req_shm(KEY_1);
    }
    if (id_shm_2 == -1) {
        id_shm_2 = req_shm(KEY_2);
    }
    shmaddr_1 = in_shm(id_shm_1);
    shmaddr_2 = in_shm(id_shm_2);

    shared* shvar_1 = (shared*)shmaddr_1;
    shared* shvar_2 = (shared*)shmaddr_2;

    while(1) {
        sleep(2);
        while(shvar_1->lock == 0 && shvar_2->lock == 0);
        printf("[Вторая программа] -> Процесс готовится ко входу в критическую секцию\n");
        if (shvar_1->lock == 0 && shvar_2->lock == 1) {
            /* Критическая секция */
            printf("Массив из первой программы: ");
            for (int i = 0; i < SIZE_BUF; ++i) {
                printf("%d ", shvar_1->number[i]);
                shvar_2->number[i] = shvar_1->number[i] - 1;
            }
            printf("\n");
        }
        shvar_2->lock = 0;
        printf("[Вторая программа] -> Процесс вышел из критической секции\n");
        --run_count;
        if (!run_count) {
            break;
        }
    }

    und_shm(shmaddr_2);
    dest_shm(id_shm_2);

    return EXIT_SUCCESS;
}