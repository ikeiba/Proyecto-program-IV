#include "gestionMensaje.h"
#include "../baseDatos/baseDatos.h"
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

extern Usuario** usuarios;
extern int numUsuarios;

extern Grupo** grupos;
extern int numGrupos;

extern int* idUsuarios;
extern int* idGrupos;
extern int* idConversacion;
extern int numConversaciones;

extern Mensaje** mensajes;
extern int numMensajes;

char* currentEmail;

Usuario** usuariosCliente;
int numUsuariosCliente;
Grupo** gruposCliente;
int numGruposCliente;
Mensaje** mensajesCliente;
int numMensajesCliente;
int* idUsuariosCliente;
int* idGruposCliente;
int* idConversacionCliente;
int numConversacionesCliente;

Grupo** filtrarGruposPorEmail(char* email, Usuario** usuarios, int numUsuarios, Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int numConversaciones,
    int* numGruposFiltradosC) {
    //Quiero los grupos en los que el current email es miembro
    Grupo** gruposFiltrados = (Grupo**)malloc(sizeof(Grupo*) * numGrupos);
    int numGruposFiltrados = 0;
    int idUsuario = usuarioPorEmail(email, usuarios, numUsuarios);
    for(int i = 0; i < numConversaciones; i++){
        if(idUsuario == idUsuarios[i]){
            int idGrupo = idGrupos[i];
            Grupo* g = grupoPorId(idGrupo, grupos, numGrupos);
            gruposFiltrados[numGruposFiltrados] = g;
            numGruposFiltrados++;
        }
    }
    //Redimensionar el array de grupos filtrados
    gruposFiltrados = (Grupo**)realloc(gruposFiltrados, sizeof(Grupo*) * numGruposFiltrados);
    //Devolver el array de grupos filtrados
    *numGruposFiltradosC = numGruposFiltrados;

    return gruposFiltrados;
}

int usuarioPorEmail(char* email, Usuario** usuarios, int numUsuarios) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i]->email, email) == 0) {
            return usuarios[i]->id;
        }
    }
    return -1; // No encontrado
}

Grupo* grupoPorId(int idGrupo, Grupo** grupos, int numGrupos) {
    for (int i = 0; i < numGrupos; i++) {
        if (grupos[i]->id == idGrupo) {
            return grupos[i];
        }
    }
    return NULL; // No encontrado
}

Usuario* usuarioPorId(int idUsuario, Usuario** usuarios, int numUsuarios) {
    for (int i = 0; i < numUsuarios; i++) {
        if (usuarios[i]->id == idUsuario) {
            return usuarios[i];
        }
    }
    return NULL;
}

Usuario** filtrarUsuariosPorGrupo(Usuario** usuarios, int numUsuarios, Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int numConversaciones,
    int* numUsuariosFiltrados) {
    // Asumimos que como máximo, todos los miembros de todos los grupos son únicos
    int maxUsuarios = numUsuarios;

    Usuario** usuariosFiltrados = (Usuario**)malloc(sizeof(Usuario*) * maxUsuarios);
    *numUsuariosFiltrados = 0;

    for (int i = 0; i < numGrupos; i++) {
        Grupo* grupo = grupos[i];
        for (int j = 0; j < numConversaciones; j++) {

            if(grupo->id == idGrupos[j]) {
                int yaAgregado = 0;
                for (int k = 0; k < *numUsuariosFiltrados; k++) {
                    if (usuariosFiltrados[k]->id == idUsuarios[j]) {
                        yaAgregado = 1;
                        break;
                    }
                }

                if (!yaAgregado) {
                    Usuario* usuario = usuarioPorId(idUsuarios[j], usuarios, numUsuarios);
                    usuariosFiltrados[*numUsuariosFiltrados] = usuario;
                    (*numUsuariosFiltrados)++;
                }
            }
            
        }
    }

    printf("numUsuariosFiltrados: %d\n", *numUsuariosFiltrados);

    // Redimensionar al tamaño exacto
    usuariosFiltrados = (Usuario**)realloc(usuariosFiltrados, sizeof(Usuario*) * (*numUsuariosFiltrados));
    return usuariosFiltrados;
}

