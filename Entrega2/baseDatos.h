#ifndef baseDatos_H_
#define baseDatos_H_
#include "sqlite3.h"


// Metodo para abrir la base de datos
sqlite3* open_database(const char *db_name);

// Metodo para ejecutar las sentencias sql para crear las tablas de la base de datos
void ejecutarTablas(sqlite3 *db, const char *sql);

// Metodo para crear la base de datos
int crearBD();

// Metodo para comprobar inicio de sesion
int comprobarCredenciales(char *email, char *contrasena);


#endif