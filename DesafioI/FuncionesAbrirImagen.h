#ifndef FUNCIONESABRIRIMAGEN_H
#define FUNCIONESABRIRIMAGEN_H
#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>

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
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif // FUNCIONESABRIRIMAGEN_H
