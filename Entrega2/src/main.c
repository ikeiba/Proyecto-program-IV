#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu/menu.h"
#include "baseDatos/sqlite3.h"
#include "baseDatos/baseDatos.h"
#include "utils/config.h"
#include "estructuras.h"
#include "data/csvReader.h"
#include "data/csvWrite.h"
//#include "logger.h"

// COMANDO PARA COMPILAR: 
// WINDOWS:
// gcc -I src -I src/baseDatos -I src/data -I src/menu -I src/utils src\*.c src\baseDatos\*.c src\data\*.c src\utils\*.c src\menu\*.c -o main.exe
// Usuario y constraseña: admin
int main(){
    leerConfig();
    borrarTablas();
    crearBD();

    exportarBD_CSV();
    //! INICIO USUARIOS (carga desde csv e insercion en la base de datos) 
    // Carga los usuarios del csv a un array de usuarios
    Usuario* usuarios = leerCsvUsuarios();

    //! INICIO GRUPOS (carga desde csv e insercion en la base de datos)
    // Carga los grupos del csv a un array de grupos
    Grupo* grupos = leerCsvGrupos(usuarios);

    //! INICIO MENSAJES (carga desde csv e insercion en la base de datos)
    Mensaje* mensajes = leerCsvMensajes(usuarios, grupos);

    // INICIO CONVERSACIONES (insertar usuarios en el array de miembros de los grupos)
    leerCsvConversaciones(usuarios, grupos);

    //! CARGA DATOS A BASE DE DATOS
    cargarDatosCsvEnBD(usuarios, grupos, mensajes);

    liberarMensajes(mensajes, 530);

    liberarGrupos(grupos, 67);

    liberarUsuarios(usuarios, 50);
    
    //menuMorrarLog();

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