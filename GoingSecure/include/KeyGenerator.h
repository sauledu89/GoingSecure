#pragma once
#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <string>
#include <bitset>

std::string generateRandomKey();
void printKeyHex(const std::string& key);
std::bitset<64> stringToBitset(const std::string& key);

#endif