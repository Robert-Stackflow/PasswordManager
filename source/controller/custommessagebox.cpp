#include "controller/custommessagebox.h"
#include <QMouseEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QClipboard>
#include <QMetaEnum>
#include "qdebug.h"
#include "ui_custommessagebox.h"
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
CustomMessageBox::CustomMessageBox(CustomMessageBox::type messageBoxType,const QString &title, const QString &text,QWidget *parent):
    QDialog(parent),
    sResult(NONE),
    ui(new Ui::CustomMessageBox)
{
    ui->setupUi(this);
    initWidget();
    setTitle(title);
    setText(text);
    setType(messageBoxType);
    if(messageBoxType==INFO){
        ui->cancelButton->hide();
        setIcon(QIcon(":custom/icons/info.png"));
    }else if(messageBoxType==QUESTION){
        setIcon(QIcon(":custom/icons/question.png"));
    }else if(messageBoxType==WARNING){
        ui->cancelButton->hide();
        setIcon(QIcon(":custom/icons/warning.png"));
    }else if(messageBoxType==CRITICAL){
        ui->cancelButton->hide();
        ui->confirmButton->setText("退出");
        setIcon(QIcon(":custom/icons/critical.png"));
    }
    if(parent!=nullptr){
        QPoint globalPos = parent->mapToGlobal(QPoint(0,0));
        int x = globalPos.x() + (parent->width() - this->width())/2;
        int y = globalPos.y() + (parent->height() - this->height())/2;
        this->move(mapFromGlobal(QPoint(x,y)));
    }
    QApplication::beep();
}
void CustomMessageBox::initWidget()
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(167,166,170));
    setGraphicsEffect(shadow);
    ui->widgetGridLayout->setMargin(20);
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->copyButton,SIGNAL(clicked()),this,SLOT(onCopyClicked()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
}

void CustomMessageBox::keyPressEvent( QKeyEvent* event)
{
    if(event->matches(QKeySequence::Copy))
        onCopyClicked();
}
void CustomMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void CustomMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void CustomMessageBox::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
CustomMessageBox::RETURNCODE CustomMessageBox::result()
{
    return sResult;
}
void CustomMessageBox::onMinimizeClicked()
{
    this->showMinimized();
}
void CustomMessageBox::onConfirmClicked()
{
    sResult=YES;
    this->close();
}
void CustomMessageBox::onCancelClicked()
{
    sResult=NO;
    this->close();
}
void CustomMessageBox::onCopyClicked()
{
    QClipboard* clip=QApplication::clipboard();
    clip->setText(toString());
}
QString CustomMessageBox::toString()
{
    QString str;
    QMetaEnum typeMeta = QMetaEnum::fromType<CustomMessageBox::type>();
    str.append("type:").append(typeMeta.valueToKey(messageBoxType)).append("\n");
    str.append("title:").append(ui->titleLabel->text()).append("\n");
    str.append("message:").append(ui->infoLabel->text());
    return str;
}
void CustomMessageBox::onCloseClicked()
{
    sResult=NONE;
    this->close();
}
void CustomMessageBox::setTitle(const QString &title)
{
    setWindowTitle(title);
    ui->titleLabel->setText(title);
}
void CustomMessageBox::setText(const QString &text)
{
    ui->infoLabel->setText(text);
}
void CustomMessageBox::setIcon(QIcon icon)
{
    ui->iconLabel->setPixmap(icon.pixmap(20,20));
}
void CustomMessageBox::setType(type messageBoxType)
{
    this->messageBoxType=messageBoxType;
}
CustomMessageBox::~CustomMessageBox()
{
    delete ui;
}
bool CustomMessageBox::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void CustomMessageBox::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
int CustomMessageBox::information(QWidget *parent, const QString &title,const QString& text)
{
    CustomMessageBox* msgBox=new CustomMessageBox(CustomMessageBox::INFO,title,text,parent);
    msgBox->exec();
    return msgBox->result();
}
int CustomMessageBox::question(QWidget *parent, const QString &title,const QString& text)
{
    CustomMessageBox* msgBox=new CustomMessageBox(CustomMessageBox::QUESTION,title,text,parent);
    msgBox->exec();
    return msgBox->result();
}
int CustomMessageBox::warning(QWidget *parent, const QString &title,const QString& text)
{
    CustomMessageBox* msgBox=new CustomMessageBox(CustomMessageBox::WARNING,title,text,parent);
    msgBox->exec();
    return msgBox->result();
}
int CustomMessageBox::critical(QWidget *parent, const QString &title,const QString& text)
{
    CustomMessageBox* msgBox=new CustomMessageBox(CustomMessageBox::CRITICAL,title,text,parent);
    msgBox->exec();
    return msgBox->result();
}
