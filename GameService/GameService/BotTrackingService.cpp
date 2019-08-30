// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotTrackingService.h"

#include <MessageMediatorTcpSocket.h>

BotTrackingService::BotTrackingService()
{
    QObject::connect(&m_botTrackingTcpServer, &QTcpServer::newConnection, [this]()
    {
        m_botLocationReceiver.reset(new BotLocationReceiver(QScopedPointer<MessageMediator>(new MessageMediatorTcpSocket(m_botTrackingTcpServer.nextPendingConnection()))));
        QObject::connect(m_botLocationReceiver.get(), &BotLocationReceiver::newBotLocations, this, &BotTrackingService::newBotLocations);
    });
}

bool BotTrackingService::listen(const QHostAddress& address, unsigned short port)
{
    return m_botTrackingTcpServer.listen(address, port);
}
