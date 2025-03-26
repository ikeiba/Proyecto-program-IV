#ifndef _CONFIG_H_
#define _CONFIG_H_
#define TAM 30
typedef struct 
{
    char nombreBD[TAM];
    char admin[TAM];
    char con[TAM];
}Config;
Config leerConfiguracion(char *nombrefichero);
#endif