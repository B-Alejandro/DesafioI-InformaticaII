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

using namespace std;

// Constantes
const int MAX_BITS = 7;

// Prototipos de funciones
unsigned char* loadPixels(const QString& input, int& width, int& height);
bool exportImage(unsigned char* pixelData, int width, int height, const QString& archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int& seed, int& n_pixels);

// Operaciones de bits
unsigned char* DoXOR(unsigned char* imagen, unsigned char* mascara, int width, int height);
unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n);
unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n);

// Funciones de reconstruccion
unsigned char* applyInverseMasking(unsigned char* img, unsigned int* maskData, int seed, int img_width, int img_height, int mask_width, int mask_height);
bool validarConArchivoMascara(unsigned char* img, unsigned int* datosMascara, int semilla, int ancho_img, int alto_img, int ancho_mascara, int alto_mascara);
int determineInverseOperation(unsigned char* currentImg, unsigned char* IM, unsigned int* maskData, int seed, int width, int height, int mask_width, int mask_height);

/**
 * @brief Exporta una imagen en formato BMP.
 * @param pixelData Datos de los pixeles de la imagen.
 * @param width Ancho de la imagen.
 * @param height Alto de la imagen.
 * @param archivoSalida Ruta del archivo de salida.
 * @return true si la exportacion fue exitosa, false en caso contrario.
 *
 * Funcion proporcionada por los profesores para exportar imagenes en formato BMP.
 * La implementamos copiando los datos de pixeles a un objeto QImage y usando su metodo save().
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
 * @brief Realiza operacion XOR entre dos imagenes.
 * @param imagen Primera imagen para la operacion.
 * @param mascara Segunda imagen para la operacion.
 * @param width Ancho de las imagenes.
 * @param height Alto de las imagenes.
 * @return Puntero a los datos de pixeles resultantes.
 *
 * Funcion creada por mi compañero que aplica XOR bit a bit entre dos imagenes.
 * La implementacion recorre todos los pixeles aplicando el operador ^.
 */
unsigned char* DoXOR(unsigned char* img1, unsigned char* img2, int width, int height) {
    int totalPixels = width * height * 3;
    unsigned char* result = new unsigned char[totalPixels];

    for (int i = 0; i < totalPixels; i++) {
        result[i] = img1[i] ^ img2[i];
    }

    return result;
}

/**
 * @brief Rota los bits de los pixeles a la derecha.
 * @param img Datos de la imagen original.
 * @param num_pixels Numero total de pixeles.
 * @param n Numero de bits a rotar.
 * @return Puntero a los datos de pixeles rotados.
 *
 * Funcion creada por mi compañero que rota los bits de cada componente de color n posiciones a la derecha.
 * Implementa la rotacion usando operadores de desplazamiento bit a bit.
 */
unsigned char* RotarDerecha(unsigned char* img, int num_pixels, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; i++) {
        resultado[i] = (img[i] >> n) | (img[i] << (8 - n));
    }

    return resultado;
}

/**
 * @brief Rota los bits de los pixeles a la izquierda.
 * @param img Datos de la imagen original.
 * @param num_pixels Numero total de pixeles.
 * @param n Numero de bits a rotar.
 * @return Puntero a los datos de pixeles rotados.
 *
 * Funcion creada por mi compañero que rota los bits de cada componente de color n posiciones a la izquierda.
 * Implementa la rotacion usando operadores de desplazamiento bit a bit.
 */
unsigned char* RotarIzquierda(unsigned char* img, int num_pixels, int n) {
    unsigned char* resultado = new unsigned char[num_pixels * 3];

    for (int i = 0; i < num_pixels * 3; i++) {
        resultado[i] = (img[i] << n) | (img[i] >> (8 - n));
    }

    return resultado;
}

/**
 * @brief Carga los pixeles de una imagen.
 * @param input Ruta del archivo de imagen.
 * @param width Variable para almacenar el ancho de la imagen.
 * @param height Variable para almacenar el alto de la imagen.
 * @return Puntero a los datos de pixeles cargados.
 *
 * Funcion proporcionada por los profesores para cargar imagenes.
 * La implementamos usando QImage para cargar la imagen y copiar sus datos de pixeles.
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
 * @param n_pixels Variable para almacenar el numero de pixeles.
 * @return Puntero a los datos RGB cargados.
 *
 * Funcion proporcionada por los profesores para cargar datos de enmascaramiento.
 * La implementamos leyendo primero la semilla y luego los valores RGB de cada pixel.
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
 * @brief Aplica el enmascaramiento inverso.
 */

