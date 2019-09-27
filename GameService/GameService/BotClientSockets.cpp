// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotClientSockets.h"
#include <MessageMediatorTcpSocket.h>

BotClientSockets::BotClientSockets()
{
    QObject::connect(&m_botClientServer, &QTcpServer::newConnection, this, [this]()
    {
        auto newBotClient = QSharedPointer<MessageMediatorTcpSocket>::create(m_botClientServer.nextPendingConnection());
        m_botClients << newBotClient;
        QObject::connect(newBotClient.get(), &MessageMediator::disconnected, this, [this, newBotClient]
        {
            m_botClients.removeAll(newBotClient);
        });
    });
}

bool BotClientSockets::listen(const QHostAddress& address, unsigned short port)
{
    return m_botClientServer.listen(address, port);
}

void BotClientSockets::sendState(const QJsonObject& doc)
{
    for (auto botClient : m_botClients)
    {
        botClient->sendMessage(doc);
    }
}
