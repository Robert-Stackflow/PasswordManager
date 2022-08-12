#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QWidget>

namespace Ui {
class UserDialog;
}

class UserDialog : public QWidget
{
    Q_OBJECT
public:
    explicit UserDialog(QPixmap portrait,const QString& username,QWidget *parent = nullptr);
    ~UserDialog();
    void setWindowCustomTitle(const QString& newWindowTitle);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void logout();
    void initWidget();
private:
    Ui::UserDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
private slots:
    void onLogoutClicked();
    void onLockClicked();
    void onDeleteAccountClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onChangeInfoClicked();
    void onNcActived(bool active);
    void onUsernameAccept(const QString& newUsername);
    void onDeleteAccountAccept(const QString& password);
    void onPasswordAccept(const QString& oldPassword,const QString& newPassword);
signals:
    void ncActive(bool);
    void lockAccept();
    void logoutAccept();
};

#endif // USERDIALOG_H
