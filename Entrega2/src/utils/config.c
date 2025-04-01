#include "config.h"
#include <stdio.h>
#include <string.h>

Config config;

void leerConfig() {
    char* nombrefichero = "src\\utils\\configuracion.config";
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
            }else if (strncmp(clave, "cleanBBDD", 9)==0)
            {
                strcpy(con.cleanBBDD, valor);
            }else if (strncmp(clave, "createBBDD", 10)==0)
            {
                strcpy(con.createBBDD, valor);
            }else if (strncmp(clave, "deleteBBDD", 10)==0)
            {
                strcpy(con.deleteBBDD, valor);
            }else if (strncmp(clave, "loadCSV", 7)==0)
            {
                strcpy(con.loadCSV, valor);
            }else if (strncmp(clave, "loggerBD", 8)==0)
            {
                strcpy(con.loggerBD, valor);
            }else if (strncmp(clave, "CSVconversaciones", 17)==0)
            {
                strcpy(con.CSVconversaciones, valor);
            }else if (strncmp(clave, "CSVgrupos", 9)==0)
            {
                strcpy(con.CSVgrupos, valor);
            }else if (strncmp(clave, "CSVmensajes", 11)==0)
            {
                strcpy(con.CSVmensajes, valor);
            }else if (strncmp(clave, "CSVusuarios", 11)==0)
            {
                strcpy(con.CSVusuarios, valor);
            }
        }
        fclose(punterofichero);
    }
    config = con;
}