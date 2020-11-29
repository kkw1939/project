#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>

#define BUFSIZE 100
#define MAX_CLNT 100
#define MAX_IP 30
#define SIZE 20

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void* recv_msg(void* arg);
void* send_clnt();
char *serverState(int count);
void menu(char port[]);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
char msg[BUFSIZE];
pthread_mutex_t mutx;



int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	void *thread_return;
	pthread_t t_id;
	pthread_t rcv_thread;
	pthread_t snd_thread;

	if(argc != 2) {
		printf(" Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	menu(argv[1]);

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		printf("bind error");
		exit(1);
	}
	if(listen(serv_sock, 5) == -1) {
		printf("listen error");
		exit(1);
	}

	while(1) {
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
		
		pthread_create(&snd_thread, NULL, send_clnt, (void*)&clnt_sock);
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_create(&rcv_thread, NULL, recv_msg, (void*)&clnt_sock);
		pthread_join(rcv_thread, &thread_return);
		pthread_join(snd_thread, &thread_return);
		pthread_detach(t_id);
		printf(" Connected client IP : %s ", inet_ntoa(clnt_adr.sin_addr));
		printf(" chatter (%d/100)\n", clnt_cnt);
	}
	close(serv_sock);
	return 0;
}

void *handle_clnt(void *arg) {
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	int i;
	char msg[BUFSIZE];

	while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
		send_msg(msg, str_len);
	pthread_mutex_lock(&mutx);
	for(i = 0; i < clnt_cnt; i++) {
		if(clnt_sock == clnt_socks[i]) {
			while(i++ < clnt_cnt - 1) 
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_msg(char* msg, int len) {
	int i;
	pthread_mutex_lock(&mutx);
	for(i = 0; i < clnt_cnt; i++) 
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
char* serverState(int count) {
	char* stateMsg = malloc(sizeof(char) * 20);
	strcpy(stateMsg, "None");

	if(count < 5) strcpy(stateMsg, "Good");
	else strcpy(stateMsg, "Bad");
	return stateMsg;
}

void menu(char port[]) {
	system("clear");
	printf(" chat server \n");
	printf(" server port   : %s\n", port);
	printf(" server state  : %s\n", serverState(clnt_cnt));
	printf(" max connection: %d\n", MAX_CLNT);
	printf(" ----- log -----\n\n");
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

void* send_clnt(){

	int i;
	while(1) {
		fgets(msg, BUFSIZE, stdin);

		for(i = 0;i< clnt_cnt;i++)
		write(clnt_socks[i], msg, strlen(msg));
	}
	return NULL;
}

