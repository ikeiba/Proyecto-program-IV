#include "config.h"
#include <stdio.h>
#include <string.h>

Config leerConfi(char *nombrefichero) {
    FILE *punterofichero;
    Config con;

    char linea[100];
    char clave[TAM];
    char valor[TAM];

    punterofichero = fopen(nombrefichero, "r");

    if (punterofichero != (FILE*) NULL) {
        while (fgets(linea,100,punterofichero) != NULL) { //lee 100 lineas como mucho y lo guarda en el puntero si queremos que lea mas lo cambiamos, dara null en todo caso si se pasa
            sscanf(linea, "%s = %s", clave, valor);  //he guardado admin(clave) = Alvaro(valor) (ejemplo)
            if (strncmp(clave, "nombreBD", 8)==0) //el numero es las letras que tiene la palabra nombreBD, y esa linea dice que si la variable campo tiene nombreBD
            {
                strcpy(con.nombreBD, valor); //copia el valor y lo mete en valor
            }else if (strncmp(clave, "administrador", 13)==0)
            {
                strcpy(con.administrador, valor);
            }else if (strncmp(clave, "contrasena", 10)==0)
            {
                strcpy(con.contrasena, valor);
            }
        }
        fclose(punterofichero);
    }
    return con;
    
}