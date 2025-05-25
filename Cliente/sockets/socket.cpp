#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "socket.h"
#include <sstream>
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

SOCKET s;
char sendBuff[32768], recvBuff[32768];
Usuario** usuarios;
int numUsuarios;

Grupo** grupos;
int numGrupos;

Mensaje** mensajes;
int numMensajes;

Usuario* cliente;

int ultimoIdMensaje = 0;
int ultimoIdConversacion = 0;

int inicializarSocket(){
    WSADATA wsaData;
	struct sockaddr_in server;

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

    return 0;
}

//Si el usuario y contraseña son correctos devuelve 1, sino devuelve 0
int inicioSesion(const char* email, const char* contrasenya){
    inicializarSocket();
    sprintf(sendBuff, "INI;%s;%s", email, contrasenya);
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Data received: %s \n", recvBuff);
	sprintf(sendBuff, "Bye");
    if(strcmp(recvBuff, "ERROR") == 0){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return -1;
    }
    if(strcmp(recvBuff, "CORRECT") == 0){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return 1;
    }
	send(s, sendBuff, sizeof(sendBuff), 0);



	closesocket(s);
	WSACleanup();
	return 0;
}

int registrarse(const char* usuario, const char* email, const char* telefono, const char* f_nacimiento, const char* contrasenya) {
	inicializarSocket();
	sprintf(sendBuff, "REG;%s,%s,%s,%s,%s",usuario,email,telefono,f_nacimiento,contrasenya);
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Data received: %s \n", recvBuff);
	sprintf(sendBuff, "Bye;");
	printf(sendBuff);
	if(strcmp(recvBuff, "ERROR") == 0){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return -1;
    }
    if(strcmp(recvBuff, "CORRECT") == 0){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return 1;
    }
	send(s, sendBuff, sizeof(sendBuff), 0);

	closesocket(s);
	WSACleanup();
	return 0;
}

int contarElementos(char* recvBuffOriginal) {
    char copia[32768];

    strcpy(copia, recvBuffOriginal);
    int count = 0;
    char* token = strtok(copia, ";");
    while (token != NULL) {	
        count++;
        token = strtok(NULL, ";");
    }
    return count;
}

void leerUsuarios(Usuario*** usuarios, int* numUsuarios, char* recvBuff) {
    char copia[32768];
    strcpy(copia, recvBuff);

    *numUsuarios = contarElementos(copia);
    *usuarios = new Usuario*[*numUsuarios];

    char* token = strtok(copia, ";");
    int i = 0;

    while (token != NULL) {

        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int id;
        std::string user, email, tel, fecha, pass;

        std::getline(ss, campo, ','); id = std::stoi(campo);
        std::getline(ss, user, ',');
        std::getline(ss, email, ',');
        std::getline(ss, tel, ',');
        std::getline(ss, fecha, ',');
        std::getline(ss, pass, ',');

        // Creamos el usuario
        (*usuarios)[i] = new Usuario(id, user.c_str(), email.c_str(), tel.c_str(), fecha.c_str(), pass.c_str());

        token = strtok(NULL, ";");
        i++;
    }

    printf("Total usuarios leidos: %d\n", i);
}

Usuario* obtenerUsuarioPorEmail(const char* email, Usuario** usuarios, int numUsuarios) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i]->getEmail(), email) == 0) {
            return usuarios[i];
        }
    }
    return NULL; 
}

Usuario* obtenerUsuarioPorId(int id, Usuario** usuarios, int tamanyo){
    Usuario* u;
    for(int i = 0; i<tamanyo; i++){
        if(usuarios[i]->getId() == id){
            u = usuarios[i];
        }
    }
    return u;
}

Grupo* obtenerGrupoPorId(int id, Grupo** grupos, int tamanyo){
    Grupo* g;
    for(int i = 0; i<tamanyo; i++){
        if(grupos[i]->getId() == id){
            g = grupos[i];
        }
    }
    return g;
}

void leerGrupos(Grupo*** grupos, int* numGrupos, char* recvBuff) {
    // Hacemos una copia del buffer para no modificar el original
	//printf("Recibido: %s\n", recvBuff);
    char copia[32768];
    strcpy(copia, recvBuff);

    *numGrupos = contarElementos(copia);
    *grupos = new Grupo*[*numGrupos];

    // Rehacemos la copia porque strtok la destruye
    strcpy(copia, recvBuff);
    char* token = strtok(copia, ";");
    int i = 0;

    while (token != NULL && i < *numGrupos) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idGrupo;
        std::string nombre, descripcion, fechaCreacion;
        int idUsuario;

        std::getline(ss, campo, ','); 
		idGrupo = std::stoi(campo);
        std::getline(ss, nombre, ',');
        std::getline(ss, descripcion, ',');
        std::getline(ss, campo, ','); 
        idUsuario = std::stoi(campo);
        std::getline(ss, fechaCreacion, ',');

        Usuario* creador = obtenerUsuarioPorId(idUsuario, NULL, 0);

        (*grupos)[i] = new Grupo(idGrupo, nombre.c_str(), descripcion.c_str(), creador, fechaCreacion.c_str(), NULL, 0);

        token = strtok(NULL, ";");
        i++;
    }
    printf("Total grupos leidos: %d\n", i);
}