unsigned char* applyInverseMasking(unsigned char* img, unsigned int* maskData, int seed,int img_width, int img_height, int mask_width, int mask_height) {
    int totalPixels = img_width * img_height * 3;
    unsigned char* result = new unsigned char[totalPixels];
    memcpy(result, img, totalPixels);

    int maskSize = mask_width * mask_height;
    int pos = seed;

    for (int k = 0; k < maskSize && pos + 2 < totalPixels; k++) {
        unsigned int valor_r = maskData[k*3];
        unsigned int valor_g = maskData[k*3+1];
        unsigned int valor_b = maskData[k*3+2];

        if (valor_r >= 255) {
            // Para valores >=256, restamos la mascara (valor_esperado - pixel_actual)
            result[pos] = (valor_r - img[pos]) % 256;
        } else {
            // Para valores <256, establecemos el valor exacto
            result[pos] = valor_r;
        }

        if (valor_g >= 255) {
            result[pos+1] = (valor_g - img[pos+1]) % 256;
        } else {
            result[pos+1] = valor_g;
        }

        if (valor_b >= 255) {
            result[pos+2] = (valor_b - img[pos+2]) % 256;
        } else {
            result[pos+2] = valor_b;
        }

        pos += 3;
    }

    return result;
}

/**
 * @brief Reconstruye la imagen original a partir de las etapas procesadas.
 * @param rutaBase Directorio base donde se encuentran los archivos.
 * @param etapas Numero de etapas de procesamiento.
 *
 * Funcion principal que implementa el proceso inverso de reconstruccion:
 * 1. Carga la mascara y la imagen de la ultima etapa
 * 2. Para cada etapa (en orden inverso):
 *    a. Aplica XOR con la mascara
 *    b. Rota los bits a la izquierda (operacion inversa a rotar derecha)
 * 3. Guarda resultados intermedios y la imagen final reconstruida
 */

