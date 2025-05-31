#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define NUM_STATES 8
#define NUM_CHAR 255
#define MAX 100

int TT[NUM_STATES][NUM_CHAR];

char* TokenIds[] = {
    "class", "main", "{", "}", "(", ")"
};

int TokenIdsNums[] = {
    1, 2, 3, 4, 5, 6
};

char* identifierTable[MAX];
int identifierTableNum[MAX];
int totalOfIdentifiers = 0;

void StartTable(){
    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_CHAR; j++) {
            TT[i][j] = 7; // Estado de error
        }
    }

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
    TT[1]['b'] = 2;
    TT[1]['{'] = 2;
    TT[1]['}'] = 2;
    TT[1]['('] = 2;
    TT[1][')'] = 2;
}

char classify(char currentCh){
    if (isalpha(currentCh)) return 'l';
    else if (isdigit(currentCh)) return 'd';
    else if (isspace(currentCh) || currentCh < 0) return 'b';
    return currentCh;
}

int getTokenId(char* currentToken){
    for (int i = 0; i < sizeof(TokenIdsNums) / sizeof(int); i++) {
        if (strcmp(TokenIds[i], currentToken) == 0) {
            return TokenIdsNums[i];
        }
    }
    return 7; // identificador
}

int getExistingIdentifier(char* currentWord) {
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

void recordToken(char palabra[200], FILE* tokenFile){
    char token[30];
    int id = getTokenId(palabra);

    if (id == 7) { // identificador
        int idIdentifier = insertIdentifier(palabra);
        sprintf(token, "<%d,%d>", id, idIdentifier);
        fprintf(tokenFile, "%s\n", token);
    } else {
        sprintf(token, "<%d>", id);
        fprintf(tokenFile, "%s\n", token);
    }
}

bool Acceptance(int state){
    return (state >= 2 && state <= 6);
}

bool Error(int state){
    return state == 7;
}

bool Advance(int state, char ch){
    return (ch == 'l' || ch == 'd' || ch == '_') && (state == 0 || state == 1);
}

int main(){
    FILE *processFile = fopen("./casos_de_prueba/ExampleC.txt", "r");
    FILE *tokenFile = fopen("tokens.txt", "w");

    if (!processFile || !tokenFile) {
        printf("Error al abrir los archivos.\n");
        return 1;
    }

    StartTable();
    int currentCh = fgetc(processFile);

    while (currentCh != EOF) {
        int state = 0;
        char palabra[200] = "";
        int index = 0;

        while (!Acceptance(state) && !Error(state)) {
            char ch = classify(currentCh);
            state = TT[state][ch];

            if (ch == 'b') {
                currentCh = fgetc(processFile);
                if (currentCh == EOF) break;
            } else if (Advance(state, ch)) {
                palabra[index++] = currentCh;
                currentCh = fgetc(processFile);
            } else {
                break;
            }
        }

        if (Acceptance(state)) {
            if (state > 2 && state < 7) {
                palabra[index++] = currentCh;
                palabra[index] = '\0';
                recordToken(palabra, tokenFile);
                currentCh = fgetc(processFile);
            } else {
                palabra[index] = '\0';
                recordToken(palabra, tokenFile);
            }
        } else if (Error(state)) {
            // Guardar palabra si se había comenzado
            if (index > 0) {
                palabra[index] = '\0';
                recordToken(palabra, tokenFile);
                index = 0;
            }
            // Luego ignorar símbolo inválido
            currentCh = fgetc(processFile);
        }
    }

    fclose(processFile);
    fclose(tokenFile);
    return 0;
}