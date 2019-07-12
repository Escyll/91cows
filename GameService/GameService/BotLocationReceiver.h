#ifndef GAMESERVICEREQUESTHANDLER_H
#define GAMESERVICEREQUESTHANDLER_H

#include <QObject>
#include <QScopedPointer>
#include <MessageMediator.h>

class BotLocationReceiver : public QObject
{
    Q_OBJECT
public: 
    explicit BotLocationReceiver(QScopedPointer<MessageMediator>&& mediator);

signals:
    void newBotLocations(const QJsonObject& jsonMessage) const;

private:
    QScopedPointer<MessageMediator> m_mediator;
};

#endif // GAMESERVICEREQUESTHANDLER_H
