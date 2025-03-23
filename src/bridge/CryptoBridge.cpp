//
// Created by shingesa on 23/03/25.
//

#include "CryptoBridge.h"

CryptoBridge::CryptoBridge(QObject *parent):QObject(parent), keyValid(false) {

}

bool CryptoBridge::isKeyValid()const {
    return keyValid;
}

QString CryptoBridge::encrypt(const QString& plainText) {
    if (!keyValid) {
        emit encryptionCompleted("Error: Key not set");
        return "Error: Key not set";
    }
    const std::string result = sdes.encrypt(plainText.toStdString());
    QString qResult = QString::fromStdString(result);
    emit encryptionCompleted(qResult);
    return qResult;
}


QString CryptoBridge::decrypt(const QString& cipherText) {
    if (!keyValid) {
        emit decryptionCompleted("Error: Key not set");
        return "Error: Key not set";
    }
    const std::string result = sdes.decrypt(cipherText.toStdString());
    QString qResult = QString::fromStdString(result);
    emit decryptionCompleted(qResult);
    return qResult;
}

QString CryptoBridge::textToBinary(const QString& text) {
    std::string binary = SDES::textToBinary(text.toStdString());
    return QString::fromStdString(binary);
}

QString CryptoBridge::binaryToText(const QString& binary) {
    std::string text = SDES::binaryToText(binary.toStdString());
    return QString::fromStdString(text);
}