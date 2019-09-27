// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "GameOptions.h"
#include <QJsonArray>

GameOptions::GameOptions(const QSize& mazeSize, bool removeDeadEnds, int numberOfWallRemovals, const QMap<ActionItemType, int> numberOfActionItems)
    : mazeSize(mazeSize)
    , removeDeadEnds(removeDeadEnds)
    , numberOfWallRemovals(numberOfWallRemovals)
    , numberOfActionItems(numberOfActionItems)
{
}

QString GameOptions::actionItemTypeToString(ActionItemType actionItemType)
{
    switch (actionItemType)
    {
    case ActionItemType::Coin:
        return "Coin";
    case ActionItemType::TreasureChest:
        return "TreasureChest";
    case ActionItemType::EmptyChest:
        return "EmptyChest";
    case ActionItemType::MimicChest:
        return "MimicChest";
    case ActionItemType::SpikeTrap:
        return "SpikeTrap";
    case ActionItemType::Bottle:
        return "Bottle";
    case ActionItemType::TestTube:
        return "TestTube";
    }
}

QJsonObject GameOptions::toJson()
{
    QJsonObject json;
    json["mazeSize"] = QJsonArray({mazeSize.width(), mazeSize.height()});
    json["removeDeadEnds"] = removeDeadEnds;
    json["numberOfWallsToRemove"] = numberOfWallRemovals;
    json["numberOfActionItems"] = QJsonObject
    {
        {"Coin", numberOfActionItems[ActionItemType::Coin]},
        {"TreasureChest", numberOfActionItems[ActionItemType::TreasureChest]},
        {"EmptyChest", numberOfActionItems[ActionItemType::EmptyChest]},
        {"MimicChest", numberOfActionItems[ActionItemType::MimicChest]},
        {"SpikeTrap", numberOfActionItems[ActionItemType::SpikeTrap]},
        {"Bottle", numberOfActionItems[ActionItemType::Bottle]},
        {"TestTube", numberOfActionItems[ActionItemType::TestTube]}
    };
    return json;
}

GameOptions GameOptions::fromJson(const QJsonObject& jsonObject)
{
    auto mazeSizeArray = jsonObject["mazeSize"].toArray({0, 0});
    QMap<ActionItemType, int> numberOfActionItems;
    numberOfActionItems[ActionItemType::Coin] = jsonObject["numberOfActionItems"].toObject()["Coin"].toInt();
    numberOfActionItems[ActionItemType::TreasureChest] = jsonObject["numberOfActionItems"].toObject()["TreasureChest"].toInt();
    numberOfActionItems[ActionItemType::EmptyChest] = jsonObject["numberOfActionItems"].toObject()["EmptyChest"].toInt();
    numberOfActionItems[ActionItemType::MimicChest] = jsonObject["numberOfActionItems"].toObject()["MimicChest"].toInt();
    numberOfActionItems[ActionItemType::SpikeTrap] = jsonObject["numberOfActionItems"].toObject()["SpikeTrap"].toInt();
    numberOfActionItems[ActionItemType::Bottle] = jsonObject["numberOfActionItems"].toObject()["Bottle"].toInt();
    numberOfActionItems[ActionItemType::TestTube] = jsonObject["numberOfActionItems"].toObject()["TestTube"].toInt();
    return GameOptions(QSize(mazeSizeArray[0].toInt(0), mazeSizeArray[1].toInt(0)),
            jsonObject["removeDeadEnds"].toBool(false),
            jsonObject["numberOfWallsToRemove"].toInt(0),
            numberOfActionItems);
}
