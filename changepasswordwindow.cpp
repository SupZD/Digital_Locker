#include "changepasswordwindow.h"
#include "User.h"
#include "ui_changepasswordwindow.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordWindow)
{
    ui->setupUi(this);

    setWindowTitle("Change Password");

    connect(ui->confirmPasswordLineEdit,
            &QLineEdit::returnPressed,
            ui->changeButton,
            &QPushButton::click);

    ui->changeButton->setDefault(true);
    ui->changeButton->setAutoDefault(true);

    ui->statusLabel->clear();
    ui->usernameLineEdit->setFocus();
}

ChangePasswordWindow::~ChangePasswordWindow()
{
    delete ui;
}

void ChangePasswordWindow::showStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(isError ? "color: #c0392b; font-weight: bold;"
                                           : "color: #27ae60; font-weight: bold;");
}

void ChangePasswordWindow::on_showPasswordCheck_toggled(bool checked)
{
    const QLineEdit::EchoMode mode = checked ? QLineEdit::Normal : QLineEdit::Password;
    ui->currentPasswordLineEdit->setEchoMode(mode);
    ui->newPasswordLineEdit->setEchoMode(mode);
    ui->confirmPasswordLineEdit->setEchoMode(mode);
}

void ChangePasswordWindow::on_changeButton_clicked()
{
    const QString username = ui->usernameLineEdit->text().trimmed();
    const QString currentPassword = ui->currentPasswordLineEdit->text().trimmed();
    const QString newPassword = ui->newPasswordLineEdit->text().trimmed();
    const QString confirmPassword = ui->confirmPasswordLineEdit->text().trimmed();

    if (username.isEmpty() || currentPassword.isEmpty() || newPassword.isEmpty()) {
        showStatus("Please fill all fields.", true);
        return;
    }

    if (newPassword.length() < 6) {
        showStatus("New password must be at least 6 characters.", true);
        return;
    }

    if (newPassword != confirmPassword) {
        showStatus("New passwords don't match.", true);
        return;
    }

    if (!User::authenticate(username, currentPassword)) {
        showStatus("Current username or password is incorrect.", true);
        return;
    }

    if (User::changePassword(username, currentPassword, newPassword)) {
        QMessageBox::information(this, "Success", "Password changed successfully.");
        accept();
    } else {
        showStatus("Could not change password. Please try again.", true);
    }
}