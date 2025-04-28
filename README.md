# 📸 Proyecto de Enmascaramiento y Restauración de Imágenes BMP

![Estado del Proyecto](https://img.shields.io/badge/ESTADO-EN%20Validaciones-green?style=flat-square)


**Autores**: Alejandro Bedoya Zuluaga y Esteban Rodríguez Monsalve

---

## ✨ Descripción General

Este proyecto aborda la manipulación y desenmascaramiento de imágenes BMP mediante operaciones de bajo nivel en memoria. Utilizando el framework Qt y programación en C++, se implementó un sistema capaz de aplicar operaciones sobre los componentes RGB directamente a nivel de bits.

La solución propuesta se basa en el uso de punteros dinámicos para la gestión de memoria, evitando estructuras de datos complejas como listas, pilas o colas. Esta estrategia permitió un manejo flexible, eficiente y directo de grandes volúmenes de datos, accediendo y modificando la información sin necesidad de copias intermedias.

---

## ⚙️ Estrategias y Técnicas Utilizadas

### 1. 🔗 Manejo de Memoria Dinámica

- Uso de arreglos dinámicos de tipo `char` para almacenar la información de las imágenes.
- Acceso y modificación directa de componentes RGB bit a bit.

> 💡 Cada `char` ocupa 8 bits, lo que facilita las operaciones a nivel de bit.

---

### 2. 🔐 Operaciones de Enmascaramiento

Se implementaron diversas operaciones para modificar y restaurar las imágenes:

- **XOR** entre la imagen y una máscara.
- **Rotaciones** de bits para dispersar información.
- **Sumas y desplazamientos** de bits para alterar el patrón de los datos.

```cpp
pixeles[i] = imagen[i] ^ mascara[i];
resultado[i] = (byte >> n) | (byte << (8 - n));
```

---

### 3. 🔎 Verificación y Flujo del Programa

- Se desarrollaron funciones de verificación automática para identificar las operaciones utilizadas en cada etapa de enmascaramiento.
- Comparación de resultados intermedios contra archivos de referencia.
- Aplicación de las funciones de restauración en el orden correcto hasta desenmascarar por completo la imagen.

---

## 📦 Resultados Obtenidos

✅ El proyecto permitió:

- Un tratamiento eficaz de grandes volúmenes de datos de imagen.
- La correcta identificación de las operaciones de enmascaramiento aplicadas.
- Una restauración satisfactoria de imágenes encriptadas.
- La optimización del rendimiento gracias al manejo manual de memoria.

---

---

## 🧠 Conclusión
Este fortaleció la comprensión de los procesos internos de gestión de datos en sistemas informáticos. La experiencia adquirida será una base sólida para futuros desarrollos en cifrado y tratamiento de imágenes a bajo nivel.
