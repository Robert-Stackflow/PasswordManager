#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "structure/groupType.h"
#include <QWidget>

namespace Ui {
class FieldDialog;
}

class FieldDialog : public QWidget
{
    Q_OBJECT

public:
    QString groupTypeName;
    AbstractCustomField* newField;
public:
    explicit FieldDialog(const QString& groupTypeName,QWidget *parent = nullptr);
    void setWindowCustomTitle(const QString& newWindowTitle);
    ~FieldDialog();
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::FieldDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
    customLineEdit* fieldName;
    customComboBox* fieldType;
    customLineEdit* fieldPlaceholderText;
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void newFieldAccept(const QString& groupTypeName,const FieldData& fieldData);
};

#endif // FIELDDIALOG_H
