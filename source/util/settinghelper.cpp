#include "util/settinghelper.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
SettingHelper *SettingHelper::sinstance=0;
bool SettingHelper::lockerEnabled=true;
int SettingHelper::lockerTimer=15;
bool SettingHelper::PasswordAsLocker=true;
QString SettingHelper::defaultExportType="CSV文件(.csv)";
bool SettingHelper::defaultExportLocationEnabled=true;
QString SettingHelper::defaultExportLocation="export/";
QsLogging::Level SettingHelper::logLevel=QsLogging::InfoLevel;
int SettingHelper::logFileMaxSize=25;
int SettingHelper::logFileMaxCount=5;
bool SettingHelper::listTipEnabled=true;
int SettingHelper::getLogFileMaxSize()
{
    return logFileMaxSize;
}

void SettingHelper::setLogFileMaxSize(int newLogFileMaxSize)
{
    logFileMaxSize = newLogFileMaxSize;
}

int SettingHelper::getLogFileMaxCount()
{
    return logFileMaxCount;
}

void SettingHelper::setLogFileMaxCount(int newLogFileMaxCount)
{
    logFileMaxCount = newLogFileMaxCount;
}

bool SettingHelper::getListTipEnabled()
{
    return listTipEnabled;
}

void SettingHelper::setListTipEnabled(bool newListTipEnabled)
{
    listTipEnabled = newListTipEnabled;
}

SettingHelper::SettingHelper()
{
}
SettingHelper &SettingHelper::instance()
{
    if (!sinstance)
        sinstance = new SettingHelper();
    return *sinstance;
}
void SettingHelper::deleteInstance()
{
    delete sinstance;
}
bool SettingHelper::getLockerEnabled()
{
    return lockerEnabled;
}

void SettingHelper::setLockerEnabled(bool newLockerEnabled)
{
    lockerEnabled = newLockerEnabled;
}

int SettingHelper::getLockerTimer()
{
    return lockerTimer;
}

void SettingHelper::setLockerTimer(int newLockerTimer)
{
    lockerTimer = newLockerTimer;
}

bool SettingHelper::getPasswordAsLocker()
{
    return PasswordAsLocker;
}

void SettingHelper::setPasswordAsLocker(bool newPasswordAsLocker)
{
    PasswordAsLocker = newPasswordAsLocker;
}

const QString &SettingHelper::getDefaultExportType()
{
    return defaultExportType;
}

void SettingHelper::setDefaultExportType(const QString &newDefaultExportType)
{
    defaultExportType = newDefaultExportType;
}

bool SettingHelper::getDefaultExportLocationEnabled()
{
    return defaultExportLocationEnabled;
}

void SettingHelper::setDefaultExportLocationEnabled(bool newDefaultExportLocationEnabled)
{
    defaultExportLocationEnabled = newDefaultExportLocationEnabled;
}

const QString &SettingHelper::getDefaultExportLocation()
{
    return defaultExportLocation;
}

void SettingHelper::setDefaultExportLocation(const QString &newDefaultExportLocation)
{
    defaultExportLocation = newDefaultExportLocation;
}

QsLogging::Level SettingHelper::getLogLevel()
{
    return logLevel;
}

void SettingHelper::setLogLevel(QsLogging::Level newLogLevel)
{
    logLevel = newLogLevel;
}
