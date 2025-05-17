#include "grupo.h"
#include <string.h>

// Constructor con parámetros
Grupo::Grupo(int id, const char* nombre, const char* fCreacion, Usuario* creador, const char* descripcion, Usuario** miembros, int size)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
    this->fCreacion = new char[strlen(fCreacion) + 1];
    strcpy(this->fCreacion, fCreacion);
    this->descripcion = new char[strlen(descripcion) + 1];
    strcpy(this->descripcion, descripcion);
    
    // Asignar el creador (como es un puntero a un objeto Usuario, no necesitamos hacer una copia profunda, ya que lo que queremos es la dirección de memoria del objeto)
    this->creador = creador;

    // Asignar miembros
    this->miembros = new Usuario*[size];
    for (int i = 0; i < size; i++) {
        this->miembros[i] = miembros[i]; // No hacemos copia profunda porque es un puntero a un objeto Usuario
    }
    
    this->size = size;
}

// Constructor copia
Grupo::Grupo(const Grupo& grupo)
{
    // Asignar el id (como es int no necesitamos hacer nada especial, ya que la copia se hace byte a byte)
    this->id = grupo.id;

    // Asignar los demás atributos (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->nombre = new char[strlen(grupo.nombre) + 1];
    strcpy(this->nombre, grupo.nombre);
    this->fCreacion = new char[strlen(grupo.fCreacion) + 1];
    strcpy(this->fCreacion, grupo.fCreacion);
    this->descripcion = new char[strlen(grupo.descripcion) + 1];
    strcpy(this->descripcion, grupo.descripcion);
    
    // Asignar el creador (como es un puntero a un objeto Usuario, no necesitamos hacer una copia profunda, ya que lo que queremos es la dirección de memoria del objeto)
    this->creador = grupo.creador;

    // Asignar miembros
    this->miembros = new Usuario*[grupo.size];
    for (int i = 0; i < grupo.size; i++) {
        this->miembros[i] = grupo.miembros[i]; // No hacemos copia profunda porque es un puntero a un objeto Usuario
    }
    
    this->size = grupo.size;
}

// Destructor
Grupo::~Grupo()
{
    // Liberar la memoria de los atributos que son punteros
    delete[] nombre;
    delete[] fCreacion;
    delete[] descripcion;

    // Liberar la memoria de miembros (no eliminamos los objetos Usuario, ya que el hecho de que el puntero a Usuario sea un 
    // puntero a un objeto Usuario no significa que tengamos que liberar la memoria de ese objeto, ya que puede estar compartido 
    // por otros objetos)
    delete[] miembros;
}

// Getters
int Grupo::getId() const
{
    return id;
}

const char* Grupo::getNombre() const
{
    return nombre;
}

const char* Grupo::getFCreacion() const
{
    return fCreacion;
}

Usuario* Grupo::getCreador() const
{
    return creador;
}

const char* Grupo::getDescripcion() const
{
    return descripcion;
}

Usuario** Grupo::getMiembros() const
{
    return miembros;
}

int Grupo::getSize() const
{
    return size;
}

// Setters (no hacemos setters para id porque lo cogemos directamente de la base de datos y no deberiamos modificarlo)
void Grupo::setNombre(const char* nombre)
{
    // Liberar la memoria del atributo anterior
    delete[] this->nombre;

    // Asignar el nuevo valor (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->nombre = new char[strlen(nombre) + 1];
    strcpy(this->nombre, nombre);
}

void Grupo::setFCreacion(const char* fCreacion)
{
    // Liberar la memoria del atributo anterior
    delete[] this->fCreacion;

    // Asignar el nuevo valor (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->fCreacion = new char[strlen(fCreacion) + 1];
    strcpy(this->fCreacion, fCreacion);
}

void Grupo::setCreador(Usuario* creador)
{
    this->creador = creador;
}

void Grupo::setDescripcion(const char* descripcion)
{
    // Liberar la memoria del atributo anterior
    delete[] this->descripcion;

    // Asignar el nuevo valor (como son char* necesitamos hacer una copia "profunda", para no asignar la dirección de memoria del char* que recibimos)
    this->descripcion = new char[strlen(descripcion) + 1];
    strcpy(this->descripcion, descripcion);
}

void Grupo::setMiembros(Usuario** miembros)
{
    // Liberar la memoria del atributo anterior
    delete[] this->miembros;

    // Asignar el nuevo valor 
    this->miembros = new Usuario*[size];
    for (int i = 0; i < size; i++) {
        this->miembros[i] = miembros[i]; // No hacemos copia profunda porque es un puntero a un objeto Usuario
    }
}

void Grupo::setSize(int size)
{
    this->size = size;
}

void Grupo::addMiembro(Usuario* nuevoMiembro)
{
    // Aumentar el tamaño del array de miembros
    Usuario** nuevosMiembros = new Usuario*[size + 1];
    for (int i = 0; i < size; i++) {
        nuevosMiembros[i] = miembros[i]; // No hacemos copia profunda porque es un puntero a un objeto Usuario
    }
    nuevosMiembros[size] = nuevoMiembro; // Añadir el nuevo miembro al final del array

    // Liberar la memoria del array anterior
    delete[] miembros;

    // Asignar el nuevo array de miembros
    miembros = nuevosMiembros;
    size++;
}

void Grupo::removeMiembro(Usuario* miembroAEliminar)
{
    // Buscar el miembro a eliminar
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (miembros[i] == miembroAEliminar) {
            index = i;
            break;
        }
    }

    // Si no se encontró el miembro, no hacemos nada
    if (index == -1) {
        return;
    }

    // Crear un nuevo array de miembros sin el miembro a eliminar
    Usuario** nuevosMiembros = new Usuario*[size - 1];
    for (int i = 0, j = 0; i < size; i++) {
        if (i != index) {
            nuevosMiembros[j++] = miembros[i]; // No hacemos copia profunda porque es un puntero a un objeto Usuario
        }
    }

    // Liberar la memoria del array anterior
    delete[] miembros;

    // Asignar el nuevo array de miembros
    miembros = nuevosMiembros;
    size--;
}