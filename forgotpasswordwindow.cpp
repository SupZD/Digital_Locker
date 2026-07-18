#include "forgotpasswordwindow.h"
#include "User.h"
#include "ui_forgotpasswordwindow.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

ForgotPasswordWindow::ForgotPasswordWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgotPasswordWindow)
{
    ui->setupUi(this);

    setWindowTitle("Reset Password");

    setStepTwoEnabled(false);
    ui->questionLabel->clear();
    ui->statusLabel->clear();

    ui->usernameLineEdit->setFocus();
}

ForgotPasswordWindow::~ForgotPasswordWindow()
{
    delete ui;
}

void ForgotPasswordWindow::showStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(isError ? "color: #c0392b; font-weight: bold;"
                                           : "color: #27ae60; font-weight: bold;");
}

void ForgotPasswordWindow::setStepTwoEnabled(bool enabled)
{
    ui->answerLineEdit->setEnabled(enabled);
    ui->newPasswordLineEdit->setEnabled(enabled);
    ui->confirmPasswordLineEdit->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
}

void ForgotPasswordWindow::on_showPasswordCheck_toggled(bool checked)
{
    const QLineEdit::EchoMode mode = checked ? QLineEdit::Normal : QLineEdit::Password;
    ui->newPasswordLineEdit->setEchoMode(mode);
    ui->confirmPasswordLineEdit->setEchoMode(mode);
}

void ForgotPasswordWindow::on_findAccountButton_clicked()
{
    const QString username = ui->usernameLineEdit->text().trimmed();

    if (username.isEmpty()) {
        showStatus("Please enter your username.", true);
        return;
    }

    const QString question = User::securityQuestion(username);

    if (question.isEmpty()) {
        showStatus("No account found with that username.", true);
        setStepTwoEnabled(false);
        ui->questionLabel->clear();
        return;
    }

    ui->questionLabel->setText(question);
    setStepTwoEnabled(true);
    showStatus("Answer the question below to continue.", false);
    ui->answerLineEdit->setFocus();
}

void ForgotPasswordWindow::on_resetButton_clicked()
{
    const QString username = ui->usernameLineEdit->text().trimmed();
    const QString answer = ui->answerLineEdit->text().trimmed();
    const QString newPassword = ui->newPasswordLineEdit->text().trimmed();
    const QString confirmPassword = ui->confirmPasswordLineEdit->text().trimmed();

    if (answer.isEmpty()) {
        showStatus("Please answer the security question.", true);
        return;
    }

    if (!User::verifySecurityAnswer(username, answer)) {
        showStatus("That answer doesn't match our records.", true);
        return;
    }

    if (newPassword.length() < 6) {
        showStatus("New password must be at least 6 characters.", true);
        return;
    }

    if (newPassword != confirmPassword) {
        showStatus("Passwords don't match.", true);
        return;
    }

    if (User::resetPassword(username, newPassword)) {
        QMessageBox::information(this, "Success", "Password reset successfully.");
        accept();
    } else {
        showStatus("Could not reset password. Please try again.", true);
    }
}