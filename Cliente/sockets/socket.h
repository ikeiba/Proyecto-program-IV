#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "../dominio/usuario.h"
#include "../dominio/grupo.h"
#include "../dominio/mensaje.h"

int inicializarSocket();
int inicioSesion(const char* email, const char* contrasenya);
int registrarse(const char* usuario, const char* email, const char* telefono, const char* f_nacimiento, const char* contrasenya);
void leerUsuarios(Usuario*** usuarios, int* numUsuarios, char* recvBuff);
void leerGrupos(Grupo*** grupos, int* numGrupos, char* recvBuff);
void leerMensajes(Mensaje*** mensajes, int* numMensajes, char* recvBuff);
void leerConversacion(char* recvBuff, Grupo** grupos, int numGrupos, Usuario** usuarios, int numUsuarios);
Usuario* obtenerUsuarioPorId(int id, Usuario** usuarios, int tamanyo);
Grupo* obtenerGrupoPorId(int id, Grupo** grupos, int tamanyo);
int contarElementos(const char* recvBuff);
int getUsuario(const char* email);
int getGrupos(void);
int getMensajes(void);
int getConversaciones(void);
int getGeneral(const char* email);
void actualizarDatos();
int enviarMensaje(const char* fecha, const char* hora, const char* contenido, int idEmisor, int idGrupo);
int crearGrupo(const char* nombre, const char* fCreacion, int idCreador, const char* descripcion);
int aniadirUsuarioAGrupo(int idUsuario, int idGrupo);
int abandonarGrupo(int idUsuario, int idGrupo);
#endif