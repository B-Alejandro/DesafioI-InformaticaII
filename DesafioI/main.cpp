#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <QDir> // Necesario para cambiar el directorio
#include <ModulosEncriptacion.h>

using namespace std;

/**
 * @brief Verifica si una sección de la imagen es resultado de una operación XOR con una máscara.
 *
 * Esta función compara los primeros 60 bytes (a partir de un índice seed) de la imagen con la máscara,
 * aplicando la operación XOR y comparando con los datos de enmascaramiento proporcionados.
 *
 * @param imagen Puntero al arreglo de bytes de la imagen original.
 * @param mascara Puntero al arreglo de bytes de la máscara.
 * @param maskingData Puntero al arreglo de bytes con los datos esperados tras la operación XOR.
 * @param seed Índice desde donde se empieza a comparar en la imagen.
 * @return int Retorna 1 si se detecta que la imagen es resultado de XOR con la máscara, 0 en caso contrario.
 */
int EsXor(unsigned char* imagen, unsigned char* mascara, unsigned char* maskingData, int seed) {
    for (int i = 0; i < 60; ++i) {
        if ((imagen[seed + i] ^ mascara[i]) != maskingData[i]) {
            return 0; // No es XOR
        }
    }
    return 1; // Es XOR
}

/**
 * @brief Verifica si una sección de la imagen es resultado de una rotación de bits con respecto a una máscara.
 *
 * Esta función intenta detectar si la imagen ha sido rotada a la derecha o izquierda en bits,
 * comparando con la máscara. Prueba rotaciones de 1 a 8 bits.
 *
 * @param imagen Puntero al arreglo de bytes de la imagen original.
 * @param mascara Puntero al arreglo de bytes de la máscara.
 * @param maskingData Puntero al arreglo de bytes con los datos esperados tras la rotación.
 * @param seed Índice desde donde se empieza a comparar en la imagen.
 * @return int Retorna un valor > 0 indicando el tipo y cantidad de rotación detectada, o 0 si no se detecta rotación.
 */
int EsRotacion(unsigned char* imagen, unsigned char* mascara, unsigned char* maskingData, int seed) {
    unsigned char resultado[20];
    for (int p = 1; p <= 8; ++p) {
        // Rotación a la derecha
        bool esRotacionDerecha = true;
        for (int n = 0; n < 20; ++n) {
            resultado[n] = (imagen[seed + n] >> p) | (imagen[seed + n] << (8 - p));
            if (resultado[n] != maskingData[n]) {
                esRotacionDerecha = false;
                break;
            }
        }
        if (esRotacionDerecha) {
            return p + 20; // Código para rotación derecha
        }

        // Rotación a la izquierda
        bool esRotacionIzquierda = true;
        for (int n = 0; n < 20; ++n) {
            resultado[n] = (imagen[seed + n] << p) | (imagen[seed + n] >> (8 - p));
            if (resultado[n] != maskingData[n]) {
                esRotacionIzquierda = false;
                break;
            }
        }
        if (esRotacionIzquierda) {
            return p + 30; // Código para rotación izquierda
        }
    }
    return 0; // No se detectó rotación válida
}

/**
 * @brief Verifica si una sección de la imagen es resultado de un enmascaramiento por resta con una máscara.
 *
 * Esta función compara los primeros 60 bytes (a partir de un índice seed) de la imagen con la máscara,
 * verificando que la resta entre imagen y máscara coincida con los datos de enmascaramiento.
 *
 * @param imagen Puntero al arreglo de bytes de la imagen original.
 * @param mascara Puntero al arreglo de bytes de la máscara.
 * @param maskingData Puntero al arreglo de bytes con los datos esperados tras la operación de enmascaramiento.
 * @param seed Índice desde donde se empieza a comparar en la imagen.
 * @return int Retorna 4 si se detecta enmascaramiento, 0 en caso contrario.
 */
int EsEnmascaramiento(unsigned char* imagen, unsigned char* mascara, unsigned char* maskingData, int seed) {
    for (int i = 0; i < 60; ++i) {
        if ((imagen[seed + i] - mascara[i]) != maskingData[i]) {
            return 0; // No es enmascaramiento
        }
    }
    return 4; // Es enmascaramiento
}

/**
 * @brief Función principal del programa.
 *
 * Cambia el directorio de trabajo dos niveles arriba para que coincida con la estructura del proyecto,
 * y define rutas base para el procesamiento de datos.
 *
 * @return int Código de estado de la ejecución (0 si todo fue correcto).
 */
int main() {
    // Cambiar el directorio de trabajo dos niveles arriba para que esté con el código
    if (!QDir::setCurrent(QDir::cleanPath(QDir::currentPath() + "/../.."))) {
        cerr << "No se pudo cambiar el directorio de trabajo." << endl;
        return 1;
    }

    // Declarar rutas importantes
    QString rutaBase = "Datos/Caso 1/";
    int numEtapas = 3;

    // Aquí puedes agregar la lógica para procesar las imágenes, máscaras, etc.

    return 0;
}
