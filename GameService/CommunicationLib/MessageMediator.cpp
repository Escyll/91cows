#include "MessageMediator.h"

#include <QJsonDocument>
#include <QDebug>

void MessageMediator::sendMessage(const QJsonObject& object)
{
    sendMessage(QJsonDocument(object).toJson(QJsonDocument::JsonFormat::Compact));
}

void MessageMediator::emitAsJsonObject(const QByteArray& message)
{
    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(message, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject())
    {
        qDebug() << "Received malformed data";
        return;
    }
    emit jsonMessageReceived(doc.object());
}
