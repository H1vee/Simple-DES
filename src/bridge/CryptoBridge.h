//
// Created by shingesa on 23/03/25.
//

#ifndef CRYPTOBRIDGE_H
#define CRYPTOBRIDGE_H

#include <QObject>
#include <QString>
#include "../core/SDES.h"

class CryptoBridge: public QObject {

    Q_OBJECT

public:
    explicit CryptoBridge(QObject *parent = nullptr);

public slots:
    bool setKey(const  QString &key);

    bool isKeyValid() const;

    QString encrypt(const  QString &plainText);

    QString decrypt(const  QString &cipherText);

    QString textToBinary(const  QString &text);
    QString binaryToText(const QString &binary);

signals:

    void keyChanged(bool valid);

    void encryptionCompleted(const QString& result);
    void decryptionCompleted(const QString& result);

private:
    SDES sdes;
    bool keyValid;
};



#endif //CRYPTOBRIDGE_H
