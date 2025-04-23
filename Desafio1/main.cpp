/**
 * @file reconstruccion_imagen.cpp
 * @author [Esteban Rodriguez Monsave] y [Alejandro Bedoya Zuluaga]
 * @brief Programa para reconstruir una imagen a partir de etapas de procesamiento usando operaciones XOR, desplazamientos de bits y rotación de bits.
 *
 * Este programa implementa un proceso inverso para reconstruir una imagen original a partir de una serie de etapas
 * de procesamiento que incluyeron operaciones XOR con una máscara, desplazamientos y rotaciones de bits.
 */

#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Prototipos de funciones
unsigned char* loadPixels(const QString& input, int& width, int& height);
bool exportImage(unsigned char* pixelData, int width, int height, const QString& archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int& seed, int& n_pixels);

/**
 * @brief Exporta una imagen en formato BMP.
 * @param pixelData Datos de los píxeles de la imagen.
 * @param width Ancho de la imagen.
 * @param height Alto de la imagen.
 * @param archivoSalida Ruta del archivo de salida.
 * @return true si la exportación fue exitosa, false en caso contrario.
 *
 * Función proporcionada por los profesores para exportar imágenes en formato BMP.
 * La implementamos copiando los datos de píxeles a un objeto QImage y usando su método save().
 */
bool exportImage(unsigned char* pixelData, int width, int height, const QString& archivoSalida) {
    QImage outputImage(width, height, QImage::Format_RGB888);

    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }

    if (!outputImage.save(archivoSalida, "BMP")) {
        cout << "Error: No se pudo guardar la imagen BMP." << endl;
        return false;
    }
    return true;
}

/**
 * @brief Realiza operación XOR entre dos imágenes.
 * @param imagen Primera imagen para la operación.
 * @param mascara Segunda imagen para la operación.
 * @param width Ancho de las imágenes.
 * @param height Alto de las imágenes.
 * @return Puntero a los datos de píxeles resultantes.
 *
 * Función creada por mi compañero que aplica XOR bit a bit entre dos imágenes.
 * La implementación recorre todos los píxeles aplicando el operador ^.
 */
unsigned char* DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height) {
    int numPixels = width * height;
    unsigned char* resultado = new unsigned char[numPixels * 3];

    for (int i = 0; i < numPixels * 3; ++i) {
        resultado[i] = imagen[i] ^ mascara[i];
    }

    return resultado;
}

/**
 * @brief Rota los bits de los píxeles a la derecha.
 * @param img Datos de la imagen original.
 * @param num_pixels Número total de píxeles.
 * @param n Número de bits a rotar.
 * @return Puntero a los datos de píxeles rotados.
 *
 * Función creada por mi compañero que rota los bits de cada componente de color n posiciones a la derecha.
 * Implementa la rotación usando operadores de desplazamiento bit a bit.
 */
unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return resultado;
}

/**
 * @brief Rota los bits de los píxeles a la izquierda.
 * @param img Datos de la imagen original.
 * @param num_pixels Número total de píxeles.
 * @param n Número de bits a rotar.
 * @return Puntero a los datos de píxeles rotados.
 *
 * Función creada por mi compañero que rota los bits de cada componente de color n posiciones a la izquierda.
 * Implementa la rotación usando operadores de desplazamiento bit a bit.
 */
unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; ++i) {
        resultado[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return resultado;
}

/**
 * @brief Carga los píxeles de una imagen.
 * @param input Ruta del archivo de imagen.
 * @param width Variable para almacenar el ancho de la imagen.
 * @param height Variable para almacenar el alto de la imagen.
 * @return Puntero a los datos de píxeles cargados.
 *
 * Función proporcionada por los profesores para cargar imágenes.
 * La implementamos usando QImage para cargar la imagen y copiar sus datos de píxeles.
 */
unsigned char* loadPixels(const QString& input, int& width, int& height) {
    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen." << endl;
        return nullptr;
    }

    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
    height = imagen.height();
    int dataSize = width * height * 3;

    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        memcpy(pixelData + y * width * 3, imagen.scanLine(y), width * 3);
    }

    return pixelData;
}

/**
 * @brief Carga la semilla y datos de enmascaramiento desde un archivo.
 * @param nombreArchivo Ruta del archivo de entrada.
 * @param seed Variable para almacenar la semilla.
 * @param n_pixels Variable para almacenar el número de píxeles.
 * @return Puntero a los datos RGB cargados.
 *
 * Función proporcionada por los profesores para cargar datos de enmascaramiento.
 * La implementamos leyendo primero la semilla y luego los valores RGB de cada píxel.
 */
