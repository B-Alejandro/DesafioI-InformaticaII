#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>
#include "FuncionesAbrirImagen.h"

using namespace std;

/**
 * @brief Función para sumar todos los valores RGB de una imagen.
 * @return Suma total de los valores de píxeles (sin implementar).
 */
int SumaPixeles() {
    // TODO: Implementar si es necesario
    return 0;
}

/**
 * @brief Realiza una operación XOR entre dos imágenes.
 *
 * @param imagen Arreglo de bytes con la información de los píxeles de la imagen original.
 * @param mascara Arreglo de bytes con la información de los píxeles de la máscara.
 * @param width Ancho de las imágenes.
 * @param height Alto de las imágenes.
 * @return unsigned char* Nuevo arreglo con el resultado del XOR entre imagen y máscara.
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
 * @brief Rota los bits de cada byte a la derecha.
 *
 * @param num_pixels Número total de píxeles de la imagen.
 * @param img Arreglo de píxeles original.
 * @param n Número de bits a rotar.
 * @return unsigned char* Nuevo arreglo con los bits rotados.
 */
unsigned char* RotarDerecha(int num_pixels, unsigned char* img, int n) {
    /**
 * @brief Rota los bits de cada byte a la izquierda.
 *
 * @param num_pixels Número total de píxeles de la imagen.
 * @param img Arreglo de píxeles original.
 * @param n Número de bits a rotar.
 * @return unsigned char* Nuevo arreglo con los bits rotados.
 */
    n %= 8;
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return resultado;
}


unsigned char* RotarIzquierda(int num_pixels, unsigned char* img, int n) {
    n %= 8;
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return resultado;
}
unsigned char* Enmascaramiento(unsigned char* originales, unsigned char* mascara, int cantidad, int seed) {
    unsigned char* mask = originales + seed;
    unsigned char* sumar=new unsigned char[cantidad];
    for (int i = 0; i < cantidad; ++i) {
        sumar[i] = mask[i] + mascara[i];  // modifica directamente la imagen original
        cout<<int (sumar[i])<<endl;
    }

    return sumar-seed;  // devuelve el puntero original
}

int main() {
    // Rutas de los archivos BMP

    QString ruta = "Datos/Caso 1/";
    QString rutaMascara  = ruta + "I_M.bmp";
    QString rutaImagen   = ruta + "I_O.bmp";
    QString rutaMasking  = ruta + "M.bmp";
    QString rutaImagenP2 = ruta + "P2.bmp";
    QString rutaMaskTxt  = ruta + "M2.txt";
    cout<<rutaImagenP2.toStdString()<<endl;
    string rutaMaskTXT=rutaMaskTxt.toStdString();
    int width = 0, height = 0;

    // Cargar imágenes BMP
    unsigned char* Mascara = loadPixels(rutaMascara, width, height);
    unsigned char* Imagen  = loadPixels(rutaImagen, width, height);

    if (!Mascara || !Imagen) {
        cerr << "Error al cargar las imágenes." << endl;
        delete[] Mascara;
        delete[] Imagen;
        return 1;
    }

    int numPixels = width * height;

    // Paso 1: XOR entre Imagen y Mascara
    unsigned char* paso1 = DoXOR(Imagen, Mascara, width, height);

    // Paso 2: Rotación de bits a la derecha
    unsigned char* paso2 = RotarDerecha(numPixels, paso1, 3);

    // Paso 3: Segundo XOR con la máscara
    unsigned char* resultadoFinal = DoXOR(paso2, Mascara, width, height);

    // Cargar y mostrar datos de enmascaramiento (M.bmp)
    int widthM = 0, heightM = 0;
    unsigned char* maskingData = loadPixels(rutaMasking, widthM, heightM);

    if (!maskingData) {
        cout<< "Error al cargar la imagen de enmascaramiento." << endl;

    }
    // Cargar segunda imagen y máscara (P2.bmp y M2.txt)
    unsigned char* p2 = loadPixels(rutaImagenP2, width, height);
    exportImage(p2,width,height,"C:/Users/alejb/Desktop/Ij.bmp");
    int seed = 0, npixel = 0;

    unsigned int* mask = loadSeedMasking(rutaMaskTXT, seed, npixel);
    int c;
    unsigned char* p3=Enmascaramiento( p2,maskingData,(widthM * heightM),seed);
    for (int i = 0; i < 100; ++i) {
        c=i*3;
        int r = p3[c];
        int g = p3[c + 1];
        int b = p3[c + 2];
        cout <<"pixel "<<i<<" = "<< r << "," << g << "," << b << endl;
    }

    // Liberar memoria
    delete[] Mascara;
    delete[] Imagen;
    delete[] paso1;
    delete[] paso2;
    delete[] resultadoFinal;
    delete[] maskingData;
    delete[] p2;
    delete[] mask;

    return 0;
}

// int main() {
//     int width = 0, height = 0;

//
//     QString rutaBsalida = "C:/Users/alejb/Desktop/pruebas/";

//     QString rutaI_M = rutaEntrada + "I_M.bmp";
//     QString rutaI_D = rutaEntrada + "I_D.bmp";

//     // Cargar imágenes
//     unsigned char* I_D = loadPixels(rutaI_D, width, height);
//     if (!I_D) {
//         cerr << "Error al cargar imagen original." << std::endl;
//         return 1;
//     }

//     int widthM = 0, heightM = 0;
//     unsigned char* I_M = loadPixels(rutaI_M, widthM, heightM);
//     if (!I_M) {
//         cerr << "Error al cargar máscara." << std::endl;
//         delete[] I_D;
//         return 1;
//     }

