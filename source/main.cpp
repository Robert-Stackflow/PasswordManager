#include "util/application.h"
#include "widgets/framelesswidget.h"
#include "widgets/loginwidget.h"
#include "ui_loginwidget.h"
#include "ui_widget.h"
#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    Application a(argc, argv);
    LoginWidget login;
    login.show();
    login.move((QApplication::desktop()->width() - login.width())/2,(QApplication::desktop()->height() - login.height())/2);
    Widget w;
    QObject::connect(&login,SIGNAL(loginAccept()),&w,SLOT(loginReceive()));
    a.setWidget(&w);
    return a.exec();
}
