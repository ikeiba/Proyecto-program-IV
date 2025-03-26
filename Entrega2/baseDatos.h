#ifndef baseDatos_H_
#define baseDatos_H_
#include "sqlite3.h"


void execute_sql(sqlite3 *db, const char *sql);

int crearBD();


#endif