#include <fstream>
#include <iostream>

#include <QString>
#include <QDir> // Necesario para cambiar el directorio
#include <ModulosEncriptacion.h>
#include <cstdint> // Para los tipos exactos como int8_t

using namespace std;


/**
 * @brief Verifica si una sección de la imagen hace parte de un proceso de enmascaramiento.
 *
 * Esta función ejecuta las verificaciones de enmascaramiento usando las funciones EsXor, EsRotacion y EsEnmascaramiento.
 * Si alguna de ellas retorna un valor diferente de 0, se guarda dicho resultado.
 *
 * @param imagen Puntero al arreglo de bytes de la imagen original.
 * @param mascara Puntero al arreglo de bytes de la máscara (para funciones que lo requieran).
 * @param maskingData Puntero al arreglo de bytes con los datos esperados tras las operaciones.
 * @param seed Índice desde donde se empieza a comparar en la imagen.
 * @return int8_t Retorna el valor del primer tipo de enmascaramiento detectado (diferente de 0), 0 si no se detecta enmascaramiento.
 */
int8_t verificarEnmascaramiento(unsigned char* imagen, unsigned char* mascara, unsigned char* maskingData, int8_t seed) {
    // Variable para almacenar el resultado de cada función
    int8_t resultado = 0;

    // Verificar si es XOR
    resultado = EsXor(imagen, mascara, maskingData, seed);
    if (resultado != 0) {
        std::cout << "Enmascaramiento detectado: XOR" << std::endl;
        return resultado;
    }

    // Verificar si es Rotación
    resultado = EsRotacion(imagen, maskingData, seed);
    if (resultado != 0) {
        std::cout << "Enmascaramiento detectado: Rotación" << std::endl;
        return resultado;
    }

    // Verificar si es Enmascaramiento por resta
    resultado = EsEnmascaramiento(imagen, mascara, maskingData, seed);
    if (resultado != 0) {
        std::cout << "Enmascaramiento detectado: Enmascaramiento por resta" << std::endl;
        return resultado;
    }

    // Si ninguna función detecta enmascaramiento
    std::cout << "No se detectó enmascaramiento." << std::endl;
    return 0;
}

/**
 * @brief Función principal del programa.
 *
 * Cambia el directorio de trabajo dos niveles arriba para que coincida con la estructura del proyecto,
 * y define rutas base para el procesamiento de datos.
 *
 * @return int8_t Código de estado de la ejecución (0 si todo fue correcto).
 */
int main() {
    // Cambiar el directorio de trabajo dos niveles arriba para que este con el código
    if (!QDir::setCurrent(QDir::cleanPath(QDir::currentPath() + "/../.."))) {
        cerr << "No se pudo cambiar el directorio de trabajo." << endl;
        return 1;
    }

    // Declarar rutas importantes (una sola vez)
    QString rutaBase = "Datos/";
    QString rutaBaseC = rutaBase + "Caso 1/";
    rutaBase
    // Obtener el número de etapas (aunque no lo usemos por ahora)
    uint8_t numEtapas = obtenerNumeroEtapas(rutaBaseC);
    cout << "Procesando " << static_cast<int>(numEtapas) << " etapas" << endl;

    // Reconstruir el caso 1
    reconstruirCaso1(rutaBaseC);

    return 0;
}
