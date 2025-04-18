
# 🔐 Encriptación de Imágenes BMP 
![Estado del Proyecto](https://img.shields.io/badge/ESTADO-EN%20DESARROLLO-yellow?style=flat-square)

Proyecto de encriptación y desencriptación de imágenes BMP mediante operaciones a nivel de bit como XOR, rotaciones y desplazamientos. Esta es la primera versión del algoritmo y representa un acercamiento inicial al sistema de cifrado por enmascaramiento.

---

## ✨ Objetivos de la Actualización

Esta actualización introduce operaciones bit a bit para el procesamiento de imágenes, específicamente:

- Aplicación de XOR entre imágenes y máscaras.
- Rotación de bytes.
- Enmascaramiento de píxeles como método de cifrado.
- Implementación de un flujo básico de codificación/decodificación para el **Caso 1**.

---

## ⚙️ Procesos de Encriptación

### 1. 🔁 Operación XOR

Se realiza una operación **bitwise XOR** entre los píxeles de la imagen original y una máscara (`I_M`). Esta operación transforma los datos de manera reversible.

```cpp
pixeles[i] = imagen[i] ^ mascara[i];
```

> ⚡ Utilizada tanto para cifrar como para descifrar.

---

### 2. 🔄 Rotación de Bytes

Se aplica una rotación circular a cada byte de color (R, G, B). Esto se logra con desplazamientos hacia la derecha o izquierda seguidos de una operación OR, lo cual permite:

- Llevar bits de un extremo al otro.
- Aumentar la dispersión de datos para el cifrado.

```cpp
resultado[i] = (byte >> n) | (byte << (8 - n));
```

---

### 3. ⬅️➡️ Desplazamientos

Desplazamientos simples a nivel de bit se emplean para modificar la estructura binaria de cada byte. Esta operación refuerza el efecto de los enmascaramientos al alterar la posición de los bits.

---

## 📦 Resultados Obtenidos

✅ Los primeros resultados muestran que:

- Las operaciones bit a bit son altamente efectivas para modificar la imagen de forma visible.
- El uso combinado de XOR y rotación logra una buena dispersión de datos.
- Es posible revertir el proceso siempre que se conserven los mismos parámetros y máscaras.

---

## 🧪 Próximos Pasos
-
- Incluir nuevas máscaras dinámicas.
- Implementar algoritmos para calcular enmascaramiento imagen M.
- Automatizar el análisis de similitud entre imagen original y cifrada.

---

## 🧠 Conclusión

Este primer acercamiento nos permite comprender mejor cómo operan los algoritmos de cifrado visual y sienta las bases para desarrollar sistemas más complejos y seguros en futuras versiones.

---
