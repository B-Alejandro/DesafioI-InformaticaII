#include "funvalidacion.h"
#include "bitabit.h"

#include "exportarimg.h"
#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Constantes
//const int MAX_BITS = 7;

// Función para validar el resultado del desenmascaramiento
bool ValidarDesenmascaramiento(unsigned char* imgDesenmascarada, unsigned int* valoresEsperados, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto) {
    int DimensionMascara = mask_ancho * mask_alto;
    int pos = semilla;
    int totalPixeles = anchoIMG * altoIMG * 3;

    cout << "Iniciando validacion con posicion inicial: " << pos << endl;
    cout << "Dimension de mascara: " << DimensionMascara << " elementos ("
         << DimensionMascara*3 << " componentes de color)" << endl;

    for (int k = 0; k < DimensionMascara && pos + 2 < totalPixeles; k++) {
        unsigned char r = imgDesenmascarada[pos];
        unsigned char g = imgDesenmascarada[pos+1];
        unsigned char b = imgDesenmascarada[pos+2];

        // Valores esperados (del archivo)
        unsigned int esperado_r = valoresEsperados[k*3] % 255;
        unsigned int esperado_g = valoresEsperados[k*3+1] % 255;
        unsigned int esperado_b = valoresEsperados[k*3+2] % 255;

        cout << "\nPixel " << k + 1 << " (posicion " << pos << " a " << pos+2 << "):" << endl;
        cout << "  RGB Imagen: (" << (int)r << ", " << (int)g << ", " << (int)b << ")" << endl;
        cout << "  Valor Esperado: (" << esperado_r << ", " << esperado_g << ", " << esperado_b << ")" << endl;

        if (r != esperado_r || g != esperado_g || b != esperado_b) {
            cout << "VALIDACION FALLIDA en el pixel " << k + 1 << endl;
            cout << "  Diferencia encontrada: R(" << (int)r << " vs " << esperado_r << "), "
                 << "G(" << (int)g << " vs " << esperado_g << "), "
                 << "B(" << (int)b << " vs " << esperado_b << ")" << endl;
            return false;
        }

        cout << "  Validacion exitosa para este pixel" << endl;
        pos += 3;
    }

    cout << "\nTodos los pixeles se validaron correctamente!" << endl;
    return true;
}

// Función para validar operación XOR
bool validarXOR(unsigned char* actualIMG, unsigned char* IM, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto) {
    unsigned char* xorResult = DoXOR(actualIMG, IM, anchoIMG, altoIMG);
    bool esValido = ValidarDesenmascaramiento(xorResult, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto);
    delete[] xorResult;
    return esValido;
}

// Función para validar rotación izquierda
bool validarRotarIzquierda(unsigned char* actualIMG, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits) {
    int totalPixeles = anchoIMG * altoIMG;
    unsigned char* rotado = RotarIzquierda(actualIMG, totalPixeles, bits);
    bool esValido = ValidarDesenmascaramiento(rotado, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto);
    delete[] rotado;
    return esValido;
}

// Función para validar rotación derecha
bool validarRotarDerecha(unsigned char* actualIMG, unsigned int* datosMascara, int semilla,  int anchoIMG, int altoIMG, int mask_ancho, int mask_alto, int bits) {
    int totalPixeles = anchoIMG * altoIMG;
    unsigned char* rotado = RotarDerecha(actualIMG, totalPixeles, bits);
    bool esValido = ValidarDesenmascaramiento(rotado, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto);
    delete[] rotado;
    return esValido;
}

// bool validarConArchivoMascara(unsigned char* imgOriginal, unsigned int* datosMascara, int semilla, int anchoIMG, int altoIMG, int mask_ancho, int mask_alto) {
//     // 1. Primero aplicar el desenmascaramiento
//     unsigned char* imgDesenmascarada = AplicarMascaraInversa(
//         imgOriginal, datosMascara, semilla, anchoIMG, altoIMG, mask_ancho, mask_alto);

//     // 2. Preparar parámetros para validación
//     int DimensionMascara = mask_ancho * mask_alto;
//     int pos = semilla;
//     int totalPixeles = anchoIMG * altoIMG * 3;

//     cout << "Iniciando validacion con posicion inicial: " << pos << endl;
//     cout << "Dimension de mascara: " << DimensionMascara << " elementos ("
//          << DimensionMascara*3 << " componentes de color)" << endl;

//     bool validacionExitosa = true;

//     // 3. Validar cada pixel
//     for (int k = 0; k < DimensionMascara && pos + 2 < totalPixeles; k++) {
//         unsigned char r = imgDesenmascarada[pos];
//         unsigned char g = imgDesenmascarada[pos+1];
//         unsigned char b = imgDesenmascarada[pos+2];

//         // Los valores esperados son los mismos datosMascara pero interpretados diferente
//         unsigned int esperado_r = datosMascara[k*3] % 256;
//         unsigned int esperado_g = datosMascara[k*3+1] % 256;
//         unsigned int esperado_b = datosMascara[k*3+2] % 256;

//         cout << "\nPixel " << k + 1 << " (pos " << pos << "-" << pos+2 << "):" << endl;
//         cout << "  Obtenido: (" << (int)r << "," << (int)g << "," << (int)b << ")" << endl;
//         cout << "  Esperado: (" << esperado_r << "," << esperado_g << "," << esperado_b << ")" << endl;

//         if (r != esperado_r || g != esperado_g || b != esperado_b) {
//             cout << "  ERROR: No coinciden los valores" << endl;
//             validacionExitosa = false;
//             // Puedes decidir si quieres continuar o salir aquí
//         } else {
//             cout << "  OK: Coincidencia correcta" << endl;
//         }

//         pos += 3;
//     }

//     // 4. Limpiar memoria
//     delete[] imgDesenmascarada;

//     if (validacionExitosa) {
//         cout << "\nVALIDACION EXITOSA: Todos los pixeles coinciden" << endl;
//     } else {
//         cout << "\nVALIDACION FALLIDA: Algunos pixeles no coinciden" << endl;
//     }

//     return validacionExitosa;
// }