unsigned int* loadSeedMasking(const char* nombreArchivo, int& seed, int& n_pixels) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    archivo >> seed;
    n_pixels = 0;
    int r, g, b;

    while (archivo >> r >> g >> b) {
        n_pixels++;
    }

    archivo.close();
    archivo.open(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    unsigned int* RGB = new unsigned int[n_pixels * 3];
    archivo >> seed;

    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    archivo.close();
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de pixeles leidos: " << n_pixels << endl;

    return RGB;
}

/**
 * @brief Reconstruye la imagen original a partir de las etapas procesadas.
 * @param rutaBase Directorio base donde se encuentran los archivos.
 * @param etapas Número de etapas de procesamiento.
 *
 * Función principal que implementa el proceso inverso de reconstrucción:
 * 1. Carga la máscara y la imagen de la última etapa
 * 2. Para cada etapa (en orden inverso):
 *    a. Aplica XOR con la máscara
 *    b. Rota los bits a la izquierda (operación inversa a rotar derecha)
 * 3. Guarda resultados intermedios y la imagen final reconstruida
 */
void reconstruirImagen(const QString& rutaBase, int etapas) {
    int width, height;

    // Cargar mascara
    QString archivoMascara = rutaBase + "I_M.bmp";
    unsigned char* Mascara = loadPixels(archivoMascara, width, height);
    if (!Mascara) {
        cerr << "Error al cargar la mascara." << endl;
        return;
    }

    // Cargar imagen de la ultima etapa
    QString archivoImagen = rutaBase + "P" + QString::number(etapas + 1) + ".bmp";
    unsigned char* Imagen = loadPixels(archivoImagen, width, height);
    if (!Imagen) {
        cerr << "Error al cargar la imagen." << endl;
        delete[] Mascara;
        return;
    }

    // Proceso de reconstruccion inversa
    for (int etapa = etapas; etapa >= 1; --etapa) {
        cout << "Reconstruyendo etapa " << etapa << endl;

        // 1. Aplicar XOR con la mascara (usando función DoXOR de mi compañero)
        unsigned char* pasoXOR = DoXOR(Imagen, Mascara, width, height);

        // 2. Rotar bits a la izquierda (inverso de rotar derecha, usando función RotarIzquierda)
        int num_pixels = width * height;
        unsigned char* pasoRotado = RotarIzquierda(pasoXOR, num_pixels, 3);

        // Liberar memoria intermedia
        delete[] pasoXOR;
        delete[] Imagen;

        // Actualizar imagen para siguiente etapa
        Imagen = pasoRotado;

        // Guardar resultado intermedio (usando función exportImage proporcionada)
        QString nombreSalida = rutaBase + "P" + QString::number(etapa) + "_reconstruida.bmp";
        exportImage(Imagen, width, height, nombreSalida);
    }

    // Guardar imagen final reconstruida
    QString nombreFinal = rutaBase + "I_O_reconstruida.bmp";
    exportImage(Imagen, width, height, nombreFinal);

    // Liberar memoria
    delete[] Imagen;
    delete[] Mascara;
}

/**
 * @brief Función principal del programa.
 * @return 0 si la ejecución fue exitosa.
 */
int main() {
    QString rutaBase = "C:/Users/esteb/OneDrive/Escritorio/DES/codigo/Desafio1/Caso 2/";
    int etapas = 6;

    // Reconstruir la imagen
    reconstruirImagen(rutaBase, etapas);

    // Verificar archivos de mascara (usando loadSeedMasking proporcionada)
    int seed;
    int n_pixels = 0;
    for (int etapa = 1; etapa <= etapas; ++etapa) {
        QString archivoMascara = rutaBase + "M" + QString::number(etapa) + ".txt";
        unsigned int* maskingData = loadSeedMasking(archivoMascara.toStdString().c_str(), seed, n_pixels);

        if (maskingData) {
            cout << "Datos de mascara para etapa " << etapa << ":" << endl;
            for (int i = 0; i < 5 && i < n_pixels; ++i) {
                cout << "Pixel " << i << ": ("
                     << maskingData[i*3] << ", "
                     << maskingData[i*3+1] << ", "
                     << maskingData[i*3+2] << ")" << endl;
            }
            delete[] maskingData;
        }
    }

    return 0;
}
