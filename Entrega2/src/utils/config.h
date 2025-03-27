#ifndef _CONFIG_H_
#define _CONFIG_H_
#define TAM 30
typedef struct 
{
    char nombreBD[TAM];
    char administrador[TAM];
    char contrasena[TAM];
}Config;
Config leerConfig(char *nombrefichero);
#endif