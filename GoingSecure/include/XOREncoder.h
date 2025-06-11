#pragma once
#include "Prerequisites.h"

/**
 * @class XOREncoder
 * @brief Clase que implementa un sistema de cifrado XOR y ataques por fuerza bruta.
 *
 * Esta clase permite codificar y decodificar textos utilizando el operador XOR,
 * convertir texto a formato hexadecimal, validar la legibilidad del resultado
 * y realizar ataques por fuerza bruta para recuperar claves de cifrado simples.
 *
 * @note En el desarrollo de videojuegos, el cifrado XOR puede aplicarse a la
 * protección de archivos de guardado, mensajes entre cliente y servidor, y
 * control básico de trampas. Aunque simple, debe usarse con precauciones.
 */
class XOREncoder {
public:

    /**
     * @brief Constructor por defecto de XOREncoder.
     */
    XOREncoder() = default;

    /**
     * @brief Destructor por defecto de XOREncoder.
     */
    ~XOREncoder() = default;

    /**
     * @brief Codifica un texto aplicando XOR con una clave dada.
     *
     * Aplica el operador XOR entre cada carácter del texto original
     * y el correspondiente carácter de la clave, repitiendo la clave
     * si es necesario para cubrir toda la longitud del texto.
     *
     * @param input Texto original a codificar.
     * @param key Clave de cifrado.
     * @return std::string Texto cifrado.
     *
     * @note Este tipo de cifrado se puede usar como capa ligera de
     * ofuscación en juegos, aunque no garantiza seguridad real.
     */
    std::string encode(const std::string& input, const std::string& key) {
        std::string output = input;
        for (int i = 0; i < input.size(); i++) {
            output[i] = input[i] ^ key[i % key.size()];
        }
        return output;
    }

    /**
     * @brief Convierte una cadena hexadecimal a un vector de bytes.
     *
     * Interpreta cada valor hexadecimal (en texto) como un byte y
     * construye un vector con los valores binarios correspondientes.
     *
     * @param input Cadena con valores hexadecimales separados por espacio.
     * @return std::vector<unsigned char> Vector de bytes resultantes.
     */
    std::vector<unsigned char> HexToBytes(const std::string& input) {
        std::vector<unsigned char> bytes;
        std::istringstream iss(input);
        std::string hexValue;

        while (iss >> hexValue) {
            if (hexValue.size() == 1) {
                hexValue = "0" + hexValue;
            }
            unsigned int byte;
            std::stringstream ss;
            ss << std::hex << hexValue;
            ss >> byte;
            bytes.push_back(static_cast<unsigned char>(byte));
        }

        return bytes;
    }

    /**
     * @brief Imprime el contenido de una cadena en formato hexadecimal.
     *
     * Cada carácter se representa como dos dígitos hexadecimales, con ceros
     * a la izquierda para asegurar longitud uniforme.
     *
     * @param input Texto a imprimir en hexadecimal.
     *
     * @note Este método es útil para visualizar datos cifrados u ofuscados.
     */
    void printHex(const std::string& input) {
        for (unsigned char c : input) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (int)c << " ";
        }
    }

    /**
     * @brief Verifica si un texto contiene únicamente caracteres legibles.
     *
     * Considera válido cualquier carácter imprimible, espacio o salto de línea.
     *
     * @param data Texto a evaluar.
     * @return true Si todo el contenido es legible.
     * @return false Si hay caracteres no imprimibles.
     */
    bool isValidText(const std::string& data) {
        return std::all_of(data.begin(), data.end(), [](unsigned char c) {
            return std::isprint(c) || std::isspace(c) || c == '\n' || c == ' ';
            });
    }

    /**
     * @brief Realiza ataque de fuerza bruta con claves de 1 byte.
     *
     * Intenta todas las combinaciones posibles de claves simples (256 valores)
     * y muestra solo los resultados considerados legibles.
     *
     * @param cifrado Vector de bytes cifrados.
     *
     * @note Esta técnica revela cuán débil es usar XOR sin una clave fuerte.
     * Ideal para ejercicios de concienciación sobre seguridad en videojuegos.
     */
    void bruteForce_1Byte(const std::vector<unsigned char>& cifrado) {
        for (int clave = 0; clave < 256; ++clave) {
            std::string result;
            for (unsigned char c : cifrado) {
                result += static_cast<unsigned char>(c ^ clave);
            }

            if (isValidText(result)) {
                std::cout << "=============================\n";
                std::cout << "Clave 1 byte  : '" << static_cast<char>(clave)
                    << "' (0x" << std::hex << std::setw(2)
                    << std::setfill('0') << clave << ")\n";
                std::cout << "Texto posible : " << result << "\n";
            }
        }
    }

    /**
     * @brief Realiza ataque de fuerza bruta con claves de 2 bytes.
     *
     * Prueba todas las combinaciones posibles de dos bytes (65536 claves)
     * y muestra las que generan resultados legibles.
     *
     * @param cifrado Vector de bytes cifrados.
     */
    void bruteForce_2Byte(const std::vector<unsigned char>& cifrado) {
        for (int b1 = 0; b1 < 256; ++b1) {
            for (int b2 = 0; b2 < 256; ++b2) {
                std::string result;
                unsigned char key[2] = {
                  static_cast<unsigned char>(b1),
                  static_cast<unsigned char>(b2)
                };

                for (int i = 0; i < cifrado.size(); i++) {
                    result += cifrado[i] ^ key[i % 2];
                }

                if (isValidText(result)) {
                    std::cout << "=============================\n";
                    std::cout << "Clave 2 bytes : '" << static_cast<char>(b1)
                        << static_cast<char>(b2) << "' (0x"
                        << std::hex << std::setw(2) << std::setfill('0') << b1
                        << " 0x" << std::setw(2) << std::setfill('0') << b2
                        << ")\n";
                    std::cout << "Texto posible : " << result << "\n";
                }
            }
        }
    }

    /**
     * @brief Realiza ataque por diccionario con claves comunes conocidas.
     *
     * Usa una lista de claves débiles o populares para intentar decodificar
     * el texto cifrado. Se muestran los resultados válidos.
     *
     * @param cifrado Vector de bytes cifrados.
     *
     * @note Práctica útil para demostrar la importancia de no usar contraseñas
     * predecibles en configuraciones o archivos internos de juegos.
     */
    void bruteForceByDictionary(const std::vector<unsigned char>& cifrado) {
        std::vector<std::string> clavesComunes = {
          "clave", "admin", "1234", "root", "test", "abc", "hola", "user",
          "pass", "12345", "0000", "password", "default"
        };

        for (const auto& clave : clavesComunes) {
            std::string result;
            for (int i = 0; i < cifrado.size(); i++) {
                result += static_cast<unsigned char>(
                    cifrado[i] ^ clave[i % clave.size()]);
            }

            if (isValidText(result)) {
                std::cout << "=============================\n";
                std::cout << "Clave de diccionario: '" << clave << "'\n";
                std::cout << "Texto posible : " << result << "\n";
            }
        }
    }

private:
    // No se requiere almacenamiento interno en esta versión.
};
