#ifndef EXPORTARIMG_H
#define EXPORTARIMG_H

#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Constantes
const int MAX_BITS = 7;

unsigned char* loadPixels(const QString& input, int& width, int& height);
bool exportImage(unsigned char* pixelData, int width, int height, const QString& archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int& seed, int& n_pixels);

#endif // EXPORTARIMG_H
