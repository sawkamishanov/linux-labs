#include "func.c"

int main(int argc, char* argv[]) {
    int id_msg;
    msgbuf buf;

    if ((id_msg = msgget(KEY, IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (read_message(id_msg, &buf, 1) == -1) {
        printf("Первая программа не работает\n");
        exit(EXIT_SUCCESS);
    }

    pid_t pid = buf.pid;
    send_message(id_msg, &buf, 2, "Ну сообщение и че");
    kill(pid, SIGINT);

    exit(EXIT_SUCCESS);
}