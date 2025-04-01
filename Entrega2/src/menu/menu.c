#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "menu.h"
#include "../baseDatos/baseDatos.h"
#include "../utils/logger.h"
#include "../data/csvWrite.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Limpia hasta el final de la línea
}


bool tlfInvalido(char *tlf) {
    // Verificar que la longitud sea exactamente 9
    if (strlen(tlf) != 9) {
        return true;
    }
    // Verificar que todos los caracteres sean dígitos
    for (int i = 0; i < 9; i++) {
        if (!isdigit(tlf[i])) {
            return true;
        }
    }
    return false; // Teléfono válido
}

bool fechaInvalida(char *fecha) {
    if (strlen(fecha) != 10) {
        return true;
    }
    // Verificar formato YYYY-MM-DD
    for (int i = 0; i < 10; i++) {
        if ((i == 4 || i == 7) && fecha[i] != '-') {
            return true;
        } else if ((i != 4 && i != 7) && !isdigit(fecha[i])) {
            return true;
        }
    }

    // Extraer año, mes y día
    int year = atoi(fecha);
    int month = atoi(fecha + 5);
    int day = atoi(fecha + 8);

    // Validar rango de mes y día
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return true;
    }

    // Validar días según el mes
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return true;
    }
    if (month == 2) {
        // Verificar años bisiestos
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) {
            return true;
        }
    }

    return false; // Fecha válida
}

// Menu
void administracion(void) {
    char strIn[10];  

    while (1) {
        printf(
            "\n------------ADMINISTRACION------------\n"
            "Inicio Sesion: 1\n"
            "Salir: 0\n"
            "(Para salir tambien puedes usar CTRL + C)\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "1") == 0) {
            inicioSesion();
            break;
        } else if (strcmp(strIn, "0") == 0) {
            printf("Saliendo...\n\n");
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}


void inicioSesion(void) {
    char strEmail[100];  
    char strContr[100];

    while (1) {
        printf("\n-----------INICIO DE SESION----------\n");

        // Input email
        printf("Email: ");
        fgets(strEmail, sizeof(strEmail), stdin);
        strEmail[strcspn(strEmail, "\n")] = 0;

        // Input password
        printf("Contraseña: ");
        fgets(strContr, sizeof(strContr), stdin);
        strContr[strcspn(strContr, "\n")] = 0;

        if (comprobarCredenciales(strEmail, strContr)) {  
            printf("Email y contraseña correctos\n");
            menu();
            break;
        } else {
            printf("Email o contraseña incorrectos. Inténtalo de nuevo.\n");
        }
    }
}


void menu(void) {
    char strIn[10];  

    while (1) {
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
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "1") == 0) {
            eleccionUsuario();
        } else if (strcmp(strIn, "2") == 0) {
            reinicioServer();
        } else if (strcmp(strIn, "3") == 0) {
            logs();
        } else if (strcmp(strIn, "4") == 0) {
            copiaBDMenu();
        } else if (strcmp(strIn, "5") == 0) {
            registrarAdmin();
        } else if (strcmp(strIn, "0") == 0) {
            printf("Saliendo...\n\n");
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}



// Server
void reinicioServer(void) {
    char strIn[10];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n----------REINICIAR SERVER----------\n"
            "¿Estas seguro de que quieres reiniciar el servidor?\n"
            "Si: y\n"
            "No: n\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "y") == 0) {
            printf("Server reiniciado\n");
            menu();
            break;
        } else if (strcmp(strIn, "n") == 0) {
            menu();
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}



// Usuarios 
void eleccionUsuario(void) {
    char strIn[100];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n--------ELECCION DE USUARIOS--------\n"
            "Insertar email del usuario: (0 para volver)\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "0") == 0) {
            menu();
            break;
        } else if (getExisteEmail(strIn)) {
            printf("Email correcto\n");
            adminUsuarios(strIn);
            break;
        } else {
            printf("Este email no corresponde a ningun usuario\n");
        }
    }
}


void adminUsuarios(char *email) {
    char strIn[10];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n-----ADMINISTRACION DE USUARIOS-----\n"
            "Modificar datos del usuario: 1\n"
            "Borrar usuario: 2\n"
            "Bloquear usuario: 3\n"
            "Volver: 0\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "1") == 0) {
            modificarUsuarios(email);
            break;
        } else if (strcmp(strIn, "2") == 0) {
            menuBorrarUsuario(email);
            break;
        } else if (strcmp(strIn, "3") == 0) {
            bloquearUsuario(email);
            break;
        } else if (strcmp(strIn, "0") == 0) {
            menu();
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}


