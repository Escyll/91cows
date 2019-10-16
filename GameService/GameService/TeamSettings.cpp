// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "TeamSettings.h"

#include <QDataStream>

TeamSettings::TeamSettings()
{
//    QByteArray colorData = colorDataMap.value("colorData").toByteArray();
//    QDataStream colorStream(&colorData, QIODevice::ReadOnly);
//    colorStream >> colorMap;

//    QByteArray teamNameData = teamNameDataMap.value("teamNameData").toByteArray();
//    QDataStream teamNameStream(&teamNameData, QIODevice::ReadOnly);
//    teamNameStream >> teamNameMap;
    colorDataMap.beginGroup("colorData");
    QStringList keys = colorDataMap.childKeys();
    foreach (QString key, keys) {
         colorMap[key.toInt()] = static_cast<unsigned int>(colorDataMap.value(key).toInt());
    }
    colorDataMap.endGroup();

    teamNameDataMap.beginGroup("teamNameData");
    keys = teamNameDataMap.childKeys();
    foreach (QString key, keys) {
         teamNameMap[key.toInt()] = teamNameDataMap.value(key).toString();
    }
    teamNameDataMap.endGroup();
}

void TeamSettings::registerTeam(int arucoId, const QString& teamName, const QColor& color)
{
//    colorMap[arucoId] = color;
//    QByteArray colorData;
//    QDataStream colorStream(&colorData, QIODevice::WriteOnly);
//    colorStream << colorMap;
//    colorDataMap.setValue("colorData", colorData);

    colorMap[arucoId] = color;
    colorDataMap.beginGroup("colorData");
    auto i = colorMap.constBegin();
    while (i != colorMap.constEnd()) {
         colorDataMap.setValue(QString::number(i.key()), i.value().rgb());
         ++i;
     }
    colorDataMap.endGroup();

    teamNameMap[arucoId] = teamName;
//    QByteArray teamNameData;
//    QDataStream teamNameStream(&teamNameData, QIODevice::WriteOnly);
//    teamNameStream << teamNameMap;
//    teamNameDataMap.setValue("teamNameData", teamNameData);
    teamNameDataMap.beginGroup("teamNameData");
    auto j = teamNameMap.constBegin();
    while (j != teamNameMap.constEnd()) {
         teamNameDataMap.setValue(QString::number(j.key()), j.value());
         ++j;
     }
    teamNameDataMap.endGroup();
}
