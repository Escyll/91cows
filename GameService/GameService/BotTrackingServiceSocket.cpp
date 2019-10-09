// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotTrackingServiceSocket.h"

#include <MessageMediatorTcpSocket.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>

BotTrackingServiceSocket::BotTrackingServiceSocket()
{
    QObject::connect(&m_botTrackingTcpServer, &QTcpServer::newConnection, this, [this]()
    {
        m_mediator = QSharedPointer<MessageMediatorTcpSocket>::create(m_botTrackingTcpServer.nextPendingConnection());
        QObject::connect(m_mediator.get(), qOverload<const QJsonArray&>(&MessageMediator::jsonMessageReceived), this, [this](const QJsonArray& jsonMessage)
        {
            QVector<BotInfo> parsedBotInfo;
            for (auto botValue : jsonMessage)
            {
                QJsonObject robotObject = botValue.toObject()["robot"].toObject();
                auto arucoId = robotObject["arucoId"].toInt();
                auto position = robotObject["position"].toArray();
                auto xorient = robotObject["xorient"].toArray();
                auto yorient = robotObject["yorient"].toArray();

                auto right = 0.01666666666f*QVector2D(static_cast<float>(xorient[0].toDouble()), static_cast<float>(-xorient[1].toDouble())).normalized();
                auto forward = 0.01666666666f*QVector2D(static_cast<float>(yorient[0].toDouble()), static_cast<float>(-yorient[1].toDouble())).normalized();
                parsedBotInfo << BotInfo(arucoId,
                                         QPointF(position[0].toDouble(), 1 - position[1].toDouble()),
                                         forward,
                                         right);
            }
            emit newBotLocations(parsedBotInfo);
        });
        QObject::connect(m_mediator.get(), &MessageMediator::disconnected, this, [this]
        {
//            m_mediator.reset();
        });
    });
}

bool BotTrackingServiceSocket::listen(const QHostAddress& address, unsigned short port)
{
    return m_botTrackingTcpServer.listen(address, port);
}
