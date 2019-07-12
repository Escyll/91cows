#include <QCoreApplication>
#include <QHostAddress>
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
    int tick = 0;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&gameServiceSocket, &tick]
    {
        qDebug() << "Writing tick" << tick;
        gameServiceSocket.write(QJsonDocument(QJsonObject({
            {"tick", tick++}
        })).toJson(QJsonDocument::JsonFormat::Compact));
    });
    timer.start(1000);

    return app.exec();
}
