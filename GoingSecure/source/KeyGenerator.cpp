#include "../include/KeyGenerator.h"
#include "../include/Prerequisites.h"
#include "../include/utils.h"  // Necesario para usar stringToBitset
#include <random>
#include <iostream>
#include <iomanip>

/**
 * @brief Genera una clave de 8 caracteres aleatorios (64 bits).
 * @return std::string Clave generada.
 */
std::string generateRandomKey() {
    std::string key;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    for (int i = 0; i < 8; ++i) {
        char byte = static_cast<char>(dist(gen));
        key += byte;
    }

    return key;
}

/**
 * @brief Imprime la clave generada en formato hexadecimal.
 * @param key Cadena de 8 caracteres (clave).
 */
void printKeyHex(const std::string& key) {
    std::cout << "Clave generada (hex): ";
    for (unsigned char c : key) {
        std::cout << std::hex << std::uppercase
            << std::setw(2) << std::setfill('0')
            << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;
}
