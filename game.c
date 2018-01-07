#include <stdlib.h>
#include <stdio.h>
#include <time.h>//????

typedef int bool;
#define true 1
#define false 0

#define CHANCE_TO_REMAIN_SAME 5
#define CHANCE_TO_CHANGE_PATTERN 8
#define CHANCE_TO_CHANGE_SIZE 10

#define CHANCE_TO_CHANGE_1_COLOR 6
#define CHANCE_TO_CHANGE_2_COLOR 9
#define CHANCE_TO_CHANGE_3_COLOR 10

#define COLOR_SIZE 3
#define INITIAL_SIZE 3
#define MAXIMUM_SIZE 7
/* All Global Variables*/
int pattern_size=INITIAL_SIZE;

bool remain_same=false;
bool change_size=false;
bool change_pattern=false;
bool answer=true;
bool right_ans;
bool ans_right=true;

int *current_pattern;
int *prev_pattern;

void make_choice();
void make_change_size();
void make_change_pattern();
void mem_copy(int *des,int *src,int size);

int main()
{
    /*initialize*/
    current_pattern=(int*)malloc(pattern_size*sizeof(int));//???
    prev_pattern=(int*)malloc(pattern_size*sizeof(int));
    srand(6743);

    int i;
    for(i=0;i<pattern_size;i++){
        current_pattern[i]=i+1;//rand()%COLOR_SIZE+1;
        prev_pattern[i]=current_pattern[i];
    }

    /*show current pattern*/
    for(i=0;i<pattern_size;i++){
            printf("%d ",current_pattern[i]);
    }
    printf("\n");
    /*delay 1 sec*/

    /*game loop start*/
    while(ans_right){
        make_choice();
        /*make current pattern accordingly*/
        if(remain_same){
            mem_copy(current_pattern, prev_pattern, pattern_size);
            right_ans=true;
        }
        else if(change_pattern){
             make_change_pattern();
             right_ans=false;
        }
        else if(change_size){
             make_change_size();
             right_ans=false;
        }
        /*show pattern*/
        for(i=0;i<pattern_size;i++){
           printf("%d ",current_pattern[i]);
        }
        printf("\n");
        /*delay and take ans*/
        scanf("%d",&answer);
        if(answer==right_ans) ans_right=true;
        else ans_right=false;
    }


    return 0;

}

void mem_copy(int *des,int *src, int size){
    int i;
    for(i=0;i<size;i++){
        des[i]=src[i];
    }
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
    if(pattern_size==INITIAL_SIZE) increase=true;
    else if (pattern_size==MAXIMUM_SIZE) increase=false;
    else {
        int change=rand()%2;
        if (change) increase=true;
        else increase=false;
    }
    if(increase) pattern_size+=1;
    else pattern_size-=1;

    free(current_pattern);
    current_pattern=(int*)malloc(pattern_size*sizeof(int));//???
    int i;

    if(increase) {
            for(i=0;i<pattern_size-1;i++){
                current_pattern[i]=prev_pattern[i];
            }
            current_pattern[pattern_size-1]=rand()%COLOR_SIZE+1;
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
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
    }
    else if(randomnumber<CHANCE_TO_CHANGE_2_COLOR){
        index=rand()%pattern_size;
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
        index=rand()%pattern_size;
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
    }
    else if(randomnumber<CHANCE_TO_CHANGE_3_COLOR){
        index=rand()%pattern_size;
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
        index=rand()%pattern_size;
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
        index=rand()%pattern_size;
        prev_pattern[index]=current_pattern[index]=rand()%COLOR_SIZE+1;
    }

}
