#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

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
int poker();

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
char msg[BUFSIZE];
char name[SIZE] = "[SERVER]";
pthread_mutex_t mutx;

typedef struct card{
	int value;
	char suit;
}card;

card player1[7];
card player2[7];
card common_card[5];
card cards[52];
int result_p1=0;
int result_p2=0;
int num1,num2;
char suit1,suit2;

int check(int k){
	if(cards[k].value<0)
		return -1;
	else 
		return 1;
}

int check_suit(char k){
	if(k=='c')
		return 1;
	else if(k=='d')
		return 2;
	else if(k=='h')
		return 3;
	else 
		return 4;
}
void give_card(int k,int a){
	int rand_num,check_num;
	while(1){
		rand_num = rand()%51;
		check_num = check(rand_num);
		if(check_num==1)
			break;
	}
	if(k==1){
		player1[a] = cards[rand_num];
	}
	else if(k==2){
		player2[a] = cards[rand_num];
	}
	else if(k==3){
		common_card[a] = cards[rand_num];
		player1[a+2] = cards[rand_num];
		player2[a+2] = cards[rand_num];

	}
	cards[rand_num].value = cards[rand_num].value-1000;
}

void sort(int k){
	int i,j;
	card temp;
	if(k==1){
		for(i=0;i<7;i++){
			for(j=0;j<6-i;j++){
				if(player1[j].value>player1[j+1].value){
					temp.value = player1[j].value;
					temp.suit = player1[j].suit;
					player1[j].value = player1[j+1].value;
					player1[j].suit = player1[j+1].suit;
					player1[j+1].value = temp.value;
					player1[j+1].suit = temp.suit;
				}
			}
		}
		num1 = player1[6].value;
	}
	else{
		for(i=0;i<7;i++){
			for(j=0;j<6-i;j++){
				if(player2[j].value>player2[j+1].value){
					temp.value = player2[j].value;
					temp.suit = player2[j].suit;
					player2[j].value = player2[j+1].value;
					player2[j].suit = player2[j+1].suit;
					player2[j+1].value = temp.value;
					player2[j+1].suit = temp.suit;
				}
			}
		}
		num2= player2[6].value;
	}

}


void one_pair(int k){
	int i;
	if(k==1){
		for(i=0;i<6;i++){
			if(player1[i].value==player1[i+1].value){
				result_p1 = 1;
				num1 = player1[i].value;
			}
		}
	}
	else{
		for(i=0;i<6;i++){
			if(player2[i].value==player2[i+1].value){
				result_p2 = 1;
				num2 = player2[i].value;
			}
		}
	}
}

void two_pair(int k){
	int i;
	int count = 0;
	if(k==1){
		for(i=0;i<6;i++){
			if(player1[i].value==player1[i+1].value){
				count++;
				num1 = player1[i].value;
			}
		}
		if(count>=2){
			result_p1 = 2;
		}
	}
	else{
		for(i=0;i<6;i++){
			if(player2[i].value==player2[i+1].value){
				count++;
				num2= player2[i].value;
			}
		}
		if(count>=2){
			result_p2 = 2;
		}
	}
}

void triple(int k){
	int i;
	if(k==1){
		for(i=0;i<5;i++){
			if(player1[i].value==player1[i+1].value&&player1[i+1].value==player1[i+2].value){
				result_p1 = 3;
				num1 = player1[i].value;
			}
		}
	}
	else{
		for(i=0;i<5;i++){
			if(player2[i].value==player2[i+1].value&&player2[i+1].value==player2[i+2].value){
				result_p2 = 3;
				num2 = player2[i].value;
			}
		}
	}
}

