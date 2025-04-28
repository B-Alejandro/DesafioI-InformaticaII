#include <fstream>
#include <iostream>
#include <QString>
#include <QDir> // Necesario para cambiar el directorio
#include <ModulosEncriptacion.h>
#include <cstdint> // Para los tipos exactos como int8_t

using namespace std;
// Funcion optimizada para detectar XOR
int8_t EsXor(unsigned char* imagen, unsigned char* mascara, unsigned int* maskingData, int8_t seed) {
    cout << "Verificando XOR con semilla: " << (int)seed << endl;

    // Buscar coincidencias inmediatas
    for (int8_t i = 0; i < 60; ++i) {
        unsigned char xorResult = imagen[seed + i] ^ mascara[i];

        if(maskingData[i] > 255)
            maskingData[i] %= 256;

        if (xorResult == maskingData[i]) {
            cout << "  Coincidencia XOR encontrada en byte " << (int)i << ": "
                 << "Imagen[" << (int)(seed + i) << "] = " << (int)imagen[seed + i]
                 << ", Mascara[" << (int)i << "] = " << (int)mascara[i]
                 << ", XOR = " << (int)xorResult
                 << ", Esperado = " << maskingData[i] << endl;

            // Verificar unos bytes mas para confirmar
            bool confirmado = true;
            for (int j = 1; j < 5 && (i+j) < 60; ++j) {
                unsigned char nextXor = imagen[seed + i + j] ^ mascara[i + j];
                if(maskingData[i+j] > 255)
                    maskingData[i+j] %= 256;

                if (nextXor != maskingData[i + j]) {
                    confirmado = false;
                    break;
                }
            }

            if (confirmado) {
                cout << "  Es XOR confirmado despues de " << (int)i << " bytes" << endl;
                return 1; // Retorna 1 inmediatamente al confirmar XOR
            }
        }
    }

    cout << "  No es XOR" << endl;
    return 0;
}
int8_t EsRotacion(unsigned char* imagen, unsigned int* maskingData, int8_t seed) {
    cout << "\n\nVerificando Rotacion con semilla: " << (int)seed << endl;

    for (int8_t p = 1; p <= 8; ++p) {
        cout << "  \nProbando rotacion con " << (int)p << " bits" << endl;

        for (int8_t n = 0; n < 20; ++n) {
            unsigned char rotDerecha = (imagen[seed + n] >> p) | (imagen[seed + n] << (8 - p));
            unsigned char rotIzquierda = (imagen[seed + n] << p) | (imagen[seed + n] >> (8 - p));

            if (maskingData[n] > 255)
                maskingData[n] %= 256;

            // Comprobamos rotación a la derecha
            if (rotDerecha == maskingData[n]) {
                bool confirmado = true;
                for (int j = 1; j < 3 && (n+j) < 20; ++j) {
                    unsigned char nextRot = (imagen[seed + n + j] >> p) | (imagen[seed + n + j] << (8 - p));
                    if (maskingData[n+j] > 255)
                        maskingData[n+j] %= 256;
                    if (nextRot != maskingData[n + j]) {
                        confirmado = false;
                        break;
                    }
                }
                if (confirmado) {
                    cout << "  \n\nEs Rotacion Derecha con " << (int)p << " bits" << endl;
                    return p + 30;
                }
            }

            // Comprobamos rotación a la izquierda
            if (rotIzquierda == maskingData[n]) {
                bool confirmado = true;
                for (int j = 1; j < 3 && (n+j) < 20; ++j) {
                    unsigned char nextRot = (imagen[seed + n + j] << p) | (imagen[seed + n + j] >> (8 - p));
                    if (maskingData[n+j] > 255)
                        maskingData[n+j] %= 256;
                    if (nextRot != maskingData[n + j]) {
                        confirmado = false;
                        break;
                    }
                }
                if (confirmado) {
                    cout << "  \n\nEs Rotacion Izquierda con " << (int)p << " bits" << endl;
                    return p + 20;
                }
            }
        }
    }
    return 0; // Si no encuentra ninguna rotación
}

