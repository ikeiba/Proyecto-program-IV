// IMPORTANT: Winsock Library ("ws2_32") should be linked
//gcc -I baseDatos -I utils -I sockets *.c baseDatos\*.c utils\*.c sockets\*.c -o servidor.exe -lws2_32

#include <stdio.h>
#include <winsock2.h>
#include "sockets/gestionMensaje.h"
#include "baseDatos/baseDatos.h"
#include "utils/config.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

Usuario** usuarios;
int* numUsuarios;

Grupo** grupos;
int* numGrupos;

int** idUsuarios;
int** idGrupos;
int* numConversaciones;

Mensaje** mensajes;
int* numMensajes;

int main(int argc, char *argv[]) {
    leerConfig();
    printf("HOLA");
    obtenerUsuarios(usuarios, numUsuarios);
    printf(usuarios[0]->nombre);
    obtenerGrupos(grupos, numGrupos, usuarios, *numUsuarios);
    printf("HOLA"); 
    obtenerConversaciones(idUsuarios, idGrupos, numConversaciones);
    printf("HOLA");
    obtenerMensajes(mensajes, numMensajes, grupos, *numGrupos, usuarios, *numUsuarios);
    printf(mensajes[0]->contenido);

    WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");
    do{
        //LISTEN to incoming connections (socket server moves to listening mode)
        if (listen(conn_socket, 1) == SOCKET_ERROR) {
            printf("Listen failed with error code: %d", WSAGetLastError());
            closesocket(conn_socket);
            WSACleanup();
            return -1;
        }

        //ACCEPT incoming connections (server keeps waiting for them)
        printf("Waiting for incoming connections...\n");
        int stsize = sizeof(struct sockaddr);
        comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
        // Using comm_socket is able to send/receive data to/from connected client
        if (comm_socket == INVALID_SOCKET) {
            printf("accept failed with error code : %d", WSAGetLastError());
            closesocket(conn_socket);
            WSACleanup();
            return -1;
        }
        printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
                ntohs(client.sin_port));

        // Closing the listening sockets (is not going to be used anymore)

        //SEND and RECEIVE data
        printf("Waiting for incoming messages from client... \n");
        do {
            int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            if (bytes > 0) {
                char* tipo = strtok(recvBuff, ";");
                if(strcmp(tipo, "INI") == 0){
                    gestionarMesajeINI(sendBuff, recvBuff, &comm_socket);
                }

                if(strcmp(tipo, "REG") == 0){
                    gestionarMensajeREG(sendBuff, recvBuff, &comm_socket);
                }

                if(strcmp(tipo, "GET") == 0){
                    gestionarMensajeGET(sendBuff, recvBuff, &comm_socket);
                }

                if (strcmp(tipo, "Bye") == 0){
                    break;
                }       
            }
        } while (1);

        // CLOSING the sockets and cleaning Winsock...
        closesocket(comm_socket);
    }while(1);
	
    closesocket(conn_socket);
    WSACleanup();

	return 0;
}