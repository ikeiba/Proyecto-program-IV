#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "socket.h"
#include <sstream>
#include <string>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

SOCKET s;
char sendBuff[16384], recvBuff[16384];
Usuario** usuarios;
int numUsuarios;

Grupo** grupos;
int numGrupos;

Mensaje** mensajes;
int numMensajes;

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
    char copia[16384];

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
    char copia[16384];
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

void leerGrupos(Grupo** grupos, int* numGrupos, char* recvBuff) {
    // Hacemos una copia del buffer para no modificar el original
	printf("Recibido: %s\n", recvBuff);
    char copia[16384];
    strcpy(copia, recvBuff);

    *numGrupos = contarElementos(copia);

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
        std::getline(ss, campo, ','); idUsuario = std::stoi(campo);
        std::getline(ss, fechaCreacion, ',');

        Usuario* creador = obtenerUsuarioPorId(idUsuario, NULL, 0);

        grupos[i] = new Grupo(idGrupo, nombre.c_str(), descripcion.c_str(), creador, fechaCreacion.c_str(), NULL, 0);

        token = strtok(NULL, ";");
        i++;
    }
}

void leerMensajes(Mensaje** mensajes, int* numMensajes, char* recvBuff) {
    // Copia del buffer para no modificar el original
    char copia[16384];
    strcpy(copia, recvBuff);

    *numMensajes = contarElementos(copia);

    // Hacemos la copia real para el strtok
    strcpy(copia, recvBuff);
    char* token = strtok(copia, ";");
    int i = 0;

    while (token != NULL && i < *numMensajes) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idMensaje;
        std::string contenido, tipo, fecha;
        int idUsuario, idGrupo;

        std::getline(ss, campo, ','); idMensaje = std::stoi(campo);
        std::getline(ss, contenido, ',');
        std::getline(ss, tipo, ',');
        std::getline(ss, fecha, ',');
        std::getline(ss, campo, ','); idUsuario = std::stoi(campo);
        std::getline(ss, campo, ','); idGrupo = std::stoi(campo);

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, NULL, 0);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, NULL, 0);

        mensajes[i] = new Mensaje(idMensaje, contenido.c_str(), tipo.c_str(), fecha.c_str(), usuario, grupo);

        token = strtok(NULL, ";");
        i++;
    }
}

void leerConversacion(char* recvBuff, Grupo** grupos, int numGrupos, Usuario** usuarios, int tamanyo) {
    char copia[16384];
    strcpy(copia, recvBuff);

    char* token = strtok(copia, ";");
    while (token != NULL) {
        std::string linea(token);
        std::stringstream ss(linea);
        std::string campo;

        int idUsuario, idGrupo;
        std::getline(ss, campo, ','); idUsuario = std::stoi(campo);
        std::getline(ss, campo, ','); idGrupo = std::stoi(campo);

        Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, tamanyo);
        Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, numGrupos);

        if (grupo && usuario) {
            grupo->addMiembro(usuario);
        }

        token = strtok(NULL, ";");
    }
}

int getUsuario() {
	sprintf(sendBuff, "GET;USUARIO;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	printf("Mensaje mandado: %s\n", sendBuff);
	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerUsuarios(&usuarios, &numUsuarios, recvBuff);
	printf("Usuario recibido: %s\n", usuarios[0]->getNombre());
	return 0;
	
}

int getGrupos() {
	sprintf(sendBuff, "GET;GRUPOS;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerGrupos(grupos, &numGrupos, recvBuff);
	printf("Grupos recibidos: %s\n", grupos[0]->getNombre());
	return 0;
	
}

int getMensajes() {
	sprintf(sendBuff, "GET;MENSAJES;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerMensajes(mensajes, &numMensajes, recvBuff);
	printf("Mensajes recibidos: %s\n", mensajes[0]->getContenido());
	return 0;	
}

int getConversaciones() {

	sprintf(sendBuff, "GET;CONVERSACIONES;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerConversacion(recvBuff, grupos, numGrupos, usuarios, numUsuarios);
	printf("Conversaciones recibidas:%s \n",grupos[0]->getMiembros()[0]->getNombre());
	return 0;
}

int getGeneral() {
	inicializarSocket();
	
	getUsuario();
	getGrupos();
	getMensajes();
	getConversaciones();
	
	sprintf(sendBuff, "Bye;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	closesocket(s);
	WSACleanup();
	return 0;
}

void actualizarDatos() {
	inicializarSocket();

	sprintf(sendBuff, "REFRESH;");
	send(s, sendBuff, sizeof(sendBuff), 0);

    recv(s, recvBuff, sizeof(recvBuff), 0);
	if (strcmp(recvBuff, "NOTHING") == 0) {
        printf("Sin datos nuevos.\n");
    } else {
        printf("Datos nuevos:\n%s\n", recvBuff);
    }

	sprintf(sendBuff, "Bye;");
	send(s, sendBuff, sizeof(sendBuff), 0);
	closesocket(s);
    WSACleanup();

}

int enviarMensaje(const char* fecha, const char* hora, const char* contenido, int idEmisor, int idGrupo) {
	inicializarSocket();                                           

    sprintf(sendBuff, "UPDATE;enviar;%s,%s,%s,%i,%i", fecha, hora, contenido, idEmisor, idGrupo);
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

    sprintf(sendBuff, "UPDATE;crear;%s,%s,%i,%s", nombre, fCreacion, idCreador, descripcion);
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
int aniadirUsuarioAGrupo(int idUsuario, int idGrupo) {
	inicializarSocket();

    sprintf(sendBuff, "UPDATE;aniadir;%i,%i", idUsuario, idGrupo);
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

    sprintf(sendBuff, "UPDATE;abandonar;%i,%i", idUsuario, idGrupo);
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
























