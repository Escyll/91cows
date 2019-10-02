#include "MessageMediator.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

void MessageMediator::sendMessage(const QJsonObject& object)
{
    sendMessage(QJsonDocument(object).toJson(QJsonDocument::JsonFormat::Compact));
}

void MessageMediator::emitAsJsonObject(const QByteArray& message)
{
    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(message, &parseError);
    if (parseError.error != QJsonParseError::NoError || !(doc.isObject() || doc.isArray()))
    {
        qDebug() << "Received malformed data";
        return;
    }
    if (doc.isObject())
    {
        emit jsonMessageReceived(doc.object());
    }
    if (doc.isArray())
    {
        emit jsonMessageReceived(doc.array());
    }
}
