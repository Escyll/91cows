// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "RemoteControllerSocket.h"

#include <MessageMediatorTcpSocket.h>
#include <QJsonDocument>
#include <RemoteControllerCommands.h>

RemoteControllerSocket::RemoteControllerSocket()
{
    QObject::connect(&m_remoteControllerTcpServer, &QTcpServer::newConnection, this, [this]
    {
        m_mediator = QSharedPointer<MessageMediatorTcpSocket>::create(m_remoteControllerTcpServer.nextPendingConnection());
        QObject::connect(m_mediator.get(), qOverload<const QJsonObject&>(&MessageMediator::jsonMessageReceived), this, [this](const QJsonObject& jsonMessage)
        {
            qDebug().noquote() << QJsonDocument(jsonMessage).toJson(QJsonDocument::JsonFormat::Compact);
            parseCommand(jsonMessage);
        });
        QObject::connect(m_mediator.get(), &MessageMediator::disconnected, this, [this]
        {
            m_mediator.reset();
        });
    });
}

bool RemoteControllerSocket::listen(const QHostAddress& address, unsigned short port)
{
    return m_remoteControllerTcpServer.listen(address, port);
}

void RemoteControllerSocket::parseCommand(const QJsonObject& command) const
{
    auto commandType = command["commandType"].toString();
    if (commandType == CreateGameCommand::commandType)
    {
        auto createGameCommand = CreateGameCommand::fromJson(command);
        emit createGame(createGameCommand.gameOptions);
    }
    if (commandType == StartGameCommand::commandType)
    {
        emit startGame();
    }
    if (commandType == StopGameCommand::commandType)
    {
        emit stopGame();
    }
}

