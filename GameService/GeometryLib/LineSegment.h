// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include <QPointF>

class LineSegment
{
public:
    LineSegment() = default;
    LineSegment(const QPointF& p1, const QPointF& p2);

    QPointF p1, p2;
};

#endif // LINESEGMENT_H
