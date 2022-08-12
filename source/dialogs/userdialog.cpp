#include "dialogs/userdialog.h"
#include "dialogs/userhelperdialog.h"
#include "qdebug.h"
#include "qdir.h"
#include "qeasingcurve.h"
#include "qpropertyanimation.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qtimer.h"
#include "ui_userdialog.h"
#include "util/customdatabase.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include "widgets/loginwidget.h"
#include <QFileDialog>
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QProcess>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
UserDialog::UserDialog(QPixmap portrait,const QString& username,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    initWidget();
    ui->userPortrait->setPixmap(portrait);
    ui->usernameLabel->setText(username);
    ui->usernameLabel->setToolTip(username);
}
void UserDialog::initWidget()
{
    setWindowCustomTitle("用户中心");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(167,166,170));
    setGraphicsEffect(shadow);
    ui->dialogGridLayout->setMargin(20);
    //绑定槽函数
    connect(ui->lock,SIGNAL(clicked()),this,SLOT(onLockClicked()));
    connect(ui->logout,SIGNAL(clicked()),this,SLOT(onLogoutClicked()));
    connect(ui->deleteAccount,SIGNAL(clicked()),this,SLOT(onDeleteAccountClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    connect(ui->changeUsernameButton,SIGNAL(clicked()),this,SLOT(onChangeInfoClicked()));
    connect(ui->changePasswordButton,SIGNAL(clicked()),this,SLOT(onChangeInfoClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    QFile file(":qss/custom.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString temp = this->styleSheet;
        temp += QLatin1String(file.readAll());
        this->styleSheet=temp;
        this->setStyleSheet(styleSheet);
        file.close();
    }
}
void UserDialog::onUsernameAccept(const QString& newUsername)
{
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    Account* currentAccount = sharedDataHelper.accountList[sharedDataHelper.accountList.index(ui->usernameLabel->text())];
    QString currentUsername = currentAccount->getUsername();
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    QString accountsTableName = dataBaseHelper.getAccountsTableName();
    if(newUsername!=currentUsername&&sharedDataHelper.accountList.has(newUsername)){
        CustomMessageBox::warning(this, "警告", "用户名\""+newUsername+"\"已存在!");
        UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::USERNAME,this);
        dialog->show();
        dialog->loadUsername(newUsername);
        connect(dialog,SIGNAL(usernameAccept(QString)),this,SLOT(onUsernameAccept(QString)));
    }else{
        if(newUsername!=currentUsername){
            customDataBase.setDatabaseName(databaseName);
            QSqlQuery query(customDataBase.getDatabase());
            if (customDataBase.open())
            {
                currentAccount->setUsername(newUsername);
                currentAccount->setLastEditTime();
                QString sql=QString("update "+accountsTableName+" set username='%1',lastEditTime='%2' where createTime='%3'" )
                        .arg(newUsername)
                        .arg(currentAccount->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                        .arg(currentAccount->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                if(query.exec(sql)){
                    FloatBox::Success("修改成功",this);
                    CustomMessageBox::warning(this, "提示", "用户名已修改，请重新登陆!");
                    logout();
                }else
                    FloatBox::Fail("修改失败",this);
            }
        }
    }
}
void UserDialog::onPasswordAccept(const QString& oldPassword,const QString& newPassword)
{
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    Account* currentAccount = sharedDataHelper.accountList[sharedDataHelper.accountList.index(ui->usernameLabel->text())];
    QString currentUsername = currentAccount->getUsername();
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    QString accountsTableName = dataBaseHelper.getAccountsTableName();
    //判断密码是否正确
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if (customDataBase.open())
    {
        QString selectSql = "select password from accounts where username='" + currentUsername + "'";
        query.exec(selectSql);
        if (query.first()){
            if (query.value(0).toString() == oldPassword){
                currentAccount->setLastEditTime();
                QString sql=QString("update "+accountsTableName+" set password='%1',lastEditTime='%2' where createTime='%3'" )
                        .arg(newPassword)
                        .arg(currentAccount->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                        .arg(currentAccount->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                if(query.exec(sql)){
                    FloatBox::Success("修改成功",this);
                    CustomMessageBox::warning(this, "提示", "密码已修改，请重新登陆!");
                    logout();
                }else
                    FloatBox::Fail("修改失败",this);
            }else{
                CustomMessageBox::warning(this, "警告", "旧密码不正确");
                UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::PASSWORD,this);
                dialog->show();
                dialog->loadPassword(oldPassword,newPassword);
                connect(dialog,SIGNAL(passwordAccept(QString,QString)),this,SLOT(onPasswordAccept(QString,QString)));
            }
        }
        customDataBase.close();
    }
}
void UserDialog::onDeleteAccountAccept(const QString& password)
{
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    Account* currentAccount = sharedDataHelper.accountList[sharedDataHelper.accountList.index(ui->usernameLabel->text())];
    QString currentUsername = currentAccount->getUsername();
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    QString accountsTableName = dataBaseHelper.getAccountsTableName();
    QString currentAccountdataPath = dataPathHelper.getCurrentAccountDataBasePath();
    qDebug()<<currentAccountdataPath;
    //判断密码是否正确
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if (customDataBase.open())
    {
        QString selectSql = "select password from accounts where username='" + currentUsername + "'";
        query.exec(selectSql);
        if (query.first()){
            if (query.value(0).toString() == password){
                int result=CustomMessageBox::question(this,"再次警告","请再次确认是否注销用户!");
                if(result==CustomMessageBox::YES)
                {
                    QString sql=QString("delete from "+accountsTableName+" where createTime='%1'" )
                            .arg(currentAccount->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                    if(query.exec(sql)){
                        if (QDir().exists(currentAccountdataPath)){
                            if (QFileInfo(currentAccountdataPath).isFile())
                                QFile::remove(currentAccountdataPath);
                        }
                        FloatBox::Success("注销成功",this);
                        logout();
                    }else
                        FloatBox::Fail("注销失败",this);
                }
            }else{
                CustomMessageBox::warning(this, "警告", "密码不正确");
                UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::DELETEACCOUNT,this);
                dialog->show();
                connect(dialog,SIGNAL(deleteAccountAccept(QString)),this,SLOT(onDeleteAccountAccept(QString)));
            }
        }
        customDataBase.close();
    }
}
void UserDialog::logout(){
    FloatBox::Success("重新登录",this);
    QTimer::singleShot(500,this,[=]{
        QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
        anim->setDuration(500);
        anim->setStartValue(1);
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::OutCurve);
        anim->start();
        connect(anim, &QPropertyAnimation::finished, [=] {
            close();
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
            qApp->exit();
        });
    });
}
void UserDialog::onChangeInfoClicked()
{
    if(sender()->objectName()=="changeUsernameButton"){
        UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::USERNAME,this);
        dialog->show();
        dialog->loadUsername(ui->usernameLabel->text());
        connect(dialog,SIGNAL(usernameAccept(QString)),this,SLOT(onUsernameAccept(QString)));
    }
    if(sender()->objectName()=="changePasswordButton"){
        UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::PASSWORD,this);
        dialog->show();
        connect(dialog,SIGNAL(passwordAccept(QString,QString)),this,SLOT(onPasswordAccept(QString,QString)));
    }
}
void UserDialog::onLockClicked(){
    emit lockAccept();
}
void UserDialog::onLogoutClicked(){
    emit logoutAccept();
}
void UserDialog::onDeleteAccountClicked()
{
    int result=CustomMessageBox::question(this,"注销警告","用户一经删除，所有用户信息(包括密码,日志记录等)都将被删除,无法恢复!\n请谨慎操作!");
    if(result==CustomMessageBox::YES)
    {
        UserHelperDialog* dialog=new UserHelperDialog(UserHelperDialog::DELETEACCOUNT,this);
        dialog->show();
        connect(dialog,SIGNAL(deleteAccountAccept(QString)),this,SLOT(onDeleteAccountAccept(QString)));
    }
}
UserDialog::~UserDialog()
{
    delete ui;
}
void UserDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void UserDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void UserDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
void UserDialog::onCloseClicked()
{
    this->close();
}
void UserDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void UserDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool UserDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void UserDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
