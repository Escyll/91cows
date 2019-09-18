// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "GameRunner.h"

#include <QTimerEvent>

GameRunner::GameRunner(QObject *parent)
    : QObject(parent)
{
}

void GameRunner::createNewGame()
{
    m_game = Game(GameOptions(QSize(10, 10)));
}

void GameRunner::startGame()
{
    m_timerId = startTimer(100);
    m_game.start();
}

void GameRunner::stopGame()
{
    killTimer(m_timerId);
    m_game.stop();
}

void GameRunner::timerEvent(QTimerEvent* event)
{
    if (m_timerId == event->timerId())
    {
        m_game.handleTick();
//        auto obscuredState = m_game.getObscuredState();
        emit sendOutRevealedState(m_game.getRevealedState());
    }
}
