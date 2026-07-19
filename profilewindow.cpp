#include "profilewindow.h"
#include "ui_profilewindow.h"
#include "User.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QPixmap>

ProfileWindow::ProfileWindow(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileWindow)
    , currentUsername(username)
{
    ui->setupUi(this);

    setWindowTitle("My Profile");
    ui->statusLabel->clear();

    loadProfile();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::loadProfile()
{
    const UserProfile profile = User::getProfile(currentUsername);

    ui->usernameValueLabel->setText(profile.username);
    ui->securityQuestionValueLabel->setText(
        profile.securityQuestion.isEmpty() ? "Not set" : profile.securityQuestion);
    ui->displayNameLineEdit->setText(profile.displayName);
    ui->emailLineEdit->setText(profile.email);

    selectedAvatarPath = profile.avatarPath;

    if (!selectedAvatarPath.isEmpty() && QFile::exists(selectedAvatarPath))
    {
        const QPixmap pix(selectedAvatarPath);
        ui->avatarLabel->setPixmap(
            pix.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    else
    {
        ui->avatarLabel->setText("No Photo");
    }
}

void ProfileWindow::on_changePhotoButton_clicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this, "Choose Profile Photo", QString(), "Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
        return;

    // Copy the chosen image into app data so the avatar survives even if
    // the original file is later moved, renamed, or deleted.
    const QString avatarsDir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/avatars";
    QDir().mkpath(avatarsDir);

    const QString extension = QFileInfo(filePath).suffix();
    const QString destPath = avatarsDir + "/" + currentUsername + "." + extension;

    QFile::remove(destPath);
    if (!QFile::copy(filePath, destPath))
    {
        showStatus("Could not load that image.", true);
        return;
    }

    selectedAvatarPath = destPath;

    const QPixmap pix(selectedAvatarPath);
    ui->avatarLabel->setPixmap(
        pix.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

void ProfileWindow::on_saveButton_clicked()
{
    const QString displayName = ui->displayNameLineEdit->text().trimmed();
    const QString email = ui->emailLineEdit->text().trimmed();

    if (User::updateProfile(currentUsername, displayName, email, selectedAvatarPath))
    {
        showStatus("Profile updated.", false);
    }
    else
    {
        showStatus("Could not update profile. Please try again.", true);
    }
}

void ProfileWindow::showStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(isError ? "color: #c0392b; font-weight: bold;"
                                           : "color: #27ae60; font-weight: bold;");
}
