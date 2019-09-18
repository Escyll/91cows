#include "BotClientCommunicator.h"
#include "BotTrackingService.h"
#include "RemoteAccessPoint.h"
#include "CollisionDetector.h"
#include "GameRunner.h"
#include "VisualizerAccessPoint.h"

#include <QCoreApplication>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    BotTrackingService botTrackingService;
    BotClientCommunicator botClientCommunicator;
    RemoteAccessPoint serviceAccessPoint;
//    VisualizerAccessPoint visualizerAccessPoint;

//    QObject::connect(&botTrackingService, &BotTrackingService::newBotLocations, &botClientCommunicator, &BotClientCommunicator::sendState);

    if (botTrackingService.listen(QHostAddress::Any, static_cast<quint16>(9635)))
    {
        qDebug() << "BotTrackingService listening";
    }
    else
    {
        qDebug() << "Not listening";
    }

    if (botClientCommunicator.listen(QHostAddress::Any, static_cast<quint16>(9835)))
    {
        qDebug() << "BotClientCommunicator listening";
    }
    else
    {
        qDebug() << "Not listening";
    }

    GameRunner gameRunner;
    gameRunner.createNewGame();
    gameRunner.startGame();

    QObject::connect(&gameRunner, &GameRunner::sendOutRevealedState, [&botClientCommunicator] (const QJsonObject& state)
    {
        qDebug().noquote().nospace() << "Sending state: " << QJsonDocument(state).toJson(QJsonDocument::Compact) << "\n";
        botClientCommunicator.sendState(state);
    });

    return app.exec();
}
