#include "User.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDebug>

// Each line in users.txt looks like:
//   username|passwordHash|securityQuestion|answerHash

QString User::usersFilePath()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/users.txt";
}

QString User::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}

QString User::hashAnswer(const QString &answer)
{
    // Normalize (trim + lowercase) so "Fluffy" and " fluffy " both match.
    QByteArray hash = QCryptographicHash::hash(
        answer.trimmed().toLower().toUtf8(), QCryptographicHash::Sha256);
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

bool User::saveUser(const QString &username, const QString &password,
                    const QString &securityQuestion, const QString &securityAnswer)
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
    out << username << "|"
        << hashPassword(password) << "|"
        << securityQuestion << "|"
        << hashAnswer(securityAnswer) << "\n";
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
        if (parts.size() >= 2 && parts.at(0) == username && parts.at(1) == hashed)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

QString User::securityQuestion(const QString &username)
{
    QFile file(usersFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream in(&file);
    while (!in.atEnd())
    {
        const QStringList parts = in.readLine().split('|');
        if (parts.size() >= 3 && parts.at(0) == username)
        {
            file.close();
            return parts.at(2);
        }
    }
    file.close();
    return QString();
}

bool User::verifySecurityAnswer(const QString &username, const QString &answer)
{
    QFile file(usersFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    const QString hashed = hashAnswer(answer);

    QTextStream in(&file);
    while (!in.atEnd())
    {
        const QStringList parts = in.readLine().split('|');
        if (parts.size() >= 4 && parts.at(0) == username && parts.at(3) == hashed)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool User::updatePasswordHash(const QString &username, const QString &newHashedPassword)
{
    QFile file(usersFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QStringList updatedLines;
    bool found = false;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split('|');
        if (parts.size() >= 2 && parts.at(0) == username)
        {
            parts[1] = newHashedPassword;
            line = parts.join('|');
            found = true;
        }
        updatedLines << line;
    }
    file.close();

    if (!found)
        return false;

    QFile outFile(usersFilePath());
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;

    QTextStream out(&outFile);
    for (const QString &l : updatedLines)
        out << l << "\n";
    outFile.close();

    return true;
}

bool User::changePassword(const QString &username, const QString &oldPassword,
                          const QString &newPassword)
{
    if (!authenticate(username, oldPassword))
        return false;

    return updatePasswordHash(username, hashPassword(newPassword));
}

bool User::resetPassword(const QString &username, const QString &newPassword)
{
    if (!userExists(username))
        return false;

    return updatePasswordHash(username, hashPassword(newPassword));
}