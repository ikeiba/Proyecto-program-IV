#ifndef _CSV_READER_
#define _CSV_READER_
#include "estructuras.h"

Usuario* obtenerUsuarioPorId(int id, Usuario* usuarios, int tamanyo);

Usuario* leerCsvUsuarios();

void liberarUsuarios(Usuario* usuarios, int cantidad);

Grupo* leerCsvGrupos();

Grupo* obtenerGrupoPorId(int id, Grupo* grupos, int tamanyo);

Mensaje* leerCsvMensajes();

void liberarMensajes(Mensaje* mensajes, int cantidad);

#endif