void flush(int k){
	int i;
	int c=0,d=0,h=0,s=0;
	int check[4]={0,};
	if(k==1){
		for(i=0;i<7;i++){
			if(player1[i].suit=='c'){
				c++;
				check[0]=player1[i].value;
			}
			else if(player1[i].suit=='d'){
				d++;
				check[1]=player1[i].value;
			}
			else if(player1[i].suit=='h'){
				h++;
				check[2]=player1[i].value;
			}
			else if(player1[i].suit=='s'){
				s++;
				check[3]=player1[i].value;
			}
		}
		if(c>=5){
			result_p1 = 5;
			suit1 = 'c';
			num1 = check[0];
		}
		else if(d>=5){
			result_p1 = 5;
			suit1 = 'd';
			num1 = check[1];
		}
		else if(h>=5){
			result_p1 = 5;
			suit1 = 'h';
			num1 = check[2];
		}
		else if(s>=5){
			result_p1 = 5;
			suit1 = 's';
			num1 = check[3];
		}
	}
	else{
		for(i=0;i<7;i++){
			if(player2[i].suit=='c')
				c++;
			else if(player2[i].suit=='d')
				d++;
			else if(player2[i].suit=='h')
				h++;
			else if(player2[i].suit=='s')
				s++;
		}
		if(c>=5){
			result_p2 = 5;
			suit2 = 'c';
			num2 = check[0];
		}
		else if(d>=5){
			result_p2 = 5;
			suit2 = 'd';
			num2 = check[1];
		}
		else if(h>=5){
			result_p2 = 5;
			suit2 = 'h';
			num2 = check[2];
		}
		else if(s>=5){
			result_p2 = 5;
			suit2 = 's';
			num2 = check[3];
		}
	}
}

void straight(int m){
	int i,n=0;
	int temp;
	int count=0;
	int ace=0,ten=0,j=0,q=0,k=0;
	if(m==1){
		for(i=0;i<6;i++){
			temp = 0;
			temp = player1[i+1].value-player1[i].value;
			if(count==4){
				num1 = player1[i].value;
				break;
			}
			if(temp==1){
				count++;
			}
			else if(temp==0){
			}
			else{
				count=0;
			}
			if(count==4&&i==5)
				num1 = player1[i].value;
		}

		for(i=0;i<7;i++){
			if(player1[i].value==1)
				ace++;
			else if(player1[i].value==10)
				ten++;
			else if(player1[i].value==11)
				j++;
			else if(player1[i].value==12)
				q++;
			else if(player1[i].value==13)
				k++;
		}
		if(count==4){
			result_p1 = 4;
		}
		else if(count>4){
			result_p1 = 4;
			num1 = player1[6].value;
		}
		if(ace>=1&&ten>=1&&j>=1&&q>=1&&k>=1){
			result_p1 = 4;
			num1 = 5;
		}
	}
	else{
		for(i=0;i<6;i++){
			temp = 0;
			temp = player2[i+1].value-player2[i].value;
			if(count==4){
				num2 = player2[i].value;
				break;
			}
			if(temp==1){
				count++;
			}
			else if(temp==0){
			}
			else{
				count=0;
			}
			if(count==4&&i==5)
				num2 = player2[i].value;
		}

		for(i=0;i<7;i++){
			if(player2[i].value==1)
				ace++;
			else if(player2[i].value==10)
				ten++;
			else if(player2[i].value==11)
				j++;
			else if(player2[i].value==12)
				q++;
			else if(player2[i].value==13)
				k++;
		}
		if(count==4){
			result_p2 = 4;
		}
		else if(count>4){
			result_p1 = 4;
			num1 = player1[6].value;
		}
		if(ace>=1&&ten>=1&&j>=1&&q>=1&&k>=1){
			result_p2 = 4;
			num1 = 5;
		}
	}
}

void full_house(int k){
	int i;
	int count_value[14]={0,};
	int count_fullhouse1=0;
	int count_fullhouse2=0;
	int temp,a;
	if(k==1){
		for(i=0;i<7;i++){
			temp = player1[i].value;
			count_value[temp]++;
		}
		for(i=0;i<14;i++){
			if(count_value[i]==2){
				count_fullhouse1++;
			}
			if(count_value[i]==3){
				count_fullhouse2++;
				a = i;
			}
		}
		if(count_fullhouse1>=1&&count_fullhouse2>=1){
			result_p1 = 6;
			num1 = a;
		}

	}
	else{
		for(i=0;i<7;i++){
			temp = player2[i].value;
			count_value[temp]++;
		}
		for(i=0;i<14;i++){
			if(count_value[i]==2){
				count_fullhouse1++;
			}
			if(count_value[i]==3){
				count_fullhouse2++;
				a = i;
			}
		}
		if(count_fullhouse1>=1&&count_fullhouse2>=1){
			result_p2 = 6;
			num2 = a;
		}

	}
}

