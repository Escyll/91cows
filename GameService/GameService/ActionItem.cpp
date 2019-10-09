// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#include "ActionItem.h"

int ActionItem::nextId = 0;

bool operator==(const ActionItem& lhs, const ActionItem& rhs)
{
    return lhs.id == rhs.id;
}
