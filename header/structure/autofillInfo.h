#ifndef AUTOFILLINFO_H
#define AUTOFILLINFO_H
#include <QStringList>
class AutofillInfo{
private:
    QStringList mails;
    QStringList mobiles;
    QStringList websites;
    QStringList mailAliases;
public:
    const QStringList &getMails() const;
    const QStringList &getMobiles() const;
    const QStringList &getWebsites() const;
    const QStringList &getMailAliases() const;
    void addMail(const QString newMail,const QString newMailAlias="");
    void setMailAlias(const QString mail,const QString newMailAlias);
    void addMobile(const QString newMobile);
    void addWebsite(const QString newWebsite);
    void removeMail(const QString mail);
    void removeMobile(const QString mobile);
    void removeWebsite(const QString website);
};
#endif // AUTOFILLINFO_H
