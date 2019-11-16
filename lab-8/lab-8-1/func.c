#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define KEY 0x222
#define BUF_SIZE 256

typedef struct message_buffer {
    long mtype;
    char mtext[BUF_SIZE];
    pid_t pid;
} msgbuf;

int read_message(int, msgbuf*, long);
void send_message(int, msgbuf*, long, char*);

int read_message(int id_msg, msgbuf* buf, long type) {
    //printf("Чтение сообщения...\n");
	buf->mtype = type;
	if (msgrcv(id_msg, (msgbuf*)buf, BUF_SIZE, type, IPC_NOWAIT) == -1) {
        return -1;
    } else {
        printf("Тип: %ld Текст: %s\n", buf->mtype, buf->mtext);
        return 0;
    }
}

void send_message(int id_msg, msgbuf* buf, long type, char* text) {
    //printf("Посылка сообщения ...\n");
    buf->pid = getpid();
	buf->mtype = type;
	strcpy(buf->mtext, text);

	if ((msgsnd(id_msg, (msgbuf*)buf, BUF_SIZE, 0)) == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
}