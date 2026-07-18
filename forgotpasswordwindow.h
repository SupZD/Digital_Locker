#ifndef FORGOTPASSWORDWINDOW_H
#define FORGOTPASSWORDWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ForgotPasswordWindow;
}
QT_END_NAMESPACE

class ForgotPasswordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordWindow(QWidget *parent = nullptr);
    ~ForgotPasswordWindow() override;

private slots:
    void on_findAccountButton_clicked();
    void on_resetButton_clicked();
    void on_showPasswordCheck_toggled(bool checked);

private:
    void showStatus(const QString &message, bool isError);
    void setStepTwoEnabled(bool enabled);

    Ui::ForgotPasswordWindow *ui;
};

#endif // FORGOTPASSWORDWINDOW_H