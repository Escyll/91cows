// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include <QHostAddress>
#include <QTcpServer>

#ifndef VISUALIZERACCESSPOINT_H
#define VISUALIZERACCESSPOINT_H


class VisualizerAccessPoint
{
public:
    VisualizerAccessPoint();

    bool listen(const QHostAddress &address = QHostAddress::Any, unsigned short port = 0);
    void sendState(const QJsonObject& doc);

private:
    QTcpServer m_botClientServer;
};

#endif // VISUALIZERACCESSPOINT_H
