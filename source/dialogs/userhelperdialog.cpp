#include "dialogs/userhelperdialog.h"
#include "ui_userhelperdialog.h"
#include "customField/abstractCustomField.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
UserHelperDialog::UserHelperDialog(mode currentMode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserHelperDialog)
{
    ui->setupUi(this);
    initWidget();
    setCurrentMode(currentMode);
}
void UserHelperDialog::initWidget()
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
    ui->oldPassword->setValidator(FieldData::passwordValidator);
    ui->newPassword->setValidator(FieldData::passwordValidator);
    //绑定槽函数
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
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
void UserHelperDialog::loadUsername(const QString& newUsername)
{
    if(currentMode==USERNAME)
        ui->username->setText(newUsername);
}
void UserHelperDialog::loadPassword(const QString& oldPassword,const QString& newPassword)
{
    if(currentMode==PASSWORD)
        ui->oldPassword->setText(oldPassword),ui->newPassword->setText(newPassword);
}
void UserHelperDialog::setCurrentMode(mode newMode)
{
    this->currentMode=newMode;
    if(newMode==USERNAME)
        setFixedHeight(250),setWindowCustomTitle("修改用户名"),ui->username->setFocus(),ui->changePasswordWidget->hide(),ui->deleteVerifyWidget->hide();
    else if(newMode==PASSWORD)
        setFixedHeight(280),setWindowCustomTitle("修改密码"),ui->oldPassword->setFocus(),ui->changeUsernameWidget->hide(),ui->deleteVerifyWidget->hide();
    else if(newMode==DELETEACCOUNT)
        setFixedHeight(250),setWindowCustomTitle("注销确认"),ui->deleteVerify->setFocus(),ui->changeUsernameWidget->hide(),ui->changePasswordWidget->hide();
}
void UserHelperDialog::onCancelClicked(){
    close();
}
void UserHelperDialog::onConfirmClicked(){
    if(currentMode==USERNAME){
        if(ui->username->text()=="")
            return;
        emit usernameAccept(ui->username->text());
    }else if(currentMode==PASSWORD){
        if(ui->oldPassword->text()=="")
            return;
        emit passwordAccept(ui->oldPassword->text(),ui->newPassword->text());
    }else if(currentMode==DELETEACCOUNT){
        if(ui->deleteVerify->text()=="")
            return;
        emit deleteAccountAccept(ui->deleteVerify->text());
    }
    this->close();
}
UserHelperDialog::~UserHelperDialog()
{
    delete ui;
}
void UserHelperDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void UserHelperDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void UserHelperDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
        ui->confirm->click();
        break;
    default:
        break;
    }
}
void UserHelperDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
void UserHelperDialog::onCloseClicked()
{
    this->close();
}
void UserHelperDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void UserHelperDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool UserHelperDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void UserHelperDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
