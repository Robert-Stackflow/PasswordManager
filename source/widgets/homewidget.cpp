#include "widgets/homewidget.h"
#include "qdebug.h"
#include "ui_homewidget.h"
#include <QGraphicsDropShadowEffect>
HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::HomeWidget) {
    ui->setupUi(this);
    initWidget();
}

HomeWidget::~HomeWidget()
{
    delete ui;
}
void HomeWidget::initWidget()
{
    setShadow(ui->groupOverviewWidget);
    setShadow(ui->groupTypeOverviewWidget);
}
void HomeWidget::setShadow(QWidget* widget)
{
    if(widget==nullptr)
        return;
    widget->setStyleSheet(widget->styleSheet()+"QWidget#"+widget->objectName()+
                          "{border-top: 5px solid qlineargradient(y0:0, y1:1,stop: 0 #ececef, stop: 1 #f5f5f5);"
                          "border-left: 5px solid qlineargradient(x0:0, x1:1,stop: 0 #ececef, stop: 1 #f5f5f5);"
                          "border-bottom: 5px solid qlineargradient(y0:0, y1:1,stop: 0 #f5f5f5, stop: 1 #ececef);"
                          "border-right: 5px solid qlineargradient(x0:0, x1:1,stop:  0 #f5f5f5, stop: 1 #ececef);}");
}
