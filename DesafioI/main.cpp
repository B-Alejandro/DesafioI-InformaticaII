#include <fstream>
#include <iostream>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <ModulosEncriptacion.h>
#include <cstdint>
using namespace std;

// Funcion para verificar si un archivo existe
bool verificarArchivo(const QString& ruta) {
    QFileInfo check_file(ruta);
    if (check_file.exists() && check_file.isFile()) {
        cout << "Archivo encontrado: " << ruta.toStdString() << endl;
        return true;
    } else {
        cerr << "ERROR: No se encontro el archivo: " << ruta.toStdString() << endl;
        return false;
    }
}

int8_t verificarEnmascaramiento(QString rutaBase, int etapas) {
    int width = 0, height = 0, seed = 0, widthM = 0, heightM = 0, npixel = 0;
    string mask = rutaBase.toStdString();

    // Verificar que existan los archivos necesarios
    if (!verificarArchivo(rutaBase + "M.bmp") ||
        !verificarArchivo(rutaBase + "I_D.bmp") ||
        !verificarArchivo(rutaBase + "I_M.bmp")) {
        cerr << "Error: No se encontraron los archivos basicos necesarios." << endl;
        return -1;
    }

    // Cargar las imagenes
    unsigned char* M = loadPixels(rutaBase + "M.bmp", widthM, heightM);
    unsigned char* I_D = loadPixels(rutaBase + "I_D.bmp", width, height);
    unsigned char* I_M = loadPixels(rutaBase + "I_M.bmp", width, height);

    // Verificar que las imagenes se cargaron correctamente
    if (!M || !I_D || !I_M) {
        cerr << "Error: No se pudieron cargar una o mas imagenes." << endl;
        // Liberar memoria de las imagenes que si se cargaron
        if (M) delete[] M;
        if (I_D) delete[] I_D;
        if (I_M) delete[] I_M;
        return -1;
    }

    // Imagen que vamos a ir modificando (hacemos una copia para no modificar I_D directamente)
    int totalPixels = width * height * 3;
    unsigned char* imagen_resultado = new unsigned char[totalPixels];


    // Ya no necesitamos I_D
    delete[] I_D;

    for (int etapa =etapas-1; etapa >=0; --etapa) {
        cout << "Etapa " << etapa << ":" << endl;

        // Construir la ruta del archivo de mascara para esta etapa
        string rutaMascara = mask + "M" + std::to_string(etapa) + ".txt";
        QString qRutaMascara = QString::fromStdString(rutaMascara);

        // Verificar que existe el archivo de mascara para esta etapa
        if (!verificarArchivo(qRutaMascara)) {
            cerr << "Error: No se encontro el archivo de mascara para la etapa " << etapa << endl;
            continue; // Intentamos continuar con la siguiente etapa
        }

        // Cargar datos de la mascara
        unsigned int* MaskingData = loadSeedMasking(rutaMascara, seed, npixel);
        if (!MaskingData) {
            cerr << "Error: No se pudieron cargar los datos de la mascara para la etapa " << etapa << endl;
            continue; // Intentamos continuar con la siguiente etapa
        }

        int8_t resultado = 0;

        // Verificar si es XOR
        resultado = EsXor(imagen_resultado, I_M, MaskingData, seed);
        if (resultado != 0) {
            cout << "Enmascaramiento detectado: XOR" << endl;
            // Como DoXOR modifica directamente imagen_resultado, lo llamamos asi
            DoXOR(imagen_resultado, I_M, width, height);
            delete[] MaskingData; // Liberamos memoria de la mascara
            continue;
        }

        // Verificar si es Rotacion
        resultado = EsRotacion(imagen_resultado, MaskingData, seed);
        if (resultado != 0) {
            cout << "Enmascaramiento detectado: Rotacion (tipo " << (int)resultado << ")" << endl;

            int opcion = resultado / 10;
            cout<<"Resultado de la opcion"<<resultado;
            int bits = resultado % 10;

            if (opcion == 2) {
                // RotarIzquierda modifica directamente imagen_resultado
                RotarIzquierda(imagen_resultado, totalPixels, bits);
            } else if (opcion == 3) {
                // RotarDerecha modifica directamente imagen_resultado
                RotarDerecha(totalPixels, imagen_resultado, bits);
            } else {
                cout << "Opcion de rotacion no reconocida: " << opcion << endl;
            }

            delete[] MaskingData; // Liberamos memoria de la mascara
            continue;
        }

        // Verificar si es Enmascaramiento por resta
        resultado = EsEnmascaramiento(imagen_resultado, M, MaskingData, width, height, widthM, heightM, seed);
        if (resultado != 0) {
            cout << "Enmascaramiento detectado: Enmascaramiento por resta" << endl;
            // Asumiendo que Desenmascaramiento tambien modifica directamente imagen_resultado
            Desenmascaramiento(imagen_resultado, M, totalPixels, seed);
            delete[] MaskingData; // Liberamos memoria de la mascara
            continue;
        }

        // Si no se detecto nada en esta etapa
        cout << "No se detecto enmascaramiento en esta etapa." << endl;
        delete[] MaskingData; // Liberamos memoria de la mascara
    }

    // Verificar que exista el directorio para guardar el resultado
    QDir dir(rutaBase + "Pruebas");
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            cerr << "Error: No se pudo crear el directorio para resultados" << endl;
        }
    }

    // Guardar el resultado final
    bool success = exportImage(imagen_resultado, width, height, rutaBase + "Pruebas/seraOnoSera.bmp");
    if (!success) {
        cerr << "Error al guardar la imagen final" << endl;
    } else {
        cout << "Imagen final guardada correctamente" << endl;
    }

    cout << "Proceso de verificacion completado." << endl;

    // Liberar toda la memoria
    delete[] M;
    delete[] I_M;
    delete[] imagen_resultado;

    return 0;
}

int main() {
    // Cambiar el directorio de trabajo dos niveles arriba para que este con el codigo
    if (!QDir::setCurrent(QDir::cleanPath(QDir::currentPath() + "/../.."))) {
        cerr << "No se pudo cambiar el directorio de trabajo." << endl;
        return 1;
    }

    cout << "Directorio actual: " << QDir::currentPath().toStdString() << endl;

    // Declarar rutas importantes
    QString rutaBase = "Datos/";
    QString rutaBaseC = rutaBase + "Caso 2/";
    // Obtener el numero de etapas
    uint8_t numEtapas = obtenerNumeroEtapas(rutaBaseC);
    cout << "Numero de etapas detectadas: " << (int)numEtapas << endl;

    if (numEtapas == 0) {
        cerr << "Error: No se pudieron detectar etapas" << endl;
        return 1;
    }
    verificarEnmascaramiento(rutaBaseC,numEtapas);


        return 0;
}
