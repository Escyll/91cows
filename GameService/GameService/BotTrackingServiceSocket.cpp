// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotTrackingServiceSocket.h"

#include <MessageMediatorTcpSocket.h>
#include <QJsonDocument>

BotTrackingServiceSocket::BotTrackingServiceSocket()
{
    QObject::connect(&m_botTrackingTcpServer, &QTcpServer::newConnection, this, [this]()
    {
        m_mediator = QSharedPointer<MessageMediatorTcpSocket>::create(m_botTrackingTcpServer.nextPendingConnection());
        QObject::connect(m_mediator.get(), &MessageMediator::jsonMessageReceived, this, [this](const QJsonObject& jsonMessage)
        {
            emit newBotLocations(jsonMessage);
        });
        QObject::connect(m_mediator.get(), &MessageMediator::disconnected, this, [this]
        {
            m_mediator.reset();
        });
    });
}

bool BotTrackingServiceSocket::listen(const QHostAddress& address, unsigned short port)
{
    return m_botTrackingTcpServer.listen(address, port);
}