void four_card(int k){
	int i;
	if(k==1){
		for(i=0;i<4;i++){
			if(player1[i].value == player1[i+1].value
				&&player1[i+1].value == player1[i+2].value
				&&player1[i+2].value == player1[i+3].value){
					result_p1=7;
					num1 = player1[i].value;
			}
		}
	}
	else{
		for(i=0;i<4;i++){
			if(player2[i].value == player2[i+1].value
				&&player2[i+1].value == player2[i+2].value
				&&player2[i+2].value == player2[i+3].value){
					result_p2=7;
					num2 = player2[i].value;
			}
		}
	}
}

void straight_flush(int k){
	int i;
	int count=0;
	int temp=0;
	int check=0;
	card sflush[5];
	if(k==1){
		for(i=0;i<7;i++){
			if(player1[i].suit==suit1)
				sflush[count++] = player1[i];
		}
		if(count==5){
			for(i=0;i<5;i++){
				temp = sflush[i+1].value - sflush[i].value;
				if(temp==1)
					check++;
			}
			if(check==4){
				result_p1 = 8;
				num1 = sflush[4].value;
			}
		}
	}
	else{
		for(i=0;i<7;i++){
			if(player2[i].suit==suit2)
				sflush[count++] = player2[i];
		}
		if(count==5){
			for(i=0;i<5;i++){
				temp = sflush[i+1].value - sflush[i].value;
				if(temp==1)
					check++;
			}
			if(check==4){
				result_p2 = 8;
				num2= sflush[4].value;
			}
		}
	}
}

void royal_straight_flush(int k){
	int i;
	int count=0;
	char check_suit='0';
	if(k==1){
		check_suit = player1[0].suit;
		for(i=1;i<7;i++){
			if(player1[i].value==10&&player1[i].suit==check_suit)
				count++;
			if(player1[i].value==11&&player1[i].suit==check_suit)
				count++;
			if(player1[i].value==12&&player1[i].suit==check_suit)
				count++;
			if(player1[i].value==13&&player1[i].suit==check_suit)
				count++;
		}
		if(count==4)
			result_p1=9;
	}
	else{
		check_suit = player2[0].suit;
		for(i=1;i<7;i++){
			if(player2[i].value==10&&player2[i].suit==check_suit)
				count++;
			if(player2[i].value==11&&player2[i].suit==check_suit)
				count++;
			if(player2[i].value==12&&player2[i].suit==check_suit)
				count++;
			if(player2[i].value==13&&player2[i].suit==check_suit)
				count++;
		}
		if(count==4)
			result_p2=9;
	}
}

