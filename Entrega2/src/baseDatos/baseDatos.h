#ifndef baseDatos_H_
#define baseDatos_H_
#include "sqlite3.h"
#include "../estructuras.h"

// Metodo para abrir la base de datos
sqlite3 *open_database(const char *db_name);

// Metodo para ejecutar las sentencias sql para crear las tablas de la base de datos
void ejecutarTablas(sqlite3 *db, const char *sql);

// Metodo para crear la base de datos
int crearBD();

//Metodo para borrar todos los datos
void limpiarBaseDatos();

//Metodo para borrar las tablas
void borrarTablas();

// Metodo para comprobar inicio de sesion
int comprobarCredenciales(char *email, char *contrasena);

// Metodo para insertar Administrador
int insertarAdministrador(const char *nombre, const char *email, const char *telefono,
                          const char *fecha_nacimiento, int nivel, const char *contrasena);

// Metodo para insertar Usuario
int insertarUsuario(const char *nombre, const char *email, const char *telefono,
                    const char *fecha_nacimiento, const char *contrasena);

// Metodo para Borrar Usuario en la base de datos
int borrarUsuario(const char *email);

// Metodo para modificar el numero de telefono de un usuario desde su email
int cambiarTelefonoUsuario(const char *email, const char *new_phone);

// Metodo para modificar el nombre de un usuario desde su email
int cambiarNombreUsuario(const char *email, const char *new_name);

// Metodo para sacar el id de un usuario desde su email (este metodo recibe la base de datos ya abierta, porque se usa como
// metodo de "apoyo" en otros metodos)
int get_user_id(sqlite3 *db, const char *email);

// Metodo para insertar Grupo en la base de datos
int insert_group(Grupo *group);

// Metodo para insertar Mensaje en la base de datos
int insert_mensaje(Mensaje *mensaje);

//metodo si existe email
int getExisteEmail(const char* email) ;

//select de administradores
int obtenerAdministradores(Administrador **administradores, int *numAdministradores);

//select de administradores
int obtenerUsuarios(Usuario **usuarios, int *numUsuarios);

#endif