#ifndef baseDatos_H_
#define baseDatos_H_
#include "sqlite3.h"

// Metodo para abrir la base de datos
sqlite3 *open_database(const char *db_name);

// Metodo para ejecutar las sentencias sql para crear las tablas de la base de datos
void ejecutarTablas(sqlite3 *db, const char *sql);

// Metodo para crear la base de datos
int crearBD();

// Metodo para comprobar inicio de sesion
int comprobarCredenciales(char *email, char *contrasena);

// Metodo para insertar Administrador
int insertarAdministrador(const char *nombre, const char *email, const char *telefono,
                          const char *fecha_nacimiento, int nivel, const char *contrasena);

// Metodo para insertar Usuario
int insertarUsuario(const char *nombre, const char *email, const char *telefono,
                    const char *fecha_nacimiento, const char *contrasena);

// Metodo para Borrar Usuario
int borrarUsuario(const char *email);


#endif