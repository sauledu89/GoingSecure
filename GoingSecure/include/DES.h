#pragma once
#include "Prerequisites.h"

/**
 * @class DES
 * @brief Implementación didáctica y simplificada del algoritmo de cifrado DES.
 *
 * Esta clase proporciona una simulación del algoritmo DES (Data Encryption Standard),
 * usando versiones reducidas de las tablas E (expansión), P (permutación) y S-Box.
 * Permite visualizar paso a paso cómo se transforma un bloque de 64 bits a través
 * del cifrado por rondas tipo Feistel.
 *
 * @note Aunque DES ha sido superado en aplicaciones reales por algoritmos como AES,
 * su estructura lo convierte en una excelente herramienta para comprender cifrado por bloques.
 *
 * @note En videojuegos, los conceptos de cifrado por bloques pueden aplicarse a:
 * - Encriptación ligera de archivos guardados.
 * - Validación de licencias o perfiles de usuario.
 * - Protección de datos sensibles en cliente.
 */
class DES {
public:
    /**
     * @brief Constructor por defecto.
     */
    DES() = default;

    /**
     * @brief Constructor que inicializa la clave y genera las subclaves.
     * @param key Clave original de 64 bits con la que se derivan las subclaves.
     */
    DES(const std::bitset<64>& key) : key(key) {
        generateSubkeys();
    }

    /**
     * @brief Destructor por defecto.
     */
    ~DES() = default;

    /**
     * @brief Genera las 16 subclaves de 48 bits utilizadas en cada ronda Feistel.
     *
     * Esta versión usa un simple desplazamiento en bits, sin aplicar las permutaciones reales de DES.
     *
     * @note Este paso es clave en el cifrado por bloques: pequeñas variaciones en la clave
     * producen resultados completamente distintos (difusión).
     */
    void generateSubkeys() {
        for (int i = 0; i < 16; ++i) {
            std::bitset<48> subkey((key.to_ullong() >> i) & 0xFFFFFFFFFFFF);
            subkeys.push_back(subkey);
        }
    }

    /**
     * @brief Permutación inicial. En esta implementación, es una identidad.
     * @param input Bloque de entrada de 64 bits.
     * @return std::bitset<64> Resultado de la permutación.
     */
    std::bitset<64> iPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; i++) {
            output[i] = input[i];
        }
        return output;
    }

    /**
     * @brief Expande una mitad de bloque de 32 bits a 48 bits, usando tabla E.
     * @param halfBlock Mitad derecha del bloque de datos.
     * @return std::bitset<48> Resultado expandido.
     */
    std::bitset<48> expand(const std::bitset<32>& halfBlock) {
        std::bitset<48> output;
        for (int i = 0; i < 48; i++) {
            output[i] = halfBlock[32 - EXPANSION_TABLE[i]];
        }
        return output;
    }

    /**
     * @brief Sustituye el bloque expandido usando una S-Box simplificada.
     * @param input Bloque de 48 bits después del XOR con la subclave.
     * @return std::bitset<32> Bloque reducido tras sustitución.
     */
    std::bitset<32> substitute(const std::bitset<48>& input) {
        std::bitset<32> output;
        for (int i = 0; i < 8; i++) {
            int row = (input[i * 6] << 1) | input[i * 6 + 5];
            int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) |
                (input[i * 6 + 3] << 1) | input[i * 6 + 4];
            int sboxValue = SBOX[row % 4][col % 16];
            for (int j = 0; j < 4; j++) {
                output[i * 4 + j] = (sboxValue >> (3 - j)) & 1;
            }
        }
        return output;
    }

    /**
     * @brief Permutación P (simplificada). Mezcla la salida de sustitución.
     * @param input Bloque de 32 bits.
     * @return std::bitset<32> Resultado permutado.
     */
    std::bitset<32> permuteP(const std::bitset<32>& input) {
        std::bitset<32> output;
        for (int i = 0; i < 32; i++) {
            output[i] = input[32 - P_TABLE[i]];
        }
        return output;
    }

    /**
     * @brief Función Feistel que transforma la mitad derecha con la subclave.
     * @param right Mitad derecha del bloque de datos.
     * @param subkey Subclave correspondiente a la ronda actual.
     * @return std::bitset<32> Resultado de la transformación.
     */
    std::bitset<32> feistel(const std::bitset<32>& right,
        const std::bitset<48>& subkey) {
        auto expanded = expand(right);
        auto xored = expanded ^ subkey;
        auto substituted = substitute(xored);
        auto permuted = permuteP(substituted);
        return permuted;
    }

    /**
     * @brief Permutación final. En esta implementación, es idéntica al input.
     * @param input Bloque final de 64 bits.
     * @return std::bitset<64> Resultado permutado.
     */
    std::bitset<64> fPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; i++) {
            output[i] = input[i];
        }
        return output;
    }

    /**
     * @brief Cifra un bloque de 64 bits mediante 16 rondas tipo Feistel.
     * @param plaintext Texto plano de 64 bits.
     * @return std::bitset<64> Texto cifrado.
     *
     * @note La estructura Feistel permite que el mismo proceso (con claves invertidas)
     * pueda ser usado para descifrar. No se implementa aquí, pero es parte del estándar DES.
     */
    std::bitset<64> encode(const std::bitset<64>& plaintext) {
        auto data = iPermutation(plaintext);
        std::bitset<32> left(data.to_ullong() >> 32);
        std::bitset<32> right(data.to_ullong());

        for (int round = 0; round < 16; round++) {
            auto newRight = left ^ feistel(right, subkeys[round]);
            left = right;
            right = newRight;
        }

        uint64_t combined = (static_cast<uint64_t>(right.to_ullong()) << 32) |
            left.to_ullong();
        return fPermutation(std::bitset<64>(combined));
    }

private:
    std::bitset<64> key;  ///< Clave principal de 64 bits.
    std::vector<std::bitset<48>> subkeys;  ///< Subclaves derivadas (una por ronda).

    /// Tabla de expansión E (simplificada, 48 posiciones).
    const int EXPANSION_TABLE[48] = {
      32, 1, 2, 3, 4, 5,
       4, 5, 6, 7, 8, 9,
       8, 9,10,11,12,13,
      12,13,14,15,16,17,
      16,17,18,19,20,21,
      20,21,22,23,24,25,
      24,25,26,27,28,29,
      28,29,30,31,32,1
    };

    /// Tabla de permutación P (simplificada, 32 posiciones).
    const int P_TABLE[32] = {
      16, 7, 20, 21,29,12,28,17,
       1,15,23,26, 5,18,31,10,
       2, 8,24,14,32,27, 3, 9,
      19,13,30, 6,22,11, 4,25
    };

    /// S-Box de demostración (4x16 valores).
    const int SBOX[4][16] = {
      {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
      {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
      {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
      {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    };
};
