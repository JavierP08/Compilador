#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_STATES 3
#define NUM_CHAR 8 

int TT[NUM_STATES][NUM_CHAR]; // Tabla de transiciones

void StartTable(){
    TT[0]['letter'] = 1;
    TT[0]['digit'] = 7;
    TT[0]['_'] = 1;
    TT[0]['{'] = 3;
    TT[0]['}'] = 4;
    TT[0]['('] = 5;
    TT[0][')'] = 6;
    TT[0]['blank'] = 0;
    TT[0]['rare_char'] = 0;

    TT[1]['letter'] = 1;
    TT[1]['digit'] = 1;
    TT[1]['_'] = 1;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
    TT[1]['blank'] = 2;
    TT[1]['rare_char'] = 7;
}

bool Acceptance(int state){
    if(state >= 0 && state < 7){
        return true;
    }
    return false;
}

bool Error(int state){
    if(state == 7){
        return true;
    }
    return false;
}

bool Advance(int state, char currentCh){
    if(state)
    return false;
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
            state = TT[state][currentCh];

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