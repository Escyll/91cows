#include "BotLocationReceiver.h"

#include <QDebug>
#include <QJsonDocument>
#include <QThread>

BotLocationReceiver::BotLocationReceiver(QScopedPointer<MessageMediator>&& mediator)
    : m_mediator(mediator.take())
{
    QObject::connect(m_mediator.get(), &MessageMediator::jsonMessageReceived, this, [this](const QJsonObject& jsonMessage)
    {
        qDebug().noquote() << QJsonDocument(jsonMessage).toJson(QJsonDocument::JsonFormat::Compact);
        emit newBotLocations(jsonMessage);
    });
}
