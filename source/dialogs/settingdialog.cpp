#include "dialogs/settingdialog.h"
#include "QtAwesome.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include "qdebug.h"
#include "qevent.h"
#include "qgraphicseffect.h"
#include "ui_settingdialog.h"
#include "util/settinghelper.h"
#include <Windows.h>
#include <WinUser.h>
#include <QDesktopServices>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
SettingDialog::SettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingDialog)
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
void SettingDialog::initWidget()
{
    setWindowCustomTitle("设置 - Password Manager");
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
    connect(ui->advancedButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->secureButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->fileButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->logButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->feedbackButton,SIGNAL(clicked()),this,SLOT(onFeedbackClicked()));
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
    connect(ui->applyButton,SIGNAL(clicked()),this,SLOT(onOperationButtonsClicked()));
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(onOperationButtonsClicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(onOperationButtonsClicked()));
    buttonToIconMap[ui->secureButton]=0xf3ed;
    buttonToIconMap[ui->fileButton]=0xf15b;
    buttonToIconMap[ui->logButton]=0xf188;
    buttonToIconMap[ui->advancedButton]=0xf1b2;
    buttonToIconMap[ui->helpButton]=0xf059;
    buttonToIconMap[ui->feedbackButton]=0xf044;
    buttons<<ui->secureButton<<ui->fileButton<<ui->logButton<<ui->advancedButton;
    QtAwesome* awesome = new QtAwesome(qApp);
    awesome->initFontAwesome();
    QVariantMap options;
    options.insert("color",QColor(7, 193, 96));
    for(int i=0;i<buttons.size();i++)
        buttons[i]->setIcon(awesome->icon(buttonToIconMap[buttons[i]],options));
    ui->helpButton->setIcon(awesome->icon(buttonToIconMap[ui->helpButton],options));
    ui->feedbackButton->setIcon(awesome->icon(buttonToIconMap[ui->feedbackButton],options));
    ui->secureButton->click();
    loadSettings();
}
void SettingDialog::onButtonsClicked()
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
void SettingDialog::onOperationButtonsClicked()
{
    QToolButton* button=(QToolButton*)sender();
    if(button==ui->applyButton&&ui->programNameLabel->text()=="*设置 - Password Manager"){
        SettingHelper& settingHelper=SettingHelper::instance();
        settingHelper.setLockerEnabled(ui->openLocker->isChecked());
        settingHelper.setLockerTimer(ui->lockerTimer->value());
        settingHelper.setPasswordAsLocker(ui->usePasswordLocker->isChecked());
        settingHelper.setDefaultExportType(ui->defaultExportType->currentText());
        settingHelper.setDefaultExportLocationEnabled(ui->defaultExportLocation->isChecked());
        settingHelper.setDefaultExportLocation(ui->defaultExportLocationEdit->text());
        settingHelper.setLogLevel((QsLogging::Level)ui->defaultLogLevel->currentIndex());
        settingHelper.setLogFileMaxSize(ui->logSize->value());
        settingHelper.setLogFileMaxCount(ui->logCount->value());
        settingHelper.setListTipEnabled(ui->listTip->isChecked());
        emit settingsChanged();
        setWindowCustomTitle("设置 - Password Manager");
        FloatBox::Success("应用成功",this);
    }else if(button==ui->confirmButton){
        if(ui->programNameLabel->text()=="*设置 - Password Manager")
            ui->applyButton->click();
        close();
    }else if(button==ui->cancelButton){
        if(ui->programNameLabel->text()=="*设置 - Password Manager"){
            if(CustomMessageBox::question(this,"放弃修改","已经存在修改项，是否放弃修改")==CustomMessageBox::YES)
                this->close();
        }else
            this->close();
    }
}
void SettingDialog::onSettingsChanged()
{
    if(sender()==ui->openLocker)
    {
        ui->lockerTimer->setEnabled(ui->openLocker->isChecked());
        ui->usePasswordLocker->setEnabled(ui->openLocker->isChecked());
        ui->changeLoker->setEnabled(ui->openLocker->isChecked());
        ui->lockerTimerLabel->setEnabled(ui->openLocker->isChecked());
    }
    if(sender()==ui->usePasswordLocker)
    {
        ui->changeLoker->setEnabled(ui->usePasswordLocker->isChecked());
    }
    if(sender()==ui->defaultExportLocation)
    {
        ui->openDefaultExportLocation->setEnabled(ui->defaultExportLocation->isChecked());
        ui->defaultExportLocationEdit->setEnabled(ui->defaultExportLocation->isChecked());
        ui->chooseDefaultExportLocation->setEnabled(ui->defaultExportLocation->isChecked());
    }
    SettingHelper& settingHelper=SettingHelper::instance();
    //    qDebug()<<(ui->openLocker->isChecked()==settingHelper.getLockerEnabled())<<
    //              (ui->lockerTimer->value()==settingHelper.getLockerTimer())<<
    //              (ui->usePasswordLocker->isChecked()==settingHelper.getPasswordAsLocker())<<
    //              (ui->defaultExportType->currentText()==settingHelper.getDefaultExportType())<<
    //              (ui->defaultExportLocation->isChecked()==settingHelper.getDefaultExportLocationEnabled())<<
    //              (ui->defaultExportLocationEdit->text()==settingHelper.getDefaultExportLocation())<<
    //              (ui->defaultLogLevel->currentIndex()==(int)settingHelper.getLogLevel())<<
    //              (ui->logSize->value()==settingHelper.getLogFileMaxSize())<<
    //              (ui->logCount->value()==settingHelper.getLogFileMaxCount())<<
    //              (ui->listTip->isChecked()==settingHelper.getListTipEnabled());
    bool isChanged=!(ui->openLocker->isChecked()==settingHelper.getLockerEnabled()&&
                     ui->lockerTimer->value()==settingHelper.getLockerTimer()&&
                     ui->usePasswordLocker->isChecked()==settingHelper.getPasswordAsLocker()&&
                     ui->defaultExportType->currentText()==settingHelper.getDefaultExportType()&&
                     ui->defaultExportLocation->isChecked()==settingHelper.getDefaultExportLocationEnabled()&&
                     ui->defaultExportLocationEdit->text()==settingHelper.getDefaultExportLocation()&&
                     ui->defaultLogLevel->currentIndex()==(int)settingHelper.getLogLevel()&&
                     ui->logSize->value()==settingHelper.getLogFileMaxSize()&&
                     ui->logCount->value()==settingHelper.getLogFileMaxCount()&&
                     ui->listTip->isChecked()==settingHelper.getListTipEnabled());
    if(isChanged)
        setWindowCustomTitle("*设置 - Password Manager");
    else
        setWindowCustomTitle("设置 - Password Manager");
}
void SettingDialog::loadSettings()
{
    SettingHelper& settingHelper=SettingHelper::instance();
    ui->openLocker->setChecked(settingHelper.getLockerEnabled());
    ui->lockerTimer->setValue(settingHelper.getLockerTimer());
    ui->usePasswordLocker->setChecked(settingHelper.getPasswordAsLocker());
    QStringList exportTypes;
    exportTypes<<"Excel文件(.xlxs)"<<"CSV文件(.csv)"<<"图片(.png)"<<"PDF文件(.pdf)"<<"HTML文件(.html)";
    ui->defaultExportType->addItems(exportTypes);
    ui->defaultExportType->setCurrentText(settingHelper.getDefaultExportType());
    ui->defaultExportLocation->setChecked(settingHelper.getDefaultExportLocationEnabled());
    ui->defaultExportLocationEdit->setText(settingHelper.getDefaultExportLocation());
    QStringList logLevels;
    logLevels<<"Trace"<<"Debug"<<"Info"<<"Warning"<<"Error"<<"Fatal"<<"Off";
    ui->defaultLogLevel->addItems(logLevels);
    ui->defaultLogLevel->setCurrentIndex((int)settingHelper.getLogLevel());
    ui->logSize->setValue(settingHelper.getLogFileMaxSize());
    ui->logCount->setValue(settingHelper.getLogFileMaxCount());
    ui->listTip->setChecked(settingHelper.getListTipEnabled());
}
void SettingDialog::onFeedbackClicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com")));
}
SettingDialog::~SettingDialog()
{
    delete ui;
}
void SettingDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void SettingDialog::onCloseClicked()
{
    close();
}
void SettingDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void SettingDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void SettingDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
bool SettingDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void SettingDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
void SettingDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    setWindowTitle(newWindowTitle);
    ui->programNameLabel->setText(newWindowTitle);
}
