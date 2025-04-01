#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "baseDatos.h"
#include "../utils/logger.h"
#include "../estructuras.h"

// Constante con el nombre de la base de datos (A CAMBIAR: deberia estar en el .config)
const char *nombreBaseDatos = "src\\baseDatos\\deustoMessenger.db";

sqlite3 *open_database(const char *db_name)
{
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc)
    {
        //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        //printf("Opened database successfully\n");
        registrarMensaje("Opened database successfully\n");
    }
    return db;
}

void ejecutarTablas(sqlite3 *db, const char *sql)
{
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        //fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        //printf("Table created successfully\n");
        registrarMensaje("Table created successfully\n");
    }
}

int crearBD()
{
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }
    const char *sql_usuario = "CREATE TABLE IF NOT EXISTS Usuario ("
        "id_usuario INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_usuario TEXT NOT NULL, "
        "email_usuario TEXT UNIQUE NOT NULL, "
        "telefono_usuario TEXT, "
        "f_nacimiento_usuario DATE, "
        "contrasena_usuario TEXT NOT NULL"
        ");";
    ejecutarTablas(db, sql_usuario);

    const char *sql_grupo = "CREATE TABLE IF NOT EXISTS Grupo ("
        "id_grupo INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_grupo TEXT NOT NULL, "
        "fecha_creacion_grupo DATE NOT NULL, "
        "id_creador INTEGER, "
        "descripcion_grupo TEXT, "
        "FOREIGN KEY (id_creador) REFERENCES Usuario(id_usuario) ON DELETE SET NULL"
        ");";
    ejecutarTablas(db, sql_grupo);

    const char *sql_conversacion = "CREATE TABLE IF NOT EXISTS Conversacion ("
        "id_usuario INTEGER, "
        "id_grupo INTEGER, "
        "PRIMARY KEY (id_usuario, id_grupo), "
        "FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario) ON DELETE CASCADE, "
        "FOREIGN KEY (id_grupo) REFERENCES Grupo(id_grupo)"
        ");";
    ejecutarTablas(db, sql_conversacion);

    const char *sql_mensaje = "CREATE TABLE IF NOT EXISTS Mensaje ("
        "id_mensaje INTEGER PRIMARY KEY AUTOINCREMENT, "
        "fecha_mensaje DATE NOT NULL, "
        "hora_mensaje TEXT NOT NULL, "
        "contenido_mensaje TEXT NOT NULL, "
        "id_emisor INTEGER, "
        "id_grupo INTEGER, "
        "FOREIGN KEY (id_emisor) REFERENCES Usuario(id_usuario) ON DELETE CASCADE, "
        "FOREIGN KEY (id_grupo) REFERENCES Grupo(id_grupo)"
        ");";
    ejecutarTablas(db, sql_mensaje);

    const char *sql_admin = "CREATE TABLE IF NOT EXISTS Administrador ("
        "id_admin INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_admin TEXT NOT NULL, "
        "email_admin TEXT UNIQUE NOT NULL, "
        "telefono_admin TEXT, "
        "f_nacimiento_admin DATE, "
        "Nivel INTEGER NOT NULL, "
        "contrasena_admin TEXT NOT NULL"
        ");";
    ejecutarTablas(db, sql_admin);
    // Close database
    sqlite3_close(db);
    registrarMensaje("Base de datos cerrada\n");
    return 0;
}

void limpiarBaseDatos() {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return; // Error al abrir la base de datos
    }
    const char *sql_delete = 
        "DELETE FROM Mensaje;"
        "DELETE FROM Conversacion;"
        "DELETE FROM Grupo;"
        "DELETE FROM Usuario;"
        "DELETE FROM Administrador;";

    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql_delete, 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        printf("Error al limpiar la base de datos: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Base de datos limpiada correctamente.\n");
    }
}

void borrarTablas() {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return; // Error al abrir la base de datos
    }
    const char *sql_drop = 
        "DROP TABLE IF EXISTS Mensaje;"
        "DROP TABLE IF EXISTS Conversacion;"
        "DROP TABLE IF EXISTS Grupo;"
        "DROP TABLE IF EXISTS Usuario;"
        "DROP TABLE IF EXISTS Administrador;";

    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql_drop, 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        printf("Error al borrar las tablas: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Tablas eliminadas correctamente.\n");
    }
}

int comprobarCredenciales(char *email, char *contrasena)
{
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Administrador WHERE email_admin = ? AND contrasena_admin = ?";
    int exists = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
    }

    sqlite3_finalize(stmt);

    // Close database
    sqlite3_close(db);
    registrarMensaje("Base de datos cerrada\n");

    return exists;
}

