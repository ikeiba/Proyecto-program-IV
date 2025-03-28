#include "sqlite3.h"
#include "baseDatos.h"
#include "..\utils\logger.h"
#include <stdio.h>

// Constante con el nombre de la base de datos (A CAMBIAR: deberia estar en el .config)
const char *nombreBaseDatos = "src\\baseDatos\\deustoMessenger.db";

sqlite3 *open_database(const char *db_name)
{
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        printf("Opened database successfully\n");
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
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    else
    {
        printf("Table created successfully\n");
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
        "f_nacimiento_usuario TEXT, "
        "contrasena_usuario TEXT NOT NULL"
        ");";
    ejecutarTablas(db, sql_usuario);

    const char *sql_grupo = "CREATE TABLE IF NOT EXISTS Grupo ("
        "id_grupo INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_grupo TEXT NOT NULL, "
        "fecha_creacion_grupo TEXT NOT NULL, "
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
        "fecha_mensaje TEXT NOT NULL, "
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
        "f_nacimiento_admin TEXT, "
        "Nivel INTEGER NOT NULL, "
        "contrasena_admin TEXT NOT NULL"
        ");";
    ejecutarTablas(db, sql_admin);
    // Close database
    sqlite3_close(db);
    registrarMensaje("Base de datos cerrada\n");
    return 0;
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
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Phone number updated successfully for %s to %s.\n", email, new_phone);
        registrarMensaje("Phone number updated successfully for %s to %s.\n", email, new_phone);
        return 1;
    } else {
        fprintf(stderr, "Failed to update phone number: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to update phone number: %s\n", sqlite3_errmsg(db));

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
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("User name updated successfully for %s to %s.\n", email, new_name);
        registrarMensaje("User name updated successfully for %s to %s.\n", email, new_name);
        return 1;
    } else {
        fprintf(stderr, "Failed to update user name: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to update user name: %s\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "Error: No user found with email %s\n", group->creador->email);
        registrarMensaje("Error: No user found with email %s\n", group->creador->email);
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "INSERT INTO Grupo (nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
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
        printf("Group '%s' created successfully.\n", group->nombre);
        registrarMensaje("Group '%s' created successfully.\n", group->nombre);
    } else {
        fprintf(stderr, "Failed to create group: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to create group: %s\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "Failed to insert mensaje: %s\n", sqlite3_errmsg(db));
        registrarMensaje("Failed to insert mensaje: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Mensaje inserted successfully\n");
        registrarMensaje("Mensaje inserted successfully\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}