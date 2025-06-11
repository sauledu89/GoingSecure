/**
 * @file main.cpp
 * @brief Menú principal para pruebas de algoritmos de criptografía.
 *
 * Este programa permite probar diferentes algoritmos clásicos y modernos
 * de criptografía: Cifrado César, codificación XOR, conversión ASCII-Binario
 * y cifrado DES. Diseñado como laboratorio educativo para comprender
 * fundamentos criptográficos útiles en seguridad informática y videojuegos.
 */

#include <iostream>
#include "../include/CesarEncryption.h"
#include "../include/XOREncoder.h"
#include "../include/AsciiBinary.h"
#include "../include/DES.h"

 /// ======================== FUNCIONES DE PRUEBA ========================

 /**
  * @brief Ejecuta una prueba del cifrado César con rotación fija.
  *
  * Utiliza un mensaje predefinido para demostrar el cifrado, descifrado
  * y una posible clave deducida por análisis de frecuencia.
  *
  * @note Útil para entender principios de cifrado por sustitución. Este
  * enfoque puede emplearse en videojuegos para puzzles, acertijos o codificación
  * básica de datos temporales.
  */
void testCesar() {
    std::cout << "\n========= PRUEBA DEL CIFRADO CESAR =========\n";

    std::string mensaje =
        "Bienvenidos a la clase de seguridad para videojuegos. "
        "En esta materia, aprenderan a cifrar mensajes y descifrar codigos ocultos";

    int rotacion = 4;
    CesarEncryption cesar;

    std::string cifrado = cesar.encode(mensaje, rotacion);
    std::cout << "Texto cifrado      : " << cifrado << std::endl;

    std::string descifrado = cesar.decode(cifrado, rotacion);
    std::cout << "Texto descifrado   : " << descifrado << std::endl;

    std::cout << "Clave estimada     : "
        << cesar.evaluatePossibleKey(cifrado) << std::endl;
}

/**
 * @brief Ejecuta una prueba del cifrado XOR con clave variable.
 *
 * Incluye una prueba de codificación, decodificación, impresión
 * en hexadecimal y ataque por fuerza bruta con diccionario.
 *
 * @note El cifrado XOR es una técnica fundamental en muchos
 * juegos online para proteger comunicación entre cliente y servidor.
 *
 * @warning No usar XOR sin clave segura ni en datos persistentes
 * sin protección adicional como hashing o salting.
 */
void testXorEncoder() {
    std::cout << "\n=========== PRUEBA DEL CIFRADO XOR ===========\n";

    XOREncoder xorEncoder;
    std::string mensaje = "Hola Mundo";
    std::string clave = "clave";

    std::cout << "Mensaje original   : " << mensaje << std::endl;

    std::string cifrado = xorEncoder.encode(mensaje, clave);
    std::cout << "Texto cifrado      : " << cifrado << std::endl;

    std::string descifrado = xorEncoder.encode(cifrado, clave);
    std::cout << "Texto cifrado HEX  : ";
    xorEncoder.printHex(cifrado);
    std::cout << std::endl;

    std::cout << "Mensaje descifrado : " << descifrado << std::endl;

    std::vector<unsigned char> bytesCifrados(cifrado.begin(), cifrado.end());

    std::cout << "\n--- Fuerza bruta (diccionario de claves) ---\n";
    xorEncoder.bruteForceByDictionary(bytesCifrados);
}

/**
 * @brief Prueba de conversión entre texto ASCII y binario.
 *
 * Convierte una cadena ASCII a su representación binaria
 * y luego la vuelve a transformar a texto.
 *
 * @note Este tipo de conversión es útil para entender el
 * nivel más bajo de datos, lo cual es clave para videojuegos
 * que implementen sistemas de almacenamiento binario (guardados, redes, etc.).
 */
void testAsciiBinary() {
    std::cout << "\n====== PRUEBA DE CONVERSION ASCII-BINARIO ======\n";

    AsciiBinary ab;
    std::string input = "Hello, World!";
    std::cout << "Mensaje original   : " << input << std::endl;

    std::string binary = ab.stringToBinary(input);
    std::cout << "Texto a binario    : " << binary << std::endl;

    std::string decoded = ab.binaryToString(binary);
    std::cout << "Binario a texto    : " << decoded << std::endl;
}

/**
 * @brief Ejecuta una prueba básica del algoritmo de cifrado DES.
 *
 * Usa texto plano y clave predefinidos para generar un texto cifrado.
 *
 * @note Aunque DES está obsoleto para seguridad real, es útil
 * para comprender operaciones de cifrado por bloques, relevantes
 * para estructuras de datos protegidas en videojuegos.
 *
 * @warning No usar DES en aplicaciones reales; preferir AES o ChaCha20.
 */
void testDes() {
    std::cout << "\n=========== PRUEBA DEL CIFRADO DES ===========\n";

    std::bitset<64> plaintext("0001001000110100010101100111100010011010101111001101111011110001");
    std::bitset<64> key("0001001100110100010101110111100110011011101111001101111111110001");

    DES des(key);
    auto ciphertext = des.encode(plaintext);

    std::cout << "Texto plano        : " << plaintext << std::endl;
    std::cout << "Clave              : " << key << std::endl;
    std::cout << "Texto cifrado      : " << ciphertext << std::endl;
}

/// ======================== MENÚ PRINCIPAL ========================

/**
 * @brief Punto de entrada principal del programa.
 *
 * Despliega un menú interactivo en consola para seleccionar
 * diferentes pruebas de cifrado.
 *
 * @return int Código de salida (0 si finaliza correctamente).
 */
int main() {
    int opcion;

    do {
        std::cout << "\n=============================================\n";
        std::cout << "           GoingSecure Key Forge       \n";
        std::cout << "=============================================\n";
        std::cout << "  [1] Cifrado Cesar\n";
        std::cout << "  [2] Codificacion XOR\n";
        std::cout << "  [3] Conversion ASCII-Binario\n";
        std::cout << "  [4] Cifrado DES\n";
        std::cout << "  [5] Clave aleatoria DES (no implementado)\n";
        std::cout << "  [0] Salir\n";
        std::cout << "---------------------------------------------\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        std::cin.ignore(); // Limpieza del buffer por seguridad

        switch (opcion) {
        case 1:
            testCesar();
            break;
        case 2:
            testXorEncoder();
            break;
        case 3:
            testAsciiBinary();
            break;
        case 4:
            testDes();
            break;
        case 0:
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
