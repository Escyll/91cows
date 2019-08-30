// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef REMOTEACCESSPOINT_H
#define REMOTEACCESSPOINT_H

#include <QObject>

class RemoteAccessPoint : public QObject
{
    Q_OBJECT
public:
    explicit RemoteAccessPoint(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REMOTEACCESSPOINT_H
