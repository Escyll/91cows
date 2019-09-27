// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTINFO_H
#define BOTINFO_H

#include <QVector2D>

class BotInfo
{
public:
    BotInfo() = default;
    BotInfo(int arucoId, const QPointF& location, const QVector2D& forward, const QVector2D& right);

    int arucoId;
    QPointF location;
    QVector2D forward;
    QVector2D right;
};

#endif // BOTINFO_H
