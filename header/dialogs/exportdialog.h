#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "qtoolbutton.h"
#include <QWidget>
#include <QMap>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();
    void setWindowCustomTitle(const QString& newWindowTitle);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
    Ui::ExportDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
    QMap<QToolButton*,int> buttonToIconMap;
    QList<QToolButton*> buttons;
public slots:
    void onCloseClicked();
    void onMinimizeClicked();
    void onButtonsClicked();
    void onOperationButtonsClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void settingsChanged();
};

#endif // EXPORTDIALOG_H
