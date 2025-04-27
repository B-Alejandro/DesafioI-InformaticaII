#ifndef FUNVALIDACION_H
#define FUNVALIDACION_H

#include "exportarimg.h"
#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Constantes
//const int MAX_BITS = 7;

bool ValidarDesenmascaramiento(unsigned char* imgDesenmascarada, unsigned int* valoresEsperados, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
bool validarXOR(unsigned char* actualIMG, unsigned char* IM, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
bool validarRotarIzquierda(unsigned char* actualIMG, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits);
bool validarRotarDerecha(unsigned char* actualIMG, unsigned int* datosMascara, int semilla,  int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits);
//bool validarConArchivoMascara(unsigned char* img, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);

#endif // FUNVALIDACION_H
