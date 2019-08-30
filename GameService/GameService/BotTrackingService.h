// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTTRACKINGSERVICE_H
#define BOTTRACKINGSERVICE_H

#include "BotLocationReceiver.h"

#include <QScopedPointer>
#include <QTcpServer>

class BotTrackingService : public QObject
{
    Q_OBJECT
public:
    BotTrackingService();
    bool listen(const QHostAddress& address = QHostAddress::Any, unsigned short port = 0);

signals:
    void newBotLocations(const QJsonObject& jsonMessage) const;

private:
    QScopedPointer<BotLocationReceiver> m_botLocationReceiver;
    QTcpServer m_botTrackingTcpServer;
};

#endif // BOTTRACKINGSERVICE_H
