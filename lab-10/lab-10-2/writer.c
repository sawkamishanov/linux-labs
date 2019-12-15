#include "func.c"


int main(int argc, char* argv[]) {
    int id_shm = req_shm();
    void* shmaddr = in_shm(id_shm);
    shared* shvar = (shared*)shmaddr;

    /* Инициализация семафоров */
    if (shvar->init == 0) {
        sem_init(&(shvar->write), 1, 1);
        sem_init(&(shvar->read), 1, 1);
        sem_init(&(shvar->mutex_1), 1, 1);
        sem_init(&(shvar->mutex_2), 1, 1);
        shvar->write_count = 0;
        shvar->read_count = 0;
        shvar->init = 1;
    }

    const char* text = "Писатель PID:%d записал в файл %d раз\n";
    for (int i = 0; i < WRITE_COUNT; ++i) {
        sleep(1);
        sem_wait(&(shvar->mutex_2));
        ++(shvar->write_count);
        if (shvar->write_count == 1) {
            sem_wait(&(shvar->read));
        }
        sem_post(&(shvar->mutex_2));

        /* Запись */
        sem_wait(&(shvar->write));
        open_file("a+");
        printf("[Строка, записываемая в файл]: ");
        printf(text, getpid(), i);
        fprintf(file, text, getpid(), i);
        close_file();
        sem_post(&(shvar->write));

        sem_wait(&(shvar->mutex_2));
        --(shvar->write_count);
        if (shvar->write_count == 0) {
            sem_post(&(shvar->read));
        }
        sem_post(&(shvar->mutex_2));
    }

    dest_shm(id_shm);
    close_file();

    return EXIT_SUCCESS;
}