#ifndef MODULOSENCRIPTACION_H
#define MODULOSENCRIPTACION_H

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>

using namespace std;

// Carga los pixeles de una imagen desde la ruta 'input'
// Devuelve un puntero a unsigned char con los datos de píxeles
// width y height se pasan por referencia para devolver las dimensiones de la imagen
unsigned char* loadPixels(QString input, int &width, int &height);

// Exporta una imagen con los datos de píxeles 'pixelData' y dimensiones 'width' x 'height'
// Guarda la imagen en el archivo 'archivoSalida'
// Devuelve true si la exportación fue exitosa, false en caso contrario
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

// Carga una máscara de semillas desde un archivo
// Devuelve un puntero a unsigned int con la máscara
// seed y n_pixels se pasan por referencia para devolver información adicional
unsigned int* loadSeedMasking(string nombreArchivo, int &seed, int &n_pixels);

// Realiza una operación XOR entre dos imágenes.
void DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height);

// Rota los bits de cada byte hacia la derecha.
void RotarDerecha(int num_pixels, unsigned char* img, int n);

// Rota los bits de cada byte hacia la izquierda.
void RotarIzquierda(unsigned char* img, int num_pixels, int n);

// Realiza un enmascaramiento sumando valores de la máscara al arreglo original con un desplazamiento inicial (semilla).

void Desenmascaramiento(unsigned char* originales, unsigned char* mascara, int cantidad, int seed);

int8_t EsXor(unsigned char* imagen, unsigned char* mascara, unsigned int* maskingData, int8_t seed);
int8_t EsRotacion(unsigned char* imagen, unsigned int* maskingData, int8_t seed);
int EsEnmascaramiento(
    unsigned char* imagen,    // puntero a datos de la imagen (RGB)
    unsigned char* mascara,   // puntero a datos de la máscara (RGB) - no se usa en esta función, pero lo dejamos por si se necesita
    unsigned int* maskingData,// puntero a datos esperados (pueden ser >255)
    int ancho_img,
    int alto_img,
    int ancho_mascara,
    int alto_mascara,
    int seed                 // posición inicial en la imagen (en bytes)
    );
int8_t obtenerNumeroEtapas(QString rutaBase);
#endif // MODULOSENCRIPTACION_H
