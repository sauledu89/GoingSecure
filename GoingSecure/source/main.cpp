﻿/**
 * @file main.cpp
 * @brief Entrada principal para cifrado y descifrado de archivos de texto.
 *
 * Este archivo contiene un menú básico que permite al usuario:
 *  - Seleccionar un archivo .txt de entrada
 *  - Elegir una operación: cifrar o descifrar
 *  - Seleccionar un algoritmo: César, XOR, Vigenere, DES
 *  - Escribir una clave y procesar el archivo
 *
 */

#include "../include/Prerequisites.h"
#include "../include/CesarEncryption.h"
#include "../include/XOREncoder.h"
#include "../include/Vigenere.h"
#include "../include/DES.h"
#include "../include/KeyGenerator.h"
#include "../include/utils.h"
#include <fstream>    // para ifstream, ofstream
#include <sstream>    // para ostringstream
#include <iostream>
#include <bitset> void procesarArchivo();

int main() {
    // Solo esta función queda activa para entrega del examen
    procesarArchivo();
    return 0;
}

void procesarArchivo() {
    std::cout << "\n--- Cifrado/Descifrado de Archivos ---\n";

    std::string rutaEntrada, rutaSalida;
    std::cout << "Ruta del archivo de entrada: ";
    std::getline(std::cin, rutaEntrada);

    std::cout << "Ruta del archivo de salida: ";
    std::getline(std::cin, rutaSalida);

    std::cout << "Operacion: [1] Cifrar  [2] Descifrar: ";
    int operacion;
    std::cin >> operacion;
    std::cin.ignore();

    std::cout << "Algoritmo:\n";
    std::cout << "1. Cesar\n2. XOR\n3. Vigenere\n4. DES\nSeleccione: ";
    int algoritmo;
    std::cin >> algoritmo;
    std::cin.ignore();

    std::string clave;
    if (algoritmo == 1 || algoritmo == 2 || algoritmo == 3) {
        std::cout << "Ingrese la clave: ";
        std::getline(std::cin, clave);
    }

    std::ifstream inFile(rutaEntrada, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error al abrir el archivo de entrada.\n";
        return;
    }

    std::ostringstream oss;
    oss << inFile.rdbuf();
    std::string contenido = oss.str();
    inFile.close();

    std::string resultado;

    switch (algoritmo) {
    case 1: { // César
        CesarEncryption cesar;
        int rotacion = std::stoi(clave);
        resultado = (operacion == 1)
            ? cesar.encode(contenido, rotacion)
            : cesar.decode(contenido, rotacion);
        break;
    }
    case 2: { // XOR
        XOREncoder xorEnc;
        resultado = xorEnc.encode(contenido, clave);
        break;
    }
    case 3: { // Vigenere
        Vigenere vig(clave);
        resultado = (operacion == 1)
            ? vig.encode(contenido)
            : vig.decode(contenido);
        break;
    }
    case 4: { // DES
        if (clave.length() != 8) {
            std::cerr << "La clave DES debe tener 8 caracteres.\n";
            return;
        }
        std::bitset<64> keyBits = stringToBitset(clave);
        DES des(keyBits);

        // Solo se procesa el primer bloque (64 bits = 8 caracteres)
        std::string bloque = contenido.substr(0, 8);
        std::bitset<64> dataBits = stringToBitset(bloque);

        std::bitset<64> resultadoBits = (operacion == 1)
            ? des.encode(dataBits)
            : des.decode(dataBits);

        resultado = bitsetToString(resultadoBits);
        break;
    }
    default:
        std::cerr << "Algoritmo no valido.\n";
        return;
    }

    std::ofstream outFile(rutaSalida, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo de salida.\n";
        return;
    }

    outFile << resultado;
    outFile.close();

    std::cout << "Operacion completada y archivo guardado en: " << rutaSalida << "\n";
}

/**
 * NOTA:
 * Las funciones extra como testCesar(), testXorEncoder(), testCryptoGenerator(), etc.
 * se han comentado o eliminado del flujo para evitar errores y entregar solo lo requerido.
 */