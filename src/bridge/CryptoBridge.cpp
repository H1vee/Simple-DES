//
// Created by shingesa on 23/03/25.
//

#include "CryptoBridge.h"

CryptoBridge::CryptoBridge(QObject *parent):QObject(parent), keyValid(false) {

}

void CryptoBridge::setKey(const QString &key) {
    const std::string strKey = key.toStdString();
    keyValid = sdes.setKey(strKey);

    emit keyChanged(keyValid);
}

void CryptoBridge::encrypt(const QString& plainText) {
    if (!keyValid) {
        emit encryptionCompleted("Error: Key not set");
        return;
    }
    qDebug()<< "Encrypting text:" << plainText;
    const std::string result = sdes.encrypt(plainText.toStdString());

    qDebug() << "Encryption result:" << QString::fromStdString(result);

    const QString qResult = QString::fromStdString(result);

    // Emit the signal with the result
    emit encryptionCompleted(qResult);
}


void CryptoBridge::decrypt(const QString& cipherText) {
    if (!keyValid) {
        emit decryptionCompleted("Error: Key not set");
        return;
    }
    const std::string result = sdes.decrypt(cipherText.toStdString());
    QString qResult = QString::fromStdString(result);
    emit decryptionCompleted(qResult);
}

QString CryptoBridge::textToBinary(const QString& text) {
    std::string binary = SDES::textToBinary(text.toStdString());
    return QString::fromStdString(binary);
}

QString CryptoBridge::binaryToText(const QString& binary) {
    std::string text = SDES::binaryToText(binary.toStdString());
    return QString::fromStdString(text);
}