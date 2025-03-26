#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdio.h>


void registrarMensaje(char *texto) {
    FILE *archivo;
    time_t momentoActual;
    char *fechaStr;

    archivo = fopen(FICHERO_LOG, "a"); //modo añadir al final
    if (archivo != NULL) {
        momentoActual = time(NULL);  //Obtener la fecha y hora actual
        fechaStr = ctime(&momentoActual);
        fechaStr[strcspn(fechaStr, "\n")] = '\0'; //eliminar el salto de línea
        fprintf(archivo, "%s - %s\n", fechaStr, texto);
        fclose(archivo);
    }
}

void mostrarMensaje(char *texto) {
    time_t momentoActual;
    char *fechaStr;

    momentoActual = time(NULL);
    fechaStr = ctime(&momentoActual);
    printf("%s - %s\n", fechaStr, texto);
}