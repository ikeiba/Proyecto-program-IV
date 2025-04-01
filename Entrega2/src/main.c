#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include "menu/menu.h"
#include "baseDatos/sqlite3.h"
#include "baseDatos/baseDatos.h"
#include "utils/config.h"
#include "estructuras.h"
#include "data/csvReader.h"
#include "data/csvWrite.h"
//#include "logger.h"

// COMANDO PARA COMPILAR WINDOWS: 
// gcc -I src -I src/baseDatos -I src/data -I src/menu -I src/utils src\*.c src\baseDatos\*.c src\data\*.c src\utils\*.c src\menu\*.c -o main.exe
// Usuario y constraseña: admin

void configurarConsola();

int main(){
    configurarConsola();
    leerConfig();
    borrarTablas();
    crearBD();
    
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

    insertarAdministrador("nombreAdmin", "admin", "666666666", "1999-10-12", 5, "admin");

    liberarMensajes(mensajes, 530);

    liberarGrupos(grupos, 67);

    liberarUsuarios(usuarios, 50);
    
    //menuMorrarLog();

    administracion();

    return 0;
} 


void configurarConsola()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    system("chcp 65001 > nul");
    setlocale(LC_ALL, ".UTF-8");
#endif
}