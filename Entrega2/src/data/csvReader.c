#include "csvReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Usuario* leerCsvUsuarios() {
    FILE *file = fopen("C:/Proyecto-program-IV/Entrega2/src/data/usuarios.csv", "r");
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
