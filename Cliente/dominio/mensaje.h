#ifndef MENSAJE_H_
#define MENSAJE_H_
#include "usuario.h"
#include "grupo.h"

class Mensaje 
{
    private:
        int id;
        char* fecha;
        char* hora;
        char* contenido;
        Usuario* emisor;
        Grupo* grupo;
    public:
        // Constructor con parámetros
        Mensaje(int id, const char* fecha, const char* hora, const char* contenido, Usuario* emisor, Grupo* grupo);
        // Constructor copia
        Mensaje(const Mensaje& mensaje);

        // Destructor
        ~Mensaje();

        // Getters
        int getId() const;
        const char* getFecha() const;
        const char* getHora() const;
        const char* getContenido() const;
        Usuario* getEmisor() const;
        Grupo* getGrupo() const;

        // Setters (no hacemos setters para id porque lo cogemos directamente de la base de datos y no deberiamos modificarlo)
        void setFecha(const char* fecha);
        void setHora(const char* hora);
        void setContenido(const char* contenido);
        void setEmisor(Usuario* emisor);
        void setGrupo(Grupo* grupo);
};

#endif