#include "estructuras.h"
#include "baseDatos.h"

void cargarDatosCsvEnBD(Usuario* usuarios, Grupo* grupos, Mensaje* mensajes){
    for(int i = 0; i<50; i++){
        insertarUsuario(usuarios[i].nombre, usuarios[i].email, usuarios[i].telefono, usuarios[i].fNacimiento, usuarios[i].contra);
    }
}