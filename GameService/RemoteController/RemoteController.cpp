// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "RemoteController.h"

#include <GameOptions.h>
#include <MessageMediatorTcpSocket.h>
#include <RemoteControllerCommands.h>

void RemoteController::connect(const QString& address)
{
    auto socket = new QTcpSocket();
    socket->connectToHost(address, static_cast<quint16>(9935));
    m_messageMediator = QSharedPointer<MessageMediatorTcpSocket>::create(socket);
}

void RemoteController::createGame(const QSize mazeSize, bool removeDeadEnds, int numberOfWallsToRemove,
                                  int coins,
                                  int treasureChests,
                                  int emptyChests,
                                  int mimicChests,
                                  int spikeTraps,
                                  int bottles,
                                  int testTubes)
{
    if (m_messageMediator)
    {
        CreateGameCommand command {GameOptions(mazeSize, removeDeadEnds, numberOfWallsToRemove, QMap<GameOptions::ActionItemType, int> {
                                                   {GameOptions::ActionItemType::Coin, coins},
                                                   {GameOptions::ActionItemType::TreasureChest, treasureChests},
                                                   {GameOptions::ActionItemType::EmptyChest, emptyChests},
                                                   {GameOptions::ActionItemType::MimicChest, mimicChests},
                                                   {GameOptions::ActionItemType::SpikeTrap, spikeTraps},
                                                   {GameOptions::ActionItemType::Bottle, bottles},
                                                   {GameOptions::ActionItemType::TestTube, testTubes},
                                               })};
        m_messageMediator->sendMessage(command.toJson());
    }
}

void RemoteController::startGame()
{
    if (m_messageMediator)
    {
        StartGameCommand command;
        m_messageMediator->sendMessage(command.toJson());
    }
}

void RemoteController::stopGame()
{
    if (m_messageMediator)
    {
        StopGameCommand command;
        m_messageMediator->sendMessage(command.toJson());
    }
}

void RemoteController::registerTeam(int arucoId, const QString& teamName, const QColor& color)
{
    if (m_messageMediator)
    {
        RegisterTeamCommand command{arucoId, teamName, color};
        m_messageMediator->sendMessage(command.toJson());
    }
}
