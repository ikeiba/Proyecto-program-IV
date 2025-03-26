#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "sqlite3.h"
#include "baseDatos.h"

int main(){
    crearBD();
    printf("\n\n%d\n",comprobarCredenciales("iker", "ibarrola"));
    administracion();
    

    
    return 0;
} 