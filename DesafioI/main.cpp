


#include <iostream>
#include <QDir>

#include "ManipulacionImagenes.h"



using namespace std;

#include <iostream>
#include <QDir>

#include "ManipulacionImagenes.h"

using namespace std;

int main()
{
    // Cambiar el directorio de trabajo dos niveles arriba para que esté con el código
    if (!QDir::setCurrent(QDir::cleanPath(QDir::currentPath() + "/../.."))) {
        cerr << "No se pudo cambiar el directorio de trabajo." << endl;
        return 1;
    }
    cout << "Directorio de trabajo actual: " << QDir::currentPath().toStdString() << endl;

    // Declarar rutas importantes
    QString rutaBase = "Datos/Caso 1/";
    //Flujo General
    return 0;
}
