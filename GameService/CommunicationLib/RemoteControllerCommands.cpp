// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "RemoteControllerCommands.h"

const QString CreateGameCommand::commandType = "createGame";
const QString StartGameCommand::commandType = "startGame";
const QString StopGameCommand::commandType = "stopGame";

QJsonObject CreateGameCommand::toJson()
{
    QJsonObject json;
    json["commandType"] = commandType;
    json["gameOptions"] = gameOptions.toJson();
    return json;
}

CreateGameCommand CreateGameCommand::fromJson(const QJsonObject& json)
{
    return {GameOptions::fromJson(json["gameOptions"].toObject())};
}

QJsonObject StartGameCommand::toJson()
{
    QJsonObject json;
    json["commandType"] = commandType;
    return json;
}

QJsonObject StopGameCommand::toJson()
{
    QJsonObject json;
    json["commandType"] = commandType;
    return json;
}
