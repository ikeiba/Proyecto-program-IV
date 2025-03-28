#ifndef _CSV_READER_
#define _CSV_READER_
#include "estructuras.h"

Usuario* obtenerUsuarioPorId(int id, Usuario* usuarios, int tamanyo);

Usuario* leerCsvUsuarios();

void liberarUsuarios(Usuario* usuarios, int cantidad);

Grupo* leerCsvGrupos(Usuario* Usuarios);

void liberarGrupos(Grupo* grupos, int cantidad);

Grupo* obtenerGrupoPorId(int id, Grupo* grupos, int tamanyo);

void leerCsvConversaciones(Usuario* usuarios, Grupo* grupos);

Mensaje* leerCsvMensajes(Usuario* usuarios, Grupo* grupos);

void liberarMensajes(Mensaje* mensajes, int cantidad);

#endif