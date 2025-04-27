#ifndef BITABIT_H
#define BITABIT_H
#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Constantes
//const int MAX_BITS = 7;

unsigned char* DoXOR(unsigned char* img1, unsigned char* img2, int width, int height);
unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n);
unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n);

#endif // BITABIT_H
