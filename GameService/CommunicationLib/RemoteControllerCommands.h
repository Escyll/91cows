// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef REMOTECONTROLLERCOMMANDS_H
#define REMOTECONTROLLERCOMMANDS_H

#include "GameOptions.h"

#include <QColor>

class CreateGameCommand
{
public:
    GameOptions gameOptions;
    static const QString commandType;

    QJsonObject toJson();
    static CreateGameCommand fromJson(const QJsonObject& json);
};

class StartGameCommand
{
public:
    static const QString commandType;

    QJsonObject toJson();
};

class StopGameCommand
{
public:
    static const QString commandType;

    QJsonObject toJson();
};

class RegisterTeamCommand
{
public:
    static const QString commandType;

    QJsonObject toJson();
    static RegisterTeamCommand fromJson(const QJsonObject& json);

    int arucoId;
    QString teamName;
    QColor color;
};

#endif // REMOTECONTROLLERCOMMANDS_H
