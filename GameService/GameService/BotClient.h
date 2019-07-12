// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTCLIENT_H
#define BOTCLIENT_H

#include <MessageMediator.h>
#include <QObject>

class BotClient : public QObject
{
    Q_OBJECT
public:
    explicit BotClient(QScopedPointer<MessageMediator>&& mediator);

    void sendNewBotLocations(const QJsonObject& jsonMessage);

private:
    QScopedPointer<MessageMediator> m_mediator;
};

#endif // BOTCLIENT_H
