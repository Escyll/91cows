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
    Q_INVOKABLE void createGame(const QSize mazeSize, bool removeDeadEnds, int numberOfWallsToRemove,
                                int coins = 20,
                                int treasureChests = 15,
                                int emptyChests = 17,
                                int mimicChests = 13,
                                int spikeTraps = 18,
                                int bottles = 17,
                                int testTubes = 12);
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();

private:
    QSharedPointer<MessageMediator> m_messageMediator;
};

#endif // REMOTECONTROLLER_H