Mensaje** filtrarMensajesPorGrupos(Grupo** grupos,int numGrupos, Mensaje** mensajes, int numMensajes, int* numMensajesFiltrados) {
    // Asumimos que como máximo, todos los mensajes de todos los grupos son únicos
    int maxMensajes = numMensajes;

    Mensaje** mensajesFiltrados = (Mensaje**)malloc(sizeof(Mensaje*) * maxMensajes);
    *numMensajesFiltrados = 0;

    for (int i = 0; i < numMensajes; i++) {
        Mensaje* mensaje = mensajes[i];
        Grupo* grupo = mensaje->grupo;

        // Verificar si el grupo del mensaje está en la lista de grupos
        int grupoEncontrado = 0;
        for (int j = 0; j < numGrupos; j++) {
            if (grupo == grupos[j]) {
                grupoEncontrado = 1;
                break;
            }
        }

        // Si el grupo no está en la lista, lo saltamos
        if (!grupoEncontrado) {
            continue;
        }

        // Verificar si ya está en la lista (por id)
        int yaAgregado = 0;
        for (int k = 0; k < *numMensajesFiltrados; k++) {
            if (mensajesFiltrados[k]->id == mensaje->id) {
                yaAgregado = 1;
                break;
            }
        }

        if (!yaAgregado) {
            mensajesFiltrados[*numMensajesFiltrados] = mensaje;
            (*numMensajesFiltrados)++;
        }
    }

    // Redimensionar al tamaño exacto
    mensajesFiltrados = (Mensaje**)realloc(mensajesFiltrados, sizeof(Mensaje*) * (*numMensajesFiltrados));
    return mensajesFiltrados;
}

int filtrarConversacionesPorGrupos(Grupo** grupos, int numGrupos, int* idUsuarios, int* idGrupos, int* idConversacion, int numConversaciones, int** idUsuariosFiltrados, int** idGruposFiltrados, int** idConversacionesFiltradas, int* numConversacionesFiltradas) {
    // Conversaciones son relaciones entre usuario-grupo, esto es miembros de grupos
    // Asumimos que como máximo, todos los mensajes de todos los grupos son únicos
    int maxConversaciones = numConversaciones;

    int* idUsuariosFiltradosTemp = (int*)malloc(sizeof(int) * maxConversaciones);
    int* idGruposFiltradosTemp = (int*)malloc(sizeof(int) * maxConversaciones);
    int* idConversacionTemp = (int*)malloc(sizeof(int) * maxConversaciones);
    *numConversacionesFiltradas = 0;
    for (int i = 0; i < numConversaciones; i++) {
        int idUsuario = idUsuarios[i];
        int idGrupo = idGrupos[i];

        // Verificar si el grupo del mensaje está en la lista de grupos
        int grupoEncontrado = 0;
        for (int j = 0; j < numGrupos; j++) {
            if (idGrupo == grupos[j]->id) {
                grupoEncontrado = 1;
                break;
            }
        }

        // Si el grupo no está en la lista, lo saltamos
        if (!grupoEncontrado) {
            continue;
        }

        // Verificar si ya está en la lista (por id)
        int yaAgregado = 0;
        for (int k = 0; k < *numConversacionesFiltradas; k++) {
            if (idUsuariosFiltradosTemp[k] == idUsuario && idGruposFiltradosTemp[k] == idGrupo) {
                yaAgregado = 1;
                break;
            }
        }

        if (!yaAgregado) {
            idUsuariosFiltradosTemp[*numConversacionesFiltradas] = idUsuario;
            idGruposFiltradosTemp[*numConversacionesFiltradas] = idGrupo;
            idConversacionTemp[*numConversacionesFiltradas] = idConversacion[i];
            (*numConversacionesFiltradas)++;
        }
    }
    // Redimensionar al tamaño exacto
    idUsuariosFiltradosTemp = (int*)realloc(idUsuariosFiltradosTemp, sizeof(int) * (*numConversacionesFiltradas));
    idGruposFiltradosTemp = (int*)realloc(idGruposFiltradosTemp, sizeof(int) * (*numConversacionesFiltradas));
    idConversacionTemp = (int*)realloc(idConversacionTemp, sizeof(int) * (*numConversacionesFiltradas));
    *idUsuariosFiltrados = idUsuariosFiltradosTemp;
    *idGruposFiltrados = idGruposFiltradosTemp;
    *idConversacionesFiltradas = idConversacionTemp;
    return 0;
}

