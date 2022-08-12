#ifndef KEYITEMDIALOG_H
#define KEYITEMDIALOG_H

#include "structure/groupType.h"
#include "structure/keyItem.h"
#include <QWidget>

namespace Ui {
class KeyItemDialog;
}

class KeyItemDialog : public QWidget
{
    Q_OBJECT

public:
    QString groupName;
    KeyItem* newKeyItem;
    QList<AbstractCustomField*> controllerList;
public:
    explicit KeyItemDialog(const QString& groupName,QWidget *parent = nullptr);
    void setWindowCustomTitle(const QString& newWindowTitle);
    ~KeyItemDialog();
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::KeyItemDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
    QVBoxLayout* infoLayout;
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void newKeyItemAccept(const QString& groupName,KeyItem* newKeyItem);
};

#endif // KEYITEMDIALOG_H
