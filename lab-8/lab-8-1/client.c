#include "func.c"

int init_sleep(void);
void signal_handler(int);

/* argv[1] - время ожидания в сек */
int main(int argc, char* argv[]) {
    //key_t key;
    int id_msg;

    /* Параметры сообщения */
    msgbuf buf;

    /* Установка реакции на сигнал от сервера */
    signal(SIGINT, signal_handler);

    //key = ftok("./", 'a');

    /* Создание очереди с флагами
        0644 - чтение/запись для владельца и чтение для членов группы и прочих пользователей
        IPC_CREAT, IPC_EXCL - проверка на наличие очереди с тем же ключом
    */
    if ((id_msg = msgget(KEY, (0644 | IPC_CREAT | IPC_EXCL))) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    for (int timer = init_sleep(); timer > 0; --timer) {
        printf("Ожидание сообщения %d сек\n", timer);
        /* Сообщение серверу о запуске клиента */
        send_message(id_msg, &buf, 1, "");
        /* Чтение сообщения от сервера */
        read_message(id_msg, &buf, 2);
        sleep(1);
    }

    /* Уничтожение очереди */
    msgctl(id_msg, IPC_RMID, 0);

    exit(EXIT_SUCCESS);
}

int init_sleep() {
    int timer;
    printf("Введите время ожидания (в сек): ");
    scanf("%d", &timer);
    return timer;
}

void signal_handler(int signum) {
    signal(SIGINT, signal_handler);
}