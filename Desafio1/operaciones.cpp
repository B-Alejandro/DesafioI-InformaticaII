#include "operaciones.h"

#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

unsigned char* DoXOR(unsigned char* img1, unsigned char* img2, int width, int height) {
    int totalPixels = width * height * 3;
    unsigned char* result = new unsigned char[totalPixels];

    for (int i = 0; i < totalPixels; i++) {
        result[i] = img1[i] ^ img2[i];
    }

    return result;
}

unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n) {
    unsigned char* result = new unsigned char[num_pixels * 3];

    n = n % 8; // Asegurar que n está entre 0-7

    for (int i = 0; i < num_pixels * 3; i++) {
        result[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return result;
}

unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n) {
    unsigned char* result = new unsigned char[num_pixels * 3];

    n = n % 8; // Asegurar que n está entre 0-7

    for (int i = 0; i < num_pixels * 3; i++) {
        result[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return result;
}
