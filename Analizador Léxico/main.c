#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_STATES 3
#define NUM_CHAR 8

int TT[NUM_STATES][NUM_CHAR];

void StartTable(){
    TT[0]['letter'] = 1;
    TT[0]['digit'] = 7;
    TT[0]['_'] = 1;
    TT[0]['{'] = 3;
    TT[0]['}'] = 4;
    TT[0]['('] = 5;
    TT[0][')'] = 6;
    TT[0]['blank'] = 0;

    TT[1]['letter'] = 1;
    TT[1]['digit'] = 1;
    TT[1]['_'] = 1;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
    TT[1]['blank'] = 2;
}

bool Acceptance(int state){
    return true;
}

bool Error(int state){
    return false;
}

bool Advance(int state, char currentCh){
    return true;
}

void recordToken(char currentCh){

}

int main(){
    FILE *file;
    file = fopen("example.cpp", "r");

    char currentCh = fgetc(file);

    while(currentCh != EOF){
        int state = 0; /* start */
        
        while(!Acceptance(state) && !Error(state)){
            state = 1;
            if(Advance(state, currentCh)){
                currentCh = fgetc(file);
            }
        }
        if(Acceptance(state)){
            printf("%c\n", currentCh);
            recordToken(currentCh);
        }else{
            printf("Error en token");
        }
    }
}