void modificarUsuarios(char *email) {
    char strIn[10];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n----MODIFICAR DATOS DEL USUARIO----\n"
            "Modificar nombre: 1\n"
            "Modificar tlf: 2\n"
            "Volver: 0\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "1") == 0) {
            modificarNombre(email);
            break;
        } else if (strcmp(strIn, "2") == 0) {
            modificarTlf(email);
            break;
        } else if (strcmp(strIn, "0") == 0) {
            adminUsuarios(email);
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}


void modificarNombre(char *email) {
    char strNombre[100];  

    while (1) {
        printf(
            "\n----------MODIFICAR NOMBRE----------\n"
            "Insertar nuevo nombre: (dejar vacio para salir)\n"
            "--------------------------------------\n\n"
        );

        fgets(strNombre, sizeof(strNombre), stdin);
        strNombre[strcspn(strNombre, "\n")] = 0;

        if (strlen(strNombre) == 0) {
            modificarUsuarios(email);
            break;
        } else {
            if (cambiarNombreUsuario(email, strNombre)) {
                printf("Nombre modificado con exito\n\n");
                modificarUsuarios(email);
                break;
            } else {
                printf("No se pudo modificar el nombre\n");
                menu();
                break;
            }
        }
    }
}



void modificarTlf(char *email) {
    char strTlf[20];  

    while (1) {
        printf(
            "\n\n---------MODIFICAR TELEFONO---------\n"
            "Insertar nuevo telefono: (dejar vacio para salir)\n"
            "--------------------------------------\n\n"
        );

        fgets(strTlf, sizeof(strTlf), stdin);
        strTlf[strcspn(strTlf, "\n")] = 0;
        if (tlfInvalido(strTlf))
        {
            printf("\nEl telefono introducido es invalido, introduzcalo de nuevo.");
            printf("\nTelefono: ");
            clearInputBuffer();
            fgets(strTlf, sizeof(strTlf), stdin);
            strTlf[strcspn(strTlf, "\n")] = 0;
        }

        if (strlen(strTlf) == 0) {
            modificarUsuarios(email);
            break;
        } else {
            if (cambiarTelefonoUsuario(email, strTlf)) {
                printf("Telefono modificado con exito\n");
                modificarUsuarios(email);
                break;
            } else {
                printf("No se pudo modificar el telefono\n");
                menu();
                break;
            }
        }
    }
}




void menuBorrarUsuario(char *email) {
    char strIn[10];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n----------BORRAR USUARIO----------\n"
            "¿Estas seguro de que quieres borrar este usuario?\n"
            "Si: y\n"
            "No: n\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "y") == 0) {
            if (borrarUsuario(email)) {
                printf("Usuario borrado\n");
            } else {
                printf("No se pudo borrar el usuario\n");
            }
            menu();
            break;
        } else if (strcmp(strIn, "n") == 0) {
            modificarUsuarios(email);
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}


