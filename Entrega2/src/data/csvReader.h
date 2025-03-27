#ifndef _CSV_READER_
#define _CSV_READER_
#include "estructuras.h"

Usuario* obtenerUsuarioPorId(int id, Usuario* usuarios, int tamanyo);

Usuario* leerCsvUsuarios();

void liberarUsuarios(Usuario* usuarios, int cantidad);

Grupo* leerCsvGrupos(Usuario* Usuarios);

Grupo* obtenerGrupoPorId(int id, Grupo* grupos, int tamanyo);

Mensaje* leerCsvMensajes(Usuario* usuarios, Grupo* grupos);

void liberarMensajes(Mensaje* mensajes, int cantidad);

#endif