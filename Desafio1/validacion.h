#ifndef VALIDACION_H
#define VALIDACION_H

#include "procesamiento.h"
#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

bool ValidarDesenmascaramiento(unsigned char* imgDesenmascarada, unsigned int* valoresEsperados, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
bool validarXOR(unsigned char* actualIMG, unsigned char* IM, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
bool validarRotarIzquierda(unsigned char* actualIMG, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits);
bool validarRotarDerecha(unsigned char* actualIMG, unsigned int* datosMascara, int semilla,  int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits);

#endif // VALIDACION_H
