// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

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
