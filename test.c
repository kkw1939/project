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


int main(void){
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


