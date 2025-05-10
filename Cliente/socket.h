#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

int inicializarSocket();
int inicioSesion(const char* usuario, const char* contrasenya);

#ifdef __cplusplus
}
#endif

#endif