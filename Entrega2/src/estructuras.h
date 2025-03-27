#ifndef _CSV_READER_
#define _CSV_READER_

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
    char* fCreacion;
    Usuario creador;
    char* descripcion;

}Grupo;

typedef struct
{
    int id;
    char* fecha;
    int hora[2];
    char* contenido;
    Usuario emisor;
    Grupo grupo;
    Usuario* miembros;
}Mensaje;

#endif