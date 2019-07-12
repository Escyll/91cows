#ifndef MESSAGEMEDIATORTCPSOCKET_H
#define MESSAGEMEDIATORTCPSOCKET_H

#include "MessageMediator.h"

#include <QTcpSocket>

class MessageMediatorTcpSocket : public MessageMediator
{
public:
    explicit MessageMediatorTcpSocket(QTcpSocket* socket);

protected:
    void sendMessage(const QByteArray& message) override;

private:
    QTcpSocket* m_socket;
    QString m_string;
    QTextStream m_textStream;
};

#endif // MESSAGEMEDIATORTCPSOCKET_H
