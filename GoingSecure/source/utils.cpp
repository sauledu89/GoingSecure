#include "../include/utils.h" 
#include <bitset>

std::bitset<64> stringToBitset(const std::string& str) {
    std::bitset<64> bits;
    for (size_t i = 0; i < str.size() && i < 8; ++i) {
        std::bitset<8> byte(str[i]);
        for (size_t j = 0; j < 8; ++j) {
            bits[i * 8 + j] = byte[j];
        }
    }
    return bits;
}

std::string bitsetToString(const std::bitset<64>& bits) {
    std::string str(8, '\0');
    for (size_t i = 0; i < 8; ++i) {
        std::bitset<8> byte;
        for (size_t j = 0; j < 8; ++j) {
            byte[j] = bits[i * 8 + j];
        }
        str[i] = static_cast<char>(byte.to_ulong());
    }
    return str;
}