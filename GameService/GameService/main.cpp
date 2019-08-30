#include "BotClientCommunicator.h"
#include "BotTrackingService.h"
#include "RemoteAccessPoint.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    BotTrackingService botTrackingService;
    BotClientCommunicator botClientCommunicator;
    RemoteAccessPoint serviceAccessPoint;

    QObject::connect(&botTrackingService, &BotTrackingService::newBotLocations, &botClientCommunicator, &BotClientCommunicator::sendState);

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



    return app.exec();
}
