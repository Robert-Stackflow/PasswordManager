#ifndef SETTINGHELPER_H
#define SETTINGHELPER_H

#include <QString>
#include "QsLog.h"
class SettingHelper
{
private:
    static SettingHelper *sinstance;
    static bool lockerEnabled;
    static int lockerTimer;
    static bool PasswordAsLocker;
    static QString defaultExportType;
    static bool defaultExportLocationEnabled;
    static QString defaultExportLocation;
    static QsLogging::Level logLevel;
    static int logFileMaxSize;
    static int logFileMaxCount;
    static bool listTipEnabled;
    SettingHelper();
public:
    static SettingHelper& instance();
    static void deleteInstance();
    static bool getLockerEnabled();
    static void setLockerEnabled(bool newLockerEnabled);
    static int getLockerTimer();
    static void setLockerTimer(int newLockerTimer);
    static bool getPasswordAsLocker();
    static void setPasswordAsLocker(bool newPasswordAsLocker);
    static const QString &getDefaultExportType();
    static void setDefaultExportType(const QString &newDefaultExportType);
    static bool getDefaultExportLocationEnabled();
    static void setDefaultExportLocationEnabled(bool newDefaultExportLocationEnabled);
    static const QString &getDefaultExportLocation();
    static void setDefaultExportLocation(const QString &newDefaultExportLocation);
    static QsLogging::Level getLogLevel();
    static void setLogLevel(QsLogging::Level newLogLevel);
    static int getLogFileMaxSize();
    static void setLogFileMaxSize(int newLogFileMaxSize);
    static int getLogFileMaxCount();
    static void setLogFileMaxCount(int newLogFileMaxCount);
    static bool getListTipEnabled();
    static void setListTipEnabled(bool newListTipEnabled);
};

#endif // SETTINGHELPER_H
