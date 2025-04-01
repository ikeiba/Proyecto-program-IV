#ifndef _CONFIG_H_
#define _CONFIG_H_
#define TAM 40
typedef struct 
{
    char nombreBD[TAM];
    char loadCSV[TAM];
    char cleanBBDD[TAM];
    char createBBDD[TAM];
    char deleteBBDD[TAM];
}Config;
void leerConfig();
#endif