void leerMensajes(Mensaje*** mensajes, int* numMensajes, char* recvBuff) {
    // Copia del buffer para no modificar el original
    char copia[32768];
    strcpy(copia, recvBuff);

    *numMensajes = contarElementos(copia);
    *mensajes = new Mensaje*[*numMensajes];

    char* token = strtok(copia, ";");
    int i = 0;

    while (token != NULL && i < *numMensajes) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idMensaje;
        std::string contenido, hora, fecha;
        int idUsuario, idGrupo;

        std::getline(ss, campo, '*'); 
        idMensaje = std::stoi(campo);
        std::getline(ss, fecha, '*');
        std::getline(ss, hora, '*');
        std::getline(ss, contenido, '*');
        std::getline(ss, campo, '*'); 
        idUsuario = std::stoi(campo);
        std::getline(ss, campo, '*'); 
        idGrupo = std::stoi(campo);

        if (idMensaje > ultimoIdMensaje) {
            ultimoIdMensaje = idMensaje;
        }

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, numUsuarios);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        (*mensajes)[i] = new Mensaje(idMensaje, fecha.c_str(), hora.c_str(), contenido.c_str(), usuario, grupo);

        token = strtok(NULL, ";");
        i++;
    }
    printf("Total mensajes leidos: %d\n", i);
}

void leerConversacion(char* recvBuff, Grupo** grupos, int numGrupos, Usuario** usuarios, int tamanyo) {
    char copia[32768];
    strcpy(copia, recvBuff);

    char* token = strtok(copia, ";");
    while (token != NULL) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idUsuario, idGrupo, idConversacion;
        std::getline(ss, campo, ','); idUsuario = std::stoi(campo);
        std::getline(ss, campo, ','); idGrupo = std::stoi(campo);
        std::getline(ss, campo, ','); idConversacion = std::stoi(campo);

        if(idConversacion > ultimoIdConversacion) {
            ultimoIdConversacion = idConversacion;
        }

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, tamanyo);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        if (grupo && usuario) {
            grupo->addMiembro(usuario);
        }

        token = strtok(NULL, ";");
    }
}

int getUsuario(const char* email) {
	sprintf(sendBuff, "GET;USUARIO;%s",email);
	send(s, sendBuff, sizeof(sendBuff), 0);

	printf("Mensaje mandado: %s\n", sendBuff);
	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerUsuarios(&usuarios, &numUsuarios, recvBuff);
    cliente = obtenerUsuarioPorEmail(email, usuarios, numUsuarios);
	printf("Usuario recibido: %s\n", usuarios[0]->getNombre());
	return 0;
	
}

int getGrupos() {
	sprintf(sendBuff, "GET;GRUPO;");
	send(s, sendBuff, sizeof(sendBuff), 0);
    printf("Mensaje mandado: %s\n", sendBuff);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerGrupos(&grupos, &numGrupos, recvBuff);
	printf("Grupos recibidos: %s\n", grupos[0]->getNombre());
	return 0;
}

