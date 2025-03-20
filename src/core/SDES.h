//
// Created by shingesa on 20/03/25.
//

#ifndef SDES_H
#define SDES_H
#include <vector>
#include <bitset>
#include <string>


class SDES {
public:
  SDES();

  bool setKey(const std::string& keyStr);

  std::bitset<8> encryptBlock(const std::bitset<8>& plainText);
  std::bitset<8> decryptBlock(const std::bitset<8>& cipherText);

  std::string encrypt(const std::string& plainText);
  std::string decrypt(const std::string& cipherText);

  static std::string textToBinary(const std::string& text);
  static std::string binaryToText(const std::string& binary);

private:
  std::bitset<10> key;
  std::bitset<8> k1;
  std::bitset<8> k2;

  void generateSubKeys();

  std::bitset<8> inititalPermutation(const std::bitset<8>& block);
  std::bitset<8> inverseInititalPermutation(const std::bitset<8>& block);
  std::bitset<4> fFunction(const std::bitset<4>& r,const std::bitset<8>& subkey);
  std::bitset<8> expandAndPermute(const std::bitset<4>& r);
  std::bitset<4> sBox(const std::bitset<8>& bits);
  std::bitset<8> p4Permutation(const std::bitset<4>& bits);

  std::bitset<10> permute10(const std::bitset<10>& bits);
  std::bitset<8> permute8(const std::bitset<10>& bits);
  std::bitset<5> leftShift(const std::bitset<5>& bits, int shift);
};

#endif //SDES_H
