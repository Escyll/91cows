// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include "Game.h"

#include <QObject>

class GameRunner : public QObject
{
    Q_OBJECT
public:
    explicit GameRunner(QObject *parent = nullptr);

public:
    void createNewGame();
    void startGame();
    void stopGame();

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override;

signals:
    void sendOutRevealedState(const QJsonObject& revealedState);

private:
    Game m_game;
    int m_timerId;
};

#endif // GAMERUNNER_H
