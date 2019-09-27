// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef REMOTECONTROLLERSOCKET_H
#define REMOTECONTROLLERSOCKET_H

#include <GameOptions.h>
#include <MessageMediator.h>
#include <QHostAddress>
#include <QObject>
#include <QTcpServer>

class RemoteControllerSocket : public QObject
{
    Q_OBJECT
public:
    RemoteControllerSocket();
    bool listen(const QHostAddress& address = QHostAddress::Any, unsigned short port = 0);

signals:
    void createGame(const GameOptions& gameOptions) const;
    void startGame() const;
    void stopGame() const;

private:
    void parseCommand(const QJsonObject& command) const;

private:
    QSharedPointer<MessageMediator> m_mediator;
    QTcpServer m_remoteControllerTcpServer;
};

#endif // REMOTECONTROLLERSOCKET_H
