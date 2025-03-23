#include "SDES.h"
#include <iostream>
#include <sstream>
#include <iomanip>

SDES::SDES() {}

bool SDES::setKey(const std::string &keyStr) {
    if (keyStr.length() != 10) {
        return false;
    }

    for (char c : keyStr) {
        if (c != '0' && c != '1') {
            return false;
        }
    }

    for (int i = 0; i < 10; i++) {
        key[i] = (keyStr[i] == '1');
    }
    generateSubKeys();
    return true;
}

void SDES::generateSubKeys() {
    std::bitset<10> permutedKey = permute10(key);

    std::bitset<5> left, right;
    for (int i = 0; i < 5; i++) {
        left[i] = permutedKey[i+5];
        right[i] = permutedKey[i];
    }

    left = leftShift(left, 1);
    right = leftShift(right, 1);

    std::bitset<10> combined;
    for (int i = 0; i < 5; i++) {
        combined[i] = right[i];
        combined[i+5] = left[i];
    }
    k1 = permute8(combined);

    left = leftShift(left, 2);
    right = leftShift(right, 2);

    for (int i = 0; i < 5; i++) {
        combined[i] = right[i];
        combined[i+5] = left[i];
    }
    k2 = permute8(combined);
}

std::bitset<10> SDES::permute10(const std::bitset<10> &bits) {
    std::bitset<10> result;
    result[0] = bits[2];
    result[1] = bits[4];
    result[2] = bits[1];
    result[3] = bits[6];
    result[4] = bits[3];
    result[5] = bits[9];
    result[6] = bits[0];
    result[7] = bits[8];
    result[8] = bits[7];
    result[9] = bits[5];
    return result;
}

std::bitset<8> SDES::permute8(const std::bitset<10> &bits) {
    std::bitset<8> result;
    result[0] = bits[5];
    result[1] = bits[2];
    result[2] = bits[6];
    result[3] = bits[3];
    result[4] = bits[7];
    result[5] = bits[4];
    result[6] = bits[9];
    result[7] = bits[8];
    return result;
}

std::bitset<5> SDES::leftShift(const std::bitset<5> &bits, int shifts) {
    return (bits << shifts) | (bits >> (5 - shifts));
    // std::bitset<5> result = bits;
    // for (int i = 0; i < shifts; i++) {
    //     bool temp = result[4];
    //     result <<= 1;
    //     result[0] = temp;
    // }
    // return result;
}

std::bitset<8> SDES::initialPermutation(const std::bitset<8> &block) {
    std::bitset<8> result;
    result[0] = block[1];
    result[1] = block[5];
    result[2] = block[2];
    result[3] = block[0];
    result[4] = block[3];
    result[5] = block[7];
    result[6] = block[4];
    result[7] = block[6];
    return result;
}

std::bitset<8> SDES::inverseInitialPermutation(const std::bitset<8> &block) {
    std::bitset<8> result;
    result[0] = block[3];
    result[1] = block[0];
    result[2] = block[2];
    result[3] = block[4];
    result[4] = block[6];
    result[5] = block[1];
    result[6] = block[7];
    result[7] = block[5];
    return result;
}

std::bitset<8> SDES::expandAndPermute(const std::bitset<4> &r) {
    std::bitset<8> result;
    result[0] = r[3];
    result[1] = r[0];
    result[2] = r[1];
    result[3] = r[2];
    result[4] = r[1];
    result[5] = r[2];
    result[6] = r[3];
    result[7] = r[0];
    return result;
}

std::bitset<4> SDES::sBox(const std::bitset<8> &bits) {
    int S0[4][4] = {
        {1, 0, 3, 2},
        {3, 2, 1, 0},
        {0, 2, 1, 3},
        {3, 1, 3, 2}
    };

    int S1[4][4] = {
        {0, 1, 2, 3},
        {2, 0, 1, 3},
        {3, 0, 1, 0},
        {2, 1, 0, 3}
    };

    int row0 = (bits[0] << 1) | bits[3];
    int col0 = (bits[1] << 1) | bits[2];

    int row1 = (bits[4] << 1) | bits[7];
    int col1 = (bits[5] << 1) | bits[6];

    int val0 = S0[row0][col0];
    int val1 = S1[row1][col1];

    std::bitset<4> result;
    result[0] = (val0 >> 1) & 1;
    result[1] = val0 & 1;
    result[2] = (val1 >> 1) & 1;
    result[3] = val1 & 1;

    return result;
}

