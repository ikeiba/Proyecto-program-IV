#ifndef _GESTIONMENSAJE_H_
#define _GESTIONMENSAJE_H_
#include <winsock2.h>
#include "../estructuras.h"

Grupo** filtrarGruposPorEmail(char* email, Usuario** usuarios, int numUsuarios, Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int numConversaciones,
    int* numGruposFiltradosC);

int usuarioPorEmail(char* email, Usuario** usuarios, int numUsuarios);

Grupo* grupoPorId(int idGrupo, Grupo** grupos, int numGrupos);

Usuario* usuarioPorId(int idUsuario, Usuario** usuarios, int numUsuarios);

Usuario** filtrarUsuariosPorGrupo(char* email, Usuario** usuarios, int numUsuarios, Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int numConversaciones,
    int* numUsuariosFiltrados);

Mensaje** filtrarMensajesPorGrupos(Grupo** grupos,int numGrupos, Mensaje** mensajes, int numMensajes, int* numMensajesFiltrados);

int filtrarConversacionesPorGrupos(Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int numConversaciones, int** idUsuariosFiltrados, int** idGruposFiltrados,int* numConversacionesFiltradas);

int gestionarMesajeINI(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

int gestionarMensajeREG(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

char* usuariosToString(Usuario** usuarios, int numUsuarios);

char* gruposToString(Grupo** grupos, int numGrupos);

char* conversacionToString(int* idUsuarios, int* idGrupos, int numConversaciones);

char* mensajeToString(Mensaje** mensajes, int numMensajes);

int gestionarMensajeGET(char* sendBuff, char* recvBuff, SOCKET* comm_socket);

int gestionarMensajeUPDATE(char* sendBuff, char* recvBuff, SOCKET* comm_socket);


#endif