#ifndef _LOGGER_H_
#define _LOGGER_H_

#define FICHERO_LOG "src\\utils\\BDlogger.log"

void registrarMensaje(const char *formato, ...);
void mostrarMensaje(char *texto);

#endif