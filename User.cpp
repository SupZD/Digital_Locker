#include "User.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDebug>

QString User::usersFilePath()
{
    // Store credentials in a proper per-user app-data folder instead of
    // whatever the current working directory happens to be.
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/users.txt";
}

QString User::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}

bool User::userExists(const QString &username)
{
    QFile file(usersFilePath());
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        const QStringList parts = in.readLine().split('|');
        if (!parts.isEmpty() && parts.at(0) == username)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool User::saveUser(const QString &username, const QString &password)
{
    if (userExists(username))
        return false;

    QFile file(usersFilePath());
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qWarning() << "Could not open users file for writing:" << usersFilePath();
        return false;
    }

    QTextStream out(&file);
    out << username << "|" << hashPassword(password) << "\n";
    file.close();
    return true;
}

bool User::authenticate(const QString &username, const QString &password)
{
    QFile file(usersFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    const QString hashed = hashPassword(password);

    QTextStream in(&file);
    while (!in.atEnd())
    {
        const QStringList parts = in.readLine().split('|');
        if (parts.size() == 2 && parts.at(0) == username && parts.at(1) == hashed)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}