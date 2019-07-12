#include "MessageMediatorTcpSocket.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QObject>

MessageMediatorTcpSocket::MessageMediatorTcpSocket(QTcpSocket* socket)
    : m_socket(socket)
    , m_textStream(&m_string)
{
    m_socket->setParent(this);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, [this]
    {
        m_textStream << m_socket->readAll();
        QString object;
        while (m_textStream.readLineInto(&object))
        {
            emitAsJsonObject(object.toLatin1());
        }
    });
}

void MessageMediatorTcpSocket::sendMessage(const QByteArray& message)
{
    m_socket->write(message.trimmed() + '\n');
    m_socket->flush();
}
