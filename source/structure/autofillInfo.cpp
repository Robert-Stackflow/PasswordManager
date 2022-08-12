#include "structure/autofillInfo.h"
const QStringList &AutofillInfo::getMails() const
{
    return mails;
}

const QStringList &AutofillInfo::getMobiles() const
{
    return mobiles;
}

const QStringList &AutofillInfo::getWebsites() const
{
    return websites;
}

const QStringList &AutofillInfo::getMailAliases() const
{
    return mailAliases;
}
void AutofillInfo::addMail(const QString newMail,const QString newMailAlias){
    if(!mails.contains(newMail)||newMail=="")
        mails<<newMail,mailAliases<<newMailAlias;
}
void AutofillInfo::setMailAlias(const QString mail,const QString newMailAlias){
    int index=mails.indexOf(mail);
    if(index==-1){
        addMail(mail,newMailAlias);
    }else{
        mailAliases[index]=newMailAlias;
    }
}
void AutofillInfo::addMobile(const QString newMobile){
    if(!mails.contains(newMobile)||newMobile=="")
        mobiles<<newMobile;
}
void AutofillInfo::addWebsite(const QString newWebsite){
    if(!mails.contains(newWebsite)||newWebsite=="")
        websites<<newWebsite;
}
void AutofillInfo::removeMail(const QString mail){
    mails.removeOne(mail);
}
void AutofillInfo::removeMobile(const QString mobile){
    mobiles.removeOne(mobile);
}
void AutofillInfo::removeWebsite(const QString website){
    websites.removeOne(website);
}
