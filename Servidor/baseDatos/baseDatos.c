#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "baseDatos.h"
#include "../estructuras.h"

extern Config config;

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
        //registrarMensaje("Opened database successfully\n");
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
        //registrarMensaje("Table created successfully\n");
    }
}

int comprobarCredenciales(char *email, char *contrasena)
{
    sqlite3 *db = open_database(config.nombreBD);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Usuario WHERE email_usuario = ? AND contrasena_usuario = ?";
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
    //registrarMensaje("Base de datos cerrada\n");

    return exists;
}

int insertarUsuario(const char *nombre, const char *email, const char *telefono,
                          const char *fecha_nacimiento, const char *contrasena)
{
    sqlite3 *db = open_database(config.nombreBD);
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
            //registrarMensaje("Usuario insertado correctamente.\n");
        }
        else
        {
            fprintf(stderr, "Error al insertar Usuario: %s\n", sqlite3_errmsg(db));
            //registrarMensaje("Error al insertar Usuario: %s\n", sqlite3_errmsg(db));
        }
    }
    else
    {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return resultado;
}

int borrarUsuario(const char *email) {
    sqlite3 *db = open_database(config.nombreBD);
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
            //registrarMensaje("Usuario con email %s eliminado correctamente.\n", email);
        } else {
            fprintf(stderr, "Error al eliminar usuario: %s\n", sqlite3_errmsg(db));
            //registrarMensaje("Error al eliminar usuario: %s\n", sqlite3_errmsg(db));
        }
    } else {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return resultado;
}


// Metodo para modificar el numero de telefono de un usuario desde su email
int cambiarTelefonoUsuario(const char *email, const char *new_phone) {
    
    sqlite3 *db = open_database(config.nombreBD);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "UPDATE Usuario SET telefono_usuario = ? WHERE email_usuario = ?";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Número de teléfono actualizado correctamente del usuario %s a %s.\n", email, new_phone);
        //registrarMensaje("Número de teléfono actualizado correctamente del usuario %s a %s.\n", email, new_phone);
        return 1;
    } else {
        fprintf(stderr, "Error al actualizar el número de teléfono: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al actualizar el número de teléfono: %s\n", sqlite3_errmsg(db));
        return 0;
    }
}

// Metodo para modificar el nombre de un usuario desde su email
int cambiarNombreUsuario(const char *email, const char *new_name) {
    
    sqlite3 *db = open_database(config.nombreBD);
    if (db == NULL) {
        return 0; // Error al abrir la base de datos
    }

    const char *sql = "UPDATE Usuario SET nombre_usuario = ? WHERE email_usuario = ?";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, new_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Nombre de usuario actualizado correctamente para %s a %s.\n", email, new_name);
        //registrarMensaje("Nombre de usuario actualizado correctamente para %s a %s.\n", email, new_name);
        return 1;
    } else {
        fprintf(stderr, "Error al actualizar el nombre de usuario: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al actualizar el nombre de usuario: %s\n", sqlite3_errmsg(db));
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
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    // Get creator's user ID from email
    int creator_id = get_user_id(db, group->creador->email);
    if (creator_id == -1) {
        fprintf(stderr, "Error: ningun usuario encontrado con email %s\n", group->creador->email);
        //registrarMensaje("Error: ningun usuario encontrado con email %s\n", group->creador->email);
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "INSERT INTO Grupo (nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
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
        //registrarMensaje("Grupo '%s' creado con exito.\n", group->nombre);
    } else {
        fprintf(stderr, "Error al crear el grupo: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al crear el grupo: %s\n", sqlite3_errmsg(db));
    }

    for(int i = 0; i<group->size;i++){
        int miembro_id = get_user_id(db, group->miembros[i]->email);
        if (miembro_id == -1) {
            fprintf(stderr, "Error: ningun usuario encontrado con email %s\n", group->miembros[i]->email);
            //registrarMensaje("Error: ningun usuario encontrado con email %s\n", group->miembros[i]->email);
            sqlite3_close(db);
            return 0;
        }

        const char *sqlc = "INSERT INTO Conversacion (id_usuario, id_grupo) VALUES (?, ?)";

        sqlite3_stmt *stmtc;
        rc = sqlite3_prepare_v2(db, sqlc, -1, &stmtc, 0);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 0;
        }

        sqlite3_bind_int(stmtc, 1, miembro_id);
        sqlite3_bind_int(stmtc, 2, group->id);

        rc = sqlite3_step(stmtc);
        sqlite3_finalize(stmtc);

        if (rc == SQLITE_DONE) {
            printf("miembro '%s' insertado correctamente.\n", group->miembros[i]->nombre);
            //registrarMensaje("miembro '%s' insertado correctamente.\n", group->miembros[i]->nombre);
        } else {
            fprintf(stderr, "Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
            //registrarMensaje("Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
        }
    }

    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}


