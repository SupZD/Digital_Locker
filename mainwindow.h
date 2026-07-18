#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_showPasswordCheck_toggled(bool checked);
    void on_forgotPasswordButton_clicked();
    void on_changePasswordButton_clicked();

private:
    void showStatus(const QString &message, bool isError);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H