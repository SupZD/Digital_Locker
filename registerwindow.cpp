#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "User.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    setWindowTitle("Create Account");

    ui->securityQuestionCombo->addItem("What is your pet's name?");
    ui->securityQuestionCombo->addItem("What is your mother's maiden name?");
    ui->securityQuestionCombo->addItem("What was the name of your first school?");
    ui->securityQuestionCombo->addItem("What is your favorite book?");
    ui->securityQuestionCombo->addItem("What city were you born in?");

    connect(ui->passwordLineEdit,
            &QLineEdit::returnPressed,
            ui->registerButton,
            &QPushButton::click);

    ui->registerButton->setDefault(true);
    ui->registerButton->setAutoDefault(true);

    ui->statusLabel->clear();
    ui->usernameLineEdit->setFocus();
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::showStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(
        isError ? "color: #c0392b; font-weight: bold;"
                : "color: #27ae60; font-weight: bold;");
}

void RegisterWindow::on_showPasswordCheck_toggled(bool checked)
{
    ui->passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void RegisterWindow::on_registerButton_clicked()
{
    const QString username = ui->usernameLineEdit->text().trimmed();
    const QString password = ui->passwordLineEdit->text().trimmed();
    const QString securityQuestion = ui->securityQuestionCombo->currentText();
    const QString securityAnswer = ui->securityAnswerLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty())
    {
        showStatus("Please fill all fields.", true);
        return;
    }

    if (username.length() < 3)
    {
        showStatus("Username must be at least 3 characters.", true);
        return;
    }

    if (password.length() < 6)
    {
        showStatus("Password must be at least 6 characters.", true);
        return;
    }

    if (securityAnswer.isEmpty())
    {
        showStatus("Please answer the security question.", true);
        return;
    }

    if (User::userExists(username))
    {
        showStatus("That username is already taken.", true);
        return;
    }

    if (User::saveUser(username, password, securityQuestion, securityAnswer))
    {
        QMessageBox::information(this, "Success", "Registration Successful");
        accept();
    }
    else
    {
        showStatus("Could not save account. Please try again.", true);
        QMessageBox::warning(this, "Error", "Could not save user");
    }
}