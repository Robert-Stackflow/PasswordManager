#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H
#include <QMap>
#include "qtoolbutton.h"
#include <QWidget>
namespace Ui {
class SettingDialog;
}

class SettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();
    void setWindowCustomTitle(const QString& newWindowTitle);
protected:
    void loadSettings();
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
    Ui::SettingDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
    QMap<QToolButton*,int> buttonToIconMap;
    QList<QToolButton*> buttons;
public slots:
    void onSettingsChanged();
    void onCloseClicked();
    void onMinimizeClicked();
    void onButtonsClicked();
    void onOperationButtonsClicked();
    void onFeedbackClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void settingsChanged();
};

#endif // SETTINGDIALOG_H