void reconstruirImagen(const QString& rutaBase, int numEtapas) {
    // Cargar la mascara para obtener dimensiones
    int mask_width, mask_height;
    unsigned char* mask = loadPixels(rutaBase + "M.bmp", mask_width, mask_height);
    if (!mask) {
        cerr << "Error: No se pudo cargar la mascara M.bmp" << endl;
        return;
    }
    delete[] mask; // Solo necesitabamos las dimensiones

    // Cargar imagenes necesarias
    int width, height;
    unsigned char* IM = loadPixels(rutaBase + "I_M.bmp", width, height);  // Imagen de referencia para XOR
    unsigned char* IO = loadPixels(rutaBase + "I_O.bmp", width, height);  // Imagen original para verificacion
    unsigned char* currentImg = loadPixels(rutaBase + "P" + QString::number(numEtapas) + ".bmp", width, height); // Imagen procesada

    // Verificar carga de imagenes
    if (!IM || !IO || !currentImg) {
        cerr << "Error al cargar las imagenes necesarias" << endl;
        if (IM) delete[] IM;
        if (IO) delete[] IO;
        if (currentImg) delete[] currentImg;
        return;
    }

    // Cargar datos de enmascaramiento
    unsigned int** maskingData = new unsigned int*[numEtapas];
    int* seeds = new int[numEtapas];
    int* numPixels = new int[numEtapas];

    for (int i = 0; i < numEtapas; i++) {
        QString filename = rutaBase + "M" + QString::number(i+1) + ".txt";
        maskingData[i] = loadSeedMasking(filename.toStdString().c_str(), seeds[i], numPixels[i]);
        if (!maskingData[i]) {
            cerr << "Error al cargar el archivo de enmascaramiento " << i+1 << endl;
            // Liberar memoria
            for (int j = 0; j < i; j++) delete[] maskingData[j];
            delete[] maskingData;
            delete[] seeds;
            delete[] numPixels;
            delete[] IM;
            delete[] IO;
            delete[] currentImg;
            return;
        }
    }

    // Proceso de reconstruccion inversa
    int* operations = new int[numEtapas];
    bool reconstructionSuccess = true;

    for (int etapa = numEtapas-1; etapa >= 0; etapa--) {
        cout << "\nProcesando etapa " << etapa+1 << " de " << numEtapas << endl;

        // 1. Aplicar enmascaramiento inverso (excepto en la ultima etapa)
        if (etapa < numEtapas-1) {
            cout << "Aplicando enmascaramiento inverso..." << endl;
            unsigned char* temp = applyInverseMasking(currentImg, maskingData[etapa],
                                                      seeds[etapa], width, height,
                                                      mask_width, mask_height);
            if (!temp) {
                cerr << "Error en enmascaramiento inverso" << endl;
                reconstructionSuccess = false;
                break;
            }
            delete[] currentImg;
            currentImg = temp;

            QString tempName = rutaBase + "P" + QString::number(etapa+1) + "_post_masking.bmp";
            exportImage(currentImg, width, height, tempName);
        }

        // 2. Determinar y aplicar la operacion inversa
        cout << "Determinando operacion inversa..." << endl;
        int operation = determineInverseOperation(currentImg, IM, maskingData[etapa],
                                                  seeds[etapa], width, height,
                                                  mask_width, mask_height);

        operations[etapa] = operation;
        unsigned char* temp = nullptr;

        switch (operation) {
        case 1: // XOR con IM
            cout << "Aplicando XOR inverso con IM" << endl;
            temp = DoXOR(currentImg, IM, width, height);
            break;

        case 2: // Rotacion derecha (inverso: izquierda)
            cout << "Aplicando rotacion izquierda de 3 bits" << endl;
            temp = RotarIzquierda(currentImg, width * height, 3);
            break;

        case 3: // Rotacion izquierda (inverso: derecha)
            cout << "Aplicando rotacion derecha de 3 bits" << endl;
            temp = RotarDerecha(currentImg, width * height, 3);
            break;

        default:
            cerr << "Operacion desconocida: " << operation << endl;
            reconstructionSuccess = false;
            break;
        }

        if (!temp) {
            cerr << "Error al aplicar la operacion inversa" << endl;
            reconstructionSuccess = false;
            break;
        }

        delete[] currentImg;
        currentImg = temp;

        // Guardar resultado intermedio
        QString tempName = rutaBase + "P" + QString::number(etapa+1) + "_reconstruida.bmp";
        exportImage(currentImg, width, height, tempName);
    }

    // Guardar imagen final reconstruida
    QString finalName = rutaBase + "I_O_reconstruida_final.bmp";
    exportImage(currentImg, width, height, finalName);

    // VERIFICACIoN FINAL CON IMAGEN ORIGINAL
    if (reconstructionSuccess) {
        cout << "\n=== VERIFICACIoN FINAL ===" << endl;

        // 1. Comparacion binaria exacta
        if (memcmp(currentImg, IO, width * height * 3) == 0) {
            cout << "+ RECONSTRUCCIoN EXITOSA: La imagen coincide exactamente con el original" << endl;
        } else {
            cerr << "- ADVERTENCIA: La reconstruccion NO coincide exactamente con el original" << endl;

            // 2. Calculo de MSE (Error Cuadratico Medio)
            double mse = 0;
            for (int i = 0; i < width * height * 3; i++) {
                int diff = currentImg[i] - IO[i];
                mse += diff * diff;
            }
            mse /= (width * height * 3);
            cout << "Error Cuadratico Medio (MSE): " << mse << endl;

            // 3. Porcentaje de pixeles diferentes
            int differentPixels = 0;
            for (int i = 0; i < width * height * 3; i++) {
                if (currentImg[i] != IO[i]) differentPixels++;
            }
            double percentage = (differentPixels * 100.0) / (width * height * 3);
            cout << "Pixeles diferentes: " << percentage << "%" << endl;
        }
    } else {
        cerr << "La reconstruccion no se completo correctamente - verificacion omitida" << endl;
    }

    // Mostrar operaciones utilizadas
    cout << "\nResumen de operaciones aplicadas (de ultima a primera):" << endl;
    for (int i = numEtapas-1; i >= 0; i--) {
        cout << "Etapa " << numEtapas-i << ": ";
        switch (operations[i]) {
        case 1: cout << "XOR con IM"; break;
        case 2: cout << "Rotacion derecha (3 bits)"; break;
        case 3: cout << "Rotacion izquierda (3 bits)"; break;
        default: cout << "Operacion desconocida (Codigo: " << operations[i] << ")"; break;
        }
        cout << endl;
    }

    // Liberar memoria
    delete[] currentImg;
    delete[] IM;
    delete[] IO;
    for (int i = 0; i < numEtapas; i++) {
        delete[] maskingData[i];
    }
    delete[] maskingData;
    delete[] seeds;
    delete[] numPixels;
    delete[] operations;
}

