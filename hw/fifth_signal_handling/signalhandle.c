#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void signal_handler(int sig){
	if (sig == SIGINT){ // SIGINT(ctrl + c) 받았을 때
		printf("Are you sure you want to quit this game? y/n");
		char answer;
		scanf("%c", &answer);
		if (answer == 'y'){
			printf("Goodbye\n");
			exit(0);
		}
		else{
			printf("continue playing\n");
		}
	}
	else if (sig == SIGQUIT){ // SIGQUIT(ctrl + \)
		printf("Quit without core dump\n"); 
		exit(0);
	}
}

void play(int i){
	printf("game %d\n", i);
	int guess, number;
	srand(time(NULL));
	number = rand() % 100;
	while (guess != number){
		printf("enter a number: ");
		scanf("%d", &guess);
		if(guess > 100 || guess < 0)
			printf("numbers should be betwwen 0 and 100");
		else if(guess > number)
			printf("down\n");
		else if (guess < number)
			printf("up\n");
		else if (guess == number){
			printf("congrats!\n\n");
			return;
		}
	}
}

void start_game(){
	int games;
	printf("guessing number : how many times will you play ? \n");
	scanf("%d", &games);
	printf("the range of number is 0 ~ 100\n");
	for (int i = 0; i < games; i++){
		play(i+1);
	}
}

int main(){
	signal(SIGINT, signal_handler); // 시그널 핸들러 활성화
	signal(SIGQUIT, signal_handler);
	start_game();
}
