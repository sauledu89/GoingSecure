/**
 * @file main.cpp
 * @brief Entrada principal para cifrado y descifrado de archivos de texto.
 *
 * Este archivo contiene un menú básico que permite al usuario:
 *  - Seleccionar un archivo .txt de entrada desde una carpeta preestablecida
 *  - Elegir una operación: cifrar o descifrar
 *  - Seleccionar un algoritmo: César, XOR, Vigenere, DES
 *  - Escribir una clave y procesar el archivo
 */

#include "../include/Prerequisites.h"
#include "../include/CesarEncryption.h"
#include "../include/XOREncoder.h"
#include "../include/Vigenere.h"
#include "../include/DES.h"
#include "../include/KeyGenerator.h"
#include "../include/utils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>
#include <cstdlib>

 // -------- FUNCIONES AUXILIARES --------
std::vector<std::string> listarArchivos(const std::string& carpeta) {
    std::vector<std::string> archivos;
    struct _finddata_t file;
    intptr_t handle;

    std::string filtro = carpeta + "\\*.txt";
    handle = _findfirst(filtro.c_str(), &file);
    if (handle == -1) return archivos;

    do {
        archivos.push_back(file.name);
    } while (_findnext(handle, &file) == 0);

    _findclose(handle);
    return archivos;
}

std::string seleccionarArchivoDesdeCarpeta(const std::string& carpeta) {
    std::vector<std::string> archivos = listarArchivos(carpeta);
    if (archivos.empty()) {
        std::cout << "No se encontraron archivos .txt en la carpeta.\n";
        return "";
    }

    std::cout << "\nArchivos disponibles:\n";
    for (size_t i = 0; i < archivos.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << archivos[i] << "\n";
    }

    int seleccion = 0;
    do {
        std::cout << "Seleccione un archivo por numero: ";
        std::cin >> seleccion;
    } while (seleccion < 1 || seleccion > archivos.size());

    return carpeta + "\\" + archivos[seleccion - 1];
}

// -------- PROGRAMA PRINCIPAL --------
void procesarArchivo();

int main() {
    procesarArchivo();
    return 0;
}

void procesarArchivo() {
    std::cout << "\n--- Cifrado/Descifrado de Archivos ---\n";

    // Selección desde carpeta DatosCrudos
    std::string rutaEntrada = seleccionarArchivoDesdeCarpeta("DatosCrudos");
    if (rutaEntrada.empty()) return;

    // Crear carpeta de salida si no existe
    _mkdir("DatosCif");

    // Selección desde carpeta DatosCif
    std::string rutaSalida = seleccionarArchivoDesdeCarpeta("DatosCif");
    if (rutaSalida.empty()) return;

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

        std::string bloque = contenido.substr(0, 8); // solo primer bloque
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
