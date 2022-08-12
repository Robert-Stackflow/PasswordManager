#ifndef USERHELPERDIALOG_H
#define USERHELPERDIALOG_H

#include <QWidget>

namespace Ui {
class UserHelperDialog;
}

class UserHelperDialog : public QWidget
{
    Q_OBJECT

public:
    enum mode{
        USERNAME,
        PASSWORD,
        DELETEACCOUNT
    };
    explicit UserHelperDialog(mode currentMode,QWidget *parent = nullptr);
    ~UserHelperDialog();
    void loadUsername(const QString& newUsername);
    void loadPassword(const QString& oldPassword, const QString& newPassword);
    void setWindowCustomTitle(const QString& newWindowTitle);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *event);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
    void setCurrentMode(mode newMode);
private:
    mode currentMode;
    Ui::UserHelperDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void deleteAccountAccept(const QString& newUsername);
    void usernameAccept(const QString& newUsername);
    void passwordAccept(const QString& oldPassword,const QString& newPassword);
};

#endif // USERHELPERDIALOG_H
