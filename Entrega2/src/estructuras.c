#include "estructuras.h"
#include "baseDatos.h"

void cargarDatosCsvEnBD(Usuario* usuarios, Grupo* grupos, Mensaje* mensajes){
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