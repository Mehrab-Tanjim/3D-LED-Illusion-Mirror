#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>//????

uint16_t counter1, counter2;

typedef int bool;
#define true 1
#define false 0

#define CHANCE_TO_REMAIN_SAME 5
#define CHANCE_TO_CHANGE_PATTERN 8
#define CHANCE_TO_CHANGE_SIZE 10

#define CHANCE_TO_CHANGE_1_COLOR 6
#define CHANCE_TO_CHANGE_2_COLOR 9
#define CHANCE_TO_CHANGE_3_COLOR 10

#define COLOR_SIZE 7
#define INITIAL_SIZE 3
#define MAXIMUM_SIZE 9

#define ALL 7
#define BLUE_RED 6
#define GREEN_BLUE 5
#define BLUE 4
#define GREEN_RED 3
#define RED 2
#define GREEN 1
#define NONE 0


/* All Global Variables*/
int get_colors[COLOR_SIZE]={ALL,GREEN,RED,BLUE,GREEN_RED,GREEN_BLUE,BLUE_RED};
int pattern_size=INITIAL_SIZE;
int yes=0,no=0,in=0,set=0;
int silly_count_yes=0,silly_count_no=0;
int restart,change_mode;
int initial_size=INITIAL_SIZE;
int maximum_size=MAXIMUM_SIZE;

bool remain_same=false;
bool change_size=false;
bool change_pattern=false;
bool answer=true;
bool answered=false;
bool right_ans;
bool ans_right;

int *current_pattern;
int *prev_pattern;

void make_choice();
void make_change_size();
void make_change_pattern();
void mem_copy();

int main(void)
{
	
	/*initialize*/
	
	DDRB= 0b11111100;//input
	DDRC= 0b11111110;
	DDRD = 0b00000111;//output
	DDRA= 0b00000110;
	PORTD=NONE;//~0;//only for backward, default
	PORTA=6;
	srand(6743);//bad start give a new start every time find a timer here
	
	
	
	/*game loop start*/
	while(1){
		while(1){
			restart=(PINC)&0x01;
			change_mode=(PINC>>1)&0x01;
			if(change_mode){
				change_mode=0;
				break;
			}
			PORTA=0;
			if(restart){
				
				game();
				restart=0;
			}
		}
		while(1){
			restart=(PINC)&0x01;
			change_mode=(PINC>>1)&0x01;
			if(change_mode){
				change_mode=0;
				break;
			}
			PORTA=4;
			if(restart){
				PORTA=6;
				restart=0;
				while(1){
					int i;
					for(i=0;i<pattern_size;i++){
						PORTD=get_colors[i+1];
						for(counter1=0; counter1 <1; counter1++ )
							for (counter2 = 0; counter2 < 50000; counter2++)
								{
									restart=(PINC)&0x01;
									if(restart) break;
								}
						if(restart) {							
							break;
						}	
					}
					if(restart) {
						restart=0;
						break;
					}
				}
				PORTD=NONE;				
			}
		}		
		
	}
	

}

