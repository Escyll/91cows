// Copyright (c) 2019 by Phenom-World B.V.
// All rights reserved. This file includes confidential and proprietary information of Phenom-World B.V.

#ifndef VISUALIZERCOMMUNICATOR_H
#define VISUALIZERCOMMUNICATOR_H

#include <QObject>

class VisualizerCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit VisualizerCommunicator(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VISUALIZERCOMMUNICATOR_H