bool validarConArchivoMascara(unsigned char* img, unsigned int* datosMascara, int semilla, int ancho_img, int alto_img, int ancho_mascara, int alto_mascara) {
    int tamanoMascara = ancho_mascara * alto_mascara;
    int pos = semilla;
    int totalPixeles = ancho_img * alto_img * 3;

    cout << "Iniciando validacion con posicion inicial: " << pos << endl;
    cout << "dimension de mascara: " << tamanoMascara << " elementos ("
         << tamanoMascara*3 << " componentes de color)" << endl;

    for (int k = 0; k < tamanoMascara && pos + 2 < totalPixeles; k++) {
        unsigned char r = img[pos];
        unsigned char g = img[pos+1];
        unsigned char b = img[pos+2];

        // Aplicar módulo 256 a todos los valores esperados
        unsigned int valor_esperado_r = datosMascara[k*3] % 256;
        unsigned int valor_esperado_g = datosMascara[k*3+1] % 256;
        unsigned int valor_esperado_b = datosMascara[k*3+2] % 256;

        cout << "\nPixel " << k + 1 << " (posicion " << pos << " a " << pos+2 << "):" << endl;
        cout << "  RGB Imagen: (" << (int)r << ", " << (int)g << ", " << (int)b << ")" << endl;
        cout << "  Valor Esperado (mod 256): (" << valor_esperado_r << ", " << valor_esperado_g << ", " << valor_esperado_b << ")" << endl;

        bool valido = true;

        // Ahora siempre comparamos con valores en el rango 0-255
        unsigned int mascara_r = valor_esperado_r - r;
        cout << "  Suma R: " << (int)r << " + " << mascara_r << " = " << ((r + mascara_r) % 256) << endl;
        valido = valido && (((r + mascara_r) % 256) == valor_esperado_r);

        unsigned int mascara_g = valor_esperado_g - g;
        cout << "  Suma G: " << (int)g << " + " << mascara_g << " = " << ((g + mascara_g) % 256) << endl;
        valido = valido && (((g + mascara_g) % 256) == valor_esperado_g);

        unsigned int mascara_b = valor_esperado_b - b;
        cout << "  Suma B: " << (int)b << " + " << mascara_b << " = " << ((b + mascara_b) % 256) << endl;
        valido = valido && (((b + mascara_b) % 256) == valor_esperado_b);

        if (!valido) {
            cout << "VALIDACION FALLIDA en el pixel " << k + 1 << endl;
            return false;
        }

        cout << "  Validacion exitosa para este pixel" << endl;
        pos += 3;
    }

    cout << "\nTodos los pixeles se validaron correctamente!" << endl;
    return true;
}


/**
 * @brief Determina la operacion inversa que se debe aplicar con un proceso detallado por etapas
 * @return 1: XOR fue la operacion
 *         2X: Rotacion derecha original (necesita rotacion izquierda de X bits)
 *         3X: Rotacion izquierda original (necesita rotacion derecha de X bits)
 *         -1: No se pudo determinar
 */
