#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "config.h"


extern Config config;

void registrarMensaje(const char *formato, ...) {
    FILE *archivo;
    time_t momentoActual;
    char *fechaStr;
    char mensaje[512];  // Buffer para el mensaje formateado
    
    archivo = fopen(config.loggerBD, "a"); // modo añadir al final
    if (archivo != NULL) {
        // Obtener la fecha y hora actual
        momentoActual = time(NULL);
        fechaStr = ctime(&momentoActual);
        fechaStr[strcspn(fechaStr, "\n")] = '\0'; // Eliminar el salto de línea

        // Procesar argumentos variables
        va_list args;
        va_start(args, formato);
        vsnprintf(mensaje, sizeof(mensaje), formato, args);
        va_end(args);

        // Escribir en el archivo de log
        fprintf(archivo, "%s - %s\n", fechaStr, mensaje);
        fclose(archivo);
    }
}

void mostrarMensaje(int n) {
    FILE *archivo;
    char lineas[1000][512]; 
    int countadorLineas = 0;

    // Al haber espacios entre cada linea del .log, hay que decirle que muestre el doble de lineas
    n = 2 * n;

    //leer el archivo línea por linea 
    archivo = fopen(config.loggerBD, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de log");
        return;
    }
    
    while (fgets(lineas[countadorLineas % 1000], sizeof(lineas[0]), archivo)) {
        countadorLineas++;
    }
    fclose(archivo);

    //calcular el numero de lineas que quiero enseñar
    int empezar;
    if (countadorLineas > n) {
        empezar = countadorLineas - n;
    } else {
        empezar = 0;
    }
    
    //imprimir las últimas n lineas
    for (int i = empezar; i < countadorLineas; i++) {
        printf("%s", lineas[i % 1000]);
    }
       
}
    


void borrarLog() 
{
    if (remove(config.loggerBD) == 0) {
        printf("Archivo borrado con exito\n");
    } else {
        perror("Error al borrar el archivo");
    }
}