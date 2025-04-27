#ifndef OPERACIONES_H
#define OPERACIONES_H

#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

unsigned char* DoXOR(unsigned char* img1, unsigned char* img2, int width, int height);
unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n);
unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n);

#endif // OPERACIONES_H
