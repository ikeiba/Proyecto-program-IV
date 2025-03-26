#include "sqlite3.h"
#include "baseDatos.h"
#include <stdio.h>

// Constante con el nombre de la base de datos (A CAMBIAR: deberia estar en el .config)
const char* nombreBaseDatos = "deustoMessenger.db";


sqlite3* open_database(const char *db_name) {
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    } else {
        printf("Opened database successfully\n");
    }
    return db;
}

void ejecutarTablas(sqlite3 *db, const char *sql) {
    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Table created successfully\n");
    }
}


int crearBD()
{
    sqlite3 *db = open_database(nombreBaseDatos);

    // SQL statements to create tables
    const char *sql_usuario = "CREATE TABLE IF NOT EXISTS Usuario ("
        "id_usuario INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_usuario TEXT NOT NULL, "
        "email_usuario TEXT UNIQUE NOT NULL, "
        "telefono_usuario TEXT, "
        "f_nacimiento_usuario TEXT, "
        "contrasena_usuario TEXT NOT NULL"
        ");";
    ejecutarTablas(db, sql_usuario);

    const char *sql_conversacion = "CREATE TABLE IF NOT EXISTS Conversacion ("
        "id_usuario INTEGER, "
        "id_grupo INTEGER, "
        "PRIMARY KEY (id_usuario, id_grupo), "
        "FOREIGN KEY (id_usuario) REFERENCES Usuario(id_usuario), "
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
        "FOREIGN KEY (id_emisor) REFERENCES Usuario(id_usuario), "
        "FOREIGN KEY (id_grupo) REFERENCES Grupo(id_grupo)"
        ");";
    ejecutarTablas(db, sql_mensaje);

    const char *sql_grupo = "CREATE TABLE IF NOT EXISTS Grupo ("
        "id_grupo INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_grupo TEXT NOT NULL, "
        "fecha_creacion_grupo TEXT NOT NULL, "
        "id_creador INTEGER, "
        "descripcion_grupo TEXT, "
        "FOREIGN KEY (id_creador) REFERENCES Usuario(id_usuario)"
        ");";
    ejecutarTablas(db, sql_grupo);

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
    return 0;

}



int comprobarCredenciales(char *email, char *contrasena)
{
    sqlite3 *db = open_database(nombreBaseDatos);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Administrador WHERE email_admin = ? AND contrasena_admin = ?";
    int exists = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
    }

    sqlite3_finalize(stmt);
    
    // Close database
    sqlite3_close(db);

    return exists;
}

