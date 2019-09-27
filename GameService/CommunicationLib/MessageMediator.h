#ifndef MESSAGEMEDIATOR_H
#define MESSAGEMEDIATOR_H

#include <QObject>
#include <QJsonObject>

class MessageMediator : public QObject
{
    Q_OBJECT
public:
    void sendMessage(const QJsonObject& object);

protected:
    virtual void sendMessage(const QByteArray& message) = 0;
    void emitAsJsonObject(const QByteArray& message);

signals:
    void jsonMessageReceived(const QJsonObject& message);
    void disconnected();
};

#endif // MESSAGEMEDIATOR_H
