/*
 * Programa demostrativo de manipulaciónprocesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 * Asistencia de ChatGPT para mejorar la forma y presentación del código fuente
 */

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "FuncionesAbrirImagen.h"  // Asegúrate de que esté correctamente nombrado y en el path

using namespace std;

// XOR entre dos imágenes
unsigned char* DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height) {
    int numPixels = width * height;
    unsigned char* pixeles = new unsigned char[numPixels * 3];

    for (int i = 0; i < numPixels * 3; ++i) {
        pixeles[i] = imagen[i] ^ mascara[i];
    }

    return pixeles;
}

// Rotar bits a la derecha
unsigned char* RotarDerecha(int num_pixels, unsigned char* img, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return resultado;
}

// Rotar bits a la izquierda
unsigned char* RotarIzquierda(int num_pixels, unsigned char* img, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return resultado;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Ruta de imágenes
    QString archivoMascara = "C:/Users/alejb/Desktop/DesafioIPublicar/DesafioI/Caso 1/I_M";
    QString archivoImagen  = "C:/Users/alejb/Desktop/DesafioIPublicar/DesafioI/Caso 1/I_O";

    int width = 0, height = 0;

    // Cargar imágenes
    unsigned char* Mascara = loadPixels(archivoMascara, width, height);
    if (!Mascara) {
        cerr << "Error al cargar la máscara." << endl;
        return -1;
    }

    unsigned char* Imagen = loadPixels(archivoImagen, width, height);
    if (!Imagen) {
        cerr << "Error al cargar la imagen." << endl;
        delete[] Mascara;
        return -1;
    }

    // XOR 1
    unsigned char* paso1 = DoXOR(Imagen, Mascara, width, height);

    // Rotar bits
    int num_pixels = width * height;
    unsigned char* paso2 = RotarDerecha(num_pixels, paso1, 3);

    // XOR 2
    unsigned char* resultadoFinal = DoXOR(paso2, Mascara, width, height);
    // Liberar memoria
    delete[] resultadoFinal;
    delete[] paso2;
    delete[] paso1;
    delete[] Imagen;
    delete[] Mascara;
    int seed = 0;
    int n_pixels = 0;

    // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
    unsigned int *maskingData = loadSeedMasking("C:/Users/alejb/Desktop/DesafioIPublicar/DesafioI/Caso 1/M1.txt", seed, n_pixels);

    // Muestra en consola los primeros valores RGB leídos desde el archivo de enmascaramiento
    for (int i = 0; i < n_pixels * 3; i += 3) {
        cout << "Pixel " << i / 3 << ": ("
             << maskingData[i] << ", "
             << maskingData[i + 1] << ", "
             << maskingData[i + 2] << ")" << endl;
    }

    return 0;
}


/*
int example()
{
    // Definición de rutas de archivo de entrada (imagen original) y salida (imagen modificada)
    QString archivoEntrada = "/home/qjiope/Desktop/DesafíoI/Caso 2/I_M.bmp";
    QString archivoSalida = "/home/qjiope/Desktop/DesafíoI/Caso 2/PrUEBA.bmp";

    // Variables para almacenar las dimensiones de la imagen
    int height = 0;
    int width = 0;

    // Carga la imagen BMP en memoria dinámica y obtiene ancho y alto
    unsigned char *pixelData = loadPixels(archivoEntrada, width, height);

    // Simula una modificación de la imagen asignando valores RGB incrementales
    // (Esto es solo un ejemplo de manipulación artificial)
    for (int i = 0; i < width * height * 3; i += 3) {
        pixelData[i] = i;     // Canal rojo
        pixelData[i + 1] = i; // Canal verde
        pixelData[i + 2] = i; // Canal azul
    }

    // Exporta la imagen modificada a un nuevo archivo BMP
    bool exportI = exportImage(pixelData, width, height, archivoSalida);

    // Muestra si la exportación fue exitosa (true o false)
    cout << exportI << endl;

    // Libera la memoria usada para los píxeles
    delete[] pixelData;
    pixelData = nullptr;

    // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
    int seed = 0;
    int n_pixels = 0;

    // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
    unsigned int *maskingData = loadSeedMasking("/home/qjiope/Desktop/DesafíoI/Caso 2/M1.txt", seed, n_pixels);

    // Muestra en consola los primeros valores RGB leídos desde el archivo de enmascaramiento
    for (int i = 0; i < n_pixels * 3; i += 3) {
        cout << "Pixel " << i / 3 << ": ("
             << maskingData[i] << ", "
             << maskingData[i + 1] << ", "
             << maskingData[i + 2] << ")" << endl;
    }

    // Libera la memoria usada para los datos de enmascaramiento
    if (maskingData != nullptr){
        delete[] maskingData;
        maskingData = nullptr;
    }

    return 0; // Fin del programa
}
*/
