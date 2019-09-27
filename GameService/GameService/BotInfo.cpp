// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "BotInfo.h"

BotInfo::BotInfo(int arucoId, const QPointF& location, const QVector2D& forward, const QVector2D& right)
    : arucoId(arucoId)
    , location(location)
    , forward(forward)
    , right(right)
{
}
