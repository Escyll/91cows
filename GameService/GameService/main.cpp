#include <MessageMediatorTcpSocket.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>

#include "BotClient.h"
#include "BotLocationReceiver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QList<BotClient*> botClients;
    QScopedPointer<BotLocationReceiver> botLocationReceiver;

    QTcpServer botDetectionServer;
    QTcpServer botClientServer;

    QObject::connect(&botDetectionServer, &QTcpServer::newConnection, [&botDetectionServer, &botLocationReceiver, &botClients]()
    {
        botLocationReceiver.reset(new BotLocationReceiver(QScopedPointer<MessageMediator>(new MessageMediatorTcpSocket(botDetectionServer.nextPendingConnection()))));
        QObject::connect(botLocationReceiver.get(), &BotLocationReceiver::newBotLocations, [&botClients] (const QJsonObject& jsonMessage)
        {
            for (auto botClient : botClients)
            {
                botClient->sendNewBotLocations(jsonMessage);
            }
        });
    });

    QObject::connect(&botClientServer, &QTcpServer::newConnection, [&botClientServer, &botClients]()
    {
        botClients << new BotClient(QScopedPointer<MessageMediator>(new MessageMediatorTcpSocket(botClientServer.nextPendingConnection())));
    });

    if (botDetectionServer.listen(QHostAddress::Any, static_cast<quint16>(9635)))
    {
        qDebug() << "Listening on port" << botDetectionServer.serverPort();
    }
    else
    {
        qDebug() << "Not listening";
    }

    if (botClientServer.listen(QHostAddress::Any, static_cast<quint16>(9835)))
    {
        qDebug() << "Listening on port" << botClientServer.serverPort();
    }
    else
    {
        qDebug() << "Not listening";
    }

    auto returnCode = app.exec();

    qDeleteAll(botClients);

    return returnCode;
}
