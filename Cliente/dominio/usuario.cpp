#include "usuario.h"
#include <string.h>
#include <iostream>

using namespace std;

// Constructor con parámetros
Usuario::Usuario(int id, const char* nombre, const char* email, const char* telefono, const char* fNacimiento, const char* contra)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
    this->email = new char[strlen(email) + 1];
    strcpy(this->email, email);
    this->telefono = new char[strlen(telefono) + 1];
    strcpy(this->telefono, telefono);
    this->fNacimiento = new char[strlen(fNacimiento) + 1];
    strcpy(this->fNacimiento, fNacimiento);
    this->contra = new char[strlen(contra) + 1];
    strcpy(this->contra, contra);
}

// Constructor copia
Usuario::Usuario(const Usuario& usuario)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = usuario.id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->nombre = new char[strlen(usuario.nombre) + 1];
    strcpy(this->nombre, usuario.nombre);
    this->email = new char[strlen(usuario.email) + 1];
    strcpy(this->email, usuario.email);
    this->telefono = new char[strlen(usuario.telefono) + 1];
    strcpy(this->telefono, usuario.telefono);
    this->fNacimiento = new char[strlen(usuario.fNacimiento) + 1];
    strcpy(this->fNacimiento, usuario.fNacimiento);
    this->contra = new char[strlen(usuario.contra) + 1];
    strcpy(this->contra, usuario.contra);
}

// Destructor
Usuario::~Usuario()
{
    // Liberar la memoria de los atributos que son punteros
    delete[] nombre;
    delete[] email;
    delete[] telefono;
    delete[] fNacimiento;
    delete[] contra;
}

// Getters
int Usuario::getId() const
{
    return id;
}

const char* Usuario::getNombre() const
{
    return nombre;
}

const char* Usuario::getEmail() const
{
    return email;
}

const char* Usuario::getTelefono() const
{
    return telefono;
}

const char* Usuario::getFNacimiento() const
{
    return fNacimiento;
}   

const char* Usuario::getContra() const
{
    return contra;
}   

// Setters
void Usuario::setNombre(const char* nombre)
{
    // Liberar la memoria del atributo anterior
    delete[] this->nombre;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
}

void Usuario::setEmail(const char* email)
{
    // Liberar la memoria del atributo anterior
    delete[] this->email;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->email = new char[strlen(email) + 1];
    strcpy(this->email, email);
}

void Usuario::setTelefono(const char* telefono)
{
    // Liberar la memoria del atributo anterior
    delete[] this->telefono;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->telefono = new char[strlen(telefono) + 1];
    strcpy(this->telefono, telefono);
}


void Usuario::setFNacimiento(const char* fNacimiento)
{
    // Liberar la memoria del atributo anterior
    delete[] this->fNacimiento;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->fNacimiento = new char[strlen(fNacimiento) + 1];
    strcpy(this->fNacimiento, fNacimiento);
}

void Usuario::setContra(const char* contra)
{
    // Liberar la memoria del atributo anterior
    delete[] this->contra;

    // Asignar el nuevo valor (haciendo una copia profunda)
    this->contra = new char[strlen(contra) + 1];
    strcpy(this->contra, contra);
}