int gestionarMesajeINI(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    char* email = strtok(NULL, ";");
    char* contrasenya = strtok(NULL, ";");

    int existe = comprobarCredenciales(email, contrasenya); 
    //printf("%d\n", existe);
    if(existe){
        printf("Sending reply... \n");
        strcpy(sendBuff, "CORRECT");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return 1;
    }
    printf("Sending reply... \n");
    strcpy(sendBuff, "ERROR");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
    return 0;
}

int gestionarMensajeREG(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    char* nombre = strtok(NULL, ",");
    char* email = strtok(NULL, ",");
    char* telefono = strtok(NULL, ",");
    char* f_nacimiento = strtok(NULL, ",");
    char* contrasenya = strtok(NULL, ",");

    if(insertarUsuario(nombre, email, telefono, f_nacimiento, contrasenya) == 0){
        printf("Sending reply... \n");
        strcpy(sendBuff, "ERROR");
        borrarUsuario("amayamanuela@gmail.com");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Data sent: %s \n", sendBuff);
        return -1;
    }

    //Codigo de prueba 
    printf("Sending reply... \n");
    strcpy(sendBuff, "CORRECT");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
    return 1;
}

char* usuariosToString(Usuario** usuarios, int numUsuarios){
    char* result = (char*)malloc(32768);
    strcpy(result, "");
    for(int i = 0; i < numUsuarios; i++){
        char buffer[256];
        sprintf(buffer, "%d,%s,%s,%s,%s,%s;", usuarios[i]->id, usuarios[i]->nombre, usuarios[i]->email, usuarios[i]->telefono, usuarios[i]->fNacimiento, usuarios[i]->contra);
        strcat(result, buffer);
    }
    return result;
}

char* gruposToString(Grupo** grupos, int numGrupos){
    char* result = (char*)malloc(32768);
    strcpy(result, "");
    for(int i = 0; i < numGrupos; i++){

        char buffer[1024];
        sprintf(buffer, "%d,%s,%s,%d,%s;", grupos[i]->id, grupos[i]->nombre, grupos[i]->fCreacion, grupos[i]->creador->id, grupos[i]->descripcion);
        strcat(result, buffer);
    }
    return result;
}

char* mensajeToString(Mensaje** mensajes, int numMensajes){
    char* result = (char*)malloc(32768);
    strcpy(result, "");
    for(int i = 0; i < numMensajes; i++){
        char buffer[1024];
        sprintf(buffer, "%d*%s*%s*%s*%d*%d;", mensajes[i]->id, mensajes[i]->fecha, mensajes[i]->hora, mensajes[i]->contenido, mensajes[i]->emisor->id, mensajes[i]->grupo->id);
        strcat(result, buffer);
    }
    return result;
}

char* conversacionToString(int* idUsuarios, int* idGrupos, int* idConversacion,  int numConversaciones){
    char* result = (char*)malloc(16384);
    strcpy(result, "");
    for(int i = 0; i < numConversaciones; i++){
        char buffer[1024];
        sprintf(buffer, "%d,%d,%d;", (idUsuarios)[i], (idGrupos)[i], (idConversacion)[i]);
        strcat(result, buffer);
    }
    return result;
}

int gestionarMensajeGET(char* sendBuff, char* recvBuff, SOCKET* comm_socket){
    //Reiniciar buffer
    memset(sendBuff, 0, sizeof(sendBuff));
    // Obtener el tipo de dato
    char* tipo = strtok(NULL, ";");
    if(strcmp(tipo, "USUARIO") == 0){
        currentEmail = strtok(NULL, ";");
        gruposCliente = filtrarGruposPorEmail(currentEmail, usuarios, numUsuarios, grupos, numGrupos, idUsuarios, idGrupos, numConversaciones, &numGruposCliente);
        usuariosCliente = filtrarUsuariosPorGrupo(usuarios, numUsuarios, gruposCliente, numGruposCliente, idUsuarios, idGrupos, numConversaciones, &numUsuariosCliente);
        strcpy(sendBuff, usuariosToString(usuariosCliente, numUsuariosCliente));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }
    
    if(strcmp(tipo, "GRUPO") == 0){
        strcpy(sendBuff, gruposToString(gruposCliente, numGruposCliente));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }

    if(strcmp(tipo, "MENSAJE") == 0){
        mensajesCliente = filtrarMensajesPorGrupos(gruposCliente, numGruposCliente, mensajes, numMensajes, &numMensajesCliente);
        strcpy(sendBuff, mensajeToString(mensajesCliente, numMensajesCliente));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }

    if(strcmp(tipo, "CONVERSACION") == 0){
        //Filtrar las conversaciones por los grupos
        filtrarConversacionesPorGrupos(gruposCliente, numGruposCliente, idUsuarios, idGrupos, idConversacion, numConversaciones, &idUsuariosCliente, &idGruposCliente, &idConversacionCliente, &numConversacionesCliente);
        strcpy(sendBuff, conversacionToString(idUsuariosCliente, idGruposCliente, idConversacionCliente, numConversacionesCliente));
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }

    strcpy(sendBuff, "ERROR");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
}

