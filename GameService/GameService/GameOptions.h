// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include <QSize>

#ifndef GAMEOPTIONS_H
#define GAMEOPTIONS_H

class GameOptions
{
public:
    GameOptions(const QSize& mazeSize);
    QSize mazeSize;
};

#endif // GAMEOPTIONS_H
