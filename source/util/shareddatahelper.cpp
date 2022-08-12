#include "util/shareddatahelper.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
SharedDataHelper* SharedDataHelper::sinstance=nullptr;
Accounts SharedDataHelper::accountList=Accounts();
Groups SharedDataHelper::groupList=Groups();
GroupTypes SharedDataHelper::groupTypeList=GroupTypes();
FieldTypes SharedDataHelper::fieldTypeList=FieldTypes();
AutofillInfo SharedDataHelper::autofillInfo=AutofillInfo();
SharedDataHelper::SharedDataHelper(){
    //初始化字段类型列表
    fieldTypeList.insert("单行文本",FieldData(FieldData::LINEEDIT,FieldData::NORMAL));
    fieldTypeList.insert("多行文本",FieldData(FieldData::TEXTEDIT,FieldData::NORMAL));
    fieldTypeList.insert("密码",FieldData(FieldData::LINEEDIT,FieldData::PASSWORD));
    fieldTypeList.insert("邮箱",FieldData(FieldData::LINEEDIT,FieldData::MAIL));
    fieldTypeList.insert("电话",FieldData(FieldData::LINEEDIT,FieldData::MOBILE));
    fieldTypeList.insert("网址",FieldData(FieldData::LINEEDIT,FieldData::WEBSITE));
    fieldTypeList.insert("自动填充-邮箱",FieldData(FieldData::COMBOBOX,FieldData::MAIL));
    fieldTypeList.insert("自动填充-电话",FieldData(FieldData::COMBOBOX,FieldData::MOBILE));
    fieldTypeList.insert("自动填充-网址",FieldData(FieldData::COMBOBOX,FieldData::WEBSITE));
}
SharedDataHelper& SharedDataHelper::instance(){
    if (!sinstance)
        sinstance = new SharedDataHelper();
    return *sinstance;
}
void SharedDataHelper::deleteInstace(){
    delete sinstance;
}
