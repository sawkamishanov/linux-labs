#include "func.c"

/* argv[1] - количество строк, argv[2] - период записи */
int main(int argc, char* argv[]) {
    void* shmaddr = NULL; /* Указатель на виртуальный адрес */
    int count_str, period;

    if (argc != 3) {
        exit(EXIT_FAILURE);
    } else {
        count_str = atoi(argv[1]);
        period = atoi(argv[2]);
    }

    if (id_shm == -1) {
        id_shm = req_shm();
    }
    shmaddr = in_shm(id_shm);

    shared* shared_var = (shared*)shmaddr;
    while (1) {
        /* Период записи */
        sleep(period);

        printf("[Первая программа] -> Процесс готовится ко входу в критическую секцию\n");
        lock(shared_var, FIRST);

        /* Критическая секция */
        if (count_str > 0) {
            open_file();
            fputs("Первая программа\n", file);
            close_file();
            --count_str;
        } else {
            ++(shared_var->stop);
            unlock(shared_var, FIRST);
            break;
        }

        unlock(shared_var, FIRST);
        printf("[Первая программа] -> Процесс вышел из критической секции\n");
    }

    if (shared_var->stop == 3) {
        close_file();
        und_shm(shmaddr);
        dest_shm(id_shm);
    }

    return EXIT_SUCCESS;
}