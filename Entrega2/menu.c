#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"



void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



// Menu
void administracion(void){
    char strIn[2];  

    printf(
        "\n------------ADMINISTRACION------------\n"
        "Inicio Sesion: 1\n"
        "Salir: 0\n"
        "(Para salir tambien puedes usar CTRL + C)\n"
        "--------------------------------------\n\n"
    );

    
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        administracion();
    } else if (strIn[0] == '1') {
        inicioSesion();        
    } else if (strIn[0] == '0') {
        printf("Saliendo...\n\n");
    } else {
        printf("Opción no válida.\n\n");
        administracion();
    }
}


void inicioSesion(void){
    char strEmail[50];  
    char strContr[50];
    bool porImplementar =true;

    printf(
        "\n-----------INICIO DE SESION----------\n"
        "Email: 1\n"
        "Contraseña: 0\n"
        "--------------------------------------\n\n"
    );

    fgets(strEmail, sizeof(strEmail), stdin);
    fgets(strContr, sizeof(strContr), stdin);


    if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {  
        printf("Email y contraseña correctos\n");
        menu();
    } else if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {
        printf("Email incorrecto\n");
        inicioSesion();
    } else if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {
        printf("Contraseña incorrecta\n");
        inicioSesion();
    } else {
        printf("Opción no válida.\n\n");
        inicioSesion();
    }
}


void menu(void){
    char strIn[2];  

    printf(
        "\n-----------------MENU-----------------\n"
        "Administracion de usuarios: 1\n"
        "Reiniciar servidor: 2\n"
        "Ver logs: 3\n"
        "Ejecucion SQL: 4\n"
        "Registrar administradores: 5\n"
        "Salir: 0\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        menu();
    } else if (strIn[0] == '1') {
        eleccionUsuario();
    } else if (strIn[0] == '2') {
        reinicioServer();
    } else if (strIn[0] == '3') {
        logs();         
    } else if (strIn[0] == '4') {
        sql();
    } else if (strIn[0] == '5') {
        registrarAdmin();       
    } else if (strIn[0] == '0') {
        printf("Saliendo...\n\n");
    } else {
        printf("Opción no válida.\n\n");
        menu();
    }
};



// Server
void reinicioServer(void){
    char strIn[2];  

    printf(
        "\n----------REINICIAR SERVER----------\n"
        "¿Estas seguro de que quieres reiniciar el servidor?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        reinicioServer();
    } else if (strIn[0] == 'y') {
        printf("Server reiniciado");
        menu();
        //Por implementar      
    } else if (strIn[0] == 'n') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        reinicioServer();
    }
};



// Usuarios
void eleccionUsuario(void){
    char strIn[2];   

    printf(
        "\n--------ELECCION DE USUARIOS--------\n"
        "Insertar nombre de usuario: (0 para volver)\n"
        "--------------------------------------\n\n");


    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '0') {
        menu();
    } else if (strIn[0] == '1') {
        printf("Nombre correcto\n");
        adminUsuarios();        
    } else {
        printf("Este nombre no corresponde a ningun usuario\n");
        eleccionUsuario();
    }
    /*Mirar si esta en la BD (Por implementar)*/

};


void adminUsuarios(void){
    char strIn[2];  

    printf(
        "\n-----ADMINISTRACION DE USUARIOS-----\n"
        "Modificar datos del usuario: 1\n"
        "Borrar usuario: 2\n"
        "Bloquear usuario: 3\n"
        "Volver: 0\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        adminUsuarios();
    } else if (strIn[0] == '1') {
        modificarUsuarios();
        
    } else if (strIn[0] == '2') {
        borrarUsuario();

    } else if (strIn[0] == '3') {
        bloquearUsuario();
        
    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        adminUsuarios();
    }
};


void modificarUsuarios(void){
    char strIn[2];  

    printf(
        "\n----MODIFICAR DATOS DEL USUARIO----\n"
        "Modificar nombre: 1\n"
        "Modificar tlf: 2\n"
        "Borrar foto de perfil: 3\n"
        "Volver: 0\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        modificarUsuarios();
        clearInputBuffer();

    } else if (strIn[0] == '1') {
        modificarNombre();
        clearInputBuffer();

    } else if (strIn[0] == '2') {
        modificarTlf();
        clearInputBuffer();

    } else if (strIn[0] == '3') {
        //Borrar foto de perfil
        
    } else if (strIn[0] == '0') {
        adminUsuarios();
        clearInputBuffer();

    } else {
        printf("Opción no válida.\n\n");
        modificarUsuarios();
        clearInputBuffer();

    }
};


