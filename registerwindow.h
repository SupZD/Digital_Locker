#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class RegisterWindow;
}
QT_END_NAMESPACE

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow() override;

private slots:
    void on_registerButton_clicked();
    void on_showPasswordCheck_toggled(bool checked);

private:
    void showStatus(const QString &message, bool isError);

    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H