#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include "controller/customdialog.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
namespace Ui {
class loginDialog;
}
class loginDialog : public QDialog
{
    Q_OBJECT
public:
    Ui::loginDialog *ui;
    //辅助变量，便于不同函数间传递数据
    QString tempPortraitPath="";
    //自定义控件
    CustomTitleBar* m_titleBar;
    customComboBox* username;
    customLineEdit* password;
private:
    QString styleSheet;
public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();
    bool eventFilter(QObject *obj, QEvent *event);
    QString copyFileToPath(QString srcPath,QString desDir);
    QPixmap PixmapToRound(const QPixmap &src, int radius);
private:
    void initOther();
    void initDialog();
    void loadAccounts();
    void loadQss(const QString &qssFilePath);
    void loadPortrait(const QString& portraitPath);
private slots:
    void onConfirmClicked();
    void onPortraitDesPathChanged();
    void onUsernameCurrentIndexChanged();
signals:
    void portraitDesPathChanged();
};

#endif // LOGINDIALOG_H
