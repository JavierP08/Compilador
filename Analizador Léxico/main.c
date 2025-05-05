#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define NUM_STATES 10
#define NUM_CHAR 12
#define MAX 100

int TT[NUM_STATES][NUM_CHAR]; // Tabla de transiciones

char* TokenIds[] = {
    "class", "struct", "def",
    "void", "short", "int",
    "float", "double", "char",
    "bool", "long", "string",
    "{", "}", "(", ")", "<", ">",
    "#"
}; // Tabla de tokens

int TokenIdsNums[] = {
    1, 2, 3, 4, 5,
    6, 7, 8, 9, 10,
    11, 12, 13, 14, 15,
    16, 17, 18, 19
}; // Tabla de ids de los tokens

char* identifierTable[MAX]; // tabla de los identificadores
int identifierTableNum[MAX]; // tabla de ids de los identificadores 
int totalOfIdentifiers = 0; // número de identificadores obtenidos

// Función que inicia la tabla de transiciones
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

// Función que ayuda a clasificar entre
// 'l' = una letra
// 'd' = un digito
// 'b' = un valor en blanco
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

// Se obtiene el token id asignado con la tabla de tokens ids
int getTokenId(char* currentToken){
    for(int i = 0; i < sizeof(TokenIdsNums) / sizeof(int); i++){
        if(strcmp(TokenIds[i], currentToken) == 0){
            return TokenIdsNums[i];
        }
    }
    return 20;
}

int getExistingIdentifier(char* currentWord) {
    printf("Total de identificadores: %d\n Identificador buscando: %s\n\n", totalOfIdentifiers, currentWord);
    for (int i = 0; i < totalOfIdentifiers; i++) {
        if (strcmp(identifierTable[i], currentWord) == 0) {
            return identifierTableNum[i];
        }
    }
    return -1;
}

int insertIdentifier(char* currentWord) {
    int numberId = getExistingIdentifier(currentWord);
    if (numberId == -1) {
        char* copy = strdup(currentWord);
        identifierTable[totalOfIdentifiers] = copy;
        identifierTableNum[totalOfIdentifiers] = totalOfIdentifiers + 1;

        totalOfIdentifiers++;
        return identifierTableNum[totalOfIdentifiers - 1];
    }

    return numberId;
}


// Función que guarda el token id en el archivo txt de tokens
void recordToken(char palabra[200], FILE* tokenFile){
    char token[20];
    int id = getTokenId(palabra);

    if(id == 20){
        int idIdentifier = insertIdentifier(palabra);
        sprintf(token, "<%d, %d>", id, idIdentifier);
        fprintf(tokenFile, "%s\n", token);
    }else{
        sprintf(token, "<%d>", id);
        fprintf(tokenFile, "%s\n", token);
    }
}

// Saber si el estado acutal es de aceptación
bool Acceptance(int state){
    if(state == 0 || state == 1 || state == 10){
        return false;
    }

    return true;
}

// Saber si el estado actual es de error
bool Error(int state){
    if(state == 10){
        return true;
    }
    return false;
}

// Saber si en el estado actual puede proseguir a guardar el caracter
bool Advance(int state, char currentCh){
    char ch = classify(currentCh);
    if ((ch == 'l' || ch == 'd' || ch == '_') && (state == 0 || state == 1)){
        return true;
    }
    return false;
}

// Main del analizador léxico
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