// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotClientCommunicator.h"
#include <MessageMediatorTcpSocket.h>

BotClientCommunicator::BotClientCommunicator()
{
    QObject::connect(&m_botClientServer, &QTcpServer::newConnection, [this]()
    {
        m_botClients << QSharedPointer<BotClient>::create(QScopedPointer<MessageMediator>(new MessageMediatorTcpSocket(m_botClientServer.nextPendingConnection())));
    });
}

bool BotClientCommunicator::listen(const QHostAddress& address, unsigned short port)
{
    return m_botClientServer.listen(address, port);
}

void BotClientCommunicator::sendState(const QJsonObject& doc)
{
    for (auto botClient : m_botClients)
    {
       botClient->sendState(doc);
    }
}