void modificarNombre(void){
    char strNombre[50];  

    printf(
        "\n----------MODIFICAR NOMBRE----------\n"
        "Insertar nuevo nombre: (dejar vacio para salir)\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strNombre, sizeof(strNombre), stdin);

    if (strNombre[0] == '\n') {  
        modificarUsuarios();
    } else {
        printf("Nombre modificado con exito\n\n");
        modificarUsuarios();
    }
};


void modificarTlf(void){
    char strTlf[20];  

    printf(
        "\n---------MODIFICAR TELEFONO---------\n"
        "Insertar nuevo telefono: (dejar vacio para salir)\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strTlf, sizeof(strTlf), stdin);

    if (strTlf[0] == '\n') {  
        modificarUsuarios();
    } else {
        printf("Telefono modificado con exito\n\n");
        modificarUsuarios();
    }
};


void borrarUsuario(void){
    char strIn[2];  

    printf(
        "\n----------BORRAR USUARIO----------\n"
        "¿Estas seguro de que quieres borrar este usuario?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );

    
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        borrarUsuario();
    } else if (strIn[0] == 'y') {
        printf("Usuario borrado");
        menu();
        //Por implementar      
    } else if (strIn[0] == 'n') {
        modificarUsuarios();
    } else {
        printf("Opción no válida.\n\n");
        borrarUsuario();
    }
};


void bloquearUsuario(void){
    char strIn[2];  

    printf(
        "\n----------BLOQUEAR USUARIO----------\n"
        "¿Estas seguro de que quieres bloquear este usuario?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        bloquearUsuario();
    } else if (strIn[0] == 'y') {
        printf("Usuario bloqueado");
        menu();
        //Por implementar      
    } else if (strIn[0] == 'n') {
        modificarUsuarios();
    } else {
        printf("Opción no válida.\n\n");
        bloquearUsuario();
    }
};



// Logs
void logs(void){
    char strIn[2];  

    printf(
        "\n-----------------LOGS-----------------\n"
        "Log base de datos: 1\n"
        "Log servidor: 2\n"
        "Borrar log base de datos: 3\n"
        "Borrar log servidor: 4\n"
        "Salir: 0\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        logs();
    } else if (strIn[0] == '1') {
        printf("Log base de datos\n\n");

    } else if (strIn[0] == '2') {
        printf("Log servidor\n");

    } else if (strIn[0] == '3') {
        borrarLog();
    } else if (strIn[0] == '4') {
        borrarLog();
    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        logs();
    }
};


void borrarLog(void){
    char strIn[2];  

    printf(
        "\n-------------BORRAR LOG-------------\n"
        "¿Estas seguro de que quieres borrar el log?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        borrarUsuario();
    } else if (strIn[0] == 'y') {
        printf("Log borrado exitosamente");
        //Por implementar      
    } else if (strIn[0] == 'n') {
        modificarUsuarios();
    } else {
        printf("Opción no válida.\n\n");
        borrarUsuario();
    }
};



// SQL
void sql(void){
    char strIn[2];  

    printf(
        "\n-----------------SQL-----------------\n"
        "Select: 1\n"
        "Modify: 2\n"
        "Delete: 3\n"
        "Salir: 0\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        sql();
    } else if (strIn[0] == '1') {
        printf("Select\n\n");

    } else if (strIn[0] == '2') {
        printf("Modify\n");

    } else if (strIn[0] == '3') {
        printf("Delete\n");

    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        sql();
    }
};



// Registro Admins
void registrarAdmin(void){
    char strEmail[50];  
    char strContr[50];
    char strRango[2];
    bool porImplementar = true;

    printf(
        "\n------REGISTRAR ADMINISTRADOR-------\n"
        "Email: 1\n"
        "Contraseña: 0\n"
        "Rango (1-3):\n"  
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strEmail, sizeof(strEmail), stdin);
    fgets(strContr, sizeof(strContr), stdin);
    fgets(strRango, sizeof(strRango), stdin);


    if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {  
        printf("Datos introducidos con exito\n");
        menu();
    } else if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {
        printf("Email incorrecto\n");
        registrarAdmin();
    } else if (/*Mirar si esta en la BD (Por implementar)*/porImplementar) {
        printf("Contraseña incorrecta\n");
        registrarAdmin();
    } else {
        printf("Opción no válida.\n\n");
        registrarAdmin();
    }
};


void confirmarAdmin(void){
    char strIn[2];  

    printf(
        "\n-------CONFIRMAR ADMINISTRADOR-------\n"
        "¿Estas seguro de que quieres crear este administrador?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );

    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        confirmarAdmin();
    } else if (strIn[0] == 'y') {
        printf("Administrador registrado con exito");
        menu();
        //Por implementar      
    } else if (strIn[0] == 'n') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        confirmarAdmin();
    }
};



