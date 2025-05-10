#include <stdlib.h>
#include "estructuras.h"
#include "baseDatos.h"
#include "config.h"

extern Config config;

void cargarDatosCsvEnBD(Usuario* usuarios, Grupo* grupos, Mensaje* mensajes){
    if(atoi(config.loadCSV)){
        // Loop para insertar usuarios en la base de datos
        for (int i = 0; i < 50; i++)
        {
            Usuario usuarioActual = usuarios[i];
            insertarUsuario(usuarioActual.nombre, usuarioActual.email, usuarioActual.telefono, usuarioActual.fNacimiento, usuarioActual.contra);
        }

        // Loop para insertar grupos en la base de datos
        for (int i = 0; i < 67; i++)
        {
            Grupo GrupoActual = grupos[i];
            insert_group(&GrupoActual);
        }

        // Loop para insertar mensajes en la base de datos
        for (int i = 0; i < 530; i++)
        {
            Mensaje mensajeActual = mensajes[i];
            insert_mensaje(&mensajeActual);
        }
    }
}