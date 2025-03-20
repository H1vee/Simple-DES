//
// Created by shingesa on 20/03/25.
//

#include "SDES.h"
#include <iostream>
#include <sstream>

SDES::SDES(){}

bool SDES::setKey(const std::string &keyStr) {
    if (keyStr.length()!=10) {
        return false;
    }

    for (char c : keyStr) {
        if (c != '0' && c != '1') {
            return false;
        }
    }

    for (int i = 1; i < 10; i++) {
        key[9-i] = (keyStr[i]=='1');
    }
    generateSubKeys();
    return true;
}

void SDES::generateSubKeys() {
    std::bitset<10> permutedKey = permute10(key);

    std::bitset<5>left,right;
    for (int i = 0; i < 5; i++) {
        left[i] = permutedKey[i+5];
        right[i] = permutedKey[i];
    }

    left = leftShift(left,1);
    right = leftShift(right,1);

    std::bitset<10>combined;
    for (int i = 0; i < 5; i++) {
        combined[i] = right[i];
        combined[i+5] = left[i];
    }
    k1 = permute8(combined);

    left = leftShift(left,2);
    right = leftShift(right,2);

    for (int i = 0; i < 5; i++) {
        combined[i] = right[i];
        combined[i+5] = left[i];
    }
    k2 = permute8(combined);
}