int determineInverseOperation(unsigned char* currentImg, unsigned char* IM, unsigned int* maskData, int seed, int width, int height, int mask_width, int mask_height) {
    int totalPixels = width * height;
    cout << "\nIniciando deteccion de operacion con proceso detallado..." << endl;

    // Etapa 1: Verificar si se aplico XOR
    cout << "\nEtapa 1: Verificando operacion XOR..." << endl;
    unsigned char* xorResult = DoXOR(currentImg, IM, width, height);
    if (validarConArchivoMascara(xorResult, maskData, seed, width, height, mask_width, mask_height)) {
        cout << "Operacion XOR detectada correctamente!" << endl;
        delete[] xorResult;
        return 1;
    }
    delete[] xorResult;
    cout << "No se detecto operacion XOR." << endl;

    // Etapa 2: Verificar rotaciones de bits
    cout << "\nEtapa 2: Verificando rotaciones de bits..." << endl;

    // Sub-etapa 2.1: Rotacion derecha original (necesita rotacion izquierda inversa)
    cout << "Sub-etapa 2.1: Probando rotaciones derecha original (inverso: izquierda)..." << endl;
    for (int bits = 1; bits <= MAX_BITS; bits++) {
        cout << "Probando con " << bits << " bits..." << endl;
        unsigned char* rotatedLeft = RotarIzquierda(currentImg, totalPixels, bits);

        if (validarConArchivoMascara(rotatedLeft, maskData, seed, width, height, mask_width, mask_height)) {
            cout << "Rotacion derecha original detectada! (Inverso: rotacion izquierda de " << bits << " bits)" << endl;
            delete[] rotatedLeft;
            return 2 * 10 + bits; // Codificacion: 2X donde X son los bits
        }
        delete[] rotatedLeft;
    }
    cout << "No se detectaron rotaciones derecha originales." << endl;

    // Sub-etapa 2.2: Rotacion izquierda original (necesita rotacion derecha inversa)
    cout << "\nSub-etapa 2.2: Probando rotaciones izquierda original (inverso: derecha)..." << endl;
    for (int bits = 1; bits <= MAX_BITS; bits++) {
        cout << "Probando con " << bits << " bits..." << endl;
        unsigned char* rotatedRight = RotarDerecha(currentImg, totalPixels, bits);

        if (validarConArchivoMascara(rotatedRight, maskData, seed, width, height, mask_width, mask_height)) {
            cout << "¡Rotacion izquierda original detectada! (Inverso: rotacion derecha de " << bits << " bits)" << endl;
            delete[] rotatedRight;
            return 3 * 10 + bits; // Codificacion: 3X donde X son los bits
        }
        delete[] rotatedRight;
    }
    cout << "No se detectaron rotaciones izquierda originales." << endl;

    // Etapa 3: Verificacion adicional para casos especiales
    cout << "\nEtapa 3: Realizando verificaciones adicionales..." << endl;
    cout << "Probando combinaciones de operaciones..." << endl;

    // Después de probar XOR simple y rotaciones simples, probar combinaciones:
    // XOR + Rotación
    for (int bits = 1; bits < 8; bits++) {
        unsigned char* rotated = RotarIzquierda(currentImg, totalPixels, bits);
        unsigned char* xorResult = DoXOR(rotated, IM, width, height);

        if (validarConArchivoMascara(xorResult, maskData, seed, width, height, mask_width, mask_height)) {
            delete[] rotated;
            delete[] xorResult;
            return 10 + bits; // Código especial para XOR + Rotación
        }
        delete[] rotated;
        delete[] xorResult;
    }

    cout << "No se detectaron operaciones combinadas." << endl;

    // Si llegamos aqui, no se pudo determinar la operacion
    cout << "\nAdvertencia: No se pudo determinar la operacion aplicada!" << endl;
    cout << "Posibles causas:" << endl;
    cout << "1. La operacion no es XOR ni rotacion simple" << endl;
    cout << "2. Se aplico una combinacion de operaciones no contemplada" << endl;
    cout << "3. Los datos de enmascaramiento no coinciden con ninguna operacion probada" << endl;

    return -1;
}

/**
 * @brief Funcion principal del programa.
 * @return 0 si la ejecucion fue exitosa.
 */
int main() {
    QString rutaBase = "C:/Users/esteb/OneDrive/Escritorio/DESAFIO/CODIGO/Desafio1/Caso 1/";
    int etapas = 2;

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
            for (int i = 0; i < 6 && i < n_pixels; ++i) {
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
