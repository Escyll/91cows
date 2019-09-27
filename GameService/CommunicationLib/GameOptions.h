// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

#include <QJsonObject>
#include <QMap>
#include <QPointF>
#include <QSize>

class GameOptions
{
public:
    enum class ActionItemType
    {
        Coin = 0,
        TreasureChest = 1,
        EmptyChest = 2,
        MimicChest = 3,
        SpikeTrap = 4,
        Bottle = 5,
        TestTube = 6
    };
    GameOptions(const QSize& mazeSize, bool removeDeadEnds, int numberOfWallRemovals, const QMap<ActionItemType, int> numberOfActionItems);

    static QString actionItemTypeToString(ActionItemType actionItemType);

    static GameOptions fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson();

public:
    QSize mazeSize;
    bool removeDeadEnds;
    int numberOfWallRemovals;
    QMap<ActionItemType, int> numberOfActionItems;
};

#endif // GAMEOPTIONS_H
