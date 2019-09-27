// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef VISUALIZATIONSOCKET_H
#define VISUALIZATIONSOCKET_H

#include <MessageMediator.h>
#include <QHostAddress>
#include <QTcpServer>

class VisualizationSocket : public QObject
{
    Q_OBJECT
public:
    VisualizationSocket();

    bool listen(const QHostAddress &address = QHostAddress::Any, unsigned short port = 0);
    void sendState(const QJsonObject& doc);

private:
    QSharedPointer<MessageMediator> m_mediator;
    QTcpServer m_visualizationServer;
};

#endif // VISUALIZATIONSOCKET_H