//     // Validar dimensiones iguales
//     if (width != widthM || height != heightM || width <= 0 || height <= 0) {
//         cerr << "Dimensiones de imagen y máscara no coinciden o inválidas." << std::endl;
//         delete[] I_D;
//         delete[] I_M;
//         return 1;
//     }

//     int numPixels = width * height;
//     const int operaciones_por_imagen = 17; // 1 XOR + 8 rotD + 8 rotI

//     // Función lambda para guardar imagen (con control de errores)
//     auto guardarImagen = [&](unsigned char* img, int etapa, int idxImagen, const QString& operacion) {
//         if (!img) {
//             cerr << "Puntero nulo al intentar guardar imagen." << endl;
//             return;
//         }
//         QString nombreArchivo = rutaBsalida + QString("etapa%1_img%2_%3.bmp").arg(etapa).arg(idxImagen).arg(operacion);
//         exportImage(img, width, height, nombreArchivo);
//     };

//     // Inicializar arreglo dinámico con la imagen original
//     int num_imagenes_actuales = 1;
//     unsigned char** imagenes_actuales = new(std::nothrow) unsigned char*[num_imagenes_actuales];
//     if (!imagenes_actuales) {
//         std::cerr << "Error al asignar memoria para arreglo de imágenes." << std::endl;
//         delete[] I_D;
//         delete[] I_M;
//         return 1;
//     }

//     imagenes_actuales[0] = new(std::nothrow) unsigned char[numPixels * 3];
//     if (!imagenes_actuales[0]) {
//         std::cerr << "Error al asignar memoria para imagen original." << std::endl;
//         delete[] imagenes_actuales;
//         delete[] I_D;
//         delete[] I_M;
//         return 1;
//     }
//     memcpy(imagenes_actuales[0], I_D, numPixels * 3);
//     delete[] I_D; // Liberar original

//     for (int etapa = 1; etapa <= 7; ++etapa) {


//         int max_imagenes_siguientes = num_imagenes_actuales * operaciones_por_imagen;
//         unsigned char** imagenes_siguientes = new(std::nothrow) unsigned char*[max_imagenes_siguientes];
//         if (!imagenes_siguientes) {
//             std::cerr << "Error al asignar memoria para imágenes siguientes." << std::endl;
//             // Liberar todo antes de salir
//             for (int i = 0; i < num_imagenes_actuales; ++i) {
//                 delete[] imagenes_actuales[i];
//             }
//             delete[] imagenes_actuales;
//             delete[] I_M;
//             return 1;
//         }

//         int contador_siguientes = 0;

//         for (int i = 0; i < num_imagenes_actuales; ++i) {
//             unsigned char* img = imagenes_actuales[i];
//             if (!img) {
//                 std::cerr << "Puntero nulo en imagen actual, saltando." << std::endl;
//                 continue;
//             }

//             // 1) XOR con la máscara
//             unsigned char* img_xor = DoXOR(img, I_M, width, height);
//             if (!img_xor) {
//                 std::cerr << "Error en DoXOR." << std::endl;
//                 continue;
//             }
//             if (etapa == 7) {
//                 guardarImagen(img_xor, etapa, i, "xor");
//             }
//             imagenes_siguientes[contador_siguientes++] = img_xor;

//             // 2) Rotaciones derecha 1 a 8 bits
//             for (int n = 1; n <= 8; ++n) {
//                 unsigned char* img_rotD = RotarDerecha(numPixels, img, n);
//                 if (!img_rotD) {
//                     std::cerr << "Error en RotarDerecha." << std::endl;
//                     continue;
//                 }
//                 if (etapa == 7) {
//                     guardarImagen(img_rotD, etapa, i, QString("rotD_%1").arg(n));
//                 }
//                 imagenes_siguientes[contador_siguientes++] = img_rotD;
//             }

//             // 3) Rotaciones izquierda 1 a 8 bits
//             for (int n = 1; n <= 8; ++n) {
//                 unsigned char* img_rotI = RotarIzquierda(numPixels, img, n);
//                 if (!img_rotI) {
//                     std::cerr << "Error en RotarIzquierda." << std::endl;
//                     continue;
//                 }
//                 if (etapa == 7) {
//                     guardarImagen(img_rotI, etapa, i, QString("rotI_%1").arg(n));
//                 }
//                 imagenes_siguientes[contador_siguientes++] = img_rotI;
//             }

//             // Liberar imagen actual
//             delete[] img;
//         }

//         // Liberar arreglo anterior
//         delete[] imagenes_actuales;

//         imagenes_actuales = imagenes_siguientes;
//         num_imagenes_actuales = contador_siguientes;

//         // Validar que no se exceda el máximo esperado
//         if (num_imagenes_actuales > max_imagenes_siguientes) {
//             std::cerr << "Error: contador de imágenes excede el máximo asignado." << std::endl;
//             // Liberar todo y salir
//             for (int i = 0; i < num_imagenes_actuales; ++i) {
//                 delete[] imagenes_actuales[i];
//             }
//             delete[] imagenes_actuales;
//             delete[] I_M;
//             return 1;
//         }
//     }

//     // Liberar imágenes restantes
//     for (int i = 0; i < num_imagenes_actuales; ++i) {
//         delete[] imagenes_actuales[i];
//     }
//     delete[] imagenes_actuales;

//     delete[] I_M;

//     std::cout << "Procesamiento completado correctamente." << std::endl;
//     return 0;
// }
//
