//
// Created by shingesa on 20/03/25.
//

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "bridge/CryptoBridge.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    CryptoBridge cryptoBridge;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cryptoBridge", &cryptoBridge);

    const QUrl url(QStringLiteral("SDES/src/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](const QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);


    return app.exec();
}