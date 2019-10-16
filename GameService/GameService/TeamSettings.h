// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef TEAMSETTINGS_H
#define TEAMSETTINGS_H

#include <QColor>
#include <QMap>
#include <QSettings>

class TeamSettings
{
public:
    TeamSettings();
    void registerTeam(int arucoId, const QString& teamName, const QColor& color);

    QMap<int, QColor> colorMap;
    QMap<int, QString> teamNameMap;
    QSettings colorDataMap;
    QSettings teamNameDataMap;
};

#endif // TEAMSETTINGS_H
