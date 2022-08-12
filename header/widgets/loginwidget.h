#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "framelesswidget.h"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
private:
    enum LoginType{
        SIGNUP,
        SIGNIN
    };
    int accountIndex=0;
    QString styleSheet;
    Ui::LoginWidget *ui;
    LoginType loginType;
    QString tempPortraitPath="";
private:
    void initWidget();
    void loadAccounts();
    void loadNewAccountLayout();
    void loadAccountLayout(int index);
    void loadQss(const QString &qssFilePath);
    QPixmap loadPortrait(const QString& portraitPath);
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event);
public:
    static QString copyFileToPath(QString srcPath,QString desDir,QWidget* w);
    static QPixmap PixmapToRound(const QPixmap &src, int radius);
private slots:
    void onCloseClicked();
    void onSwitchClicked();
    void onSigninConfirmClicked();
    void onSignupConfirmClicked();
    void onLeftButtonClicked();
    void onRightButtonClicked();
    void onPortraitDesPathChanged(QPixmap);
    void onAccountCountChanged(const QString& currentUsername);
signals:
    void loginAccept();
    void newAccountSignal();
    void portraitDesPathChanged(QPixmap);
    void accountCountChanged(const QString& currentUsername);
};

#endif // LOGINWIDGET_H
