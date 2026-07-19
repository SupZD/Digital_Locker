#ifndef LOCKERWINDOW_H
#define LOCKERWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LockerWindow;
}
QT_END_NAMESPACE

class LockerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LockerWindow(const QString &username, QWidget *parent = nullptr);
    ~LockerWindow() override;

private slots:
    void on_profileButton_clicked();
    void on_logoutButton_clicked();

private:
    Ui::LockerWindow *ui;
    QString currentUsername;
};

#endif // LOCKERWINDOW_H
