#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu\menu.h"
#include "baseDatos\sqlite3.h"
#include "baseDatos\baseDatos.h"
#include "utils\config.h"
//#include "logger.h"

// COMANDO PARA COMPILAR: 
//gcc -I src -I src/baseDatos -I src/menu -I src/utils src\*.c src\baseDatos\*.c src\utils\*.c src\menu\*.c -o main.exe

int main(){
    crearBD();
    printf("\n\n%d\n",comprobarCredenciales("iker", "ibarrola"));
    printf("\n\n%d\n",insertarAdministrador("xabi", "xabi@em.com","658465921","12-12-12",3,"1234"));
    printf("\n\n%d\n",insertarUsuario("Chaff", "chaff@em.com","89491849","11-11-11","0987"));
    printf("\n\n%d\n",cambiarTelefonoUsuario("chaff@em.com", "123456789"));
    printf("\n\n%d\n",cambiarNombreUsuario("chaff@em.com", "ChaffBobo"));
    printf("\n\n%d\n",borrarUsuario("chaff@em.com"));

    administracion();

    /*
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
            //writeLog("Autenticación exitosa");
        } else {
            printf("Clave incorrecta\n");
            //writeLog("Error de autenticación: clave incorrecta");
        }
    } else {
        printf("Usuario no reconocido\n");
        //writeLog("Error de autenticación: usuario incorrecto");
    }*/
    return 0;
} 