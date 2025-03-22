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

std::bitset<10>SDES::permute10(const std::bitset<10> &bits) {

    std::bitset<10> result;
    result[9] = bits[10-3];
    result[8] = bits[10-5];
    result[7] = bits[10-2];
    result[6] = bits[10-7];
    result[5] = bits[10-4];
    result[4] = bits[10-10];
    result[3] = bits[10-1];
    result[2] = bits[10-9];
    result[1] = bits[10-8];
    result[0] = bits[10-6];
    return result;
}

std::bitset<8>SDES::permute8(const std::bitset<10> &bits) {
    std::bitset<8> result;

    result[7] = bits[10-6];
    result[6] = bits[10-3];
    result[5] = bits[10-7];
    result[4] = bits[10-4];
    result[3] = bits[10-8];
    result[2] = bits[10-5];
    result[1] = bits[10-10];
    result[0] = bits[10-9];

    return result;
}

std::bitset<5>SDES::leftShift(const std::bitset<5> &bits, int shifts) {

    std::bitset<5> result;
    for (int i = 0; i < shifts; i++) {
        bool temp = result[4];
        result<<=1;
        result[0] = temp;
    }
    return result;
}

std::bitset<8> SDES::inititalPermutation(const std::bitset<8> &block) {
    std::bitset<8> result;
    result[7] = block[8-2];
    result[6] = block[8-6];
    result[5] = block[8-3];
    result[4] = block[8-1];
    result[3] = block[8-4];
    result[2] = block[8-8];
    result[1] = block[8-5];
    result[0] = block[8-7];

    return result;
}

std::bitset<8> SDES::inverseInititalPermutation(const std::bitset<8> &block) {
    std::bitset<8> result;
    result[7] = block[8-4];
    result[6] = block[8-1];
    result[5] = block[8-3];
    result[4] = block[8-5];
    result[3] = block[8-7];
    result[2] = block[8-2];
    result[1] = block[8-8];
    result[0] = block[8-6];

    return result;
}


std::bitset<8> SDES::expandAndPermute(const std::bitset<4> &r) {
    std::bitset<8> result;
    result[7] = r[4-4];
    result[6] = r[4-1];
    result[5] = r[4-2];
    result[4] = r[4-3];
    result[3] = r[4-2];
    result[2] = r[4-3];
    result[1] = r[4-4];
    result[0] = r[4-1];

    return result;
}

std::bitset<4> SDES::sBox(const std::bitset<8> &bits) {
    int S0[4][4]={
        {1,0,3,2},
        {3,2,1,0},
        {0,2,1,3},
        {3,1,3,2}
    };

    int S1[4][4]={
    {0,1,2,3},
        {2,0,1,3},
        {3,0,1,0},
        {2,1,0,3}
    };


    int row0 = (bits[7] << 1)|bits[4];
    int col0 = (bits[6] << 1)|bits[5];

    int row1 = (bits[3] << 1)|bits[0];
    int col1 = (bits[2] << 1)|bits[1];

    int val0 = S0[row0][col0];
    int val1 = S1[row1][col1];

    std::bitset<4> result;
    result[3]=(val0 >> 1) & 1;
    result[2]=val0 & 1;
    result[1]=(val1 >> 1) & 1;
    result[0]=val1 & 1;

    return result;
}

std::bitset<4>SDES::p4Permutation(const std::bitset<4> &bits) {
    std::bitset<4> result;
    result[3] = bits[4-2];
    result[2] = bits[4-4];
    result[1] = bits[4-3];
    result[0] = bits[4-1];

    return result;
}

std::bitset<4> SDES::fFunction(const std::bitset<4> &r, const std::bitset<8> &subkey) {
    std::bitset<8>expanded = expandAndPermute(r);

    expanded^=subkey;

    std::bitset<4>sBoxOutput = sBox(expanded);

    return p4Permutation(sBoxOutput);
}






