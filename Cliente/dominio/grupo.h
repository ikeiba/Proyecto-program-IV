#ifndef GRUPO_H_
#define GRUPO_H_
#include "usuario.h"

class Grupo 
{
    private:
        int id;
        char* nombre;
        char* fCreacion;
        Usuario* creador;
        char* descripcion;
        Usuario** miembros;
        int size;
    public:
        // Constructor con parámetros
        Grupo(int id, const char* nombre, const char* fCreacion, Usuario* creador, const char* descripcion, Usuario** miembros, int size);
        
        // Constructor copia
        Grupo(const Grupo& grupo);

        // Destructor
        ~Grupo();

        // Getters
        int getId() const;
        const char* getNombre() const;
        const char* getFCreacion() const;
        Usuario* getCreador() const;
        const char* getDescripcion() const;
        Usuario** getMiembros() const;
        int getSize() const;

        // Setters (no hacemos setters para id porque lo cogemos directamente de la base de datos y no deberiamos modificarlo)
        void setNombre(const char* nombre);
        void setFCreacion(const char* fCreacion);
        void setCreador(Usuario* creador);
        void setDescripcion(const char* descripcion);
        void setMiembros(Usuario** miembros);
        void setSize(int size);
        void addMiembro(Usuario* nuevoMiembro);
        void removeMiembro(Usuario* miembroAEliminar);
};

#endif