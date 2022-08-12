#include "util/application.h"
#include "controller/floatbox.h"
#include "qdebug.h"
#include "qdesktopwidget.h"
#include "qthread.h"
#include "qtimer.h"
#include "util/settinghelper.h"
#include "widgets/loginwidget.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
Application::Application(int &argc, char **argv)
    : QApplication(argc,argv)
{
    thread = new QThread(this);
    timer = new QTimer();
    timer->moveToThread(thread);
    timer->setInterval(600000);
//    connect(thread, SIGNAL(started()), timer, SLOT(start()));
    if(!thread->isRunning())
        thread->start();
}

Application::~Application()
{
    thread->requestInterruption();
    thread->quit();
    thread->wait();
}
void Application::setWidget(Widget* w)
{
    widget=w;
    connect(timer, SIGNAL(timeout()),w,SLOT(openLocker()));
}
bool Application::notify(QObject *obj, QEvent *e)
{
    if(thread!=nullptr&&timer!=nullptr){
        if(e->type() == QEvent::MouseMove||e->type() == QEvent::MouseButtonPress)
            thread->quit();
//        else if(e->type() == QEvent::MouseButtonRelease||!(timer->isActive()))
//            thread->start();
    }
    return QApplication::notify(obj,e);
}
