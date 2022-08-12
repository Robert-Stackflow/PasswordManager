QT += core gui sql svg xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG -= debug_and_release
CONFIG += console
DESTDIR = bin
INCLUDEPATH = header
UI_DIR = temp/ui
MOC_DIR = temp/moc
RCC_DIR = temp/rcc
OBJECTS_DIR = temp/obj

VERSION = 1.0.0.0
RC_LANG = 0x804,1200
RC_ICONS = logo.ico
QMAKE_TARGET_PRODUCT = Password Manager
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2022 Ruida Xu
QMAKE_TARGET_DESCRIPTION = A local password manager that can customize the groups and export passwords to various files.

SOURCES += \
    source/customField/fielddata.cpp \
    source/customField/fieldfactory.cpp \
    source/delegates/customstyleddelegate.cpp \
    source/dialogs/aboutdialog.cpp \
    source/widgets/lockerwidget.cpp \
    source/util/application.cpp \
    source/util/settinghelper.cpp \
    source/dialogs/settingdialog.cpp \
    source/controller/floatbox.cpp \
    source/controller/dbpage.cpp \
    source/controller/astackedwidget.cpp \
    source/controller/navlistview.cpp \
    source/customField/abstractCustomField.cpp \
    source/customField/customComboBox.cpp \
    source/customField/customLineEdit.cpp \
    source/customField/customTextEdit.cpp \
    source/dialogs/groupdialog.cpp \
    source/dialogs/groupinfodialog.cpp \
    source/dialogs/grouptypedialog.cpp \
    source/dialogs/grouptypeinfodialog.cpp \
    source/dialogs/keyitemdialog.cpp \
    source/dialogs/userhelperdialog.cpp \
    source/function/fieldFunction.cpp \
    source/function/groupFunction.cpp \
    source/function/groupTypeFunction.cpp \
    source/function/keyItemFunction.cpp \
    source/function/loadUserDataFunction.cpp \
    source/function/tableWidgetFunction.cpp \
    source/function/viewFunction.cpp \
    source/widgets/customtablewidget.cpp \
    source/widgets/framelesswidget.cpp \
    source/widgets/homewidget.cpp \
    source/widgets/loginwidget.cpp \
    source/main.cpp \
    source/structure/account.cpp \
    source/structure/accounts.cpp \
    source/structure/autofillInfo.cpp \
    source/structure/fieldTypes.cpp \
    source/structure/group.cpp \
    source/structure/groupType.cpp \
    source/structure/groupTypes.cpp \
    source/structure/groups.cpp \
    source/structure/keyItem.cpp \
    source/util/customdatabase.cpp \
    source/util/databasehelper.cpp \
    source/util/datapathhelper.cpp \
    source/util/shareddatahelper.cpp \
    source/widget.cpp \
    source/controller/custommessagebox.cpp \
    source/dialogs/fielddialog.cpp \
    source/dialogs/userdialog.cpp \
    source/dialogs/exportdialog.cpp

HEADERS += \
    header/controller/astackedwidget.h \
    header/controller/dbpage.h \
    header/controller/floatbox.h \
    header/controller/navlistview.h \
    header/customField/abstractCustomField.h \
    header/customField/customComboBox.h \
    header/customField/customLineEdit.h \
    header/customField/customTextEdit.h \
    header/customField/fielddata.h \
    header/customField/fieldfactory.h \
    header/dialogs/settingdialog.h \
    header/structure/account.h \
    header/structure/accounts.h \
    header/structure/autofillInfo.h \
    header/structure/fieldTypes.h \
    header/structure/group.h \
    header/structure/groupType.h \
    header/structure/groupTypes.h \
    header/structure/groups.h \
    header/structure/keyItem.h \
    header/util/application.h \
    header/util/customdatabase.h \
    header/util/databasehelper.h \
    header/util/datapathhelper.h \
    header/util/settinghelper.h \
    header/util/shareddatahelper.h \
    header/widget.h \
    header/widgets/framelesswidget.h \
    header/widgets/lockerwidget.h \
    header/widgets/loginwidget.h \
    header/widgets/customtablewidget.h \
    header/dialogs/groupinfodialog.h \
    header/controller/custommessagebox.h \
    header/widgets/homewidget.h \
    header/dialogs/groupdialog.h \
    header/dialogs/grouptypedialog.h \
    header/dialogs/grouptypeinfodialog.h \
    header/dialogs/fielddialog.h \
    header/dialogs/keyitemdialog.h \
    header/dialogs/userdialog.h \
    header/dialogs/userhelperdialog.h \
    header/dialogs/exportdialog.h \
    header/dialogs/aboutdialog.h \
    header/delegates/customstyleddelegate.h \
    header/delegates/Delegates.h

FORMS += \
    ui/dialogs/aboutdialog.ui \
    ui/widgets/lockerwidget.ui \
    ui/dialogs/exportdialog.ui \
    ui/dialogs/userhelperdialog.ui \
    ui/dialogs/settingdialog.ui \
    ui/dialogs/keyitemdialog.ui \
    ui/dialogs/fielddialog.ui \
    ui/dialogs/grouptypeinfodialog.ui \
    ui/controller/floatbox.ui \
    ui/dialogs/groupdialog.ui \
    ui/dialogs/grouptypedialog.ui \
    ui/dialogs/userdialog.ui \
    ui/widgets/homewidget.ui \
    ui/controller/custommessagebox.ui \
    ui/dialogs/groupinfodialog.ui \
    ui/widgets/customtablewidget.ui \
    ui/customField/customComboBox.ui \
    ui/customField/customLineEdit.ui \
    ui/customField/customTextEdit.ui \
    ui/widgets/loginwidget.ui \
    ui/widget.ui

RESOURCES += \
    qrc/dark/dark.qrc \
    qrc/custom/custom.qrc \
    qrc/win11_dark/win11_dark.qrc \
    qrc/win11_light/win11_light.qrc

QXLSX_PARENTPATH=./lib/QXlxs
QXLSX_HEADERPATH=./lib/QXlxs/QXlxsHeader/
QXLSX_SOURCEPATH=./lib/QXlxs/QXlxsSource/
include(./lib/QXlxs/QXlsx.pri)

QSLOG_PARENTPATH=./lib/QsLog
QSLOG_HEADERPATH=./lib/QsLog/
QSLOG_SOURCEPATH=./lib/QsLog/
include(./lib/QsLog/QsLog.pri)

QTAWESOME_PARENTPATH=./lib/QtAwesome
QTAWESOME_HEADERPATH=./lib/QtAwesome/
QTAWESOME_SOURCEPATH=./lib/QtAwesome/
include(./lib/QtAwesome/QtAwesome.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
