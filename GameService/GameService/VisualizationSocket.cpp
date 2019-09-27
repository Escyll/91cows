// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "VisualizationSocket.h"
#include <MessageMediatorTcpSocket.h>

VisualizationSocket::VisualizationSocket()
{
    QObject::connect(&m_visualizationServer, &QTcpServer::newConnection, this, [this]
    {
        m_mediator = QSharedPointer<MessageMediatorTcpSocket>::create(m_visualizationServer.nextPendingConnection());
        QObject::connect(m_mediator.get(), &MessageMediator::disconnected, this, [this]
        {
//            m_mediator.reset();
        });
    });
}

bool VisualizationSocket::listen(const QHostAddress& address, unsigned short port)
{
    return m_visualizationServer.listen(address, port);
}

void VisualizationSocket::sendState(const QJsonObject& doc)
{
    if (m_mediator)
    {
        m_mediator->sendMessage(doc);
    }
}
