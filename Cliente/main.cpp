#include "mainwindow.h"
#include <QApplication>
#include "sockets/socket.h"
//g++ -I sockets -I dominio main.cpp dominio/*.cpp sockets/*.cpp -o cliente.exe -lws2_32
//Sin interfaz: g++ -I sockets -I dominio main.cpp dominio/*.cpp sockets/*.cpp -o cliente.exe -lws2_32
extern Usuario** usuarios;
extern int numUsuarios;

extern Grupo** grupos;
extern int numGrupos;

extern Mensaje** mensajes;
extern int numMensajes;


int main(int argc, char *argv[]) {
	//printf("\nIasdawdawdawdawdawdawdawd..\n");

	//inicioSesion("iban76@gmail.com", "313m8f2p");
	//registrarse("Asier","asier@gmail.com","332181960","1998-08-11","kp9fgaar");

	//inicioSesion("asier@gmail.com","kp9fgaar");

	//getGeneral("asier@gmail.com");

	//actualizarDatos();

	//enviarMensaje("2025-05-22", "20:36", "primer mensaje enviado desde el cliente", 43, 1);
	//aniadirUsuarioAGrupo(2, 67);

	//actualizarDatos();

	//abandonarGrupo(2, 67);
	//actualizarDatos();
	//crearGrupo("GRUPO POR SOCKET", "2025-05-22", 500, "PRIMER GRUPO CREADO POR CLIENTE");
	//registrarse("admin1","admin","332181961","1999-08-11","admin");
	//printf("\nawdawdawdawdWwdaiwdawdaaawdawdaswdawdawdock...\n");
	
	QApplication a(argc, argv);
    MainWindow w;
    w.show();
	return a.exec();
	



	return 0;
}
