#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
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

//카드를 순서대로 정렬(마지막 결과 오픈시에만)
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

void straight(int k){
	int i;
	if(k==1){
		for(i=0;i<3;i++){
			if(player1[i].value==player1[i+1].value
				&&player1[i+1].value==player1[i+2].value
				&&player1[i+2].value==player1[i+3].value
				&&player1[i+3].value==player1[i+4].value){
				result_p1 = 4;
				num1 = player1[i].value;
			}
		}
	}
	else{
		for(i=0;i<3;i++){
			if(player2[i].value==player2[i+1].value
				&&player2[i+1].value==player2[i+2].value
				&&player2[i+2].value==player2[i+3].value
				&&player2[i+3].value==player2[i+4].value){
				result_p2 = 4;
				num2 = player2[i].value;
			}
		}
	}
}

void flush(int k){
	int i;
	int c=0,d=0,h=0,s=0;
	if(k==1){
		for(i=0;i<7;i++){
			if(player1[i].suit=='c')
				c++;
			else if(player1[i].suit=='d')
				d++;
			else if(player1[i].suit=='h')
				h++;
			else if(player1[i].suit=='s')
				s++;
		}
		if(c==5){
			result_p1 = 5;
			suit1 = 'c';
		}
		else if(d==5){
			result_p1 = 5;
			suit1 = 'd';
		}
		else if(h==5){
			result_p1 = 5;
			suit1 = 'h';
		}
		else if(s==5){
			result_p1 = 5;
			suit1 = 's';
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
		if(c==5){
			result_p2 = 5;
			suit2 = 'c';
		}
		else if(d==5){
			result_p2 = 5;
			suit2 = 'd';
		}
		else if(h==5){
			result_p2 = 5;
			suit2 = 'h';
		}
		else if(s==5){
			result_p2 = 5;
			suit2 = 's';
		}
	}
}
int main(void){
	int i;

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
	}

	for(i=0;i<7;i++){
		printf("%d%c ",player1[i].value,player1[i].suit);
	}
	printf("\n");
	for(i=0;i<7;i++){
		printf("%d%c ",player2[i].value,player2[i].suit);
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
	}

	
	
}