int getMensajes() {
	sprintf(sendBuff, "GET;MENSAJE;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerMensajes(&mensajes, &numMensajes, recvBuff);
	printf("Mensajes recibidos: %s\n", mensajes[0]->getContenido());
	return 0;	
}

int getConversaciones() {

	sprintf(sendBuff, "GET;CONVERSACION;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerConversacion(recvBuff, grupos, numGrupos, usuarios, numUsuarios);
	printf("Conversaciones recibidas:%s \n",grupos[0]->getMiembros()[0]->getNombre());
	return 0;
}

int getGeneral(const char* email) {
	inicializarSocket();
	
    memset(recvBuff, 0, sizeof(recvBuff));
	getUsuario(email);
    memset(recvBuff, 0, sizeof(recvBuff));
    getGrupos();
    memset(recvBuff, 0, sizeof(recvBuff));
	getMensajes();
    memset(recvBuff, 0, sizeof(recvBuff));
	getConversaciones();
    printf("ULTIMO ID CONVERSACION: %d\n",ultimoIdConversacion);
	
	sprintf(sendBuff, "Bye;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	closesocket(s);
	WSACleanup();
	return 0;
}

void agregarGrupo(Grupo*** grupos, int* tamanyo, Grupo* nuevoGrupo) {
    Grupo** nuevoArray = new Grupo*[*tamanyo + 1];

    for (int i = 0; i < *tamanyo; ++i) {
        nuevoArray[i] = (*grupos)[i];
    }

    nuevoArray[*tamanyo] = nuevoGrupo;

    delete[] *grupos;

    *grupos = nuevoArray;
    (*tamanyo)++;
}

void agregarUsuario(Usuario*** usuarios, int* tamanyo, Usuario* nuevoUsuario) {
    Usuario** nuevoArray = new Usuario*[*tamanyo + 1];

    for (int i = 0; i < *tamanyo; ++i) {
        nuevoArray[i] = (*usuarios)[i];
    }

    nuevoArray[*tamanyo] = nuevoUsuario;

    delete[] *usuarios;

    *usuarios = nuevoArray;
    (*tamanyo)++;
}

void leerConversacionNuevas(char* recvBuff) {
    char copia[32768];
    strcpy(copia, recvBuff);

    char* token = strtok(copia, ";");
    int i = 0;
    while (token != NULL) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idUsuario, idGrupo, idConversacion;
        std::getline(ss, campo, ','); idUsuario = std::stoi(campo);
        std::getline(ss, campo, ','); idGrupo = std::stoi(campo);
        std::getline(ss, campo, ','); idConversacion = std::stoi(campo);
        if(idUsuario == cliente->getId()){
            std::string nombre, descripcion, fechaCreacion;
            int idCreador;

            std::getline(ss, campo, ','); 
		    idGrupo = std::stoi(campo);
            std::getline(ss, nombre, ',');
            std::getline(ss, descripcion, ',');
            std::getline(ss, campo, ','); 
            idCreador = std::stoi(campo);
            std::getline(ss, fechaCreacion, ',');

            Grupo* nuevoGrupo = new Grupo(idGrupo, nombre.c_str(), descripcion.c_str(), obtenerUsuarioPorId(idUsuario,usuarios,numUsuarios), fechaCreacion.c_str(), NULL, 0);
            agregarGrupo(&grupos, &numGrupos, nuevoGrupo);
        }else{
            std::string linea(token);
            std::stringstream ss(linea);
            std::string campo;

            int id;
            std::string user, email, tel, fecha, pass;

            std::getline(ss, campo, ','); id = std::stoi(campo);
            std::getline(ss, user, ',');
            std::getline(ss, email, ',');
            std::getline(ss, tel, ',');
            std::getline(ss, fecha, ',');
            std::getline(ss, pass, ',');

            if(obtenerUsuarioPorId(idUsuario, usuarios, numUsuarios) == NULL) {
                Usuario* nuevoUsuario = new Usuario(id, user.c_str(), email.c_str(), tel.c_str(), fecha.c_str(), pass.c_str());
                agregarUsuario(&usuarios, &numUsuarios, nuevoUsuario);
            }
        }

        if(idConversacion > ultimoIdConversacion) {
            ultimoIdConversacion = idConversacion;
        }

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, numUsuarios);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        if (grupo && usuario) {
            grupo->addMiembro(usuario);
        }

        token = strtok(NULL, ";");
        i++;
    }
}

void actualizarDatosConversacion(){
    sprintf(sendBuff, "REFRESH;CONVERSACION;%d,%d", ultimoIdConversacion, cliente->getId());
    printf("Mensaje mandado: %s\n", sendBuff);
	send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
	if (strcmp(recvBuff, "NOTHING") == 0) {
        printf("Sin Conversaciones nuevas.\n");
    } else {
        leerConversacionNuevas(recvBuff);
        printf("Conversaciones nuevas recibidas: %s\n", recvBuff);
    }
}

void actualizarDatosMensaje(){
    sprintf(sendBuff, "REFRESH;MENSAJE;%d,%d", ultimoIdMensaje, cliente->getId());
    printf("Mensaje mandado: %s\n", sendBuff);
    send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
    if (strcmp(recvBuff, "NOTHING") == 0) {
        printf("Sin Mensajes nuevos.\n");
    } else {
        leerNuevosMensajes(&mensajes, &numMensajes, recvBuff);
    }
}

void actualizarDatos() {
	inicializarSocket();

    memset(recvBuff, 0, sizeof(recvBuff));
    actualizarDatosConversacion();

    memset(recvBuff, 0, sizeof(recvBuff));
    actualizarDatosMensaje();

    sprintf(sendBuff, "Bye;");
    send(s, sendBuff, sizeof(sendBuff), 0);
	
	closesocket(s);
    WSACleanup();
}

void leerNuevosMensajes(Mensaje*** mensajes, int* numMensajes, char* recvBuff) {
    // Copia del buffer para no modificar el original
    char copia[32768];
    strcpy(copia, recvBuff);

    int nuevos = contarElementos(copia);
    int total = *numMensajes + nuevos;

    // Crear nuevo array
    Mensaje** nuevoArray = new Mensaje*[total];

    // Copiar mensajes existentes
    for (int i = 0; i < *numMensajes; ++i) {
        nuevoArray[i] = (*mensajes)[i];
    }

    // Procesar nuevos mensajes
    char* token = strtok(copia, ";");
    int j = *numMensajes;  // comenzamos donde termina el anterior

    while (token != NULL && j < total) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idMensaje;
        std::string contenido, hora, fecha;
        int idUsuario, idGrupo;

        std::getline(ss, campo, '*');
        idMensaje = std::stoi(campo);
        std::getline(ss, fecha, '*');
        std::getline(ss, hora, '*');
        std::getline(ss, contenido, '*');
        std::getline(ss, campo, '*');
        idUsuario = std::stoi(campo);
        std::getline(ss, campo, '*');
        idGrupo = std::stoi(campo);

        if (idMensaje > ultimoIdMensaje) {
            ultimoIdMensaje = idMensaje;
        }

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, numUsuarios);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        nuevoArray[j] = new Mensaje(idMensaje, fecha.c_str(), hora.c_str(), contenido.c_str(), usuario, grupo);

        token = strtok(NULL, ";");
        j++;
    }

    // Liberar array antiguo
    delete[] *mensajes;

    // Actualizar punteros
    *mensajes = nuevoArray;
    *numMensajes = total;

    printf("Total mensajes añadidos: %d (Total: %d)\n", nuevos, total);
}

