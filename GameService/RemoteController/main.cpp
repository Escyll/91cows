#include "RemoteController.h"

#include <QApplication>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QTcpSocket>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<RemoteController>("swoc", 1, 0, "RemoteController");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