int insertarAdministrador(const char *nombre, const char *email, const char *telefono,
                          const char *fecha_nacimiento, int nivel, const char *contrasena)
{

    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL)
    {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "INSERT INTO Administrador (nombre_admin, email_admin, telefono_admin,f_nacimiento_admin, Nivel, contrasena_admin) VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int resultado = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, telefono, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, fecha_nacimiento, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, nivel);
        sqlite3_bind_text(stmt, 6, contrasena, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            resultado = 1; // Éxito al insertar
            printf("Administrador insertado correctamente.\n");
            registrarMensaje("Administrador insertado correctamente.\n");

        }
        else
        {
            fprintf(stderr, "Error al insertar administrador: %s\n", sqlite3_errmsg(db));
            registrarMensaje("Error al insertar administrador: %s\n", sqlite3_errmsg(db));
        }
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return resultado;
}

int insertarUsuario(const char *nombre, const char *email, const char *telefono,
                          const char *fecha_nacimiento, const char *contrasena)
{

    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL)
    {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "INSERT INTO Usuario (nombre_usuario, email_usuario, telefono_usuario,f_nacimiento_usuario, contrasena_usuario) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int resultado = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, telefono, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, fecha_nacimiento, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, contrasena, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            resultado = 1; // Éxito al insertar
            printf("Usuario insertado correctamente.\n");
            registrarMensaje("Usuario insertado correctamente.\n");
        }
        else
        {
            fprintf(stderr, "Error al insertar Usuario: %s\n", sqlite3_errmsg(db));
            registrarMensaje("Error al insertar Usuario: %s\n", sqlite3_errmsg(db));
        }
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return resultado;
}

int borrarUsuario(const char *email) {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "DELETE FROM Usuario WHERE email_usuario = ?;";
    sqlite3_stmt *stmt;
    int resultado = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            resultado = 1; // Éxito al borrar
            printf("Usuario con email %s eliminado correctamente.\n", email);
            registrarMensaje("Usuario con email %s eliminado correctamente.\n", email);
        } else {
            fprintf(stderr, "Error al eliminar usuario: %s\n", sqlite3_errmsg(db));
            registrarMensaje("Error al eliminar usuario: %s\n", sqlite3_errmsg(db));
        }
    } else {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return resultado;
}


// Metodo para modificar el numero de telefono de un usuario desde su email
int cambiarTelefonoUsuario(const char *email, const char *new_phone) {
    
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "UPDATE Usuario SET telefono_usuario = ? WHERE email_usuario = ?";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Número de teléfono actualizado correctamente del usuario %s a %s.\n", email, new_phone);
        registrarMensaje("Número de teléfono actualizado correctamente del usuario %s a %s.\n", email, new_phone);
        return 1;
    } else {
        fprintf(stderr, "Error al actualizar el número de teléfono: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al actualizar el número de teléfono: %s\n", sqlite3_errmsg(db));
        return 0;
    }
}

// Metodo para modificar el nombre de un usuario desde su email
int cambiarNombreUsuario(const char *email, const char *new_name) {
    
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "UPDATE Usuario SET nombre_usuario = ? WHERE email_usuario = ?";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Nombre de usuario actualizado correctamente para %s a %s.\n", email, new_name);
        registrarMensaje("Nombre de usuario actualizado correctamente para %s a %s.\n", email, new_name);
        return 1;
    } else {
        fprintf(stderr, "Error al actualizar el nombre de usuario: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al actualizar el nombre de usuario: %s\n", sqlite3_errmsg(db));
        return 0;
    }
}


// Metodo para sacar el id de un usuario desde su email
int get_user_id(sqlite3 *db, const char *email) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_usuario FROM Usuario WHERE email_usuario = ?";
    int user_id = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            user_id = sqlite3_column_int(stmt, 0);
        }
    }
    
    sqlite3_finalize(stmt);
    return user_id;
}

