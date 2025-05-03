#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define NUM_STATES 10
#define NUM_CHAR 12

int TT[NUM_STATES][NUM_CHAR]; // Tabla de transiciones

char PR[15][10] = {
    "class", "struct", "def", 
    "void", "int", "float",
    "double", "char", "bool",
    "long", "string", "short",
}; //Tabla de palabras reservadas

void StartTable(){
    TT[0]['l'] = 1;
    TT[0]['d'] = 7;
    TT[0]['_'] = 1;
    TT[0]['{'] = 3;
    TT[0]['}'] = 4;
    TT[0]['('] = 5;
    TT[0][')'] = 6;
    TT[0]['<'] = 7;
    TT[0]['>'] = 8;
    TT[0]['#'] = 9;
    TT[0]['b'] = 0;

    TT[1]['l'] = 1;
    TT[1]['d'] = 1;
    TT[1]['_'] = 1;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
    TT[1]['<'] = 2;
    TT[1]['>'] = 2;
    TT[1]['#'] = 2;
    TT[1]['b'] = 2;
}

char classify(char currentCh){
    if(isalpha(currentCh)){
        return 'l';
    } else if (isdigit(currentCh)) {
        return 'd';
    } else if(isspace(currentCh)) {
        return 'b';
    }

    return currentCh;
}

bool Acceptance(int state){
    if(state == 0 || state == 1 || state == 10){
        return false;
    }

    return true;
}

bool Error(int state){
    if(state == 10){
        return true;
    }
    return false;
}

bool Advance(int state, char currentCh){
    char ch = classify(currentCh);
    if ((ch == 'l' || ch == 'd' || ch == '_') && (state == 0 || state == 1)){
        return true;
    }
    return false;
}

void recordToken(char palabra[200], FILE* tokenFile){
    char token[20];
    sprintf(token, "<%s>", palabra);
    
    fprintf(tokenFile, "%s\n", token);
}


int main(){
    FILE *processFile;
    FILE *tokenFile;
    processFile = fopen("example.txt", "r");
    tokenFile = fopen("tokens.txt", "w");

    StartTable();

    char currentCh = fgetc(processFile);

    while (currentCh != EOF)
    {
        int state = 0;  /* start */
        char palabra[200] = "";
        int index = 0;

        if(currentCh == '\t' || currentCh == ' ' || currentCh == '\n'){
            currentCh = fgetc(processFile);
        }

        while (!Acceptance(state) && !Error(state)){
            char ch = classify(currentCh);
            state = TT[state][ch];
            
            if(Advance(state, currentCh)){
                printf("Estoy en mi advance state\n");
                palabra[index++] = currentCh;
                currentCh = fgetc(processFile);
            }else if(classify(currentCh) == 'b'){
                currentCh = fgetc(processFile);
            }else{
                palabra[index++] = currentCh;
                currentCh = fgetc(processFile);
            }

        }

        if(Acceptance(state)){
            palabra[index] = '\0';
            recordToken(palabra, tokenFile);
            palabra[0] = '\0';
            index = 0;
        }else if(Error(state)){
            printf("Error state");
        }

        
    }

    fclose(processFile);
    fclose(tokenFile);
}