void bloquearUsuario(char *email) {
    char strIn[10];  // Increased buffer size for safety

    while (1) {
        printf(
            "\n----------BLOQUEAR USUARIO----------\n"
            "¿Estas seguro de que quieres bloquear este usuario?\n"
            "Si: y\n"
            "No: n\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "y") == 0) {
            printf("Usuario bloqueado\n");
            menu();
            break;
        } else if (strcmp(strIn, "n") == 0) {
            modificarUsuarios(email);
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}



// Logs
void logs(void) {
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

    fgets(strIn, sizeof(strIn), stdin);
    strIn[strcspn(strIn, "\n")] = 0; // Remove newline character

    if (strIn[0] == '\0') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        logs();
    } else if (strIn[0] == '1') {
        verLogMenu();
    } else if (strIn[0] == '2') {
        printf("Por implementar\n");
        logs();
    } else if (strIn[0] == '3') {
        menuBorrarLog();
    } else if (strIn[0] == '4') {
        printf("Por implementar\n");
        logs();
    } else if (strIn[0] == '0') {
        menu();
    } else {
        printf("Opción no válida. Inténtalo de nuevo.\n\n");
        logs();
    }
}

void menuBorrarLog(void) {
    char strIn[10];  

    while (1) {
        printf(
            "\n-------------BORRAR LOG-------------\n"
            "¿Estas seguro de que quieres borrar el log?\n"
            "Si: y\n"
            "No: n\n"
            "--------------------------------------\n\n"
        );

        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0;

        if (strcmp(strIn, "y") == 0) {
            borrarLog();
            printf("Log borrado\n");
            logs();
            break;
        } else if (strcmp(strIn, "n") == 0) {
            logs();
            break;
        } else {
            printf("Opción no válida. Inténtalo de nuevo.\n\n");
        }
    }
}

void verLogMenu(void) {
    char strIn[10]; 
    int numLines;   

    while (1) {
        printf(
            "\n-------------VER LOGS---------------\n"
            "¿Cuantas lineas de Log quieres ver?: \n"
            "--------------------------------------\n\n"
        );
        clearInputBuffer();
        fgets(strIn, sizeof(strIn), stdin);
        strIn[strcspn(strIn, "\n")] = 0; 

        if (strIn[0] == '\0') {  
            printf("No ingresaste ningún carácter válido.\n\n");
            continue; 
        }

       
        numLines = atoi(strIn);

        if (numLines > 0) {
            mostrarMensaje(numLines); 
            logs(); 
            break;
        } else {
            printf("Por favor, introduce un número válido mayor que 0.\n\n");
        }
    }
}


// BD
void copiaBDMenu(void){
    char strIn[10];  

    printf(
        "\n---------COPIA DE SEGURIDAD---------\n"
        "¿Estas seguro de que quieres hacer una copia de seguridad de la base de datos?\n"
        "Si: y\n"
        "No: n\n"
        "--------------------------------------\n\n"
    );
    fgets(strIn, sizeof(strIn), stdin);
    if (strIn[0] == '\n') {  
        printf("No ingresaste ningún carácter válido.\n\n");
        copiaBDMenu();
    } else if (strIn[0] == 'y') {
        exportarBD_CSV();
        printf("Copia de seguridad realizada\n\n");
        menu();
    } else if (strIn[0] == 'n') {
        menu();
    } else {
        printf("Opción no válida.\n\n");
        copiaBDMenu();
    }
}



// Registro Admins
void registrarAdmin(void) {
    bool tlfValido = true;
    bool fechaValido = true;
    char strEmail[100];  
    char strContr[100];
    char strTlf[20];
    char strFechaNacimiento[20];
    char strNombre[100];

    printf("\n\n------REGISTRAR ADMINISTRADOR-------\n");

    // Input email
    printf("\nEmail: ");
    fgets(strEmail, sizeof(strEmail), stdin);
    strEmail[strcspn(strEmail, "\n")] = 0; 

    // Input contraseña
    printf("\nContrasena: ");
    fgets(strContr, sizeof(strContr), stdin);
    strContr[strcspn(strContr, "\n")] = 0;

    // Input Nombre
    printf("\nNombre: ");
    fgets(strNombre, sizeof(strNombre), stdin);
    strNombre[strcspn(strNombre, "\n")] = 0; 

    // Input tlf
    do {
        printf("\nTelefono (9 digitos): ");
        fgets(strTlf, sizeof(strTlf), stdin);
        strTlf[strcspn(strTlf, "\n")] = 0; // Eliminar salto de línea
        tlfValido = tlfInvalido(strTlf);
        if (tlfValido) {
            printf("\nEl telefono introducido es invalido, introduzcalo de nuevo.");
        }
    } while (tlfValido);

    // Input fecha
    do {
        printf("\nFecha nacimiento (YYYY-MM-DD): ");
        fgets(strFechaNacimiento, sizeof(strFechaNacimiento), stdin);
        strFechaNacimiento[strcspn(strFechaNacimiento, "\n")] = 0; 
        fechaValido = fechaInvalida(strFechaNacimiento);
        if (fechaValido) {
            printf("\nLa fecha introducida es invalida, introduzcala de nuevo.");
        }
    } while (fechaValido);

    printf("\n--------------------------------------\n\n");

    if (insertarAdministrador(strNombre, strEmail, strTlf, strFechaNacimiento, 1, strContr)) {  
        printf("Datos introducidos con exito\n");
        clearInputBuffer(); 
        menu();
    } else {
        printf("No se pudo insertar el administrador. Inténtalo de nuevo.\n");
        registrarAdmin();
    }
}