int gestionarMensajeUPDATE(char* sendBuff, char* recvBuff, SOCKET* comm_socket)
{
    char* tipo = strtok(NULL, ";");

    if(strcmp(tipo, "ENVIAR") == 0){
        printf("Sending reply... \n");
        
        // Obtener los diferentes apartados a partir de recvBuff
        char* fecha = strtok(NULL, ",");
        char* hora = strtok(NULL, ",");
        char* contenido = strtok(NULL, ",");
        int idEmisor = atoi(strtok(NULL, ","));
        int idGrupo = atoi(strtok(NULL, ","));

        //Insertar mensaje en la base de datos
        insertarMensajeDesdeUpdate(fecha, hora, contenido, idEmisor, idGrupo);

        // Mandar mensaje de que todo ha ido bien
        sprintf(sendBuff, "UPDATED");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }
    
    if(strcmp(tipo, "CREAR") == 0){
        printf("Sending reply... \n");
        
        // Obtener los diferentes apartados a partir de recvBuff
        char* nombre = strtok(NULL, ",");
        char* fCreacion = strtok(NULL, ",");
        int idCreador = atoi(strtok(NULL, ","));
        char* descripcion = strtok(NULL, ",");

        //Insertar mensaje en la base de datos
        insertarGrupoDesdeUpdate(nombre, fCreacion, idCreador, descripcion);

        // Mandar mensaje de que todo ha ido bien
        sprintf(sendBuff, "UPDATED");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }

    if(strcmp(tipo, "ANADIR") == 0){
        printf("Sending reply... \n");
        
        // Obtener los diferentes apartados a partir de recvBuff
        char* email = strtok(NULL, ",");
        int idUsuario = usuarioPorEmail(email, usuarios, numUsuarios);
        int idGrupo = atoi(strtok(NULL, ","));

        //Insertar mensaje en la base de datos
        insertarConversacionDesdeUpdate(idUsuario, idGrupo);

        int idConversacionInt = obetenerIdConversacion(idUsuario, idGrupo);
        // Añadir el idConversacion al array de conversaciones, teniendo en cuenta que idConversacion es un array con un tamaño predeterminado
        idConversacion = (int*) realloc(idConversacion, sizeof(int) * (numConversaciones + 1));
        idConversacion[numConversaciones] = idConversacionInt;
        printf("ID CONVERSACION: %d\n", idConversacion[numConversaciones]);
        idUsuarios = (int*) realloc(idUsuarios, sizeof(int) * (numConversaciones + 1));
        idUsuarios[numConversaciones] = idUsuario;
        printf("ID USUARIO: %d\n", idUsuarios[numConversaciones]);
        idGrupos = (int*) realloc(idGrupos, sizeof(int) * (numConversaciones + 1));
        idGrupos[numConversaciones] = idGrupo;
        numConversaciones++;


        // Mandar mensaje de que todo ha ido bien
        sprintf(sendBuff, "UPDATED");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }

    if(strcmp(tipo, "ABANDONAR") == 0){
        printf("Sending reply... \n");
        
        // Obtener los diferentes apartados a partir de recvBuff
        int idUsuario = atoi(strtok(NULL, ","));
        int idGrupo = atoi(strtok(NULL, ","));

        printf("ESTO ES UNA TONTERIAs");
        //Insertar mensaje en la base de datos
        abandonarGrupoDesdeUpdate(idUsuario, idGrupo);
        // Mandar mensaje de que todo ha ido bien
        sprintf(sendBuff, "UPDATED");
        send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        return -1;
    }
    
    printf("Sending reply... \n");
    strcpy(sendBuff, "ERROR");
    send(*comm_socket, sendBuff, strlen(sendBuff), 0);
    printf("Data sent: %s \n", sendBuff);
}

