// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTCLIENTSOCKETS_H
#define BOTCLIENTSOCKETS_H

#include <MessageMediator.h>
#include <QTcpServer>

class BotClientSockets : public QObject
{
    Q_OBJECT
public:
    BotClientSockets();

    bool listen(const QHostAddress &address = QHostAddress::Any, unsigned short port = 0);
    void sendState(const QJsonObject& doc);

private:
    QList<QSharedPointer<MessageMediator>> m_botClients;
    QTcpServer m_botClientServer;
};

#endif // BOTCLIENTSOCKETS_H
