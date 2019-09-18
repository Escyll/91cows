// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "LineSegment.h"

class CollisionDetector
{
public:
    static bool collide(const LineSegment& a, const LineSegment& b);
};

#endif // COLLISIONDETECTOR_H
