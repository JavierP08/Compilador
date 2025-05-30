#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define NUM_STATES 8   //Restar los estados que no hacen falta +1
#define NUM_CHAR 255
#define MAX 100

int TT[NUM_STATES][NUM_CHAR]; // Tabla de transiciones

char* TokenIds[] = {
    "class", "main",
    "{", "}", "(", ")"
}; // Tabla de tokens (MODIFICARLA)

int TokenIdsNums[] = {
    1, 2, 3, 4, 5,
    6
}; // Tabla de ids de los tokens (USARLA NUEVA TABLA DE IDS)

char* identifierTable[MAX]; // tabla de los identificadores
int identifierTableNum[MAX]; // tabla de ids de los identificadores 
int totalOfIdentifiers = 0; // número de identificadores obtenidos

// Función que inicia la tabla de transiciones
void StartTable(){

    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_CHAR; j++) {
            TT[i][j] = 7;
        }
    }

    TT[0]['l'] = 1;
    TT[0]['d'] = 7;
    TT[0]['_'] = 1;
    TT[0]['{'] = 3;
    TT[0]['}'] = 4;
    TT[0]['('] = 5;
    TT[0][')'] = 6;
    /*TT[0]['<'] = 7;
    TT[0]['>'] = 8;
    TT[0]['#'] = 9;
    TT[0][';'] = 10;
    TT[0][':'] = 11;*/
    TT[0]['b'] = 0;
    TT[0][7] = 7; // rare char

    TT[1]['l'] = 1;
    TT[1]['d'] = 1;
    TT[1]['_'] = 1;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
    /*TT[1]['<'] = 2;
    TT[1]['>'] = 2;
    TT[1]['#'] = 2;
    TT[1][';'] = 2;
    TT[1][':'] = 2;*/
    TT[1]['b'] = 2;
    TT[1][7] = 7; // rare char
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
    } else if(isspace(currentCh) || currentCh < 0) {
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
    return 7; //MODIFICAR POR EL TOKEN ID DE LOS IDENTIFICADORES DE NUESTRA NUEVA TABLA DE TRANCISIONES
}

// Se obtiene el id del identificador que ya esta en la tabla de simbolos
int getExistingIdentifier(char* currentWord) {
    for (int i = 0; i < totalOfIdentifiers; i++) {
        if (strcmp(identifierTable[i], currentWord) == 0) {
            return identifierTableNum[i];
        }
    }
    return -1; 
}

// Se inserta un nuevo identificador a la tabla de simbolos
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

    if(id == 7){ //CAMBIAR AL ID DE LOS IDENTIFICADORES DE LA NUEVA TABLA
        int idIdentifier = insertIdentifier(palabra);
        sprintf(token, "<%d,%d>", id, idIdentifier);
        fprintf(tokenFile, "%s\n", token);
    }else{
        sprintf(token, "<%d>", id);
        fprintf(tokenFile, "%s\n", token);
    }
}

// Saber si el estado acutal es de aceptación
bool Acceptance(int state){
    /*if(state == 0 || state == 1 || state == 7){ //CAMBIAR ESTADO DE ERROR
        return false;
    }

    return true;*/
    return state >=2 && state<= 6;
}

// Saber si el estado actual es de error
bool Error(int state){
    /*if(state == 7){
        return true;
    }
    return false;*/
    return state == 7;
}

// Saber si en el estado actual puede proseguir a guardar el caracter
bool Advance(int state, char ch){
    /*if((ch == 'l' || ch == 'd' || ch == '_') && (state == 0 || state == 1)){
        return true;
    }
    return false;*/
    return (ch == 'l' || ch == 'd' || ch == '_') && (state == 0 || state == 1);
}

// Main del analizador léxico
int main(){
    // Se abren los archivos
    // proceesFile = Archivo por procesar y revisar obtener los tokens
    // tokenFile = Se insertaran los tokens ids
    FILE *processFile;
    FILE *tokenFile;
    processFile = fopen("./casos_de_prueba/ExampleC.txt", "r");
    tokenFile = fopen("tokens.txt", "w");

    StartTable();

    // Se necesita obtener el primer carácter del archivo
    int currentCh = fgetc(processFile);

    // Ciclo while que sigue hasta que termine el archivo
    while (currentCh != EOF)
    {
        int state = 0;  /* start */
        char palabra[200] = "";
        int index = 0;

        // Ciclo while que sigue hasta que el estado sea aceptor o erroneo
        while (!Acceptance(state) && !Error(state)){
            char ch = classify(currentCh);
            state = TT[state][ch];
            
            if(ch == 'b'){
                currentCh = fgetc(processFile);
                if(currentCh == EOF) break;
            }else if(Advance(state, ch)){
                palabra[index++] = currentCh;
                currentCh = fgetc(processFile);
            }
        }

        // Si el estado es aceptor se guardara el carcter especial o la palabra acumulada
        if(Acceptance(state)){
            if(state > 2 && state < 7){ //MODIFICAR EL ESTADO DE ERROR
                palabra[index++] = currentCh;
                palabra[index] = '\0';
                recordToken(palabra, tokenFile);
                currentCh = fgetc(processFile);
            }else{
                palabra[index] = '\0';
                recordToken(palabra, tokenFile);
            }
        // Si es estado de error seguira con el siguiente caracter sin guardar
        }else if(Error(state)){
            currentCh = fgetc(processFile);
        }

        
    }

    // Se cierran los archivos
    fclose(processFile);
    fclose(tokenFile);
}