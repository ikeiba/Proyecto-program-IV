#ifndef MENU_H_
#define MENU_H_

// Menu
void administracion(void);

void inicioSesion(void);

void menu(void);


// Server
void reinicioServer(void);


// Usuarios
void eleccionUsuario(void);

void adminUsuarios(char *email);

void modificarUsuarios(char *email);

void modificarNombre(char *email);

void modificarTlf(char *email);

void menuBorrarUsuario(char *email);

void bloquearUsuario(char *email);


// Logs
void logs(void);

void menuBorrarLog(void);


// BD
void copiaBDMenu(void);

// Registro Admins
void registrarAdmin(void);





#endif