#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ProfileWindow;
}
QT_END_NAMESPACE

class ProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWindow(const QString &username, QWidget *parent = nullptr);
    ~ProfileWindow() override;

private slots:
    void on_changePhotoButton_clicked();
    void on_saveButton_clicked();

private:
    void loadProfile();
    void showStatus(const QString &message, bool isError);

    Ui::ProfileWindow *ui;
    QString currentUsername;
    QString selectedAvatarPath;
};

#endif // PROFILEWINDOW_H
