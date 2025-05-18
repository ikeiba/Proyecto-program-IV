#include "gestionMensaje.h"
#include "../baseDatos/baseDatos.h"
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

extern Usuario** usuarios;
extern int numUsuarios;

extern Grupo** grupos;
extern int numGrupos;

extern int* idUsuarios;
extern int* idGrupos;
extern int numConversaciones;

extern Mensaje** mensajes;
extern int numMensajes;

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

char* usuariosToString(Usuario** usuarios, int numUsuarios){
    char* result = (char*)malloc(1024);
    strcpy(result, "");
    for(int i = 0; i < numUsuarios; i++){
        char buffer[256];
        sprintf(buffer, "%d,%s,%s,%s,%s,%s;", usuarios[i]->id, usuarios[i]->nombre, usuarios[i]->email, usuarios[i]->telefono, usuarios[i]->fNacimiento, usuarios[i]->contra);
        strcat(result, buffer);
    }
    return result;
}

char* gruposToString(Grupo** grupos, int numGrupos){
    char* result = (char*)malloc(1024);
    strcpy(result, "");
    for(int i = 0; i < numGrupos; i++){
        char buffer[256];
        sprintf(buffer, "%d,%s,%s,%d,%s;", grupos[i]->id, grupos[i]->nombre, grupos[i]->fCreacion, grupos[i]->creador->id, grupos[i]->descripcion);
        strcat(result, buffer);
    }
    return result;
}

char* conversacionToString(int* idUsuarios, int* idGrupos, int numConversaciones){
    char* result = (char*)malloc(1024);
    strcpy(result, "");
    for(int i = 0; i < numConversaciones; i++){
        char buffer[256];
        sprintf(buffer, "%d,%d;", (idUsuarios)[i], (idGrupos)[i]);
        strcat(result, buffer);
    }
    return result;
}

char* mensajeToString(Mensaje** mensajes, int numMensajes){
    char* result = (char*)malloc(1024);
    strcpy(result, "");
    for(int i = 0; i < numMensajes; i++){
        char buffer[256];
        sprintf(buffer, "%d,%s,%s,%s,%d,%d;", mensajes[i]->id, mensajes[i]->fecha, mensajes[i]->hora, mensajes[i]->contenido, mensajes[i]->emisor->id, mensajes[i]->grupo->id);
        strcat(result, buffer);
    }
    return result;
}

int gestionarMensajeGET(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    char* tipo = strtok(NULL, ";");

    if(strcmp(tipo, "USUARIO") == 0){
        printf("Sending reply... \n");
        sprintf(sendBuff, "USUARIO;%s", usuariosToString(usuarios, numUsuarios));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }
    
    if(strcmp(tipo, "GRUPO") == 0){
        printf("Sending reply... \n");
        sprintf(sendBuff, "GRUPO;%s", gruposToString(grupos, numUsuarios));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }

    if(strcmp(tipo, "CONVERSACION") == 0){
        printf("Sending reply... \n");
        sprintf(sendBuff, "CONVERSACION;%s", conversacionToString(idUsuarios,idGrupos, numConversaciones));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }

    if(strcmp(tipo, "MENSAJE") == 0){
        printf("Sending reply... \n");
        sprintf(sendBuff, "MENSAJE;%s", mensajeToString(mensajes, numMensajes));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }
    
    printf("Sending reply... \n");
    strcpy(sendBuff, "ERROR");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
}