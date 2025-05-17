#ifndef USUARIO_H_
#define USUARIO_H_


class Usuario 
{
    private:
        int id;
        char* nombre;
        char* email;
        char* telefono;
        char* fNacimiento;
        char* contra;
    public:
        // Constructor con parámetros
        Usuario(int id, const char* nombre, const char* email, const char* telefono, const char* fNacimiento, const char* contra);
        
        // Constructor copia
        Usuario(const Usuario& usuario);

        // Destructor
        ~Usuario();

        // Getters
        int getId() const;
        const char* getNombre() const; 
        const char* getEmail() const;
        const char* getTelefono() const;
        const char* getFNacimiento() const;
        const char* getContra() const;

        // Setters (no hacemos setters para id porque lo cogemos directamente de la base de datos y no deberiamos modificarlo)
        void setNombre(const char* nombre);
        void setEmail(const char* email);
        void setTelefono(const char* telefono);
        void setFNacimiento(const char* fNacimiento);
        void setContra(const char* contra);

};

#endif