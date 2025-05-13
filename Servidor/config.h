#ifndef _CONFIG_H_
#define _CONFIG_H_
#define TAM 40

typedef struct 
{
    char nombreBD[TAM];
    char loggerBD[TAM];
}Config;

void leerConfig();
#endif