std::bitset<4> SDES::p4Permutation(const std::bitset<4> &bits) {
    std::bitset<4> result;
    result[0] = bits[1];
    result[1] = bits[3];
    result[2] = bits[2];
    result[3] = bits[0];
    return result;
}

std::bitset<4> SDES::fFunction(const std::bitset<4> &r, const std::bitset<8> &subkey) {
    std::bitset<8> expanded = expandAndPermute(r);
    expanded ^= subkey;
    std::bitset<4> sBoxOutput = sBox(expanded);
    return p4Permutation(sBoxOutput);
}

std::bitset<8> SDES::encryptBlock(const std::bitset<8> &plainText) {
    std::bitset<8> ip = initialPermutation(plainText);

    std::bitset<4> left, right;
    // Extract left and right halves (4 bits each)
    for (int i = 0; i < 4; i++) {
        left[i] = ip[i+4];
        right[i] = ip[i];
    }

    // Round 1
    std::bitset<4> fOut = fFunction(right, k1);
    std::bitset<4> newRight = left ^ fOut;
    std::bitset<4> newLeft = right;

    // Round 2
    fOut = fFunction(newRight, k2);
    std::bitset<4> finalLeft = newLeft ^ fOut;
    std::bitset<4> finalRight = newRight;

    // Combine the two halves (switch order for final permutation)
    std::bitset<8> combined;
    for (int i = 0; i < 4; i++) {
        combined[i] = finalRight[i];
        combined[i+4] = finalLeft[i];
    }

    return inverseInitialPermutation(combined);
}

std::bitset<8> SDES::decryptBlock(const std::bitset<8> &cipherText) {
    std::bitset<8> ip = initialPermutation(cipherText);

    std::bitset<4> left, right;
    // Extract left and right halves (4 bits each)
    for (int i = 0; i < 4; i++) {
        left[i] = ip[i+4];
        right[i] = ip[i];
    }

    // Round 1 (with K2 for decryption)
    std::bitset<4> fOut = fFunction(right, k2);
    std::bitset<4> newRight = left ^ fOut;
    std::bitset<4> newLeft = right;

    // Round 2 (with K1 for decryption)
    fOut = fFunction(newRight, k1);
    std::bitset<4> finalLeft = newLeft ^ fOut;
    std::bitset<4> finalRight = newRight;

    // Combine the two halves
    std::bitset<8> combined;
    for (int i = 0; i < 4; i++) {
        combined[i] = finalRight[i];
        combined[i+4] = finalLeft[i];
    }

    return inverseInitialPermutation(combined);
}
std::string SDES::encrypt(const std::string& plainText) {
    std::string result;

    // Convert plain text to binary and encrypt each byte
    for (size_t i = 0; i < plainText.size(); ++i) {
        unsigned char c = plainText[i];  // Take one byte at a time
        std::bitset<8> charBits(c);      // Convert char to bitset
        std::bitset<8> encryptedBits = encryptBlock(charBits);  // Encrypt the byte
        result.push_back(static_cast<char>(encryptedBits.to_ulong()));  // Convert back to char
    }

    return result;
}

std::string SDES::decrypt(const std::string& cipherText) {
    std::string result;

    // Convert cipher text to binary and decrypt each byte
    for (size_t i = 0; i < cipherText.size(); ++i) {
        unsigned char c = cipherText[i];  // Take one byte at a time
        std::bitset<8> charBits(c);       // Convert char to bitset
        std::bitset<8> decryptedBits = decryptBlock(charBits);  // Decrypt the byte
        result.push_back(static_cast<char>(decryptedBits.to_ulong()));  // Convert back to char
    }

    return result;
}


std::string SDES::textToBinary(const std::string &text) {
    std::string binary;
    for (unsigned char c : text) {
        std::bitset<8> bits(c);
        for (int i = 7; i >= 0; i--) {
            binary += bits[i] ? "1" : "0";
        }
    }
    return binary;
}

std::string SDES::binaryToText(const std::string &binary) {
    std::string text;
    for (size_t i = 0; i < binary.length(); i += 8) {
        if (i + 8 > binary.length()) {
            break;
        }

        std::string byte = binary.substr(i, 8);
        unsigned char c = 0;  // Changed to unsigned char
        for (int j = 0; j < 8; j++) {
            c = (c << 1) | (byte[j] == '1');
        }
        text += c;
    }
    return text;
}