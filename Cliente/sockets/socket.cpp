#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "socket.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

SOCKET s;
char sendBuff[512], recvBuff[512];

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

int getUsuario() {
	inicializarSocket();
	sprintf(sendBuff, "GET;USUARIO;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Usuario recibido: %s\n", recvBuff);
	return 0;
	
}

int getGrupos() {
	inicializarSocket();
	sprintf(sendBuff, "GET;GRUPOS;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Grupos recibidos: %s\n", recvBuff);
	return 0;
	
}

int getConversaciones() {

	sprintf(sendBuff, "GET;CONVERSACIONES;");
	send(s, sendBuff, sizeof(sendBuff), 0);

	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Conversaciones recibidas: %s\n", recvBuff);
	return 0;
}

int getGeneral() {
	inicializarSocket();
	
	getUsuario();
	getGrupos();
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
