// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTTRACKINGSERVICESOCKET_H
#define BOTTRACKINGSERVICESOCKET_H

#include "BotInfo.h"
#include "MessageMediator.h"
#include "TeamSettings.h"

#include <QScopedPointer>
#include <QTcpServer>

class BotTrackingServiceSocket : public QObject
{
    Q_OBJECT
public:
    BotTrackingServiceSocket(TeamSettings& teamSettings);
    bool listen(const QHostAddress& address = QHostAddress::Any, unsigned short port = 0);

signals:
    void newBotLocations(const QVector<BotInfo>& bots) const;

private:
    QSharedPointer<MessageMediator> m_mediator;
    QTcpServer m_botTrackingTcpServer;
    TeamSettings& m_teamSettings;
};

#endif // BOTTRACKINGSERVICESOCKET_H
