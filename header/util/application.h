#ifndef APPLICATION_H
#define APPLICATION_H

#include "widget.h"
#include "widgets/lockerwidget.h"
#include <QApplication>

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int & argc, char ** argv);
    ~Application();
    bool notify(QObject *, QEvent *);
    void setWidget(Widget* w);
private:
    LockerWidget *lockerWidget;
    Widget *widget;
    QTimer *timer;
    QThread *thread;
};

#endif // APPLICATION_H
