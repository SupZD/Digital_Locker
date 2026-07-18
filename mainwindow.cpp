#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "User.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Enter key in the password field triggers Login
    connect(ui->passwordInput,
            &QLineEdit::returnPressed,
            ui->loginButton,
            &QPushButton::click);
    // Enter key in the username field jumps to the password field
    connect(ui->usernameInput,
            &QLineEdit::returnPressed,
            ui->passwordInput,
            qOverload<>(&QLineEdit::setFocus));

    ui->loginButton->setDefault(true);
    ui->loginButton->setAutoDefault(true);

    ui->statusLabel->clear();
    ui->usernameInput->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStatus(const QString &message, bool isError)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(
        isError ? "color: #c0392b; font-weight: bold;"
                : "color: #27ae60; font-weight: bold;");
}

void MainWindow::on_showPasswordCheck_toggled(bool checked)
{
    ui->passwordInput->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void MainWindow::on_loginButton_clicked()
{
    const QString username = ui->usernameInput->text().trimmed();
    const QString password = ui->passwordInput->text().trimmed();

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
        ui->passwordInput->clear();
        return;
    }

    if (!User::authenticate(username, password))
    {
        showStatus("Incorrect username or password.", true);
        ui->passwordInput->clear();
        return;
    }

    showStatus("Login successful. Welcome, " + username + "!", false);
    QMessageBox::information(this, "Success", "Login Successful!");

    ui->usernameInput->clear();
    ui->passwordInput->clear();
    ui->usernameInput->setFocus();
}

void MainWindow::on_registerButton_clicked()
{
    const QString username = ui->usernameInput->text().trimmed();
    const QString password = ui->passwordInput->text().trimmed();

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

    if (User::userExists(username))
    {
        showStatus("That username is already taken.", true);
        return;
    }

    const bool saved = User::saveUser(username, password);

    if (saved)
    {
        showStatus("Account registered. You can log in now.", false);
        QMessageBox::information(this, "Success", "Account Registered!");
    }
    else
    {
        showStatus("Could not save account. Please try again.", true);
        QMessageBox::warning(this, "Error", "Could not save account.");
    }

    ui->usernameInput->clear();
    ui->passwordInput->clear();
    ui->usernameInput->setFocus();
}