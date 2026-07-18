#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChangePasswordWindow;
}
QT_END_NAMESPACE

class ChangePasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordWindow(QWidget *parent = nullptr);
    ~ChangePasswordWindow() override;

private slots:
    void on_changeButton_clicked();
    void on_showPasswordCheck_toggled(bool checked);

private:
    void showStatus(const QString &message, bool isError);

    Ui::ChangePasswordWindow *ui;
};

#endif // CHANGEPASSWORDWINDOW_H