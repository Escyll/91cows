// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "RemoteControllerCommands.h"

const QString CreateGameCommand::commandType = "createGame";
const QString StartGameCommand::commandType = "startGame";
const QString StopGameCommand::commandType = "stopGame";
const QString RegisterTeamCommand::commandType = "registerTeam";

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


QJsonObject RegisterTeamCommand::toJson()
{
    QJsonObject json;
    json["commandType"] = commandType;
    json["arucoId"] = arucoId;
    json["teamName"] = teamName;
    json["color"] = static_cast<int>(color.rgba());
    return json;
}

RegisterTeamCommand RegisterTeamCommand::fromJson(const QJsonObject& json)
{
    return {
        json["arucoId"].toInt(),
        json["teamName"].toString(),
        static_cast<unsigned int>(json["color"].toInt())
    };
}
