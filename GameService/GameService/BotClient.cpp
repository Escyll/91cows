// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotClient.h"

BotClient::BotClient(QScopedPointer<MessageMediator>&& mediator)
    : m_mediator(mediator.take())
{
}

void BotClient::sendNewBotLocations(const QJsonObject& jsonMessage)
{
    m_mediator->sendMessage(jsonMessage);
}
