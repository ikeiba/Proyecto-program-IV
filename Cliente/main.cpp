#include "sockets/socket.h"
//g++ -I sockets -I dominio *.cpp dominio/*.cpp sockets/*.cpp -o cliente.exe -lws2_32
extern Usuario** usuarios;
extern int numUsuarios;

extern Grupo** grupos;
extern int numGrupos;

extern Mensaje** mensajes;
extern int numMensajes;

int main(int argc, char *argv[]) {
	//inicioSesion("amayamanuela@gmail.com", "kp9fgaar");
	//registrarse("amayamanuela","amayamanuela@gmail.com","332181960","1998-08-11","kp9fgaar");
	getGeneral("leopoldo23@gmail.com");
	return 0;
}