void game(){
	pattern_size=INITIAL_SIZE;
	current_pattern=(int*)malloc(pattern_size*sizeof(int));
	prev_pattern=(int*)malloc(pattern_size*sizeof(int));

	int i;
	//pattern_size retains the previous size so can be random each time started up :P
	for(i=0;i<pattern_size;i++){
		current_pattern[i]=get_colors[rand()%COLOR_SIZE];//rand()%COLOR_SIZE+1;// cheat for now
		prev_pattern[i]=current_pattern[i];
	}
	/*show current pattern*/
	show();
	/*delay 1 sec*/
	PORTD=NONE;//~0;
	PORTA=6;
	_delay_ms(2000);
	
	ans_right=true; //start the loop
	
	while(ans_right){
		make_choice();
		/*make current pattern accordingly*/
		
		if(remain_same){
			right_ans=true;
		}
		else if(change_pattern){
			make_change_pattern();
			if(same()) right_ans=true;
			else right_ans=false;
			mem_copy(prev_pattern, current_pattern, pattern_size);
		}
		else if(change_size){
			make_change_size();
			right_ans=false;
		}
		
		/*show pattern*/
		show();
		
		/*delay and take ans*/
		PORTD=NONE;//~0;
		for(counter1=0; counter1 <1; counter1++ )
		for (counter2 = 0; counter2 < 50000; counter2++)
		{
			yes=(PINB>>1) & 0x1;
			no=PINB & 0x1;
			if(yes){
				answer=true;
				silly_count_yes++;
			}
			else if(no){
				answer=false;
				silly_count_no++;
			}
		}
		if(silly_count_yes==0&&silly_count_no==0) ans_right=false;
		else if(silly_count_yes>=1&&silly_count_no>=1) ans_right=false;
		else if(answer==right_ans) ans_right=true;
		else ans_right=false;
		silly_count_yes=silly_count_no=0;
		if(ans_right){
			PORTA=4;
			_delay_ms(200);
			PORTA=6;
		}
		else{
			PORTA=2;
			_delay_ms(200);
			PORTA=6;
		}
		_delay_ms(100);
	}
	free(current_pattern);
	free(prev_pattern);
}

void show(){
	int i;
	for(i=0;i<pattern_size;i++){
		PORTD=current_pattern[i];
		_delay_ms(500);
	}
}

void mem_copy(int *dest, int* src, int size){
	int i;
	for(i=0;i<size;i++){
		dest[i]=src[i];
	}
}

bool same(){
	int i;
	for(i=0;i<pattern_size;i++){
		if(current_pattern[i]!=prev_pattern[i]) return false;
	}
	return true;
}

void make_choice()
{
	int randomnumber= rand() % 10;

	if(randomnumber<CHANCE_TO_REMAIN_SAME){
		remain_same=true;
		change_size=false;
		change_pattern=false;
	}
	else if(randomnumber<CHANCE_TO_CHANGE_PATTERN){
		remain_same=false;
		change_size=false;
		change_pattern=true;
	}
	else if(randomnumber<CHANCE_TO_CHANGE_SIZE){
		remain_same=false;
		change_size=true;
		change_pattern=false;
	}
}

void make_change_size(){

	bool increase;
	if(pattern_size==initial_size) increase=true;
	else if (pattern_size==maximum_size) increase=false;
	else {
		int change=rand()%2;
		if (change) increase=true;
		else increase=false;
	}
	if(increase) pattern_size+=1;
	else pattern_size-=1;
	
	if(pattern_size==5||pattern_size==7) initial_size=pattern_size;
	
	free(current_pattern);
	current_pattern=(int*)malloc(pattern_size*sizeof(int));//???
	int i;

	if(increase) {
		for(i=0;i<pattern_size-1;i++){
			current_pattern[i]=prev_pattern[i];
		}
		current_pattern[pattern_size-1]=get_colors[(rand()%COLOR_SIZE)];
	}
	else{
		for(i=0;i<pattern_size;i++){
			current_pattern[i]=prev_pattern[i];
		}
	}

	free(prev_pattern);//???
	prev_pattern=(int*)malloc(pattern_size*sizeof(int));//???

	for(i=0;i<pattern_size;i++){
		prev_pattern[i]=current_pattern[i];
	}

}


void make_change_pattern(){
	int randomnumber= rand() % 10;
	int index;
	if(randomnumber<CHANCE_TO_CHANGE_1_COLOR){
		index=rand()%pattern_size;
		current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
	}
	else if(randomnumber<CHANCE_TO_CHANGE_2_COLOR){
		index=rand()%pattern_size;
		prev_pattern[index]=current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
		index=rand()%pattern_size;
		current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
	}
	else if(randomnumber<CHANCE_TO_CHANGE_3_COLOR){
		index=rand()%pattern_size;
		current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
		index=rand()%pattern_size;
		current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
		index=rand()%pattern_size;
		current_pattern[index]=get_colors[(rand()%COLOR_SIZE)];//~(rand()%COLOR_SIZE+1);
	}
}