int poker(void){
	int i,suit_num1,suit_num2;
	srand(time(NULL));
	//make card deck

	for(i=0;i<13;i++){
		cards[i].value=i%13+1;
		cards[i].suit='c';
	}	
	for(i=0;i<13;i++){
		cards[i+13].value=i%13+1;
		cards[i+13].suit='d';
	}

	for(i=0;i<13;i++){
		cards[i+26].value=i%13+1;
		cards[i+26].suit='h';
	}

	for(i=0;i<13;i++){
		cards[i+39].value=i%13+1;
		cards[i+39].suit='s';
	}

	//give first personal card
	give_card(1,0);
	give_card(2,0);

	//give first personal card
	give_card(1,1);
	give_card(2,1);

	//open common card
	for(i=0;i<5;i++){
		give_card(3,i);
	}

	//check card
	for(i=1;i<3;i++){
		sort(i);
		one_pair(i);
		two_pair(i);
		triple(i);
		straight(i);
		flush(i);
		full_house(i);
		four_card(i);
		straight_flush(i);
		royal_straight_flush(i);
	}

	for(i=0;i<7;i++){
	       	char msg[BUFSIZ];
		char suit[BUFSIZ];
		sprintf(msg, "%d", player1[i].value);
		sprintf(suit, "%c", player1[i].suit);
		strcat(msg, suit);	
		printf("player1 : %d%c ",player1[i].value,player1[i].suit);
		write(clnt_socks[0], msg, strlen(msg)); 
		
		printf("player2 : %d%c ",player2[i].value,player2[i].suit);
		sprintf(msg, "%d", player2[i].value);
		sprintf(suit, "%c", player2[i].suit);
		strcat(msg, suit);
		write(clnt_socks[1], msg, strlen(msg));
		printf("\n");
		//getchar();
	}
	printf("\n");

	switch(result_p1){
	case 0 : printf("player1 : %d high card\n",num1);
		break;
	case 1 : printf("player1 : %d one pair\n",num1); 
		break;
	case 2 : printf("player1 : %d two pair\n",num1);
		break;
	case 3 : printf("player1 : %d triple\n",num1);
		break;
	case 4 : printf("player1 : %d straight\n",num1);
		break;
	case 5 : printf("player1 : %c flush\n",suit1);
		break;
	case 6 : printf("player1 : %c full house\n",suit1);
		break;
	case 7 : printf("player1 : %c four card\n",suit1);
		break;
	case 8 : printf("player1 : %c straight flush\n",suit1);
		break;
	case 9 : printf("player1 : %c royal straight flush\n",suit1);
		break;
	}

	switch(result_p2){
	case 0 : printf("player2 : %d high card\n",num2);
		break;
	case 1 : printf("player2 : %d one pair\n",num2); 
		break;
	case 2 : printf("player2 : %d two pair\n",num2);
		break;
	case 3 : printf("player2 : %d triple\n",num2);
		break;
	case 4 : printf("player2 : %d straight\n",num2);
		break;
	case 5 : printf("player2 : %c flush\n",suit2);
		break;
	case 6 : printf("player2 : %c full house\n",suit2);
		break;
	case 7 : printf("player2 : %c four card\n",suit2);
		break;
	case 8 : printf("player2 : %c straight flush\n",suit2);
		break;
	case 9 : printf("player2 : %c royal straight flush\n",suit2);
		break;
	}

	if(result_p1>result_p2){
		printf("player1 win!\n");
	}
	else if(result_p1<result_p2){
		printf("player2 win!\n");
	}
	else{
		//if high card
		if(result_p1==0){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else {
				num1 = player1[5].value;
				num2 = player1[2].value;
				if(num1>num2)
					printf("player1 win\n");
				else if(num1<num2)
					printf("player2 win\n");
				else{
					suit_num1 = check_suit(suit1);
					suit_num2 = check_suit(suit2);
					if(suit_num1>suit_num2)
						printf("player1 win\n");
					else 
						printf("player2 win\n");
				}
			}
		}
		//if one pair
		else if(result_p1==1){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else {
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}
		}
		//if two pair
		else if(result_p1==2){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else {
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}
		}
		//if triple
		else if(result_p1==3){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else {
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}
		}

		//if straight
		else if(result_p1==4){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else{
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}

		}
		//if flush
		else if(result_p1==5){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else{
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}
		}
		//if full house
		else if(result_p1==6){
			if(num1>num2)
				printf("player1 win\n");
			else 
				printf("player2 win\n");
		}
		//if four card
		else if(result_p1==7){
			if(num1>num2)
				printf("player1 win\n");
			else 
				printf("player2 win\n");
		}
		//if straight_flush
		else if(result_p1==8){
			if(num1>num2)
				printf("player1 win\n");
			else if(num1<num2)
				printf("player2 win\n");
			else{
				suit_num1 = check_suit(suit1);
				suit_num2 = check_suit(suit2);
				if(suit_num1>suit_num2)
					printf("player1 win\n");
				else 
					printf("player2 win\n");
			}
		}
		//if royal straight flush
		else if(result_p1==9){
			suit_num1 = check_suit(suit1);
			suit_num2 = check_suit(suit2);
			if(suit_num1>suit_num2)
				printf("player1 win\n");
			else 
				printf("player2 win\n");
		}
	}
}



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
		pthread_detach(t_id);
		printf(" Connected client IP : %s ", inet_ntoa(clnt_adr.sin_addr));
		printf(" chatter (%d/100)\n", clnt_cnt);
		if(clnt_cnt == 2) {
			sleep(3);
			char start[BUFSIZE] = "Game start!\n Check your card..\n We will open the next card 10s later..\n";
			send_msg(start, strlen(start));
			sleep(10);
			poker();
		}
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
	char name_msg[SIZE+BUFSIZE];
	while(1) {
		fgets(msg, BUFSIZE, stdin);
		sprintf(name_msg, "%s %s", name, msg);
		for(i = 0;i< clnt_cnt;i++)

		write(clnt_socks[i],name_msg, strlen(name_msg));
	}
	return NULL;
}

