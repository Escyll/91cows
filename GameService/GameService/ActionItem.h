// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef ACTIONITEM_H
#define ACTIONITEM_H

#include <GameOptions.h>

class ActionItem
{
public:
    GameOptions::ActionItemType type;
    QPointF location;
    QPoint cellIndex;
    int id;
    static int nextId;
};

bool operator==(const ActionItem& lhs, const ActionItem& rhs);

#endif // ACTIONITEM_H
