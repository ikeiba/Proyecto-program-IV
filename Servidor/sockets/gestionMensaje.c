#include "gestionMensaje.h"
#include "../baseDatos/baseDatos.h"
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

int gestionarMesajeINI(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    char* email = strtok(NULL, ";");
    char* contrasenya = strtok(NULL, ";");

    printf(email);
    printf(contrasenya);
    //Hay que crear un metodo el la base de datos que compruebe si existe un usuario con ese email y contraseña

    //Codigo de prueba 
    printf("Sending reply... \n");
    strcpy(sendBuff, "ERROR");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
}

int gestionarMensajeREG(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    char* nombre = strtok(NULL, ",");
    char* email = strtok(NULL, ",");
    char* telefono = strtok(NULL, ",");
    char* f_nacimiento = strtok(NULL, ",");
    char* contrasenya = strtok(NULL, ",");

    if(insertarUsuario(nombre, email, telefono, f_nacimiento, contrasenya) == 0){
        printf("Sending reply... \n");
        strcpy(sendBuff, "ERROR");
        borrarUsuario("amayamanuela@gmail.com");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }

    //Codigo de prueba 
    printf("Sending reply... \n");
    strcpy(sendBuff, "CORRECT");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
    return 1;
}