// Metodo para insertar Grupo en la base de datos
int insert_group(Grupo *group) {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (!db) return 0;

    // Get creator's user ID from email
    int creator_id = get_user_id(db, group->creador->email);
    if (creator_id == -1) {
        fprintf(stderr, "Error: ningun usuario encontrado con email %s\n", group->creador->email);
        registrarMensaje("Error: ningun usuario encontrado con email %s\n", group->creador->email);
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "INSERT INTO Grupo (nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, group->nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, group->fCreacion, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, creator_id);
    sqlite3_bind_text(stmt, 4, group->descripcion, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Grupo '%s' creado con exito.\n", group->nombre);
        registrarMensaje("Grupo '%s' creado con exito.\n", group->nombre);
    } else {
        fprintf(stderr, "Error al crear el grupo: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al crear el grupo: %s\n", sqlite3_errmsg(db));
    }

    for(int i = 0; i<group->size;i++){
        int miembro_id = get_user_id(db, group->miembros[i]->email);
        if (miembro_id == -1) {
            fprintf(stderr, "Error: ningun usuario encontrado con email %s\n", group->miembros[i]->email);
            registrarMensaje("Error: ningun usuario encontrado con email %s\n", group->miembros[i]->email);
            sqlite3_close(db);
            return 0;
        }

        const char *sqlc = "INSERT INTO Conversacion (id_usuario, id_grupo) VALUES (?, ?)";

        sqlite3_stmt *stmtc;
        rc = sqlite3_prepare_v2(db, sqlc, -1, &stmtc, 0);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 0;
        }

        sqlite3_bind_int(stmtc, 1, miembro_id);
        sqlite3_bind_int(stmtc, 2, group->id);

        rc = sqlite3_step(stmtc);
        sqlite3_finalize(stmtc);

        if (rc == SQLITE_DONE) {
            printf("miembro '%s' insertado correctamente.\n", group->miembros[i]->nombre);
            registrarMensaje("miembro '%s' insertado correctamente.\n", group->miembros[i]->nombre);
        } else {
            fprintf(stderr, "Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
            registrarMensaje("Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
        }
    }

    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}


int insert_mensaje(Mensaje *mensaje) {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (!db) return 0;
    
    const char *sql = "INSERT INTO Mensaje (fecha_mensaje, hora_mensaje, contenido_mensaje, id_emisor, id_grupo) "
                      "VALUES (?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Bind values to the SQL statement
    sqlite3_bind_text(stmt, 1, mensaje->fecha, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, mensaje->hora, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, mensaje->contenido, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, mensaje->emisor->id);
    sqlite3_bind_int(stmt, 5, mensaje->grupo->id);

    // Execute the statement
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error al insertar el mensaje: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Error al insertar el mensaje: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Mensaje insertado correctamente\n");
        registrarMensaje("Mensaje insertado correctamente\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

int obtenerUsuarios(Usuario **usuarios, int *numUsuarios) {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0;
    }

    const char *sql = "SELECT * FROM Usuario;";
    sqlite3_stmt *stmt;

    // Preparar la sentencia SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Contar el número de filas
    *numUsuarios = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numUsuarios)++;
    }

    // Reservar memoria para almacenar los resultados
    *usuarios = (Usuario *)malloc(sizeof(Usuario) * (*numUsuarios));
    if (*usuarios == NULL) {
        fprintf(stderr, "Error al reservar memoria para usuarios\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Volver al principio de la sentencia
    sqlite3_reset(stmt);

    // Obtener los resultados y almacenarlos en la estructura
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*usuarios)[index].id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *email = (const char *)sqlite3_column_text(stmt, 2);
        const char *telefono = (const char *)sqlite3_column_text(stmt, 3);
        const char *f_nacimiento = (const char *)sqlite3_column_text(stmt, 4);
        const char *contrasena = (const char *)sqlite3_column_text(stmt, 5);

        snprintf((*usuarios)[index].nombre, sizeof((*usuarios)[index].nombre), "%s", nombre);
        snprintf((*usuarios)[index].email, sizeof((*usuarios)[index].email), "%s", email);
        snprintf((*usuarios)[index].telefono, sizeof((*usuarios)[index].telefono), "%s", telefono);
        snprintf((*usuarios)[index].fNacimiento, sizeof((*usuarios)[index].fNacimiento), "%s", f_nacimiento);
        snprintf((*usuarios)[index].contra, sizeof((*usuarios)[index].contra), "%s", contrasena);

        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

int obtenerAdministradores(Administrador **administradores, int *numAdministradores) {
    sqlite3 *db = open_database(nombreBaseDatos);
    if (db == NULL) {
        return 0;
    }

    const char *sql = "SELECT * FROM Administrador;";
    sqlite3_stmt *stmt;

    // Preparar la sentencia SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Contar el número de filas
    *numAdministradores = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numAdministradores)++;
    }

    // Reservar memoria para almacenar los resultados
    *administradores = (Administrador *)malloc(sizeof(Administrador) * (*numAdministradores));
    if (*administradores == NULL) {
        fprintf(stderr, "Error al reservar memoria para administradores\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Volver al principio de la sentencia
    sqlite3_reset(stmt);

    // Obtener los resultados y almacenarlos en la estructura
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*administradores)[index].id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        const char *email = (const char *)sqlite3_column_text(stmt, 2);
        const char *telefono = (const char *)sqlite3_column_text(stmt, 3);
        const char *f_nacimiento = (const char *)sqlite3_column_text(stmt, 4);
        (*administradores)[index].nivel = sqlite3_column_int(stmt, 5);
        const char *contrasena = (const char *)sqlite3_column_text(stmt, 6);

        snprintf((*administradores)[index].nombre, sizeof((*administradores)[index].nombre), "%s", nombre);
        snprintf((*administradores)[index].email, sizeof((*administradores)[index].email), "%s", email);
        snprintf((*administradores)[index].telefono, sizeof((*administradores)[index].telefono), "%s", telefono);
        snprintf((*administradores)[index].fNacimiento, sizeof((*administradores)[index].fNacimiento), "%s", f_nacimiento);
        snprintf((*administradores)[index].contra, sizeof((*administradores)[index].contra), "%s", contrasena);

        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

int getExisteEmail(const char* email) {
    sqlite3 *db = open_database(nombreBaseDatos);
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Usuario WHERE email_usuario = ? "
                      "UNION ALL "
                      "SELECT COUNT(*) FROM Administrador WHERE email_admin = ?";

    int existe = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            existe += sqlite3_column_int(stmt, 0);  // Sumar los valores obtenidos
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return existe > 0;  // Retorna 1 si el email existe en alguna de las tablas, 0 si no existe
}


int obtenerGrupos(int** ids, char*** nombres, char*** fechas, int** idCreador, char*** descripciones, int* numGrupos) {
    sqlite3 *db = open_database(nombreBaseDatos);
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_grupo, nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo FROM Grupo";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener los grupos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    *numGrupos = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numGrupos)++;
    }

    *ids = (int*)malloc(sizeof(int) * (*numGrupos));
    *nombres = (char**)malloc(sizeof(char*) * (*numGrupos));
    *fechas = (char**)malloc(sizeof(char*) * (*numGrupos));
    *idCreador = (int*)malloc(sizeof(int) * (*numGrupos));
    *descripciones = (char**)malloc(sizeof(char*) * (*numGrupos));

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*ids)[index] = sqlite3_column_int(stmt, 0);
        (*nombres)[index] = strdup((const char*)sqlite3_column_text(stmt, 1));
        (*fechas)[index] = strdup((const char*)sqlite3_column_text(stmt, 2));
        (*idCreador)[index] = sqlite3_column_int(stmt, 3);
        (*descripciones)[index] = strdup((const char*)sqlite3_column_text(stmt, 4));
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

int obtenerConversaciones(int** idUsuarios, int** idGrupos, int* numConversaciones) {
    sqlite3 *db = open_database(nombreBaseDatos);
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_usuario, id_grupo FROM Conversacion";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener conversaciones: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    *numConversaciones = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numConversaciones)++;
    }

    *idUsuarios = (int*)malloc(sizeof(int) * (*numConversaciones));
    *idGrupos = (int*)malloc(sizeof(int) * (*numConversaciones));

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*idUsuarios)[index] = sqlite3_column_int(stmt, 0);
        (*idGrupos)[index] = sqlite3_column_int(stmt, 1);
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}



