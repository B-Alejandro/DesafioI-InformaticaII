/**
 * @file reconstruccion_imagen.cpp
 * @author [Esteban Rodriguez Monsave] y [Alejandro Bedoya Zuluaga]
 * @brief Programa para reconstruir una imagen a partir de etapas de procesamiento usando operaciones XOR, desplazamientos de bits y rotacion de bits.
 *
 * Este programa implementa un proceso inverso para reconstruir una imagen original a partir de una serie de etapas
 * de procesamiento que incluyeron operaciones XOR con una mascara, desplazamientos y rotaciones de bits.
 */

#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>
#include <QFile>
#include <QDir>
#include <QRegularExpression>

#include "procesamiento.h"
#include "operaciones.h"
#include "validacion.h"

using namespace std;



unsigned char* AplicarMascaraInversa(unsigned char* img, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
int DeterminarOperacionInversa(unsigned char* actualIMG, unsigned char* IM, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto);
//int detectarNumEtapas(const QString& rutaBase);
bool cargarDatosBase(const QString& rutaBase, int& anchoIMG, int& altoIMG, int& mask_ancho, int& mask_alto, unsigned char*& IM, unsigned char*& IO);
bool cargarDatosEnmascaramiento(const QString& rutaBase, int numEtapas, unsigned int**& datosMascara, int*& semilla, int*& numPixels);
unsigned char* aplicarOperacionInversa(unsigned char* actualIMG, unsigned char* IM, int operation, int anchoIMG, int altoIMG);
bool procesarEtapa(int etapa, int numEtapas, unsigned char*& currentImg, unsigned char* IM, unsigned int** maskingData, int* seeds, int width, int height, int mask_width, int mask_height, int* operations, const QString& rutaBase);
void reconstruirImagen(const QString& rutaBase, int numEtapas);

unsigned char* AplicarMascaraInversa(unsigned char* img, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto) {
    int totalPixels = anchoIMG * altoIMG * 3;
    unsigned char* result = new unsigned char[totalPixels];
    memcpy(result, img, totalPixels);

    int DimensionMascara = mask_ancho * mask_alto;
    int pos = semilla;

    for (int k = 0; k < DimensionMascara && pos + 2 < totalPixels; k++) {
        unsigned int valor_r = datosMascara[k*3];
        unsigned int valor_g = datosMascara[k*3+1];
        unsigned int valor_b = datosMascara[k*3+2];

        if (valor_r >= 256) {
            result[pos] = (valor_r - img[pos]) % 256;
        } else {
            result[pos] = valor_r;
        }

        if (valor_g >= 256) {
            result[pos+1] = (valor_g - img[pos+1]) % 256;
        } else {
            result[pos+1] = valor_g;
        }

        if (valor_b >= 256) {
            result[pos+2] = (valor_b - img[pos+2]) % 256;
        } else {
            result[pos+2] = valor_b;
        }

        pos += 3;
    }

    return result;
}

// Función principal para determinar la operación inversa
int DeterminarOperacionInversa(unsigned char* actualIMG, unsigned char* IM, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto) {
    // 1. Validar XOR primero
    if (validarXOR(actualIMG, IM, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto)) {
        return 1;
    }

    // 2. Validar rotaciones
    for (int bits = 1; bits <= MAX_BITS; bits++) {
        // Rotación izquierda (inverso de rotación derecha)
        if (validarRotarIzquierda(actualIMG, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto, bits)) {
            return 2 * 10 + bits;
        }

        // Rotación derecha (inverso de rotación izquierda)
        if (validarRotarDerecha(actualIMG, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto, bits)) {
            return 3 * 10 + bits;
        }
    }

    return -1; // No se pudo determinar
}


bool cargarDatosBase(const QString& rutaBase, int& anchoIMG, int& altoIMG, int& mask_ancho, int& mask_alto, unsigned char*& IM, unsigned char*& IO) {
    // Cargar dimensiones de la mascara
    loadPixels(rutaBase + "M.bmp", mask_ancho, mask_alto);
    if (mask_ancho == 0 || mask_alto == 0) {
        cerr << "Error: Dimensiones de mascara invalidas" << endl;
        return false;
    }

    // Cargar imagenes base
    IM = loadPixels(rutaBase + "I_M.bmp", anchoIMG, altoIMG);
    IO = loadPixels(rutaBase + "I_D.bmp", anchoIMG, altoIMG);

    if (!IM || !IO) {
        cerr << "Error al cargar imagenes base" << endl;
        if (IM) delete[] IM;
        if (IO) delete[] IO;
        return false;
    }

    return true;
}

bool cargarDatosEnmascaramiento(const QString& rutaBase, int numEtapas, unsigned int**& datosMascara, int*& semilla, int*& numPixels) {
    datosMascara = new unsigned int*[numEtapas];
    semilla = new int[numEtapas];
    numPixels = new int[numEtapas];

    for (int i = 0; i < numEtapas; i++) {
        QString filename = rutaBase + "M" + QString::number(i+1) + ".txt";
        datosMascara[i] = loadSeedMasking(filename.toStdString().c_str(), semilla[i], numPixels[i]);

        if (!datosMascara[i]) {
            cerr << "Error al cargar archivo de enmascaramiento " << i+1 << endl;
            // Limpiar memoria ya asignada
            for (int j = 0; j < i; j++) delete[] datosMascara[j];
            delete[] datosMascara;
            delete[] semilla;
            delete[] numPixels;
            return false;
        }
    }
    return true;
}

unsigned char* aplicarOperacionInversa(unsigned char* actualIMG, unsigned char* IM, int operation, int anchoIMG, int altoIMG) {
    unsigned char* result = nullptr;

    switch (operation / 10) {
    case 0: // XOR
        cout << "Aplicando XOR inverso" << endl;
        result = DoXOR(actualIMG, IM, anchoIMG, altoIMG);
        break;

    case 2: // Rotación derecha original → izquierda inversa
        cout << "Aplicando rotacion izquierda de " << operation%10 << " bits" << endl;
        result = RotarIzquierda(actualIMG, anchoIMG * altoIMG, operation%10);
        break;

    case 3: // Rotación izquierda original → derecha inversa
        cout << "Aplicando rotacion derecha de " << operation%10 << " bits" << endl;
        result = RotarDerecha(actualIMG, anchoIMG * altoIMG, operation%10);
        break;

    default:
        cerr << "Operacion desconocida: " << operation << endl;
        break;
    }

    return result;
}

bool procesarEtapa(int etapa, int numEtapas, unsigned char*& currentImg, unsigned char* IM, unsigned int** maskingData, int* seeds, int width, int height, int mask_width, int mask_height, int* operations, const QString& rutaBase) {

    cout << "\nProcesando etapa " << etapa+1 << " de " << numEtapas << endl;

    // 1. Aplicar enmascaramiento inverso (excepto en la ultima etapa)
    if (etapa < numEtapas-1) {
        unsigned char* temp = AplicarMascaraInversa(currentImg, maskingData[etapa], seeds[etapa], width, height, mask_width, mask_height);
        if (!temp) {
            cerr << "Error en enmascaramiento inverso" << endl;
            return false;
        }

        delete[] currentImg;
        currentImg = temp;

        // Guardar resultado intermedio
        QString tempName = rutaBase + "Etapa_" + QString::number(etapa+1) + "_post_masking.bmp";
        exportImage(currentImg, width, height, tempName);
    }

    // 2. Determinar y aplicar operacion inversa
    int operation = DeterminarOperacionInversa(currentImg, IM, maskingData[etapa], seeds[etapa], width, height, mask_width, mask_height);
    if (operation == -1) {
        cerr << "Error al determinar operacion inversa" << endl;
        return false;
    }

    operations[etapa] = operation;
    unsigned char* temp = aplicarOperacionInversa(currentImg, IM, operation, width, height);

    if (!temp) {
        cerr << "Error al aplicar operacion inversa" << endl;
        return false;
    }

    delete[] currentImg;
    currentImg = temp;

    // Guardar resultado
    QString tempName = rutaBase + "Etapa_" + QString::number(etapa+1) + "_reconstruida.bmp";
    exportImage(currentImg, width, height, tempName);

    return true;
}

void reconstruirImagen(const QString& rutaBase, int numEtapas) {
    // 1. Cargar datos base
    int width, height, mask_width, mask_height;
    unsigned char *IM, *IO;

    if (!cargarDatosBase(rutaBase, width, height, mask_width, mask_height, IM, IO)) {
        return;
    }

    // 2. Cargar datos de enmascaramiento
    unsigned int** maskingData;
    int *seeds, *numPixels;

    if (!cargarDatosEnmascaramiento(rutaBase, numEtapas, maskingData, seeds, numPixels)) {
        delete[] IM;
        delete[] IO;
        return;
    }

    // 3. Preparar reconstrucción
    unsigned char* currentImg = IO;
    int* operations = new int[numEtapas];
    bool success = true;

    // 4. Procesar cada etapa en orden inverso
    for (int etapa = numEtapas-1; etapa >= 0; etapa--) {
        if (!procesarEtapa(etapa, numEtapas, currentImg, IM, maskingData, seeds,
                           width, height, mask_width, mask_height, operations, rutaBase)) {
            success = false;
            break;
        }
    }

    // 5. Guardar y mostrar resultados si todo fue bien
    if (success) {
        exportImage(currentImg, width, height, rutaBase + "I_O_reconstruida_final.bmp");

        cout << "\nOrden de operaciones (de ultima a primera):" << endl;
        for (int i = numEtapas-1; i >= 0; i--) {
            cout << "Etapa " << numEtapas-i << ": ";
            switch (operations[i] / 10) {
            case 0: cout << "XOR con I_M"; break;
            case 2: cout << "Rotacion derecha original (" << operations[i]%10 << " bits)"; break;
            case 3: cout << "Rotacion izquierda original (" << operations[i]%10 << " bits)"; break;
            default: cout << "Operacion desconocida"; break;
            }
            cout << endl;
        }
    }

    // 6. Liberar memoria
    delete[] currentImg;
    delete[] IM;
    for (int i = 0; i < numEtapas; i++) {
        delete[] maskingData[i];
    }
    delete[] maskingData;
    delete[] seeds;
    delete[] numPixels;
    delete[] operations;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Configuración de parámetros
    QString rutaBase = "C:/Users/esteb/OneDrive/Escritorio/DES/codigo/Desafio1/Caso 1/";
    int numEtapas = 2; // Cambiar según el número de etapas que tenga tu caso

    // Mensaje inicial
    cout << "=============================================" << endl;
    cout << "  SISTEMA DE RECONSTRUCCION DE IMAGENES" << endl;
    cout << "=============================================" << endl;
    cout << "Ruta base: " << rutaBase.toStdString() << endl;
    cout << "Numero de etapas: " << numEtapas << endl;
    cout << "Iniciando proceso..." << endl;

    try {
        // Llamar a la función principal de reconstrucción
        reconstruirImagen(rutaBase, numEtapas);

        cout << "\nProceso completado exitosamente!" << endl;
        cout << "Resultados guardados en: " << rutaBase.toStdString() << endl;
    } catch (const exception& e) {
        cerr << "\nERROR: " << e.what() << endl;
        return 1;
    }

    return a.exec();
}
