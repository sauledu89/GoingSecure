#pragma once
#include "Prerequisites.h"

/**
 * @class AsciiBinary
 * @brief Conversión entre texto ASCII y su representación binaria.
 *
 * Esta clase ofrece herramientas para convertir texto en formato ASCII
 * a su forma binaria (representada como secuencia de bits) y viceversa.
 *
 * @note En videojuegos, estos métodos son útiles para:
 * - Visualizar contenido codificado (puzzles, hacking ficticio).
 * - Comprender cómo se almacenan caracteres y datos.
 * - Aplicaciones educativas relacionadas con redes o almacenamiento binario.
 */
class AsciiBinary {
public:
    /**
     * @brief Constructor por defecto.
     */
    AsciiBinary() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~AsciiBinary() = default;

    /**
     * @brief Convierte un carácter a una cadena binaria de 8 bits.
     *
     * Usa aritmética binaria para calcular cada bit y formatear
     * el resultado como cadena de texto compuesta por '0' y '1'.
     *
     * @param c Carácter ASCII a convertir.
     * @return std::string Cadena binaria de 8 caracteres.
     *
     * @note Ideal para mostrar visualmente cómo se representa un carácter
     * en memoria, útil en herramientas tipo debugger o consola de depuración.
     */
    std::string bitset(char c) {
        std::string result(8, '0');
        int value = static_cast<unsigned char>(c);
        for (int i = 7; i >= 0; --i) {
            result[i] = (value % 2) + '0';
            value /= 2;
        }
        return result;
    }

    /**
     * @brief Convierte una cadena ASCII a su representación binaria.
     *
     * Aplica la conversión binaria carácter por carácter utilizando el
     * método `bitset()` y concatena los resultados separados por espacios.
     *
     * @param input Cadena ASCII de entrada.
     * @return std::string Texto binario con grupos de 8 bits separados por espacios.
     *
     * @note Puede utilizarse para mostrar cómo se codifican mensajes en protocolos de red o archivos binarios.
     */
    std::string stringToBinary(const std::string& input) {
        std::ostringstream oss;
        for (char c : input) {
            oss << bitset(c) << " ";
        }

        std::string output = oss.str();
        if (!output.empty()) {
            output.pop_back();  // Eliminar espacio final
        }

        return output;
    }

    /**
     * @brief Convierte una cadena binaria de 8 bits en un carácter ASCII.
     *
     * Interpreta el valor binario como un número entero y lo convierte
     * al carácter correspondiente.
     *
     * @param binary Cadena de 8 caracteres ('0' o '1').
     * @return char Carácter ASCII correspondiente.
     *
     * @warning No valida que la entrada tenga exactamente 8 bits.
     */
    char binaryToChar(const std::string& binary) {
        int value = 0;
        for (char bit : binary) {
            value = value * 2 + (bit - '0');
        }

        return static_cast<char>(value);
    }

    /**
     * @brief Convierte una secuencia binaria a texto ASCII.
     *
     * Divide la entrada en fragmentos de 8 bits, convierte cada uno
     * en carácter ASCII, y los concatena para reconstruir el texto original.
     *
     * @param binaryInput Cadena de bits separados por espacio.
     * @return std::string Texto decodificado en ASCII.
     *
     * @note Este método puede utilizarse para simular decodificadores en minijuegos,
     * terminales en interfaces tipo sci-fi o para enseñar codificación binaria.
     */
    std::string binaryToString(const std::string& binaryInput) {
        std::istringstream iss(binaryInput);
        std::string result;
        std::string binary;

        while (iss >> binary) {
            result += binaryToChar(binary);
        }

        return result;
    }

private:
    // Esta clase no requiere atributos privados persistentes.
};
