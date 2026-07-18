#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    // Registers a new user. Returns false if the username already exists
    // or the credentials file could not be written.
    static bool saveUser(const QString &username, const QString &password);

    // Verifies a username/password pair against stored (hashed) credentials.
    static bool authenticate(const QString &username, const QString &password);

    // Returns true if a username is already registered.
    static bool userExists(const QString &username);

private:
    static QString hashPassword(const QString &password);
    static QString usersFilePath();
};

#endif // USER_H