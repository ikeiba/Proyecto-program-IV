#ifndef _SOCKET_H_
#define _SOCKET_H_

int inicializarSocket();
int inicioSesion(const char* email, const char* contrasenya);
int registrarse(const char* usuario, const char* email, const char* telefono, const char* f_nacimiento, const char* contrasenya);
#endif