#include "mensaje.h"
#include <string.h>

Mensaje::Mensaje(int id, const char* fecha, const char* hora, const char* contenido, Usuario* emisor, Grupo* grupo)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->fecha = new char[strlen(fecha) + 1];
    strcpy(this->fecha, fecha);
    this->hora = new char[strlen(hora) + 1];
    strcpy(this->hora, hora);
    this->contenido = new char[strlen(contenido) + 1];
    strcpy(this->contenido, contenido);
    this->emisor = emisor;
    this->grupo = grupo;
}

// Constructor copia
Mensaje::Mensaje(const Mensaje& mensaje)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = mensaje.id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->fecha = new char[strlen(mensaje.fecha) + 1];
    strcpy(this->fecha, mensaje.fecha);
    this->hora = new char[strlen(mensaje.hora) + 1];
    strcpy(this->hora, mensaje.hora);
    this->contenido = new char[strlen(mensaje.contenido) + 1];
    strcpy(this->contenido, mensaje.contenido);
    this->emisor = mensaje.emisor;
    this->grupo = mensaje.grupo;
}

// Destructor
Mensaje::~Mensaje()
{
    // Liberar la memoria de los atributos que son punteros
    delete[] fecha;
    delete[] hora;
    delete[] contenido;
    // No liberamos emisor y grupo porque son punteros a objetos que no hemos creado nosotros
}

// Getters
int Mensaje::getId() const
{
    return id;
}

const char* Mensaje::getFecha() const
{
    return fecha;
}

const char* Mensaje::getHora() const
{
    return hora;
} 

const char* Mensaje::getContenido() const
{
    return contenido;
}

Usuario* Mensaje::getEmisor() const
{
    return emisor;
}

Grupo* Mensaje::getGrupo() const
{
    return grupo;
}

// Setters (no hacemos setters para id porque lo cogemos directamente de la base de datos y no deberiamos modificarlo)
void Mensaje::setFecha(const char* fecha)
{
    // Liberar la memoria del atributo anterior
    delete[] this->fecha;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->fecha = new char[strlen(fecha) + 1];
    strcpy(this->fecha, fecha);
}

void Mensaje::setHora(const char* hora)
{
    // Liberar la memoria del atributo anterior
    delete[] this->hora;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->hora = new char[strlen(hora) + 1];
    strcpy(this->hora, hora);
}

void Mensaje::setContenido(const char* contenido)
{
    // Liberar la memoria del atributo anterior
    delete[] this->contenido;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->contenido = new char[strlen(contenido) + 1];
    strcpy(this->contenido, contenido);
}

void Mensaje::setEmisor(Usuario* emisor)
{
    // Asignar el nuevo valor (no necesitamos liberar la memoria porque no hemos creado el objeto Usuario)
    this->emisor = emisor;
}

void Mensaje::setGrupo(Grupo* grupo)
{
    // Asignar el nuevo valor (no necesitamos liberar la memoria porque no hemos creado el objeto Grupo)
    this->grupo = grupo;
}
