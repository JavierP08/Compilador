#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_STATES 7
#define NUM_CHAR 9

int TT[NUM_STATES][NUM_CHAR]; // Tabla de transiciones

char PR[15][10] = {
    "class", "struct", "def", 
    "void", "int", "float",
    "double", "char", "bool",
    "long", "string",
}; //Tabla de palabras reservadas

void StartTable(){
    TT[0]['l'] = 1;
    TT[0]['d'] = 7;
    TT[0]['_'] = 1;
    TT[0]['{'] = 3;
    TT[0]['}'] = 4;
    TT[0]['('] = 5;
    TT[0][')'] = 6;
    TT[0]['b'] = 0;

    TT[1]['l'] = 1;
    TT[1]['d'] = 1;
    TT[1]['_'] = 1;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
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
    if(state > 1 && state < 7){
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
    if(state > 1 || state <= 7){
        return false;
    }
    return true;
}

void recordToken(char palabra[200], FILE* tokenFile){
    char token[20];
    sprintf(token, "<%s>", palabra);
    
    fprintf(tokenFile, "%s\n", token);
}

/*int main(){
    //Se abre el archivo a analizar y crear archivo de tokens
    FILE *processFile;
    FILE *tokenFile;
    processFile = fopen("example.txt", "r");
    tokenFile = fopen("tokens.txt", "w");

    //Se establece la tabla de estados
    StartTable();

    //Se obtiene el primer char del archivo
    char currentCh = fgetc(processFile);

    while(numero < 5){
        int state = 0;  //start 
        char palabra[200] = "";
        int index = 0;

        while(!Acceptance(state) && !Error(state)){
            char ch = classify(currentCh);
            state = TT[state][ch];
            printf("Current state: %d", state);

            if(Advance(state, currentCh)){
                palabra[index++] = currentCh;
                currentCh = fgetc(processFile);
            }

        }
    }
}*/

int main(){
    FILE *processFile;
    FILE *tokenFile;
    processFile = fopen("example.txt", "r");
    tokenFile = fopen("tokens.txt", "w");

    StartTable();

    char currentCh = fgetc(processFile);

    int state = 0;  /* start */
    char palabra[200] = "";
    int index = 0;

    while (currentCh != EOF)
    {
        if(currentCh == '\t'){
            currentCh = fgetc(processFile);
        }
        printf("Current state: %d, Current char: %c, Current word: %s\n", state, currentCh, palabra);

        char ch = classify(currentCh);
        state = TT[state][ch];
        palabra[index++] = currentCh;

        if(Acceptance(state)){
            palabra[index] = '\0';
            recordToken(palabra, tokenFile);
            palabra[0] = '\0';
            index = 0;
        }

        currentCh = fgetc(processFile);
    }

    recordToken(palabra, tokenFile);

    fclose(processFile);
    fclose(tokenFile);
}