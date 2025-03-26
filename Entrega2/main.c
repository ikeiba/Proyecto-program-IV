#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "sqlite3.h"
#include "baseDatos.h"
#include "config.h"
#include "logger.h"
int main(){
    crearBD();
    printf("\n\n%d\n",comprobarCredenciales("iker", "ibarrola"));
    administracion();
    
    Config configData;
    char usuario[TAM], clave[TAM];

    configData = leerConfig("configuracion.config");
    printf("Usuario Administrador: %s\n", configData.administrador);
    printf("Clave de Acceso: %s\n", configData.contrasena);
    printf("Introduce el nombre del usuario: ");
    gets(usuario);
    if (strcmp(usuario, configData.administrador) == 0) {
        printf("Ingrese su clave: ");
        gets(clave);

        if (strcmp(clave, configData.contrasena) == 0) {
            printf("Acceso concedido\n");
            writeLog("Autenticación exitosa");
        } else {
            printf("Clave incorrecta\n");
            writeLog("Error de autenticación: clave incorrecta");
        }
    } else {
        printf("Usuario no reconocido\n");
        writeLog("Error de autenticación: usuario incorrecto");
    }
    return 0;
} 