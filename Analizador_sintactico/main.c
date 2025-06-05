#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// C requiere definir las funciones antes
// debido a que se hacen llamada a ellas mismas
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

// Valores globales:
//   banderas
//   tokens para calcular porcentaje
//   archivos a utilizar

bool banderaOOP = false;
bool banderaPro = false;

int totalTokens = 0;
int currentTokens = 0;

FILE* file;
FILE* saveFile;

int token = 0;


/*
Función para non-terminal programa
<programa> → <clases> <programa>  
            | <funciones> <programa>
            | <variable> <programa>
            | ε

*/
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

/*
Función para non-terminal clases
<clases> →  class <variable> { <funciones> } <clases'>
*/
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

/*
Función para non-terminal clases'
<clases’> → <clases> 
          | <variable> main () { <instrucciones> }
          | ε
*/
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

/*
Función para non-terminal funciones
<funciones> →  <variable> (<variable>) { <instrucciones> } <funciones'>
*/
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

/*
Función para non-terminal funciones'
<funciones’> → <funciones> 
            | ε
*/
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

/*
Funciones para non-terminal instrucciones
<instrucciones>  →  <variable> { <instrucciones'> }
                | <funciones>
                | <contenido>
                |  ε
*/
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

/*
Funciones para non-terminal instrucciones'
<instrucciones’> → { <instrucciones> }
           |  ε
*/
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

/*
Funciones para non-terminal contenido
<contenido> → <clases><contenido> 
      | <funciones><contenido>
      | ε
*/
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

/*
Funciones para non-terminal variable
<variable> →  identificador <variables> 
*/
bool variable(){
    if(token == 7){
        if(!match(7)) return false;
        if(!variable_(token)) return false;
        return true;
    }else{
     error_();
    }
}

/*
Funciones para non-terminal variable'
<variable’> → <variable>
  | ε
*/
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

// función para impresión del error
void error_(){
    int porcentaje = (currentTokens / totalTokens) * 100;
    printf("Estamos un %d por ciento seguros que el código es: ", porcentaje);
    if(banderaOOP && banderaPro){
        printf("Hibrido");
    }else if(banderaOOP){
        printf("Orientado a Objetos");
    }else if(banderaPro){
        printf("Procedimental");
    }else{
        printf("Texto plano");
    }
}

// Función para limpiar el string del token y solo obtener un número
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

// Función que hace le match con el token esperado y pasa al siguiente si es correcto
bool match(int expected){
    if(token == expected){
        char line[256];
        if(fgets(line, sizeof(line), file)){
            limpiarToken(line);
            currentTokens = currentTokens + 1;
        }else{
            token = 0;
        }
        return true;
    }

    return false;
}

// Para saber el numero de lineas en el archivo
void contarLineas() {
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        totalTokens++;
    }
}

int main() {
    char line[256];
    char ruta[256];

    printf("Ingresa la ruta del archivo de tokens a analizar: ");
    fgets(ruta, sizeof(ruta), stdin);
    ruta[strcspn(ruta, "\n")] = '\0';

    contarLineas();
    fgets(line, sizeof(line), file);
    limpiarToken(line);

    programa(token);

    printf("El código a analizar es un código: ");
    if(banderaOOP && banderaPro){
        printf("Hibrido");
    }else if(banderaOOP){
        printf("Orientado a Objetos");
    }else if(banderaPro){
        printf("Procedimental");
    }else{
        printf("Texto plano");
    }

    fclose(file);
    fclose(saveFile);
}