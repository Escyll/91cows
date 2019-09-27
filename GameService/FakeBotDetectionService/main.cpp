#include <QCoreApplication>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTcpSocket gameServiceSocket;

    gameServiceSocket.connectToHost(QHostAddress(QHostAddress::LocalHost).toString(), static_cast<quint16>(9635));

    if (!gameServiceSocket.waitForConnected())
    {
        qDebug() << "Could not connect to Game Service";
        return 1;
    }
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&gameServiceSocket]
    {
        QJsonObject json;
        QJsonArray botJsonArray;
        QJsonObject botJson;
        botJson["arucoId"] = 13;
        botJson["position"] = QJsonArray() << 0.13 << 0.62;
        botJson["orientationPointX"] = QJsonArray() << 0.025 << 0.0;
        botJson["orientationPointY"] = QJsonArray() << 0.0 << 0.025;
        botJsonArray << botJson;
        json["bots"] = botJsonArray;
        gameServiceSocket.write(QJsonDocument(json).toJson(QJsonDocument::JsonFormat::Compact));
    });
    timer.start(10);

    return app.exec();
}
