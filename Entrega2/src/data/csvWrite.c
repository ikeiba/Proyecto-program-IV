#include <stdio.h>
#include <stdlib.h>
#include "../estructuras.h"
#include "baseDatos/baseDatos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

extern Config config;

void exportarBD_CSV() {
    /*
    if(atoi(config.copiaBBDD)){
        return;
    }
    */
    FILE *file;
    
    // Exportar Administradores
    Administrador *administradores = NULL;
    int numAdministradores = 0;
    printf("Intentando obtener administradores...\n");
    if (obtenerAdministradores(&administradores, &numAdministradores)) {
        printf("Administradores obtenidos: %d\n", numAdministradores);
        file = fopen("./copiaSeguridadBD/copiaAdministradores.csv", "w");
        if (!file) {
            printf("Error abriendo copiaAdministradores.csv");
            return;
        }
        fprintf(file, "ID,Nombre,Email,Telefono,FechaNacimiento,Contrasena,Nivel\n");
        for (int i = 0; i < numAdministradores; i++) {
            fprintf(file, "%d,%s,%s,%s,%s,%s,%d\n", administradores[i].id, administradores[i].nombre,
                    administradores[i].email, administradores[i].telefono, administradores[i].fNacimiento,
                    administradores[i].contra, administradores[i].nivel);
        }
        fclose(file);
    } else {
        printf("Error al obtener administradores.\n");
    }
    
    // Exportar Usuarios
    Usuario *usuarios = NULL;
    int numUsuarios = 0;
    printf("Intentando obtener usuarios...\n");
    if (obtenerUsuarios(&usuarios, &numUsuarios)) {
        printf("Usuarios obtenidos: %d\n", numUsuarios);
        file = fopen("./copiaSeguridadBD/copiaUsuarios.csv", "w");
        if (!file) {
            printf("Error abriendo copiaUsuarios.csv");
            return;
        }
        fprintf(file, "ID,Nombre,Email,Telefono,FechaNacimiento,Contrasena\n");
        for (int i = 0; i < numUsuarios; i++) {
            fprintf(file, "%d,%s,%s,%s,%s,%s\n", usuarios[i].id, usuarios[i].nombre,
                    usuarios[i].email, usuarios[i].telefono, usuarios[i].fNacimiento,
                    usuarios[i].contra);
        }
        fclose(file);
    } else {
        printf("Error al obtener usuarios.\n");
    }
    
    // Exportar Grupos
    int *ids = NULL, *idCreador = NULL, numGrupos = 0;
    char **nombres = NULL, **fechas = NULL, **descripciones = NULL;
    printf("Intentando obtener grupos...\n");
    if (obtenerGrupos(&ids, &nombres, &fechas, &idCreador, &descripciones, &numGrupos)) {
        printf("Grupos obtenidos: %d\n", numGrupos);
        file = fopen("./copiaSeguridadBD/copiaGrupos.csv", "w");
        if (!file) {
            perror("Error abriendo copiaGrupos.csv");
            return;
        }
        fprintf(file, "ID,Nombre,FechaCreacion,ID_Creador,Descripcion\n");
        for (int i = 0; i < numGrupos; i++) {
            fprintf(file, "%d,%s,%s,%d,%s\n", ids[i], nombres[i], fechas[i], idCreador[i], descripciones[i]);
        }
        fclose(file);
    } else {
        printf("Error al obtener grupos.\n");
    }
    
    // Exportar Conversaciones
    int *idUsuarios = NULL, *idGrupos = NULL, numConversaciones = 0;
    printf("Intentando obtener conversaciones...\n");
    if (obtenerConversaciones(&idUsuarios, &idGrupos, &numConversaciones)) {
        printf("Conversaciones obtenidas: %d\n", numConversaciones);
        file = fopen("./copiaSeguridadBD/copiaConversaciones.csv", "w");
        if (!file) {
            perror("Error abriendo copiaConversaciones.csv");
            return;
        }
        fprintf(file, "ID_Usuario,ID_Grupo\n");
        for (int i = 0; i < numConversaciones; i++) {
            fprintf(file, "%d,%d\n", idUsuarios[i], idGrupos[i]);
        }
        fclose(file);
    } else {
        printf("Error al obtener conversaciones.\n");
    }
    
    // Exportar Mensajes
    int *idMensajes = NULL, *idEmisores = NULL, *idGruposMensajes = NULL, numMensajes = 0;
    char **fechasMensajes = NULL, **horasMensajes = NULL, **contenidos = NULL;
    printf("Intentando obtener mensajes...\n");
    if (obtenerMensajes(&idMensajes, &fechasMensajes, &horasMensajes, &contenidos, &idEmisores, &idGruposMensajes, &numMensajes)) {
        printf("Mensajes obtenidos: %d\n", numMensajes);
        file = fopen("./copiaSeguridadBD/copiaMensajes.csv", "w");
        if (!file) {
            perror("Error abriendo copiaMensajes.csv");
            return;
        }
        fprintf(file, "ID,Fecha,Hora,Contenido,ID_Emisor,ID_Grupo\n");
        for (int i = 0; i < numMensajes; i++) {
            fprintf(file, "%d,%s,%s,%s,%d,%d\n", idMensajes[i], fechasMensajes[i], horasMensajes[i], contenidos[i], idEmisores[i], idGruposMensajes[i]);
        }
        fclose(file);
    } else {
        printf("Error al obtener mensajes.\n");
    }
}
