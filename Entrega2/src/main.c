#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu\menu.h"
#include "baseDatos\sqlite3.h"
#include "baseDatos\baseDatos.h"
#include "utils\config.h"
#include "estructuras.h"
#include "data\csvReader.h"
//#include "logger.h"

// COMANDO PARA COMPILAR: 
// WINDOWS:
// gcc -I src -I src/baseDatos -I src/data -I src/menu -I src/utils src\*.c src\baseDatos\*.c src\data\*.c src\utils\*.c src\menu\*.c -o main.exe


int main(){
    crearBD();
    printf("\n\n%d\n",comprobarCredenciales("iker", "ibarrola"));
    printf("\n\n%d\n",insertarAdministrador("xabi", "xabi@em.com","658465921","12-12-12",3,"1234"));
    printf("\n\n%d\n",insertarUsuario("xabier", "xabier@gem.com","89491849","11-11-11","0987"));
    printf("\n\n%d\n",cambiarTelefonoUsuario("xabier@gem.com", "123456789"));
    printf("\n\n%d\n",cambiarNombreUsuario("xabier@gem.com", "xabi"));
    printf("\n\n%d\n",borrarUsuario("xabier@gem.com"));

    Usuario* usuarios = leerCsvUsuarios();

    Grupo* grupos = leerCsvGrupos(usuarios);

    Mensaje* mensajes = leerCsvMensajes(usuarios, grupos);

    leerCsvConversaciones(usuarios, grupos);

    for (int i = 0; i < 50; i++) {
        printf("Usuario %d: %d, %s, %s, %s, %s, %s\n",
                i + 1,
                usuarios[i].id,
                usuarios[i].nombre,
                usuarios[i].email,
                usuarios[i].telefono,
                usuarios[i].fNacimiento,
                usuarios[i].contra);
    }

    

    for (int i = 0; i < 10; i++) {
        printf("Grupos %d: %d, %s, %s, %s, %s, %d\n",
                i + 1,
                grupos[i].id,
                grupos[i].creador->nombre,
                grupos[i].fCreacion,
                grupos[i].descripcion,
                grupos[i].nombre,
                grupos[i].size);
    }

    for (int i = 0; i < 10; i++) {
        printf("Mensajes %d: %d, %s, %s, %s, %s, %s\n",
                i + 1,
                mensajes[i].id,
                mensajes[i].contenido,
                mensajes[i].emisor->nombre,
                mensajes[i].fecha,
                mensajes[i].grupo->descripcion,
                mensajes[i].hora);
    }

    printf("Usuario grupo: %s", grupos[4].miembros[0]->nombre);

    liberarMensajes(mensajes, 530);

    liberarGrupos(grupos, 67);

    liberarUsuarios(usuarios, 50);
    menuMorrarLog();
    //administracion();

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