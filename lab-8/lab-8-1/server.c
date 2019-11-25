#include "func.c"

/* argv[1] - сообщение, отправляемое клиенту */
int main(int argc, char* argv[]) {
    int id_msg;
    msgbuf buf;
    char* msg = "Сообщение от 2-ой программы";
    int timer;

    assert(argc == 2);
    timer = atoi(argv[1]);

    if ((id_msg = msgget(KEY, IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (read_message(id_msg, &buf, 1) == -1) {
        printf("Первая программа не работает\n");
        msgctl(id_msg, IPC_RMID, 0);
        exit(EXIT_SUCCESS);
    }

    pid_t pid = buf.pid;
    send_message(id_msg, &buf, 2, msg, timer);
    kill(pid, SIGINT);

    exit(EXIT_SUCCESS);
}