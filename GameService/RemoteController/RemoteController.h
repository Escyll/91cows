// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <MessageMediator.h>
#include <QSize>

class RemoteController : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void connect(const QString& address);
    Q_INVOKABLE void createGame(const QSize mazeSize,
                                bool removeDeadEnds,
                                int numberOfWallsToRemove,
                                int coins,
                                int treasureChests,
                                int emptyChests,
                                int mimicChests,
                                int spikeTraps,
                                int bottles,
                                int testTubes);
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void registerTeam(int arucoId, const QString& teamName, const QColor& color);

private:
    QSharedPointer<MessageMediator> m_messageMediator;
};

#endif // REMOTECONTROLLER_H
