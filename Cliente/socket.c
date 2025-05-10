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
    printf("Sending message 1... \n");
    sprintf(sendBuff, "INI;%s;%s", email, contrasenya);
	send(s, sendBuff, sizeof(sendBuff), 0);

    printf("Receiving message 1... \n");
	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Data received: %s \n", recvBuff);
    closesocket(s);
	WSACleanup();
	sprintf(sendBuff, "Bye");
    if(strcmp(recvBuff, "ERROR")){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return -1;
    }
    if(strcmp(recvBuff, "CORRECT")){
		send(s, sendBuff, sizeof(sendBuff), 0);
        return 1;
    }
	send(s, sendBuff, sizeof(sendBuff), 0);

	return 0;
}