int gestionarMensajeREFRESH(char* sendBuff, char* recvBuff, SOCKET* comm_socket) {
    char* tipo = strtok(NULL, ";");
    memset(sendBuff, 0, sizeof(sendBuff));
    
    if(strcmp(tipo, "CONVERSACION") == 0){
        int ultimaIdConversacion = atoi(strtok(NULL, ","));
        int idCliente = atoi(strtok(NULL, ","));
        printf("Sending reply REFRESH CONVERSACION... \n");
        char* nuevasConversaciones = mandarNuevasConversaciones(idCliente, ultimaIdConversacion);
        
        printf("Nuevas conversaciones: %s\n", nuevasConversaciones);
        if(strcmp(nuevasConversaciones, "") == 0){
            strcpy(sendBuff, "NOTHING");
            send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        }else{
            strcpy(sendBuff, nuevasConversaciones);
            send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        }
        return -1;
    }
    
    if(strcmp(tipo, "MENSAJE") == 0){
        int ultimoIdMensaje = atoi(strtok(NULL, ","));
        int idCliente = atoi(strtok(NULL, ","));
        printf("Sending reply REFRESH MENSAJE... \n");
        char* nuevosMensajes = mandarNuevosMensajes(idCliente, ultimoIdMensaje);
        if(strcmp(nuevosMensajes, "") == 0){
            strcpy(sendBuff, "NOTHING");
            send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        }else{
            strcpy(sendBuff, nuevosMensajes);
            send(*comm_socket, sendBuff, strlen(sendBuff), 0);
        }
        return -1;
    }
}

char* mandarNuevosMensajes(int idCliente, int idUltimoMensaje){
    // Filtrar los mensajes por el cliente y la última conversación
    filtrarGruposPorEmail(usuarioPorId(idCliente, usuarios, numUsuarios)->email, usuarios, numUsuarios, grupos, numGrupos, idUsuarios, idGrupos, numConversaciones, &numGruposCliente);
    Mensaje** mensajesFiltrados = (Mensaje**)malloc(sizeof(Mensaje*) * numMensajes);
    int numMensajesFiltrados = 0;

    for (int i = 0; i < numMensajes; i++) {
        if (mensajes[i]->id <= idUltimoMensaje) {
            continue; // Saltar mensajes anteriores
        }

        printf("Mensaje: %d, Grupo: %d, Usuario: %d\n", mensajes[i]->id, mensajes[i]->grupo->id, mensajes[i]->emisor->id);

        for(int j = 0; j < numGruposCliente; j++) {
            if (mensajes[i]->grupo->id == gruposCliente[j]->id) {
                mensajesFiltrados[numMensajesFiltrados] = mensajes[i];
                numMensajesFiltrados++;
                break;
            }
        }

    }

    // Redimensionar al tamaño exacto
    mensajesFiltrados = (Mensaje**)realloc(mensajesFiltrados, sizeof(Mensaje*) * numMensajesFiltrados);

    char* result = mensajeToString(mensajesFiltrados, numMensajesFiltrados);
    free(mensajesFiltrados);
    
    return result;
}

int filtrarMiembrosViejos(int idGrupo, int idUltimaConversacion, int* idUsuariosCliente, int* idGruposCliente, int* idConversacionCliente, int* numConversacionesCliente) {
    for (int i = 0; i < numConversaciones; i++) {
        if(idConversacion[i] >= idUltimaConversacion) {
            continue;
        }

        if (idGrupos[i] == idGrupo) {
            idUsuariosCliente[*numConversacionesCliente] = idUsuarios[i];
            idGruposCliente[*numConversacionesCliente] = idGrupo;
            idConversacionCliente[*numConversacionesCliente] = idConversacion[i];
            (*numConversacionesCliente)++;
        }
    }

    return 0;
}

