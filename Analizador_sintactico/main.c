#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

char* TokenIds[] = {
    "class", "main", "{", "}", 
    "(", ")"
};

bool banderaOOP = false;
bool banderaPro = false;

bool programa(int token){
    if(token == 1){
        bool class_production = clases(token);
        bool programa_recursion = programa(token);
    }else if(token == 7){
        bool funcion_production = funciones(token);
        bool variable_production = variable(token);

        bool programa_recursion = programa(token);
    }else{
        Error();
    }
}

bool clases(int token){
    if(token == 1){
        bool variable_prodcuction = variable(token);
        if(token != 3) return false;
        bool funciones_production = funciones(token);
        if(token != 4) return false;
        bool clases_recursion = clases_(token);

        return true;
    }else{
        Error();
    }
}

bool clases_(int token){
    if(token == 1){
        bool classes_production = clases(token);
    }else if(token == 7){
        bool variable_production = variable(token);
        if(token != 2) return false;
        if(token != 5) return false;
        if(token != 6) return false;
        if(token != 3) return false;
        bool instrucciones_production = instrucciones(token);
        if(token != 4) return false;

        return true;
    }else{
        Error();
    }
}

bool funciones(int token){
    if(token == 7){
        bool variable_production = variable(token);
        if(token != 5) return false;
        bool variable_production2 = variable(token);
        if(token != 6) return false;
        if(token != 3) return false;
        bool instrucion_production = instrucciones(token);
        if(token != 4) return false;
        bool funcion_recursive = funciones_(token);

        return true;
    }else{
        Error();
    }
}

bool funciones_(int token){
    if(token == 7){
        bool funciones_production = funciones(token);
    }else{
        Error();
    }
}

bool instrucciones(int token){
    if(token == 7){
        bool variable_production = variable(token);
        if(token != 3) return false;
        bool instruction_production = instrucciones_(token);
        if(token != 4) return false;

        bool function_production = funciones(token);
    }
}

bool instrucciones_(){
    return false;
}

bool contenido(){
    return false;
}

bool variable(){
    return false;
}

bool variable_(){
    return false;
}

void Error(){

}

void limpiarToken(const char* line, char* cleaned) {
    int j = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            break;
        }
        if (line[i] != '<' && line[i] != '>') {
            cleaned[j++] = line[i];
        }
    }

    cleaned[j] = '\0';
}

bool match(int num){

}

int main() {
    FILE *file;
    char line[256];
    char token[256];

    file = fopen("./test_case3.txt", "r");

    while (fgets(line, sizeof(line), file)) {
        limpiarToken(line, token);

    }

    fclose(file);
    return 0;
}