// Funcion optimizada para detectar enmascaramiento
int EsEnmascaramiento(
    unsigned char* imagen,
    unsigned char* mascara,
    unsigned int* maskingData,
    int ancho_img,
    int alto_img,
    int ancho_mascara,
    int alto_mascara,
    int seed) {

    cout << "Verificando Enmascaramiento con semilla: " << seed << endl;
    int tamanoMascara = ancho_mascara * alto_mascara;
    int pos = seed;
    int totalPixeles = ancho_img * alto_img * 3; // 3 bytes por pixel (RGB)

    for (int k = 0; k < tamanoMascara && pos + 2 < totalPixeles; k++) {
        unsigned char r = imagen[pos];
        unsigned char g = imagen[pos + 1];
        unsigned char b = imagen[pos + 2];

        unsigned int valor_esperado_r = maskingData[k];
        unsigned int valor_esperado_g = maskingData[k + 1];
        unsigned int valor_esperado_b = maskingData[k + 2];

        bool coincidenciaEncontrada = true;

        // Validar componente R
        if (valor_esperado_r >= 256) {
            unsigned int mascara_r = valor_esperado_r - r;
            if ((r + mascara_r) != valor_esperado_r) {
                coincidenciaEncontrada = false;
            }
        } else {
            if (r != valor_esperado_r) {
                coincidenciaEncontrada = false;
            }
        }

        // Si R coincide, validar G
        if (coincidenciaEncontrada) {
            if (valor_esperado_g >= 256) {
                unsigned int mascara_g = valor_esperado_g - g;
                if ((g + mascara_g) != valor_esperado_g) {
                    coincidenciaEncontrada = false;
                }
            } else {
                if (g != valor_esperado_g) {
                    coincidenciaEncontrada = false;
                }
            }
        }

        // Si R y G coinciden, validar B
        if (coincidenciaEncontrada) {
            if (valor_esperado_b >= 256) {
                unsigned int mascara_b = valor_esperado_b - b;
                if ((b + mascara_b) != valor_esperado_b) {
                    coincidenciaEncontrada = false;
                }
            } else {
                if (b != valor_esperado_b) {
                    coincidenciaEncontrada = false;
                }
            }
        }

        // Si todos los componentes coinciden, retornar inmediatamente
        if (coincidenciaEncontrada) {
            cout << "  Coincidencia de enmascaramiento encontrada en pixel " << k << " (posicion " << pos << ")" << endl;
            cout << "  RGB Imagen: (" << (int)r << ", " << (int)g << ", " << (int)b << ")" << endl;
            cout << "  Valores Esperados: (" << valor_esperado_r %256<< ", " << valor_esperado_g%256 << ", " << valor_esperado_b %256<< ")" << endl;

            // Confirmar con unos pixeles adicionales
            bool confirmado = true;
            for (int j = 1; j < 2 && (k+j*3+2) < tamanoMascara && (pos+j*3+2) < totalPixeles; ++j) {
                // Validar siguiente pixel
                unsigned char next_r = imagen[pos + j*3];
                unsigned char next_g = imagen[pos + j*3 + 1];
                unsigned char next_b = imagen[pos + j*3 + 2];

                unsigned int next_valor_r = maskingData[k + j*3];
                unsigned int next_valor_g = maskingData[k + j*3 + 1];
                unsigned int next_valor_b = maskingData[k + j*3 + 2];

                bool next_valido = true;

                // Verificar componentes RGB del siguiente pixel
                if (next_valor_r >= 256) {
                    unsigned int next_mascara_r = next_valor_r - next_r;
                    if ((next_r + next_mascara_r) != next_valor_r) next_valido = false;
                } else if (next_r != next_valor_r) next_valido = false;

                if (next_valor_g >= 256) {
                    unsigned int next_mascara_g = next_valor_g - next_g;
                    if ((next_g + next_mascara_g) != next_valor_g) next_valido = false;
                } else if (next_g != next_valor_g) next_valido = false;

                if (next_valor_b >= 256) {
                    unsigned int next_mascara_b = next_valor_b - next_b;
                    if ((next_b + next_mascara_b) != next_valor_b) next_valido = false;
                } else if (next_b != next_valor_b) next_valido = false;

                if (!next_valido) {
                    confirmado = false;
                    break;
                }
            }

            if (confirmado) {
                cout << "¡Enmascaramiento confirmado!" << endl;
                return 4;  // Retorna 4 si se confirma el enmascaramiento
            }
        }

        pos += 3; // avanzar al siguiente pixel (3 componentes RGB)
    }

    cout << "No se detecto enmascaramiento valido" << endl;
    return 0;
}
int8_t obtenerNumeroEtapas(QString rutaBase) {
    // Buscar archivos P*.bmp
    QDir directorio(rutaBase);
    QStringList filtros;
    filtros << "M*.txt";  // Esto capturara M1.txt, M2.txt,M3.txt...Mn.txt.
    directorio.setNameFilters(filtros);

    // Obtener la lista de archivos que coinciden
    QStringList archivosP = directorio.entryList(filtros, QDir::Files);

    // El numero de archivos P
    uint8_t numArchivosP = archivosP.size();
    cout << "Numero de archivos P encontrados: " << static_cast<int>(numArchivosP) << endl;

    // Usar numArchivosP en lugar de un valor fijo
    uint8_t numEtapas = numArchivosP;
    cout << "Numero de etapas: " << static_cast<int>(numEtapas) << endl;

    return numEtapas;
}
