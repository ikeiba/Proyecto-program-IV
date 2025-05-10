#include "gestionMensaje.h"
//#include "baseDatos.h"
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
    send(*comm_socket, sendBuff, sizeof(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
}