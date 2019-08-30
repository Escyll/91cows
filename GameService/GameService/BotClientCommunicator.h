// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTCLIENTCOMMUNICATOR_H
#define BOTCLIENTCOMMUNICATOR_H

#include "BotClient.h"

#include <QTcpServer>

class BotClientCommunicator : public QObject
{
    Q_OBJECT
public:
    BotClientCommunicator();

    bool listen(const QHostAddress &address = QHostAddress::Any, unsigned short port = 0);
    void sendState(const QJsonObject& doc);

private:
    QList<QSharedPointer<BotClient>> m_botClients;
    QTcpServer m_botClientServer;
};

#endif // BOTCLIENTCOMMUNICATOR_H
