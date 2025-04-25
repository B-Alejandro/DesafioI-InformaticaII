#ifndef MANIPULACIONIMAGENES_H
#define MANIPULACIONIMAGENES_H

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
unsigned char* DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height);

// Rota los bits de cada byte hacia la derecha.
unsigned char* RotarDerecha(int num_pixels, unsigned char* img, int n);

// Rota los bits de cada byte hacia la izquierda.
unsigned char* RotarIzquierda(int num_pixels, unsigned char* img, int n);

// Realiza un enmascaramiento sumando valores de la máscara al arreglo original con un desplazamiento inicial (semilla).
void Enmascaramiento(unsigned char* originales, unsigned char* mascara, int cantidad, int seed);

#endif // MANIPULACIONIMAGENES_H
