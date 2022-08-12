#ifndef LOCKERWIDGET_H
#define LOCKERWIDGET_H

#include <QWidget>
#include "widgets/framelesswidget.h"
namespace Ui {
class LockerWidget;
}

class LockerWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit LockerWidget(QWidget *parent = nullptr);
    ~LockerWidget();
private:
    Ui::LockerWidget *ui;
    void initWidget();
private slots:
    void onCloseClicked();
    void onConfirmClicked();
signals:
    void closeLockerAccept();
};

#endif // LOCKERWIDGET_H
