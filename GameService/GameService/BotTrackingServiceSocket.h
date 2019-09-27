// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTTRACKINGSERVICESOCKET_H
#define BOTTRACKINGSERVICESOCKET_H

#include "MessageMediator.h"

#include <QScopedPointer>
#include <QTcpServer>

class BotTrackingServiceSocket : public QObject
{
    Q_OBJECT
public:
    BotTrackingServiceSocket();
    bool listen(const QHostAddress& address = QHostAddress::Any, unsigned short port = 0);

signals:
    void newBotLocations(const QJsonObject& jsonMessage) const;

private:
    QSharedPointer<MessageMediator> m_mediator;
    QTcpServer m_botTrackingTcpServer;
};

#endif // BOTTRACKINGSERVICESOCKET_H
