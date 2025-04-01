#include "csvReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Usuario* leerCsvUsuarios() {
    FILE *file = fopen("src/data/usuarios.csv", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    Usuario* usuarios = (Usuario*) malloc(50 * sizeof(Usuario));
    if (!usuarios) {
        perror("Error al asignar memoria para usuarios");
        fclose(file);
        return NULL;
    }
    
    char linea[1024];
    int i = 0;
    fgets(linea, 1024, file);
    while (fgets(linea, 1024, file) && i < 50) {
        linea[strcspn(linea, "\n")] = 0;

        char *campo = strtok(linea, ",");
        int j = 0;
        while (campo) {
            char* campoM = (char*) malloc((strlen(campo) + 1) * sizeof(char));
            if (!campoM) {
                perror("Error al asignar memoria para un campo");
                fclose(file);
                return NULL;
            }
            strcpy(campoM, campo);

            switch (j) {
                case 0:
                    usuarios[i].id = atoi(campo);
                    free(campoM);
                    break;
                case 1:
                    usuarios[i].nombre = campoM;
                    break;
                case 2:
                    usuarios[i].email = campoM;
                    break;
                case 3:
                    usuarios[i].telefono = campoM;
                    break;
                case 4:
                    usuarios[i].fNacimiento = campoM;
                    break;
                default:
                    usuarios[i].contra = campoM;
            }
            campo = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    fclose(file);
    return usuarios;
}

void liberarUsuarios(Usuario* usuarios, int cantidad){
    for(int i = 0; i<cantidad;i++){
        free(usuarios[i].nombre);
        free(usuarios[i].email);
        free(usuarios[i].fNacimiento);
        free(usuarios[i].contra);
        free(usuarios[i].telefono);
    }
    free(usuarios);
}

Usuario* obtenerUsuarioPorId(int id, Usuario* usuarios, int tamanyo){
    Usuario* u;
    for(int i = 0; i<tamanyo; i++){
        if(usuarios[i].id == id){
            u = &usuarios[i];
        }
    }
    return u;
}

Grupo* obtenerGrupoPorId(int id, Grupo* grupos, int tamanyo){
    Grupo* g;
    for(int i = 0; i<tamanyo; i++){
        if(grupos[i].id == id){
            g = &grupos[i];
        }
    }
    return g;
}

Grupo* leerCsvGrupos(Usuario* usuarios){
    FILE *file = fopen("src/data/grupos.csv", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    Grupo* grupos = (Grupo*) malloc(67 * sizeof(Grupo));
    if (!grupos) {
        perror("Error al asignar memoria para usuarios");
        fclose(file);
        return NULL;
    }

    char linea[1024];
    int i = 0;
    fgets(linea, 1024, file);
    while (fgets(linea, 1024, file) && i < 67) {
        linea[strcspn(linea, "\n")] = 0;

        char *campo = strtok(linea, ",");
        int j = 0;
        while (campo) {
            char* campoM = (char*) malloc((strlen(campo) + 1) * sizeof(char));
            Usuario* u;
            if (!campoM) {
                perror("Error al asignar memoria para un campo");
                fclose(file);
                return NULL;
            }
            strcpy(campoM, campo);

            switch (j) {
                case 0:
                    grupos[i].id = atoi(campo);
                    free(campoM);
                    break;
                case 1:
                    grupos[i].nombre = campoM;
                    break;
                case 2:
                    grupos[i].fCreacion = campoM;
                    break;
                case 3:
                    u = obtenerUsuarioPorId(atoi(campo),usuarios,50);
                    grupos[i].creador = u;
                    break;
                default:
                    grupos[i].descripcion = campoM;
            }
            campo = strtok(NULL, ",");
            j++;
        }
        grupos[i].size = 0;
        i++;
    }
    fclose(file);

    return grupos;
}

void liberarGrupos(Grupo* grupos, int cantidad){
    for(int i = 0; i<cantidad;i++){
        if(grupos[i].id == 7||grupos[i].id == 30||grupos[i].id == 40||grupos[i].id == 42||grupos[i].id == 46||grupos[i].id == 48||grupos[i].id == 55){
            continue;
        }
        free(grupos[i].descripcion);
        free(grupos[i].fCreacion);
        free(grupos[i].nombre);
        free(grupos[i].miembros);
    }
    free(grupos);
}

void leerCsvConversaciones(Usuario* usuarios, Grupo* grupos) {
    FILE* file = fopen("src/data/conversaciones.csv", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }

    char linea[1024];
    int i = 0;
    fgets(linea, 1024, file); 

    while (fgets(linea, 1024, file) && i < 227) {
        linea[strcspn(linea, "\n")] = 0;

        char *campo = strtok(linea, ",");
        int j = 0;
        Usuario* u = NULL;
        Grupo* g = NULL;

        while (campo) {
            switch (j) {
                case 0:
                    u = obtenerUsuarioPorId(atoi(campo), usuarios, 50);
                    if (!u) {
                        fprintf(stderr, "Error: Usuario con ID %d no encontrado\n", atoi(campo));
                    }
                    break;
                default:
                    g = obtenerGrupoPorId(atoi(campo), grupos, 67);
                    if (!g) {
                        fprintf(stderr, "Error: Grupo con ID %d no encontrado\n", atoi(campo));
                    }
            }
            campo = strtok(NULL, ",");
            j++;
        }

        if (!g || !u) {
            fprintf(stderr, "Error: Grupo o Usuario no válido\n");
            continue;
        }

        Usuario** miembrosM = (Usuario**) malloc(sizeof(Usuario*) * (g->size + 1));
        if (!miembrosM) {
            fprintf(stderr, "Error: No se pudo asignar memoria para miembros\n");
            fclose(file);
            return;
        }

        for (int z = 0; z < g->size; z++) {
            miembrosM[z] = g->miembros[z];
        }

        miembrosM[g->size] = u;

        if (g->size != 0) {
            free(g->miembros);
        }
        g->miembros = miembrosM;

        g->size++;

        printf("Grupo actualizado: %d, tamaño: %d\n", g->id, g->size);

        i++;
    }
    fclose(file);
}

Mensaje* leerCsvMensajes(Usuario* usuarios, Grupo* grupos){
    FILE *file = fopen("src/data/mensajes.csv", "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    Mensaje* mensajes = (Mensaje*) malloc(530 * sizeof(Mensaje));
    if (!mensajes) {
        perror("Error al asignar memoria para usuarios");
        fclose(file);
        return NULL;
    }

    char linea[1024];
    int i = 0;
    fgets(linea, 1024, file);
    while (fgets(linea, 1024, file) && i < 530) {
        linea[strcspn(linea, "\n")] = 0;

        char *campo = strtok(linea, ";");
        int j = 0;
        while (campo) {
            char* campoM = (char*) malloc((strlen(campo) + 1) * sizeof(char));
            Usuario* u;
            Grupo* g;
            if (!campoM) {
                perror("Error al asignar memoria para un campo");
                fclose(file);
                return NULL;
            }
            strcpy(campoM, campo);

            switch (j) {
                case 0:
                    mensajes[i].id = atoi(campo);
                    free(campoM);
                    break;
                case 1:
                    mensajes[i].fecha = campoM;
                    break;
                case 2:
                    mensajes[i].hora = campoM;
                    break;
                case 3:
                    mensajes[i].contenido = campoM;
                    break;
                case 4:
                    u = obtenerUsuarioPorId(atoi(campo),usuarios,50);
                    mensajes[i].emisor = u;
                    free(campoM);
                    break;
                default:
                    g = obtenerGrupoPorId(atoi(campo),grupos,67);
                    mensajes[i].grupo = g;
                    free(campoM);
            }
            campo = strtok(NULL, ";");
            j++;
        }
        i++;
    }
    fclose(file);
    return mensajes;
}

void liberarMensajes(Mensaje* mensajes, int cantidad){
    for(int i = 0; i<cantidad;i++){
        free(mensajes[i].fecha);
        free(mensajes[i].hora);
        free(mensajes[i].contenido);
    }
    free(mensajes);
}