#include "widgets/loginwidget.h"
#include "qsqlerror.h"
#include "widget.h"
#include "QsLog.h"
#include "controller/floatbox.h"
#include "controller/custommessagebox.h"
#include "ui_loginwidget.h"
#include "util/customdatabase.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QBitmap>
#include <QPainter>
#include <QSqlQuery>
#include <QCompleter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
LoginWidget::LoginWidget(QWidget *parent) :
    FramelessWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    initWidget();
    loadAccounts();
}
void LoginWidget::initWidget()
{
    //设置无边框
    setBtnClose(ui->closeButton);
    setTitleBar(ui->topWidget);
    addIgnoreWidget(ui->programNameLabel);
    //加载qss样式
    loadQss(":qss/custom.qss");
    //设置默认头像，并设置事件过滤器
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    loadPortrait(dataPathHelper.getDefaultPortraitPath());
    ui->signinPortrait->installEventFilter(this);
    ui->signupPortrait->installEventFilter(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->signinPassword->setValidator(FieldData::passwordValidator);
    ui->signupPassword->setValidator(FieldData::passwordValidator);
    //绑定槽函数
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->signinTopLeft, SIGNAL(clicked()), this, SLOT(onLeftButtonClicked()));
    connect(ui->signinTopRight, SIGNAL(clicked()), this, SLOT(onRightButtonClicked()));
    connect(ui->signinConfirm, SIGNAL(clicked()), this, SLOT(onSigninConfirmClicked()));
    connect(ui->signupConfirm, SIGNAL(clicked()), this, SLOT(onSignupConfirmClicked()));
    connect(ui->switchSignin, SIGNAL(clicked()), this, SLOT(onSwitchClicked()));
    connect(ui->switchSignup, SIGNAL(clicked()), this, SLOT(onSwitchClicked()));
    connect(this, SIGNAL(portraitDesPathChanged(QPixmap)), this, SLOT(onPortraitDesPathChanged(QPixmap)));
    connect(this, SIGNAL(accountCountChanged(QString)), this, SLOT(onAccountCountChanged(QString)));
    //新建用户数据目录
    QDir dir;
    if (!dir.exists(dataPathHelper.getDataDir()))
        if (!dir.mkpath(QDir(dataPathHelper.getDataDir()).absolutePath()))
            CustomMessageBox::critical(this,"错误","无法创建用户数据文件夹!");
    if (!dir.exists(dataPathHelper.getPortraitDir()))
        if (!dir.mkpath(QDir(dataPathHelper.getPortraitDir()).absolutePath()))
            CustomMessageBox::critical(this,"错误","无法创建用户数据文件夹!");
    if (!dir.exists(dataPathHelper.getLogsDir()))
        if (!dir.mkpath(dataPathHelper.getLogsDir()))
            CustomMessageBox::critical(this,"错误","无法创建用户数据文件夹!");
}
void LoginWidget::onCloseClicked(){
    qApp->exit();
}
void LoginWidget::loadQss(const QString& qssFilePath)
{
    QFile file(qssFilePath);
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
void LoginWidget::loadAccounts()
{
    //获取各单例类
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    //加载数据库中的账户与头像
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    QString accountsTableName = dataBaseHelper.getAccountsTableName();
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if (customDataBase.open())
    {
        if (!customDataBase.tables().contains(accountsTableName))
        {
            //如果不存在accounts表,则提示创建用户,并创建accounts表
            QString createTableSql = "create table " + accountsTableName + " (username varchar(200),password varchar(200),portraitPath text,createTime varchar(200),lastEditTime varchar(200))";
            query.exec(createTableSql);
            emit accountCountChanged("");
        }
        else
        {
            //如果存在accounts表,则加载accounts表中的账户列表
            QString selectSql = "select * from accounts";
            query.exec(selectSql);
            for (int i = 0; query.next(); i++)
            {
                QString newUsername = query.value(0).toString();
                QString newPortraitPath = query.value(2).toString();
                QDateTime newCreateTime = query.value(3).toDateTime();
                QDateTime newLastEditTime = query.value(4).toDateTime();
                sharedDataHelper.accountList << new Account(newUsername, newCreateTime, newLastEditTime, newPortraitPath);
            }
            if(sharedDataHelper.accountList.count()==0){
                emit accountCountChanged("");
            }
            else
                emit accountCountChanged(sharedDataHelper.accountList.at(0)->getUsername());
        }
        customDataBase.close();
    }
}
void LoginWidget::onLeftButtonClicked(){
    loadAccountLayout(accountIndex-1);
}
void LoginWidget::onRightButtonClicked(){
    loadAccountLayout(accountIndex+1);
}
void LoginWidget::loadNewAccountLayout()
{
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    loadPortrait(dataPathHelper.getDefaultPortraitPath());
    ui->signupConfirm->setText("创建用户");
    ui->signupConfirm->setToolTip("创建用户");
    ui->signupUsernameLabel->setText("创建用户");
    ui->signupUsername->clear();
    ui->signupPassword->clear();
    ui->signupUsername->setEnabled(true);
    ui->signupUsername->setCursor(Qt::IBeamCursor);
}
void LoginWidget::loadAccountLayout(int index){
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    accountIndex=index;
    if(accountIndex<=0)
        ui->signinTopLeft->setEnabled(false),ui->signinTopLeft->setCursor(Qt::ForbiddenCursor);
    else
        ui->signinTopLeft->setEnabled(true),ui->signinTopLeft->setCursor(Qt::PointingHandCursor);
    if(accountIndex>=sharedDataHelper.accountList.count()-1)
        ui->signinTopRight->setEnabled(false),ui->signinTopRight->setCursor(Qt::ForbiddenCursor);
    else
        ui->signinTopRight->setEnabled(true),ui->signinTopRight->setCursor(Qt::PointingHandCursor);
    if(accountIndex>=0&&accountIndex<=sharedDataHelper.accountList.count()-1){
        Account *currentAccount = sharedDataHelper.accountList[accountIndex];
        if (currentAccount == nullptr)
            return;
        QString imagePath = currentAccount->getPortraitPath();
        if (imagePath == "")
            imagePath = dataPathHelper.getDefaultPortraitPath();
        currentAccount->setPortrait(loadPortrait(imagePath));
        ui->signinConfirm->setText("登录");
        ui->signinConfirm->setToolTip("登录");
        ui->signinUsername->setEnabled(false);
        ui->signinUsername->setText(currentAccount->getUsername());
        ui->signinUsernameLabel->setText(currentAccount->getUsername());
        ui->signinUsernameLabel->setToolTip(currentAccount->getUsername());
    }
}
void LoginWidget::onSwitchClicked()
{
    QPushButton* send=(QPushButton*)sender();
    if(send->objectName()=="switchSignin"){
        loginType=SIGNIN;
        ui->switchSignin->setStyleSheet("QPushButton{color:#07c160;border:0px;}");
        ui->switchSignin->setFont(QFont("黑体",15,10));
        ui->switchSignup->setStyleSheet("QPushButton{color:black;border:0px;}");
        ui->switchSignup->setFont(QFont("黑体",10,0));
        ui->stackedWidget->setCurrentIndex(0);
        ui->signinPassword->setFocus();
        this->setWindowTitle("登录 - Password Manager");
    }else if(send->objectName()=="switchSignup"){
        loginType=SIGNUP;
        ui->switchSignin->setStyleSheet("QPushButton{color:black;border:0px;}");
        ui->switchSignin->setFont(QFont("黑体",10,0));
        ui->switchSignup->setStyleSheet("QPushButton{color:#07c160;border:0px;}");
        ui->switchSignup->setFont(QFont("黑体",15,10));
        ui->stackedWidget->setCurrentIndex(1);
        ui->signupUsername->setFocus();
        loadNewAccountLayout();
        this->setWindowTitle("注册 - Password Manager");
    }
}
void LoginWidget::onSignupConfirmClicked(){
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    QString currentUsername = ui->signupUsername->text();
    QString currentPassword = ui->signupPassword->text();
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    QString accountsTableName = dataBaseHelper.getAccountsTableName();
    //判断用户名和密码是否合法
    if (!sharedDataHelper.accountList.has(currentUsername)&&currentUsername != "" && currentPassword != "")
    {
        //创建新用户
        QString newPortraitPath = (tempPortraitPath == "")?"":dataPathHelper.getPortraitDir() + "/" + QFileInfo(tempPortraitPath).fileName();
        sharedDataHelper.accountList << new Account(currentUsername,newPortraitPath);
        //更新数据库
        customDataBase.setDatabaseName(databaseName);
        QSqlQuery query(customDataBase.getDatabase());
        if (customDataBase.open()){
            QString insertSql = "insert into " + accountsTableName + " (username,password,portraitPath,createTime,lastEditTime)"
                                                                     "VALUES (:1,:2,:3,:4,:5)";
            query.prepare(insertSql);
            query.bindValue(":1", currentUsername);
            query.bindValue(":2", currentPassword);
            query.bindValue(":3", sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->getPortraitPath());
            query.bindValue(":4", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":5", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.exec();
            customDataBase.close();
        }
        FloatBox::Success("成功创建用户",this);
        QTimer::singleShot(1000,this,[=]{

        });
        emit accountCountChanged(currentUsername);
    }
    else
    {
        QString message = "无法创建用户:\n";
        if(sharedDataHelper.accountList.has(currentUsername))
            message += "用户名\"" + currentUsername + "\"已存在!\n";
        if (currentUsername=="")
            message += "用户名不能为空!\n";
        if (currentPassword=="")
            message += "密码不能为空!";
        QsLogging::Logger::getFileLogger(currentUsername, "ACCOUNT");
        QLOG_WARN() << "Create Account Error:" << message << "!";
        CustomMessageBox::warning(this,"警告", message);
    }
}
void LoginWidget::onSigninConfirmClicked()
{
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    Account* currentAccount = sharedDataHelper.accountList[accountIndex];
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
            if (query.value(0).toString() == ui->signinPassword->text()){
                //登陆成功
                sharedDataHelper.accountList.setActiveAccount(currentUsername);
                QsLogging::Logger::getFileLogger(currentUsername, "ACCOUNT");
                QLOG_INFO() << "Login Account:Login " << currentUsername << " Successfully!";
                FloatBox::Success("登录成功",this);
                QTimer::singleShot(500,this,[=]{
                    //渐变动画窗体消失
                    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
                    anim->setDuration(500);
                    anim->setStartValue(1);
                    anim->setEndValue(0);
                    anim->setEasingCurve(QEasingCurve::OutCurve);
                    anim->start();
                    emit loginAccept();
                    connect(anim, &QPropertyAnimation::finished, [=] {
                        this->close();
                    });
                });
            }else{
                CustomMessageBox::warning(this, "警告", "密码不正确");
                QsLogging::Logger::getFileLogger(currentUsername, "ACCOUNT");
                QLOG_WARN() << "Login Account Error:Wrong Password!";
            }
        }
        customDataBase.close();
    }
}
bool LoginWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->signinPortrait || obj == ui->signupPortrait)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                DataPathHelper &dataPathHelper = DataPathHelper::instance();
                QString path = QFileDialog::getOpenFileName(this, tr("选择头像"), ".", tr("Image Files(*.jpg *.png)"));
                tempPortraitPath = copyFileToPath(path,dataPathHelper.getPortraitDir(),this);
                if (tempPortraitPath == "")
                    return false;
                else{
                    emit portraitDesPathChanged(loadPortrait(tempPortraitPath));
                }
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return QWidget::eventFilter(obj, event);
}
void LoginWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
        if(ui->stackedWidget->currentIndex()==0)
            ui->signinConfirm->click();
        else if(ui->stackedWidget->currentIndex()==1)
            ui->signupConfirm->click();
        break;
    default:
        break;
    }
}
QString LoginWidget::copyFileToPath(QString srcPath, QString desDir,QWidget* w)
{
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    //目标图片绝对路径
    QString desPath = QFileInfo(desDir).absolutePath() + '/' + desDir + "/" + QFileInfo(srcPath).baseName() + '_' + QString::number(QDateTime::currentDateTime().toTime_t()) + '.' + QFileInfo(srcPath).suffix();
    //目标图片相对路径
    QString relativeDesPath = dataPathHelper.getPortraitDir() + "/" + QFileInfo(desPath).fileName();
    //源图片相对路径
    QString relativeSrcPath = dataPathHelper.getPortraitDir() + "/" + QFileInfo(srcPath).fileName();
    //源图片绝对文件夹
    QString absoluteSrcDir = QFileInfo(srcPath).absolutePath();
    //目标图片绝对文件夹
    QString absoluteDesDir = QFileInfo(desPath).absolutePath();
    //如果源图片不存在,返回空值
    if (!QFile::exists(srcPath))
        return "";
    //如果源图片文件夹与目标图片文件夹相同，直接返回源图片相对路径
    if (absoluteSrcDir == absoluteDesDir)
        return relativeSrcPath;
    //如果目标文件夹不存在,创建目标文件夹，创建错误时返回空值
    if (!QDir(desDir).exists())
    {
        if (!QDir(desDir).mkdir(QDir(desDir).absolutePath()))
        {
            CustomMessageBox::critical(w, "错误", "无法创建用户数据文件夹!");
            return "";
        }
    }
    //复制图片
    if (!QFile::copy(srcPath, desPath))
    {
        CustomMessageBox::critical(w, "错误", "无法复制头像!");
        return "";
    }
    else
    {
        //如果复制成功，返回目标图片相对路径
        return relativeDesPath;
    }
    //其余情况都返回空值
    return "";
}
void LoginWidget::onPortraitDesPathChanged(QPixmap pixmap)
{
    DataBaseHelper &dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    QString currentUsername = (ui->stackedWidget->currentIndex()==0)?ui->signinUsername->text():ui->signupUsername->text();
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    if (!sharedDataHelper.accountList.has(currentUsername))
        return;
    else{
        QsLogging::Logger::getFileLogger(currentUsername, "ACCOUNT");
        QLOG_INFO() << "Change Portrait:" << QString("From '" + sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->getPortraitPath() + "' To '" + tempPortraitPath + "'");
        sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->setPortrait(pixmap);
        sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->setPortraitPath(tempPortraitPath);
        sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->setLastEditTime();
        //更新数据库
        QString databaseName = dataPathHelper.getAccountsDataBasePath();
        customDataBase.setDatabaseName(databaseName);
        QSqlQuery query(customDataBase.getDatabase());
        if (customDataBase.open())
        {
            QString updateSql = QString("update " + dataBaseHelper.getAccountsTableName() + " set portraitPath='%1',lastEditTime='%2' where username='%3'")
                    .arg(sharedDataHelper.accountList[sharedDataHelper.accountList.index(currentUsername)]->getPortraitPath())
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(currentUsername);
            query.exec(updateSql);
            customDataBase.close();
        }
    }
}
QPixmap LoginWidget::PixmapToRound(const QPixmap &src, int radius)
{
    if (src.isNull())
        return QPixmap();
    QSize size(2 * radius, 2 * radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(0, 0, size.width(), size.height());
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}
QPixmap LoginWidget::loadPortrait(const QString &portraitPath)
{
    QPixmap image;
    image.load(portraitPath);
    QPixmap pixMap = image.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if(loginType==SIGNIN){
        ui->signinPortrait->setToolTip("选择头像");
        ui->signinPortrait->setPixmap(PixmapToRound(pixMap, 50));
        ui->signinPortrait->setScaledContents(true);
        ui->signinPortrait->setStyleSheet("QLabel{border-radius:5px;border:0px;}");
    }else if(loginType==SIGNUP){
        ui->signupPortrait->setToolTip("选择头像");
        ui->signupPortrait->setPixmap(PixmapToRound(pixMap, 50));
        ui->signupPortrait->setScaledContents(true);
        ui->signupPortrait->setStyleSheet("QLabel{border-radius:5px;border:0px;}");
    }
    return PixmapToRound(pixMap, 50);
}
void LoginWidget::onAccountCountChanged(const QString& currentUsername){
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    int accountCount=sharedDataHelper.accountList.count();
    int index=sharedDataHelper.accountList.index(currentUsername);
    if(accountCount==0){
        ui->switchSignin->setEnabled(false);
        ui->switchSignup->click();
    }else{
        ui->switchSignin->setEnabled(true);
        ui->switchSignin->click();
        if(index!=-1)
            loadAccountLayout(index);
    }
}
LoginWidget::~LoginWidget()
{
    delete ui;
}
