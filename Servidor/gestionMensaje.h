#ifndef _GESTIONMENSAJE_H_
#define _GESTIONMENSAJE_H_
#include <winsock2.h>

int gestionarMesajeINI(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

#endif