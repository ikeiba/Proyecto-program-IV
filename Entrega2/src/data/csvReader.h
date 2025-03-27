#ifndef _CSV_READER_
#define _CSV_READER_
#include "estructuras.h"

Usuario* leerCsvUsuarios();

void liberarUsuarios(Usuario* usuarios, int cantidad);

Grupo* leerCsvGrupos();

Mensaje* leerCsvMensajes();

#endif