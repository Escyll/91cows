// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "CollisionDetector.h"

#include <QPair>
#include <QVector>

double interpolate(double a, double b, double t)
{
    return a + t*(b-a);
}

bool CollisionDetector::collide(const LineSegment& a, const LineSegment& b)
{
    using LinePointPair = QPair<const LineSegment*, QPointF>;
    QVector<LinePointPair> ySortedPoints = { LinePointPair(&a, a.p1), LinePointPair(&a, a.p2), LinePointPair(&b, b.p1), LinePointPair(&b, b.p2) };
    std::sort(ySortedPoints.begin(), ySortedPoints.end(), [](const LinePointPair& lhs, const LinePointPair& rhs) { return lhs.second.y() < rhs.second.y(); });
    QVector<LinePointPair> horizontalOrder;
    // Vertical line sweep
    for (auto sweepLpp : ySortedPoints)
    {
        switch (horizontalOrder.count())
        {
        case 0:
            horizontalOrder << sweepLpp;
            break;
        case 1:
            if (horizontalOrder[0].first == sweepLpp.first) // Segments are above each other
                return false;
            else
            {
                const LineSegment& other = *horizontalOrder[0].first;
                auto interpolationT = (sweepLpp.second.y() - other.p1.y()) / (other.p2.y() - other.p1.y());
                horizontalOrder << LinePointPair(&other, QPointF(interpolate(other.p1.x(), other.p2.x(), interpolationT), sweepLpp.second.y()));
                horizontalOrder[0] = sweepLpp;
            }
            break;
        case 2:
            const LineSegment& other = sweepLpp.first == &a ? b : a;
            auto interpolationT = (sweepLpp.second.y() - other.p1.y()) / (other.p2.y() - other.p1.y());
            LinePointPair sweepIntersectionPoint(&other, QPointF(interpolate(other.p1.x(), other.p2.x(), interpolationT), sweepLpp.second.y()));
            auto leftSegmentInHorizontalOrder = horizontalOrder[0].second.x()  < horizontalOrder[1].second.x() ? horizontalOrder[0].first : horizontalOrder[1].first;
            auto leftSegmentInCurrentSweep = sweepIntersectionPoint.second.x() < sweepLpp.second.x() ? sweepIntersectionPoint.first : sweepLpp.first;
            return leftSegmentInHorizontalOrder != leftSegmentInCurrentSweep;
        }
    }
    return false;
}
