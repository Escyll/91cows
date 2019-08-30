#include "BotClient.h"

BotClient::BotClient(QScopedPointer<MessageMediator>&& mediator)
    : m_mediator(mediator.take())
{
}

void BotClient::sendState(const QJsonObject& jsonMessage)
{
    m_mediator->sendMessage(jsonMessage);
}
