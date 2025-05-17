#ifndef _ESTRUCTURAS_
#define _ESTRUCTURAS_

typedef struct
{
    int id;
    char* nombre;
    char* email;
    char* telefono;
    char* fNacimiento;
    char* contra;
}Usuario;

typedef struct
{
    int id;
    char* nombre;
    char* email;
    char* telefono;
    char* fNacimiento;
    char* contra;
    int nivel;
}Administrador;

typedef struct
{
    int id;
    char* nombre;
    char* fCreacion;
    Usuario* creador;
    char* descripcion;
    Usuario** miembros;
    int size;
}Grupo;

typedef struct
{
    int id;
    char* fecha;
    char* hora;
    char* contenido;
    Usuario* emisor;
    Grupo* grupo;
}Mensaje;

#endif