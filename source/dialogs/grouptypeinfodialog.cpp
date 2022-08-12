#include "dialogs/grouptypeinfodialog.h"
#include "ui_grouptypeinfodialog.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
GroupTypeInfoDialog::GroupTypeInfoDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupTypeInfoDialog)
{
    ui->setupUi(this);
    initWidget();
}
void GroupTypeInfoDialog::initWidget()
{
    setWindowCustomTitle("Password Manager");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(167,166,170));
    setGraphicsEffect(shadow);
    ui->dialogGridLayout->setMargin(20);
    ui->describelabel->setText(ui->describelabel->fontMetrics().elidedText(ui->describelabel->text(),Qt::ElideRight,ui->describelabel->width()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    connect(ui->editButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->copyButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
}
void GroupTypeInfoDialog::onButtonsClicked()
{
    this->close();
    QPushButton* button=(QPushButton*)sender();
    if(button==ui->copyButton)
        emit copyClicked();
    else if(button==ui->editButton)
        emit editClicked();
    else if(button==ui->deleteButton)
        emit deleteClicked();
}
GroupTypeInfoDialog::~GroupTypeInfoDialog()
{
    delete ui;
}
void GroupTypeInfoDialog::loadGroupType(GroupType* groupType)
{
    if(groupType==nullptr)
        return;
    setWindowCustomTitle(groupType->getGroupTypeName());
    ui->describelabel->setText(groupType->getDescribe());
    ui->groupTypeNameLabel->setText(groupType->getGroupTypeName());
    ui->describelabel->setText(ui->describelabel->fontMetrics().elidedText(ui->describelabel->text(),Qt::ElideRight,ui->describelabel->width()));
    ui->createTimeLabel->setText("创建时间:"+groupType->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lastEditTimeLabel->setText("修改时间:"+groupType->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
}
void GroupTypeInfoDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void GroupTypeInfoDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void GroupTypeInfoDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
void GroupTypeInfoDialog::onCloseClicked()
{
    this->close();
}
void GroupTypeInfoDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void GroupTypeInfoDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool GroupTypeInfoDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void GroupTypeInfoDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
