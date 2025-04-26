/**
 * @file BitaBit.cpp
 * @brief Módulo de manipulación de bits para imágenes RGB.
 *
 * Este módulo proporciona un conjunto de funciones para realizar operaciones a nivel de bit
 * sobre imágenes en formato RGB, representadas como arreglos de bytes. Entre las operaciones
 * disponibles se incluyen:
 *   - Operaciones bit a bit (XOR) entre dos imágenes.
 *   - Rotaciones circulares de bits a la derecha o izquierda en cada byte de la imagen.
 *   - Enmascaramiento de imágenes mediante suma/resta de valores de una máscara.
 *   - Desplazamientos lógicos de bits en bytes individuales.
 *
 * Estas funciones están diseñadas para ser utilizadas en aplicaciones de procesamiento de imágenes,
 * criptografía visual, efectos especiales y manipulación avanzada de datos gráficos.
 *
 * Todas las funciones que retornan arreglos dinámicos requieren que el usuario libere la memoria
 * correspondiente usando `delete[]` para evitar fugas de memoria.
 *
 * @author Alejandro Bedoya Zuluaga
 */
#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <QDir>
#include "ModulosEncriptacion.h"

using namespace std;

/**
 * @brief Realiza una operación XOR entre dos imágenes.
 *
 * Esta función realiza una operación bit a bit XOR entre dos imágenes representadas
 * como arreglos de bytes RGB. El resultado es una nueva imagen con los valores XOR
 * de cada canal de color.
 *
 * @param imagen Puntero al arreglo de bytes RGB de la primera imagen.
 * @param mascara Puntero al arreglo de bytes RGB de la segunda imagen (máscara).
 * @param width Ancho de la imagen en píxeles.
 * @param height Alto de la imagen en píxeles.
 * @return unsigned char* Puntero a un arreglo dinámico con el resultado de la operación XOR.
 *                      El tamaño es width * height * 3 bytes (RGB).
 *                      El usuario es responsable de liberar la memoria con delete[].
 */
unsigned char* DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height) {
    int numPixels = width * height * 3;
    unsigned char* resultado = new unsigned char[numPixels];

    for (int i = 0; i < numPixels; i += 3) {
        resultado[i]     = imagen[i]     ^ mascara[i];
        resultado[i + 1] = imagen[i + 1] ^ mascara[i + 1];
        resultado[i + 2] = imagen[i + 2] ^ mascara[i + 2];
    }

    return resultado;
}




/**
 * @brief Rota los bits de cada byte hacia la derecha.
 *
 * Esta función rota los bits de cada byte del arreglo de imagen hacia la derecha
 * n posiciones. La rotación es circular.
 *
 * @param num_pixels Número de píxeles en la imagen.
 * @param img Puntero al arreglo de bytes RGB de la imagen.
 * @param n Número de posiciones a rotar (0-7).
 * @return unsigned char* Puntero a un arreglo dinámico con la imagen rotada.
 *                      El tamaño es num_pixels * 3 bytes (RGB).
 *                      El usuario es responsable de liberar la memoria con delete[].
 */
unsigned char* RotarDerecha(int num_pixels, unsigned char* img, int n) {
    n %= 8;
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return resultado;
}






/**
 * @brief Rota los bits de cada byte hacia la izquierda.
 *
 * Esta función rota los bits de cada byte del arreglo de imagen hacia la izquierda
 * n posiciones. La rotación es circular.
 *
 * @param num_pixels Número de píxeles en la imagen.
 * @param img Puntero al arreglo de bytes RGB de la imagen.
 * @param n Número de posiciones a rotar (0-7).
 * @return unsigned char* Puntero a un arreglo dinámico con la imagen rotada.
 *                      El tamaño es num_pixels * 3 bytes (RGB).
 *                      El usuario es responsable de liberar la memoria con delete[].
 */





unsigned char* RotarIzquierda(unsigned char* img,int num_pixels,  int n) {
    n %= 8;
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return resultado;
}

/**
 * @brief Realiza un enmascaramiento sumando valores de la máscara al arreglo original con un desplazamiento inicial (semilla).
 *
 * La operación consiste en sumar los bytes del arreglo 'mascara' a los bytes del arreglo 'originales',
 * comenzando en una posición de 'originales' definida por 'seed' y avanzando de forma lineal.
 * La cantidad de bytes a sumar está determinada por el tamaño de 'mascara'.
 *
 * @param originales Puntero al inicio del arreglo original (este arreglo será modificado).
 * @param mascara Puntero al inicio del arreglo que se utilizará como máscara.
 * @param cantidad Número de elementos (bytes) en el arreglo 'mascara'.
 * @param seed Índice dentro del arreglo 'originales' desde donde comenzará la suma.
 */



void Desenmascaramiento(unsigned char* originales, unsigned char* mascara, int cantidad, int seed) {
    for (int i = 0; i < cantidad; ++i) {
        int indexOriginal = seed + i;  // Índice base en originales para el píxel i
        int indexMascara = i;           // Índice base en mascara para el píxel i

        int maskR = mascara[indexMascara];
        int maskG = mascara[indexMascara + 1];
        int maskB = mascara[indexMascara + 2];

        // Realizar una resta
        originales[indexOriginal]     -= maskR;
        originales[indexOriginal + 1] -= maskG;
        originales[indexOriginal + 2] -= maskB;
    }
}


/**
 * @brief Realiza un desplazamiento a nivel de bit hacia la derecha en un byte.
 *
 * @param byte El byte a desplazar.
 * @param n La cantidad de bits a desplazar (0-7).
 * @return El byte con los bits desplazados hacia la derecha.
 */

// unsigned char shiftRightBit(unsigned char byte, int n,unsigned char txt) {
//     n %= 8;
//     return byte >> n;
//     //Compara la informacion anterior

// }



// /**
//  * @brief Realiza un desplazamiento a nivel de bit hacia la izquierda en un byte.
//  *
//  * @param byte El byte a desplazar.
//  * @param n La cantidad de bits a desplazar (0-7).
//  * @return El byte con los bits desplazados hacia la izquierda.
//  */
// unsigned char shiftLeftBit(unsigned char* byte, int n) {
//     n %= 8;
//     return byte << n;
// }

