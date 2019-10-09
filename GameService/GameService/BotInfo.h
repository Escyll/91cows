// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef BOTINFO_H
#define BOTINFO_H

#include <QColor>
#include <QElapsedTimer>
#include <QString>
#include <QVector2D>

class BotInfo
{
public:
    BotInfo() = default;
    BotInfo(int arucoId, const QString& name, int score, const QColor& color,const QPointF& location, const QVector2D& forward, const QVector2D& right);

    int arucoId;
    QString name;
    int score;
    QColor color;
    QPointF location;
    QVector2D forward;
    QVector2D right;
    QElapsedTimer wallColisionTimer;
    QElapsedTimer spikeCollisionTimer;
};

#endif // BOTINFO_H
