#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "socket.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

SOCKET s;
char sendBuff[10240], recvBuff[10240];
Usuario** usuarios;
int* numUsuarios;

Grupo** grupos;
int* numGrupos;

Mensaje** mensajes;
int* numMensajes;

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

int contarElementos(const char* recvBuff) {
    int count = 0;
    for (const char* p = recvBuff; *p != '\0'; ++p) {
        if (*p == ';') count++;
    }
    return count;
}

void leerUsuarios(Usuario** usuarios, int* numUsuarios, char* recvBuff){
	printf("%i\n", 1);
	char* token = strtok(recvBuff, ";");
	*numUsuarios = contarElementos(recvBuff);
	
	printf("%i\n", 2);
	while(token != NULL){
		printf("%i\n", 3);
		usuarios[*numUsuarios] = new Usuario(atoi(strtok(token, ",")), 
						strtok(NULL, ","), strtok(NULL, ","), 
						strtok(NULL, ","), strtok(NULL, ","), 
						strtok(NULL, ","));
		printf("%i\n", 4);
		token = strtok(NULL, ";");
	}
	printf("%i\n", 1);
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

void leerGrupos(Grupo** grupos, int* numGrupos, char* recvBuff){
	char* token = strtok(recvBuff, ";");
	*numGrupos = contarElementos(recvBuff);

	while(token != NULL){
		grupos[*numGrupos] = new Grupo(atoi(strtok(token, ",")), 
						strtok(NULL, ","), strtok(NULL, ","), 
						obtenerUsuarioPorId(atoi(strtok(NULL, ",")), NULL, 0),
						strtok(NULL, ","), NULL, 0);
		
		token = strtok(NULL, ";");
	}
}

void leerMensajes(Mensaje** mensajes, int* numMensajes, char* recvBuff){
	char* token = strtok(recvBuff, ";");
	*numMensajes = contarElementos(recvBuff);

	while(token != NULL){
		mensajes[*numMensajes] = new Mensaje(atoi(strtok(token, ",")), 
						strtok(NULL, ","), strtok(NULL, ","), 
						strtok(NULL, ","), obtenerUsuarioPorId(atoi(strtok(NULL, ",")), NULL, 0),
						obtenerGrupoPorId(atoi(strtok(NULL, ",")), NULL, 0));
		
		token = strtok(NULL, ";");
	}
}

void leerConversacion(char* recvBuff,Grupo** grupos, int* numGrupos,Usuario** usuarios, int tamanyo){
	//Lo que envia el serverdor es un string con el formato "idUsuario,idGrupo;idUsuario,idGrupo;..."
	char* token = strtok(recvBuff, ";");
	int i = 0;

	while(token != NULL){
		int idUsuario = atoi(strtok(token, ","));
		int idGrupo = atoi(strtok(NULL, ","));
		Usuario* usuario = obtenerUsuarioPorId(idUsuario, usuarios, tamanyo);
		Grupo* grupo = obtenerGrupoPorId(idGrupo, grupos, *numGrupos);
		grupo->addMiembro(usuario);
		token = strtok(NULL, ";");
		i++;
	}
}

int getUsuario() {
	sprintf(sendBuff, "GET;USUARIO;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	printf("Mensaje mandado: %s\n", sendBuff);
	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerUsuarios(usuarios, numUsuarios, recvBuff);
	printf("Usuario recibido: %s\n", usuarios[0]->getNombre());
	return 0;
	
}

int getGrupos() {
	sprintf(sendBuff, "GET;GRUPOS;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerGrupos(grupos, numGrupos, recvBuff);
	printf("Grupos recibidos: %s\n", grupos[0]->getNombre());
	return 0;
	
}

int getMensajes() {
	sprintf(sendBuff, "GET;MENSAJES;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerMensajes(mensajes, numMensajes, recvBuff);
	printf("Mensajes recibidos: %s\n", mensajes[0]->getContenido());
	return 0;	
}

int getConversaciones() {

	sprintf(sendBuff, "GET;CONVERSACIONES;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	leerConversacion(recvBuff, grupos, numGrupos, usuarios, *numUsuarios);
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
