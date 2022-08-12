#include "dialogs/exportdialog.h"
#include "QtAwesome.h"
#include "qgraphicseffect.h"
#include "ui_exportdialog.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include "qdebug.h"
#include "qevent.h"
#include "util/settinghelper.h"
#include <Windows.h>
#include <WinUser.h>
#include <QDesktopServices>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
ExportDialog::ExportDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    initWidget();
    if(parent!=nullptr){
        QPoint globalPos = parent->mapToGlobal(QPoint(0,0));
        int x = globalPos.x() + (parent->width() - this->width())/2;
        int y = globalPos.y() + (parent->height() - this->height())/2;
        this->move(x,y);
    }
}
void ExportDialog::initWidget()
{
    setWindowCustomTitle("导出向导");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(167,166,170));
    setGraphicsEffect(shadow);
    ui->dialogGridLayout->setMargin(20);
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
    ui->defaultLogLevel->setView(new QListView());
    ui->defaultExportType->setView(new QListView());
    connect(ui->htmlButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->excelButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->csvButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->pdfButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->openLocker,SIGNAL(toggled(bool)),this,SLOT(onSettingsChanged()));
    connect(ui->lockerTimer,SIGNAL(valueChanged(int)),this,SLOT(onSettingsChanged()));
    connect(ui->usePasswordLocker,SIGNAL(toggled(bool)),this,SLOT(onSettingsChanged()));
    connect(ui->defaultExportType,SIGNAL(currentIndexChanged(int)),this,SLOT(onSettingsChanged()));
    connect(ui->defaultExportLocation,SIGNAL(toggled(bool)),this,SLOT(onSettingsChanged()));
    connect(ui->chooseDefaultExportLocation,SIGNAL(clicked()),this,SLOT(onSettingsChanged()));
    connect(ui->defaultExportLocationEdit,SIGNAL(textChanged(QString)),this,SLOT(onSettingsChanged()));
    connect(ui->logSize,SIGNAL(valueChanged(int)),this,SLOT(onSettingsChanged()));
    connect(ui->defaultLogLevel,SIGNAL(currentIndexChanged(int)),this,SLOT(onSettingsChanged()));
    connect(ui->logCount,SIGNAL(valueChanged(int)),this,SLOT(onSettingsChanged()));
    connect(ui->listTip,SIGNAL(toggled(bool)),this,SLOT(onSettingsChanged()));
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(onOperationButtonsClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(onOperationButtonsClicked()));
    buttonToIconMap[ui->excelButton]=0xf3ed;
    buttonToIconMap[ui->csvButton]=0xf15b;
    buttonToIconMap[ui->pdfButton]=0xf188;
    buttonToIconMap[ui->htmlButton]=0xf1b2;
    buttons<<ui->excelButton<<ui->csvButton<<ui->pdfButton<<ui->htmlButton;
    QtAwesome* awesome = new QtAwesome(qApp);
    awesome->initFontAwesome();
    QVariantMap options;
    options.insert("color",QColor(7, 193, 96));
    for(int i=0;i<buttons.size();i++)
        buttons[i]->setIcon(awesome->icon(buttonToIconMap[buttons[i]],options));
    ui->excelButton->click();
}
void ExportDialog::onButtonsClicked()
{
    QToolButton* button=(QToolButton*)sender();
    for(int i=0;i<buttons.count();i++)
        buttons[i]->setStyleSheet("QToolButton{border:0px;padding-left:5px;padding-right:5px;}QToolButton:hover{background:#e2e2e2;}");
    if(button!=buttons[0])
        button->setStyleSheet(button->styleSheet()+"QToolButton{border:0px;background:#07c160;color:white;}QToolButton:hover{background:#38cd7f;}");
    else
        button->setStyleSheet(button->styleSheet()+"QToolButton{border:0px;border-top-left-radius:5px;background:#07c160;color:white;}QToolButton:hover{border-top-left-radius:5px;background:#38cd7f;}");
    ui->stackedWidget->setCurrentIndex(buttons.indexOf(button));
    QtAwesome* awesome = new QtAwesome(qApp);
    awesome->initFontAwesome();
    QVariantMap options;
    options.insert("color",QColor(7, 193, 96));
    for(int i=0;i<buttons.size();i++)
        buttons[i]->setIcon(awesome->icon(buttonToIconMap[buttons[i]],options));
    button->setIcon(awesome->icon(buttonToIconMap[button]));
}
void ExportDialog::onOperationButtonsClicked()
{
    QToolButton* button=(QToolButton*)sender();
    if(button==ui->confirmButton){
        if(ui->programNameLabel->text()=="*设置 - Password Manager")
        close();
    }else if(button==ui->cancelButton){
        if(ui->programNameLabel->text()=="*设置 - Password Manager"){
            if(CustomMessageBox::question(this,"放弃修改","已经存在修改项，是否放弃修改")==CustomMessageBox::YES)
                this->close();
        }else
            this->close();
    }
}
ExportDialog::~ExportDialog()
{
    delete ui;
}
void ExportDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void ExportDialog::onCloseClicked()
{
    close();
}
void ExportDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void ExportDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void ExportDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
bool ExportDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void ExportDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
void ExportDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    setWindowTitle(newWindowTitle);
    ui->programNameLabel->setText(newWindowTitle);
}
