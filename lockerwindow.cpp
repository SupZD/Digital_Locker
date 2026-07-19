#include "lockerwindow.h"
#include "ui_lockerwindow.h"
#include "profilewindow.h"
#include "User.h"

LockerWindow::LockerWindow(const QString &username, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LockerWindow)
    , currentUsername(username)
{
    ui->setupUi(this);

    setWindowTitle("Digital Locker");
    ui->welcomeLabel->setText("Welcome, " + username + "!");
}

LockerWindow::~LockerWindow()
{
    delete ui;
}

void LockerWindow::on_profileButton_clicked()
{
    ProfileWindow profileWindow(currentUsername, this);
    profileWindow.exec();

    // The display name may have changed, so refresh the greeting.
    const UserProfile profile = User::getProfile(currentUsername);
    const QString nameToShow = profile.displayName.isEmpty() ? currentUsername : profile.displayName;
    ui->welcomeLabel->setText("Welcome, " + nameToShow + "!");
}

void LockerWindow::on_logoutButton_clicked()
{
    close();
}
