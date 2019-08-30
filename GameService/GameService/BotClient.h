#ifndef BOTCLIENT_H
#define BOTCLIENT_H

#include <MessageMediator.h>
#include <QObject>

class BotClient : public QObject
{
    Q_OBJECT
public:
    explicit BotClient(QScopedPointer<MessageMediator>&& mediator);

    void sendState(const QJsonObject& jsonMessage);

private:
    QScopedPointer<MessageMediator> m_mediator;
};

#endif // BOTCLIENT_H
