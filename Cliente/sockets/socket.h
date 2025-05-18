#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "../dominio/usuario.h"
#include "../dominio/grupo.h"
#include "../dominio/mensaje.h"

int inicializarSocket();
int inicioSesion(const char* email, const char* contrasenya);
int registrarse(const char* usuario, const char* email, const char* telefono, const char* f_nacimiento, const char* contrasenya);
void leerUsuarios(Usuario** usuarios, int* numUsuarios, char* recvBuff);
void leerGrupos(Grupo** grupos, int* numGrupos, char* recvBuff);
void leerMensajes(Mensaje** mensajes, int* numMensajes, char* recvBuff);
void leerConversacion(char* recvBuff, Grupo** grupos, int numGrupos, Usuario** usuarios, int numUsuarios);
Usuario* obtenerUsuarioPorId(int id, Usuario** usuarios, int tamanyo);
Grupo* obtenerGrupoPorId(int id, Grupo** grupos, int tamanyo);
int contarElementos(const char* recvBuff);
int getUsuario(void);
int getGrupos(void);
int getMensajes(void);
int getConversaciones(void);
int getGeneral(void);
void actualizarDatos();
int enviarMensaje(void);
int crearGrupo(void);
int aniadirUsuarioAGrupo(void);
int abandonarGrupo(void);
#endif