int filtrarConversacionesNuevas(int idCliente, int idUltimaConversacion) {
    int* idUsuariosFiltrados = (int*)malloc(sizeof(int) * numConversaciones);
    int* idGruposFiltrados = (int*)malloc(sizeof(int) * numConversaciones);
    int* idConversacionFiltrados = (int*)malloc(sizeof(int) * numConversaciones);
    numConversacionesCliente = 0;

    for (int i = 0; i < numConversaciones; i++) {
        if(idConversacion[i] <= idUltimaConversacion) {
            continue; // Saltar conversaciones anteriores
        }
        
        int idUsuario = idUsuarios[i];
        int idGrupo = idGrupos[i];

        if(idUsuario == idCliente) {
            printf("Conversacion: %d, Grupo: %d, Usuario: %d\n", idConversacion[i], idGrupo, idUsuario);
            idUsuariosFiltrados[numConversacionesCliente] = idUsuario;
            idGruposFiltrados[numConversacionesCliente] = idGrupo;
            idConversacionFiltrados[numConversacionesCliente] = idConversacion[i];
            numConversacionesCliente++;
            filtrarMiembrosViejos(idGrupo, idConversacion[i], idUsuariosFiltrados, idGruposFiltrados, idConversacionFiltrados, &numConversacionesCliente);
            continue;
        }
        
        for (int j = 0; j < numGruposCliente; j++) {
            if (idGrupo == gruposCliente[j]->id) {
                idUsuariosCliente[numConversacionesCliente] = idUsuario;
                idGruposCliente[numConversacionesCliente] = idGrupo;
                idConversacionCliente[numConversacionesCliente] = idConversacion[i];
                numConversacionesCliente++;
                break;
            }
        }
    }

    // Redimensionar al tamaño exacto
    idUsuariosFiltrados = (int*)realloc(idUsuariosFiltrados, sizeof(int) * numConversacionesCliente);
    idGruposFiltrados = (int*)realloc(idGruposFiltrados, sizeof(int) * numConversacionesCliente);
    idConversacionFiltrados = (int*)realloc(idConversacionFiltrados, sizeof(int) * numConversacionesCliente);
    idUsuariosCliente = idUsuariosFiltrados;
    idGruposCliente = idGruposFiltrados;
    idConversacionCliente = idConversacionFiltrados;
    printf("Numero de conversaciones filtradas: %d\n", numConversacionesCliente);

    return 0;
}

char* mandarNuevasConversaciones(int idCliente, int idUltimaConversacion) {

    filtrarGruposPorEmail(usuarioPorId(idCliente, usuarios, numUsuarios)->email, usuarios, numUsuarios, grupos, numGrupos, idUsuarios, idGrupos, numConversaciones, &numGruposCliente);
    printf("Numero de grupos del cliente: %d\n", numGruposCliente);

    /**for(int i = 0; i < numGruposCliente; i++) {
        printf("Grupo %d: %s\n", gruposCliente[i]->id, gruposCliente[i]->nombre);
    }**/

    filtrarConversacionesNuevas(idCliente, idUltimaConversacion);

    printf("Numero de conversaciones del cliente: %d\n", numConversacionesCliente);

    char* result = (char*)malloc(32768);
    strcpy(result, "");
    Grupo* grupoTemp;
    Usuario* usuarioTemp;

    for(int i = 0; i < numConversacionesCliente; i++) {
        char buffer[1024];
        
        grupoTemp = grupoPorId(idGruposCliente[i], grupos, numGrupos);
        usuarioTemp = usuarioPorId(idUsuariosCliente[i], usuarios, numUsuarios);
        if(idCliente == idUsuariosCliente[i]){
            sprintf(buffer, "%d,%d,%d,%d,%s,%s,%d,%s;", idUsuariosCliente[i], idGruposCliente[i], idConversacionCliente[i], grupoTemp->id, grupoTemp->nombre, grupoTemp->fCreacion, grupoTemp->creador->id, grupoTemp->descripcion);
        }else{
            sprintf(buffer, "%d,%d,%d,%d,%s,%s,%s,%s,%s;", idUsuariosCliente[i], idGruposCliente[i], idConversacionCliente[i], usuarioTemp->id, usuarioTemp->nombre, usuarioTemp->email, usuarioTemp->telefono, usuarioTemp->fNacimiento, usuarioTemp->contra);
        }
        strcat(result, buffer);
    }

    return result;
}