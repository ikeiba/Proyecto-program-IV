#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "..\baseDatos\baseDatos.h"



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


void inicioSesion(void) {
    char *strEmail;  
    char *strContr;

    printf("\n-----------INICIO DE SESION----------\n");

    // Input email
    printf("Email: ");
    clearInputBuffer(); 
    fgets(strEmail, sizeof(strEmail), stdin);
    strEmail[strcspn(strEmail, "\n")] = 0; 

    // Input password
    printf("Contraseña: ");
    fgets(strContr, sizeof(strContr), stdin);
    strContr[strcspn(strContr, "\n")] = 0;


    if (comprobarCredenciales(strEmail, strContr)) {  
        printf("Email y contraseña correctos\n");
        menu();
    } else {
        printf("Email o contraseña incorrectos. Inténtalo de nuevo.\n");
        inicioSesion();
    }*/
}


void menu(void){
    char strIn[2];  

    printf(
        "\n-----------------MENU-----------------\n"
        "Administracion de usuarios: 1\n"
        "Reiniciar servidor: 2\n"
        "Ver logs: 3\n"
        "Hacer copia de seguridad de BD en CSV: 4\n"
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
        copiaBDMenu();  
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
    char *strIn;   
    
    printf(
        "\n--------ELECCION DE USUARIOS--------\n"
        "Insertar email del usuario: (0 para volver)\n"
        "--------------------------------------\n\n");


    clearInputBuffer();
    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '0') {
        menu();
    } else if (strIn[0] == '1') {
        printf("Nombre correcto\n");
        adminUsuarios();        
    } else {
        printf("Este email no corresponde a ningun usuario\n");
        eleccionUsuario();
    }
    /*Mirar si esta en la BD (Por implementar)*/

};


void adminUsuarios(char *email){
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
        modificarUsuarios(email);
        
    } else if (strIn[0] == '2') {
        menuBorrarUsuario(email);

    } else if (strIn[0] == '3') {
        bloquearUsuario(email);
        
    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        adminUsuarios(email);
    }
};


void modificarUsuarios(char *email){
    char strIn[2];  
    
    printf(
        "\n----MODIFICAR DATOS DEL USUARIO----\n"
        "Modificar nombre: 1\n"
        "Modificar tlf: 2\n"
        "Volver: 0\n"
        "--------------------------------------\n\n"
    );

    fgets(strIn, sizeof(strIn), stdin);

    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        modificarUsuarios(email);
        clearInputBuffer();

    } else if (strIn[0] == '1') {
        modificarNombre(email);
        clearInputBuffer();

    } else if (strIn[0] == '2') {
        modificarTlf(email);
        clearInputBuffer();

    } else if (strIn[0] == '0') {
        adminUsuarios(email);
        clearInputBuffer();

    } else {
        printf("Opción no válida.\n\n");
        modificarUsuarios(email);
        clearInputBuffer();

    }
};


void modificarNombre(char *email){
    char *strNombre;  

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
        if (cambiarNombreUsuario(const char *email, strNombre))
        {
            printf("Nombre modificado con exito\n\n");
            modificarUsuarios(email)
        } else ´{
            printf("No se pudo modificar el nombre\n");
            menu();
        }
    }
};


void modificarTlf(char *email){
    char *strTlf;  

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
        if (cambiarTelefonoUsuario(email, strTlf))
        {
            printf("Telefono modificado con exito\n\n");
            modificarUsuarios(email)
        } else ´{
            printf("No se pudo modificar el telefono\n");
            menu();
        }
    }
};


void menuBorrarUsuario(char *email){
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
        menuBorrarUsuario(email);
    } else if (strIn[0] == 'y') {
        if (borrarUsuario(email);)
        {
            printf("Usuario borrado\n");
            menu();
        } else {
            printf("No se pudo borrar el usuario\n")
            menu();
        }     
    } else if (strIn[0] == 'n') {
        modificarUsuarios(email);
    } else {
        printf("Opción no válida.\n\n");
        menuBorrarUsuario(email);
    }
};


void bloquearUsuario(char *email){
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
        // Por implementar

    } else if (strIn[0] == '2') {
        printf("Por implementar")
        logs();
    } else if (strIn[0] == '3') {
        menuBorrarLog();
    } else if (strIn[0] == '4') {
        printf("Por implementar")
        logs();
    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        logs();
    }
};


void menuBorrarLog(void){
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
        menuBorrarLog();
    } else if (strIn[0] == 'y') {
        borrarLog();
        printf("Log borrado exitosamente");   
    } else if (strIn[0] == 'n') {
        logs();
    } else {
        printf("Opción no válida.\n\n");
        menuBorrarLog();
    }
};



// BD
void copiaBDMenu(void){
    /*
    char strIn[2];  

    printf(
        "\n---------COPIA DE SEGURIDAD---------\n"
        "¿Estas seguro de que quieres hacer una copia de seguridad de la base de datos?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );
    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        copiaBDMenu();
    } else if (strIn[0] == 'y') {
        printf("Copia creada exitosamente");
        //Por implementar      
    } else if (strIn[0] == 'n') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        copiaBDMenu();
    }
        */
}


// Registro Admins
void registrarAdmin(void){
    char *strEmail;  
    char *strContr;
    char *strTlf;
    char *strFechaNacimiento;
    int strNivel;
    char *strNombre;

    

    printf("\n------REGISTRAR ADMINISTRADOR-------\n");

    // Input email
    printf("\nEmail: ");
    clearInputBuffer(); 
    fgets(strEmail, sizeof(strEmail), stdin);
    strEmail[strcspn(strEmail, "\n")] = 0; 

    // Input contra
    printf("\nContrasena: ");
    fgets(strContr, sizeof(strContr), stdin);
    strContr[strcspn(strContr, "\n")] = 0;

    // Input Nombre
    printf("\nNombre: ");
    clearInputBuffer(); 
    fgets(strNombre, sizeof(strNombre), stdin);
    strNombre[strcspn(strNombre, "\n")] = 0; 

    // Input tlf
    printf("\nTelefono: ");
    fgets(strTlf, sizeof(strTlf), stdin);
    strTlf[strcspn(strTlf, "\n")] = 0;


    // Input nivel
    printf("\nNivel: ");
    clearInputBuffer(); 
    fgets(strNivel, sizeof(strNivel), stdin);
    strNivel[strcspn(strNivel, "\n")] = 0; 

    // Input fecha
    printf("\nFecha nacimiento: ");
    fgets(strFechaNacimiento, sizeof(strFechaNacimiento), stdin);
    strFechaNacimiento[strcspn(strFechaNacimiento, "\n")] = 0; 


    printf("\n--------------------------------------\n\n");



    if (insertarAdministrador(strNombre, strEmail, strTlf,strFechaNacimiento, strNivel, strContr);) {  
        printf("Datos introducidos con exito\n");
        menu();
    } else {
        printf("Opción no válida.\n\n");
        registrarAdmin();
    }
};



