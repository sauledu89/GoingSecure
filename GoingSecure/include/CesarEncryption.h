#pragma once
#include "Prerequisites.h"

/**
 * @class CesarEncryption
 * @brief Implementa el cifrado César para codificación y decodificación de textos.
 *
 * Esta clase proporciona una implementación simple del algoritmo de cifrado César,
 * utilizado históricamente como una técnica de sustitución monoalfabética.
 * También se incluyen herramientas educativas como fuerza bruta y análisis de frecuencia.
 *
 * @note En desarrollo de videojuegos, el cifrado César puede emplearse en:
 * - Puzzles narrativos (mensajes ocultos, códigos encriptados).
 * - Ofuscación de diálogos o nombres de archivos.
 * - Introducción a conceptos de seguridad y lógica criptográfica en gameplay.
 */
class CesarEncryption {
public:
    /**
     * @brief Constructor por defecto de la clase CesarEncryption.
     */
    CesarEncryption() = default;

    /**
     * @brief Destructor por defecto de la clase CesarEncryption.
     */
    ~CesarEncryption() = default;

    /**
     * @brief Codifica un texto usando el cifrado César.
     *
     * Desplaza letras y dígitos en el alfabeto según el valor proporcionado.
     * Caracteres no alfanuméricos se mantienen sin cambios.
     *
     * @param texto Texto plano a codificar.
     * @param desplazamiento Cantidad de posiciones a desplazar.
     * @return std::string Texto cifrado resultante.
     *
     * @note Método ideal para introducir lógica criptográfica básica en juegos.
     */
    std::string encode(const std::string& texto, int desplazamiento) {
        std::string result = "";

        for (char c : texto) {
            if (c >= 'A' && c <= 'Z') {
                result += (char)(((c - 'A' + desplazamiento) % 26) + 'A');
            }
            else if (c >= 'a' && c <= 'z') {
                result += (char)(((c - 'a' + desplazamiento) % 26) + 'a');
            }
            else if (c >= '0' && c <= '9') {
                result += (char)(((c - '0' + desplazamiento) % 10) + '0');
            }
            else {
                result += c;
            }
        }

        return result;
    }

    /**
     * @brief Decodifica un texto cifrado usando el desplazamiento inverso.
     *
     * Este método invierte la codificación realizada por `encode` para recuperar el mensaje original.
     *
     * @param texto Texto cifrado a decodificar.
     * @param desplazamiento Valor original usado en el cifrado.
     * @return std::string Texto descifrado.
     */
    std::string decode(const std::string& texto, int desplazamiento) {
        return encode(texto, 26 - (desplazamiento % 26));
    }

    /**
     * @brief Realiza un ataque por fuerza bruta al texto cifrado.
     *
     * Prueba todas las posibles claves (0 a 25) e imprime cada descifrado
     * posible en consola para permitir al usuario identificar el mensaje original.
     *
     * @param texto Texto cifrado a analizar.
     *
     * @note Útil como herramienta educativa para demostrar la debilidad de cifrados de sustitución.
     */
    void bruteForceAttack(const std::string& texto) {
        std::cout << "\nIntentos de descifrado por fuerza bruta:\n";
        for (int clave = 0; clave < 26; clave++) {
            std::string intento = encode(texto, 26 - clave);
            std::cout << "Clave " << clave << ": " << intento << std::endl;
        }
    }

    /**
     * @brief Estima la clave más probable mediante análisis de frecuencia.
     *
     * Evalúa cuál letra del alfabeto aparece más frecuentemente en el texto cifrado,
     * y la compara con las letras más comunes del idioma español para calcular posibles claves.
     *
     * @param texto Texto cifrado.
     * @return int Clave sugerida más probable.
     *
     * @note Esta técnica se conoce como análisis de frecuencia, y es un método clásico
     * para romper cifrados débiles en criptografía y acertijos en videojuegos.
     */
    int evaluatePossibleKey(const std::string& texto) {
        int frecuencias[26] = { 0 };

        for (char c : texto) {
            if (c >= 'a' && c <= 'z') {
                frecuencias[c - 'a']++;
            }
            else if (c >= 'A' && c <= 'Z') {
                frecuencias[c - 'A']++;
            }
        }

        const char letrasEsp[] = { 'e', 'a', 'o', 's', 'r', 'n',
                                   'i', 'd', 'l', 'c' };

        int indiceMax = 0;
        for (int i = 1; i < 26; ++i) {
            if (frecuencias[i] > frecuencias[indiceMax]) {
                indiceMax = i;
            }
        }

        int mejorClave = 0;
        int mejorPuntaje = -1;

        for (char letraRef : letrasEsp) {
            int clave = (indiceMax - (letraRef - 'a') + 26) % 26;
            int puntaje = 0;

            std::string descifrado = encode(texto, 26 - clave);

            std::string comunes[] = { "el", "de", "la", "que", "en",
                                      "y", "los", "se" };

            for (std::string palabra : comunes) {
                if (descifrado.find(palabra) != std::string::npos) {
                    puntaje++;
                }
            }

            if (puntaje > mejorPuntaje) {
                mejorPuntaje = puntaje;
                mejorClave = clave;
            }
        }

        return mejorClave;
    }

private:
    // No se requiere almacenamiento persistente en esta clase.
};
