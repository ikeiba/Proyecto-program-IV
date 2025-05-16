#ifndef _CONFIG_H_
#define _CONFIG_H_
#define TAM 70

typedef struct 
{
    char nombreBD[TAM];
    char loggerBD[TAM];
}Config;

void leerConfig();
#endif