int insert_mensaje(Mensaje *mensaje) {
    sqlite3 *db = open_database(config.nombreBD);
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
        //registrarMensaje("Error al insertar el mensaje: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Mensaje insertado correctamente\n");
        //registrarMensaje("Mensaje insertado correctamente\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

//! MODIFICADO
int obtenerUsuarios(Usuario ***usuarios, int *numUsuarios) {
    sqlite3 *db = open_database("../Administrador/src/baseDatos/deustoMessenger.db");
    if (db == NULL) {
        return 0;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM Usuario;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener los usuarios: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    *numUsuarios = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numUsuarios)++;
    }

    *usuarios = (Usuario **)malloc(sizeof(Usuario *) * (*numUsuarios));
    if (*usuarios == NULL) {
        printf("Error al reservar memoria para punteros a usuarios\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Usuario *u = (Usuario *)malloc(sizeof(Usuario));
        if (u == NULL) {
            printf("Error al reservar memoria para usuario\n");
            continue; // O liberar todo y salir con error
        }

        u->id = sqlite3_column_int(stmt, 0);
        u->nombre = strdup((const char *)sqlite3_column_text(stmt, 1));
        u->email = strdup((const char *)sqlite3_column_text(stmt, 2));
        u->telefono = strdup((const char *)sqlite3_column_text(stmt, 3));
        u->fNacimiento = strdup((const char *)sqlite3_column_text(stmt, 4));
        u->contra = strdup((const char *)sqlite3_column_text(stmt, 5));

        (*usuarios)[index] = u;
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Usuarios obtenidos: %d\n", *numUsuarios);
    return 1;
}



int obtenerAdministradores(Administrador **administradores, int *numAdministradores) {
    printf("Entrando en obtenerAdministradores...\n");
    sqlite3 *db = open_database(config.nombreBD);
    if (db == NULL) {
        printf("Error: No se pudo abrir la base de datos.\n");
        return 0;
    }
    
    const char *sql = "SELECT * FROM Administrador;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    *numAdministradores = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numAdministradores)++;
    }
    printf("Número de administradores encontrados: %d\n", *numAdministradores);

    if (*numAdministradores == 0) {
        printf("No hay administradores en la base de datos.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
        // Reservar memoria para almacenar los resultados
        *administradores = (Administrador *)malloc(sizeof(Administrador) * (*numAdministradores));
        if (*administradores == NULL) {
            printf("Error al reservar memoria para administradores\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    
        // Volver al principio de la sentencia para extraer los datos
        sqlite3_reset(stmt);
        int index = 0;
    
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*administradores)[index].id = sqlite3_column_int(stmt, 0);
            (*administradores)[index].nombre = strdup((const char *)sqlite3_column_text(stmt, 1));
            (*administradores)[index].email = strdup((const char *)sqlite3_column_text(stmt, 2));
            (*administradores)[index].telefono = strdup((const char *)sqlite3_column_text(stmt, 3));
            (*administradores)[index].fNacimiento = strdup((const char *)sqlite3_column_text(stmt, 4));
            (*administradores)[index].nivel = sqlite3_column_int(stmt, 5);
            (*administradores)[index].contra = strdup((const char *)sqlite3_column_text(stmt, 6));
    
            index++;
        }
    
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    
        printf("Administradores obtenidos: %d\n", *numAdministradores);
        return 1;
    
    
}

int getExisteEmail(const char* email) {
    sqlite3 *db = open_database(config.nombreBD);
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

//! MODIFICADO
int obtenerGrupos(Grupo ***grupos, int *numGrupos, Usuario **usuarios, int numUsuarios) {
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) {
        return 0;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_grupo, nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo FROM Grupo";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al preparar la consulta de grupos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Contar cuántos grupos hay
    *numGrupos = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numGrupos)++;
    }

    // Reservar memoria para el array de punteros a Grupo
    *grupos = (Grupo **)malloc(sizeof(Grupo *) * (*numGrupos));
    if (!*grupos) {
        printf("Error al reservar memoria para el array de grupos\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Reiniciar el statement para recorrer de nuevo
    sqlite3_reset(stmt);

    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Grupo *g = (Grupo *)malloc(sizeof(Grupo));
        if (!g) {
            printf("Error al reservar memoria para un grupo\n");
            continue; // puedes liberar lo anterior si prefieres abortar
        }

        int idCreador = sqlite3_column_int(stmt, 3);
        Usuario *usuarioCreador = obtenerUsuarioPorId(idCreador, usuarios, numUsuarios);

        g->id = sqlite3_column_int(stmt, 0);
        g->nombre = strdup((const char *)sqlite3_column_text(stmt, 1));
        g->fCreacion = strdup((const char *)sqlite3_column_text(stmt, 2));
        g->creador = usuarioCreador;
        g->descripcion = strdup((const char *)sqlite3_column_text(stmt, 4));

        (*grupos)[index++] = g;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}



int obtenerConversaciones(int** idUsuarios, int** idGrupos, int** idConversaciones, int* numConversaciones) {
    sqlite3 *db = open_database(config.nombreBD);
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_usuario, id_grupo, id_conversacion FROM Conversacion";

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
    *idConversaciones = (int*)malloc(sizeof(int) * (*numConversaciones));

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*idUsuarios)[index] = sqlite3_column_int(stmt, 0);
        (*idGrupos)[index] = sqlite3_column_int(stmt, 1);
        int idConversacion = sqlite3_column_int(stmt, 2);
        (*idConversaciones)[index] = idConversacion;
        index++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}

//! MODIFICADO
int obtenerMensajes(Mensaje ***mensajes, int* numMensajes, Grupo **grupos, int numGrupos, Usuario **usuarios, int numUsuarios) {
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_mensaje, fecha_mensaje, hora_mensaje, contenido_mensaje, id_emisor, id_grupo FROM Mensaje";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Error al obtener los mensajes: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Contar mensajes
    *numMensajes = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        (*numMensajes)++;
    }

    // Reservar memoria para el array de punteros
    *mensajes = (Mensaje **)malloc(sizeof(Mensaje *) * (*numMensajes));
    if (!*mensajes) {
        printf("Error al reservar memoria para el array de mensajes.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_reset(stmt);
    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Mensaje *m = (Mensaje *)malloc(sizeof(Mensaje));
        if (!m) {
            printf("Error al reservar memoria para un mensaje\n");
            continue; // o maneja error y libera memoria si prefieres abortar
        }

        int idEmisor = sqlite3_column_int(stmt, 4);
        Usuario *usuarioMensaje = obtenerUsuarioPorId(idEmisor, usuarios, numUsuarios);

        int idGrupo = sqlite3_column_int(stmt, 5);
        Grupo *grupoMensaje = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        m->id = sqlite3_column_int(stmt, 0);
        m->fecha = strdup((const char *)sqlite3_column_text(stmt, 1));
        m->hora = strdup((const char *)sqlite3_column_text(stmt, 2));
        m->contenido = strdup((const char *)sqlite3_column_text(stmt, 3));
        m->emisor = usuarioMensaje;
        m->grupo = grupoMensaje;

        (*mensajes)[index++] = m;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}



Usuario* obtenerUsuarioPorId(int id, Usuario** usuarios, int tamanyo){
    Usuario* u;
    for(int i = 0; i<tamanyo; i++){
        if((*usuarios[i]).id == id){
            u = usuarios[i];
        }
    }
    return u;
}

Grupo* obtenerGrupoPorId(int id, Grupo** grupos, int tamanyo){
    Grupo* g;
    for(int i = 0; i<tamanyo; i++){
        if((*grupos[i]).id == id){
            g = grupos[i];
        }
    }
    return g;
}

// Metodo para insertar Mensaje en la base de datos
// Este metodo se usa para insertar mensajes desde el cliente, por lo que no se necesita el id del emisor ni el id del grupo
int insertarMensajeDesdeUpdate(char* fecha, char* hora, char* contenido, int idEmisor, int idGrupo) {

    sqlite3 *db = open_database(config.nombreBD);
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
    sqlite3_bind_text(stmt, 1, fecha, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hora, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contenido, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, idEmisor);
    sqlite3_bind_int(stmt, 5, idGrupo);

    // Execute the statement
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error al insertar el mensaje: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al insertar el mensaje: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Mensaje insertado correctamente\n");
        //registrarMensaje("Mensaje insertado correctamente\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

// Metodo para insertar Grupo en la base de datos
// Este metodo se usa para insertar grupo desde el cliente, por lo que no se necesita el id de los miembros
int insertarGrupoDesdeUpdate(char* nombre, char* fCreacion, int idCreador, char* descripcion) 
{
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    const char *sql = "INSERT INTO Grupo (nombre_grupo, fecha_creacion_grupo, id_creador, descripcion_grupo) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, fCreacion, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idCreador);
    sqlite3_bind_text(stmt, 4, descripcion, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Grupo '%s' creado con exito.\n", nombre);
        //registrarMensaje("Grupo '%s' creado con exito.\n", group->nombre);
    } else {
        fprintf(stderr, "Error al crear el grupo: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al crear el grupo: %s\n", sqlite3_errmsg(db));
    }



    const char *sqlc = "INSERT INTO Conversacion (id_usuario, id_grupo) VALUES (?, ?)";

    sqlite3_stmt *stmtc;
    rc = sqlite3_prepare_v2(db, sqlc, -1, &stmtc, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    int idGrupo = get_group_id(db, nombre);
    sqlite3_bind_int(stmtc, 1, idCreador);
    sqlite3_bind_int(stmtc, 2, idGrupo);

    rc = sqlite3_step(stmtc);
    sqlite3_finalize(stmtc);

    if (rc == SQLITE_DONE) {
        printf("miembro '%d' insertado correctamente.\n", idCreador);
        //registrarMensaje("miembro '%s' insertado correctamente.\n", group->miembros[i]->nombre);
    } else {
        fprintf(stderr, "Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al insertar el miembro: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

// Metodo para sacar el id de un grupo desde su nombre
int get_group_id(sqlite3 *db, const char *nombre) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_grupo FROM Grupo WHERE nombre_grupo = ?";
    int group_id = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            group_id = sqlite3_column_int(stmt, 0);
        }
    }
    
    sqlite3_finalize(stmt);
    return group_id;
}

// Metodo para insertar Conversacion en la base de datos
// Este metodo se usa para insertar Conversacion desde el cliente
int insertarConversacionDesdeUpdate(int idUsuario, int idGrupo) 
{
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    const char *sql = "INSERT INTO Conversacion (id_usuario, id_grupo) VALUES (?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, idUsuario);
    sqlite3_bind_int(stmt, 2, idGrupo);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        printf("Conversacion '%d' creado con exito.\n", idGrupo);
        //registrarMensaje("Grupo '%s' creado con exito.\n", group->nombre);
    } else {
        fprintf(stderr, "Error al crear el grupo: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al crear el grupo: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

// Metodo para obtener el id de una conversacion desde el id del usuario y el id del grupo
int obetenerIdConversacion(int idUsuario, int idGrupo) 
{
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    const char *sql = "SELECT id_conversacion FROM Conversacion WHERE id_usuario = ? AND id_grupo = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, idUsuario);
    sqlite3_bind_int(stmt, 2, idGrupo);

    int idConversacion = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        idConversacion = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return idConversacion;
}

// Metodo para abandonar Grupo en la base de datos
// Este metodo se usa para abandonar Grupo desde el cliente
int abandonarGrupoDesdeUpdate(int idUsuario, int idGrupo) 
{
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return 0;

    const char *sql = "DELETE FROM Conversacion WHERE id_usuario = ? AND id_grupo = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, idUsuario);
    sqlite3_bind_int(stmt, 2, idGrupo);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);


    if (rc == SQLITE_DONE) {
        printf("Conversacion '%d' eliminada con exito.\n", idGrupo);
        //registrarMensaje("Grupo '%s' creado con exito.\n", group->nombre);
    } else {
        fprintf(stderr, "Error al crear el grupo: %s\n", sqlite3_errmsg(db));
        //registrarMensaje("Error al crear el grupo: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    return (rc == SQLITE_DONE);
}

// Metodo para sacar el id de un mensaje desde sus datos
int get_message_id(const char *fecha, const char *hora, const char *contenido, int idEmisor, int idGrupo) {
    sqlite3 *db = open_database(config.nombreBD);
    if (!db) return -1;

    sqlite3_stmt *stmt;
    const char *sql = "SELECT id_mensaje FROM Mensaje WHERE fecha_mensaje = ? AND hora_mensaje = ? AND contenido_mensaje = ? AND id_emisor = ? AND id_grupo = ?";
    int message_id = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, fecha, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, hora, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, contenido, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, idEmisor);
        sqlite3_bind_int(stmt, 5, idGrupo);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            message_id = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return message_id;
}