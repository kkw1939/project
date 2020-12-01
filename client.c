#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

#define BUFSIZE 100
#define SIZE 20

void* send_msg(void* arg);
void* recv_msg(void* arg);
int poker();
void menu();

char name[SIZE] = "[USER]";
char msg_form[SIZE];
char serv_time[SIZE];
char msg[BUFSIZE];
char serv_port[SIZE];
char clnt_ip[SIZE];

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void* thread_return;

	if(argc != 4) {
		printf("Usage : %s <ip> <port> <name>\n", argv[0]);
		exit(1);
	}

	sprintf(name, "[%s]", argv[3]);
	sprintf(clnt_ip, "%s", argv[1]);
	sprintf(serv_port, "%s", argv[2]);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		printf("connect error");
		exit(1);
	}

	menu();

	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);
	return 0;
}

void* send_msg(void* arg) {
	int sock = *((int*)arg);
	char name_msg[SIZE+BUFSIZE];
	char myInfo[BUFSIZE];
	char* who = NULL;
	char temp[BUFSIZE];

	printf(" >> start chat ...\n");
	sprintf(myInfo, "%s's join. IP_%s\n", name, clnt_ip);
	write(sock, myInfo, strlen(myInfo));

	while(1) {
		fgets(msg, BUFSIZE, stdin);

		/* quit */
		if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
			close(sock);
			exit(0);
		}
		
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
		}
	return NULL;
}

void* recv_msg(void* arg) {
	int sock = *((int*)arg);
	char name_msg[SIZE+BUFSIZE];
	int str_len;
	
	while(1) {
		str_len = read(sock, name_msg, SIZE + BUFSIZE -1);
		if(str_len == -1) return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}

void menu() {
	system("clear");
	printf(" pocker game chatting client \n");
	printf(" server port : %s\n", serv_port);
	printf(" client IP   : %s\n", clnt_ip);
	printf(" chat name   : %s\n", name);
	printf(" server time : %s\n", serv_time);
	printf(" exit : q or Q\n\n");
}
