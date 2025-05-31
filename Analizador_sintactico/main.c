#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool programa();
bool clases();
bool clases_();
bool funciones();
bool funciones_();
bool instrucciones();
bool instrucciones_();
bool contenido();
bool variable();
bool variable_();
bool match(int expected);
void limpiarToken(const char* line);
void error_();


char* TokenIds[] = {
    "class", "main", "{", "}", 
    "(", ")"
};

bool banderaOOP = false;
bool banderaPro = false;

int totalTokens = 0;
int currentTokens = 0;

FILE* file;
FILE* saveFile;

int token = 0;

bool programa(){
    if(token == 1){
        if(!clases(token)) return false;
        banderaOOP = true;
        if(!programa(token)) return false;
        return true;
    }else if(token == 7){
        if(funciones(token)){
            if(!funciones(token)) return false;
            banderaPro = true;
            if(!programa(token)) return false;
        }else if(variable(token)){
            if(!variable(token)) return false;
            if(!programa(token)) return false;
        }
        return true;
    }else if(token == 0){
        return true;
    }else{
     error_();
    }
}

bool clases(){
    if(token == 1){
        if(!match(1)) return false;
        if(!variable(token)) return false;
        if(!match(3)) return false;
        if(!funciones(token)) return false;
        if(!match(4)) return false;
        if(!clases_(token)) return false;
        return true;
    }else{
     error_();
    }
}

bool clases_(){
    if(token == 1){
        if(!clases(token)) return false;
    }else if(token == 7){
        if(!variable(token)) return false;
        if(!match(2)) return false;
        if(!match(5)) return false;
        if(!match(6)) return false;
        if(!match(3)) return false;
        if(!instrucciones(token)) return false;
        if(!match(4)) return false;
        return true;
    }else if(token == 4 || token == 0){
        return true;
    }else{
     error_();
    }
}

bool funciones(){
    if(token == 7){
        if(!variable(token)) return false;
        if(!match(5)) return false;
        if(!variable(token)) return false;
        if(!match(6)) return false;
        if(!match(3)) return false;
        if(!instrucciones(token)) return false;
        if(!match(4)) return false;
        if(!funciones_(token)) return false;
        return true;
    }else{
     error_();
    }
}

bool funciones_(){
    if(token == 7){
        if(!funciones(token)) return false;
        return true;
    }else if(token == 1 || token == 4 || token == 0){
        return true;
    }else{
     error_();
    }
}

bool instrucciones(){
    if(token == 7){
        if(!variable(token)) return false;
        if(!match(3)) return false;
        if(!instrucciones_(token)) return false;
        if(!match(4)) return false;
        if(!funciones(token)) return false;
        return true;
    }else if(token == 1){
        if(!contenido(token)) return false;
        return true;
    }else if(token == 4){
        return true;
    }else{
     error_();
    }
}

bool instrucciones_(){
    if(token == 3){
        if(!instrucciones(token)) return false;
        if(!match(4)) return false;
        return true;
    }else if(token == 4){
        return true;
    }else{
     error_();
    }
}

bool contenido(){
    if(token == 1){
        if(!clases(token)) return false;
        if(!contenido(token)) return false;
        return true;
    }else if(token == 7){
        if(!funciones(token)) return false;
        if(!contenido(token)) return false;
        return true;
    }else if(token == 4){
        return true;
    }else{
     error_();
    }
}

bool variable(){
    if(token == 7){
        if(!match(7)) return false;
        if(!variable_(token)) return false;
        return true;
    }else{
     error_();
    }
}

bool variable_(){
    if(token == 7){
        if(!variable(token)) return false;
        return true;
    }else if(token == 1 || token == 7 || token == 0 || token == 3 || token == 2 || token == 5 || token == 4 || token == 6){
        return true;
    }else{
     error_();
    }
}


void error_(){
    int porcentaje = (currentTokens / totalTokens) * 100;
}

void limpiarToken(const char* line) {
    int j = 0;
    char clean[256];

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            break;
        }
        if (line[i] != '<' && line[i] != '>') {
            clean[j++] = line[i];
        }
    }
    clean[j] = '\0';
    sscanf(clean, "%d", &token);
}

bool match(int expected){
    if(token == expected){
        char line[256];
        if(fgets(line, sizeof(line), file)){
            limpiarToken(line);
        }else{
            token = 0;
        }
        return true;
    }

    return false;
}

int main() {
    char line[256];

    file = fopen("./test_case3.txt", "r");
    saveFile = fopen("SaveFile.txt", "w");

    fgets(line, sizeof(line), file);
    limpiarToken(line);

    programa(token);

    if(banderaOOP && banderaPro){
        printf("Es hibrido\n");
    }

    if(banderaOOP){
        printf("Es oop\n");
    }else if(banderaPro){
        printf("Es procedimental\n");
    }else{
        printf("Es texto\n");
    }

    fclose(file);
    fclose(saveFile);
}