# DesafioI-InformaticaII
# Reconstrucción y Manipulación de Imágenes BMP usando Qt

Este proyecto en C++ con Qt permite cargar, modificar y reconstruir imágenes en formato BMP, aplicando transformaciones bit a bit y verificando datos contra archivos de enmascaramiento.

## Características
- Carga de imágenes BMP y extracción de píxeles RGB.
- Exportación de imágenes modificadas en formato BMP.
- Operaciones bit a bit:
  - XOR entre imagen y máscara.
  - Rotación de bits a izquierda/derecha.
- Lectura de archivos de máscaras (con semilla y valores RGB).
- Proceso inverso para reconstruir imágenes a partir de transformaciones.

## Archivos
- `main.cpp`: archivo principal que ejecuta la reconstrucción completa.
- `loadPixels()`: carga una imagen BMP y devuelve sus datos RGB.
- `exportImage()`: exporta datos RGB a una imagen BMP.
- `loadSeedMasking()`: carga datos de un archivo de máscara (semilla + RGB).
- `DoXOR()`, `RotarDerecha()`, `RotarIzquierda()`: funciones de manipulación de bits.

## Ejecución
Modificar la variable `rutaBase` en `main()` para que apunte a la carpeta donde están almacenados:
- Las imágenes `I_O.bmp`, `I_M.bmp`, `P1.bmp`, ..., `P(n).bmp`.
- Los archivos de máscara `M1.txt`, ..., `Mn.txt`.

Por ejemplo:
```cpp
QString rutaBase = "C:/Users/esteb/OneDrive/Escritorio/DES/codigo/Desafio1/Caso 2/";
```

Luego compilar el proyecto con Qt y ejecutar para reconstruir la imagen original y verificar las máscaras.
Modificar la variable `rutaBase` en `main()` para que apunte a la carpeta donde están almacenados:
- Las imágenes `I_O.bmp`, `I_M.bmp`, `P1.bmp`, ..., `P(n).bmp`.
- Los archivos de máscara `M1.txt`, ..., `Mn.txt`.

Luego compilar el proyecto con Qt y ejecutar para reconstruir la imagen original y verificar las máscaras.

## Requisitos
- Qt 5 o superior
- Compilador C++ compatible con Qt

## Comentario del Código
Este código fue desarrollado en parte por los profesores, quienes proporcionaron las funciones `loadPixels()`, `exportImage()` y `loadSeedMasking()` como parte del material académico del curso. Las funciones `DoXOR()`, `RotarDerecha()` y `RotarIzquierda()` fueron desarrolladas por mi compañero como parte de la solución al desafío. Mi labor consistió en integrar todas estas funciones, implementar la lógica del proceso de reconstrucción de imágenes paso a paso y organizar el flujo general del programa. como parte del material académico del curso. Incluye operaciones de procesamiento de imágenes, manipulación binaria, lectura de archivos y uso de la biblioteca Qt para facilitar el manejo de imágenes BMP.

## Ejemplo de Salida
Al ejecutar el programa, se mostrarán mensajes como:
```
Reconstruyendo etapa 6
Reconstruyendo etapa 5
...
Semilla: 12345
Cantidad de pixeles leidos: 1500
Pixel 0: (23, 45, 67)
...
```

## Autoría
> © Proyecto desarrollado por los profesores para fines educativos. Algunas funciones como `loadPixels()`, `exportImage()` y `loadSeedMasking()` fueron provistas por los docentes del curso. Las funciones `DoXOR()`, `RotarDerecha()` y `RotarIzquierda()` fueron desarrolladas por mi compañero, y yo me encargué de integrarlas en el flujo general del programa, implementar la reconstrucción paso a paso y asegurar su funcionamiento en conjunto. para fines educativos.



