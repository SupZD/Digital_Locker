#ifndef USER_H
#define USER_H

#include <QString>

// Extra, non-authentication info shown on a user's profile screen.
struct UserProfile
{
    QString username;
    QString securityQuestion;
    QString displayName;
    QString email;
    QString avatarPath;
};

class User
{
public:
    // Registers a new user along with a security question/answer used for
    // password recovery. Returns false if the username already exists or
    // the credentials file could not be written.
    static bool saveUser(const QString &username, const QString &password,
                         const QString &securityQuestion, const QString &securityAnswer);

    // Verifies a username/password pair against stored (hashed) credentials.
    static bool authenticate(const QString &username, const QString &password);

    // Returns true if a username is already registered.
    static bool userExists(const QString &username);

    // Returns the stored security question for a username, or an empty
    // string if the username doesn't exist.
    static QString securityQuestion(const QString &username);

    // Checks a security answer (case-insensitive) against the stored hash.
    static bool verifySecurityAnswer(const QString &username, const QString &answer);

    // Changes a password after verifying the current password.
    static bool changePassword(const QString &username, const QString &oldPassword,
                               const QString &newPassword);

    // Sets a new password directly. Intended to be called only after the
    // caller has already verified the user's security answer.
    static bool resetPassword(const QString &username, const QString &newPassword);

    // Returns the editable profile info (display name, email, avatar path)
    // for a username, along with their security question. Fields are empty
    // if not yet set.
    static UserProfile getProfile(const QString &username);

    // Updates the editable profile fields for an existing user. Returns
    // false if the username doesn't exist.
    static bool updateProfile(const QString &username, const QString &displayName,
                              const QString &email, const QString &avatarPath);

private:
    static QString hashPassword(const QString &password);
    static QString hashAnswer(const QString &answer);
    static QString usersFilePath();
    static bool updatePasswordHash(const QString &username, const QString &newHashedPassword);
};

#endif // USER_H