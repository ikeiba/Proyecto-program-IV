#ifndef _GESTIONMENSAJE_H_
#define _GESTIONMENSAJE_H_
#include <winsock2.h>
#include "../estructuras.h"

int gestionarMesajeINI(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

int gestionarMensajeREG(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

char* usuariosToString(Usuario** usuarios, int numUsuarios);

char* gruposToString(Grupo** grupos, int numGrupos);

char* conversacionToString(int* idUsuarios, int* idGrupos, int numConversaciones);

char* mensajeToString(Mensaje** mensajes, int numMensajes);

int gestionarMensajeGET(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

#endif