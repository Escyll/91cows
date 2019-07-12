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

    gameServiceSocket.connectToHost(QHostAddress(QHostAddress::LocalHost).toString(), static_cast<quint16>(9835));

    if (!gameServiceSocket.waitForConnected())
    {
        qDebug() << "Could not connect to Game Service";
        return 1;
    }
    QString messageSource;
    QTextStream messageSourceStream(&messageSource);
    QObject::connect(&gameServiceSocket, &QTcpSocket::readyRead, [&gameServiceSocket, &messageSourceStream]
    {
        messageSourceStream << gameServiceSocket.readAll();
        QString object;
        while (messageSourceStream.readLineInto(&object))
        {
            qDebug().noquote() << object;
        }
    });

    return app.exec();
}