int enviarMensaje(const char* fecha, const char* hora, const char* contenido, int idEmisor, int idGrupo) {
	inicializarSocket();                                           

    memset(recvBuff, 0, sizeof(recvBuff));

    sprintf(sendBuff, "UPDATE;ENVIAR;%s,%s,%s,%i,%i", fecha, hora, contenido, idEmisor, idGrupo);
    send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
    printf("Data received: %s\n", recvBuff);

	sprintf(sendBuff, "Bye;");
    send(s, sendBuff, sizeof(sendBuff), 0);

	if (strcmp(recvBuff, "ERROR") == 0) {
        closesocket(s); WSACleanup();
        return -1;
    }
    if (strcmp(recvBuff, "CORRECT") == 0) {
        closesocket(s); WSACleanup();
        return 1;
    }

	closesocket(s);
    WSACleanup();
    return 0;
}
int crearGrupo(const char* nombre, const char* fCreacion, int idCreador, const char* descripcion) {
	inicializarSocket();

    memset(recvBuff, 0, sizeof(recvBuff));

    sprintf(sendBuff, "UPDATE;CREAR;%s,%s,%i,%s", nombre, fCreacion, idCreador, descripcion);
    send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
    printf("Data received: %s\n", recvBuff);

    sprintf(sendBuff, "Bye;");
    
	send(s, sendBuff, sizeof(sendBuff), 0);
	if (strcmp(recvBuff, "ERROR") == 0) {
        closesocket(s); WSACleanup();
        return -1;
    }
    if (strcmp(recvBuff, "CORRECT") == 0) {
        closesocket(s); WSACleanup();
        return 1;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
int aniadirUsuarioAGrupo(char* email, int idGrupo) {
	inicializarSocket();

    memset(recvBuff, 0, sizeof(recvBuff));

    sprintf(sendBuff, "UPDATE;ANADIR;%s,%i", email, idGrupo);
    send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
    printf("Data received: %s\n", recvBuff);

    sprintf(sendBuff, "Bye;");
    send(s, sendBuff, sizeof(sendBuff), 0);

	if (strcmp(recvBuff, "ERROR") == 0) {
        closesocket(s); WSACleanup();
        return -1;
    }
    if (strcmp(recvBuff, "CORRECT") == 0) {
        closesocket(s); WSACleanup();
        return 1;
    }

    closesocket(s);
    WSACleanup();
    return 0;

}
int abandonarGrupo(int idUsuario, int idGrupo) {
	inicializarSocket();

    memset(recvBuff, 0, sizeof(recvBuff));

    sprintf(sendBuff, "UPDATE;ABANDONAR;%i,%i", idUsuario, idGrupo);
    send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
    printf("Data received: %s\n", recvBuff);

    sprintf(sendBuff, "Bye;");
    send(s, sendBuff, sizeof(sendBuff), 0);

	if (strcmp(recvBuff, "ERROR") == 0) {
        closesocket(s); WSACleanup();
        return -1;
    }
    if (strcmp(recvBuff, "CORRECT") == 0) {
        closesocket(s); WSACleanup();
        return 1;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
