int obtenerMensajes(int** ids, char*** fechas, char*** horas, char*** contenidos, int** idEmisores, int** idGrupos, int* numMensajes) {
    sqlite3 *db = open_database(nombreBaseDatos);
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_mensaje, fecha_mensaje, hora_mensaje, contenido_mensaje, id_emisor, id_grupo FROM Mensaje";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener los mensajes: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    *numMensajes = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numMensajes)++;
    }

    *ids = (int*)malloc(sizeof(int) * (*numMensajes));
    *fechas = (char**)malloc(sizeof(char*) * (*numMensajes));
    *horas = (char**)malloc(sizeof(char*) * (*numMensajes));
    *contenidos = (char**)malloc(sizeof(char*) * (*numMensajes));
    *idEmisores = (int*)malloc(sizeof(int) * (*numMensajes));
    *idGrupos = (int*)malloc(sizeof(int) * (*numMensajes));

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*ids)[index] = sqlite3_column_int(stmt, 0);
        (*fechas)[index] = strdup((const char*)sqlite3_column_text(stmt, 1));
        (*horas)[index] = strdup((const char*)sqlite3_column_text(stmt, 2));
        (*contenidos)[index] = strdup((const char*)sqlite3_column_text(stmt, 3));
        (*idEmisores)[index] = sqlite3_column_int(stmt, 4);
        (*idGrupos)[index] = sqlite3_column_int(stmt, 5);
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

