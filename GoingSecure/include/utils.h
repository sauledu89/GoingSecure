#pragma once
#include <bitset>
#include <string>

/**
 * @brief Convierte un string (8 caracteres) a un bitset de 64 bits.
 * @param str Cadena de entrada.
 * @return bitset<64> con la representaci�n binaria.
 */
std::bitset<64> stringToBitset(const std::string& str);

/**
 * @brief Convierte un bitset<64> a su representaci�n como string de 8 caracteres.
 * @param bits Bitset a convertir.
 * @return Cadena de 8 caracteres.
 */
std::string bitsetToString(const std::bitset<64>& bits);