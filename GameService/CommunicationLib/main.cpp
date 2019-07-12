#include "JsonWebToken.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return -1;
    }

    QByteArray secret(argv[1]);

    QCoreApplication a(argc, argv);

    QJsonObject jwtPayload;
    jwtPayload["userId"] = "secretjees";
    QJsonArray roles;
    roles.append("admin");
    jwtPayload["roles"] = roles;

    JsonWebToken jwt(jwtPayload, secret);
    JsonWebToken jwt2(jwt.toToken());
    qDebug() << (jwt2.isValid(secret) ? "Working good" : "Working not so good");

    return a.exec();
}
