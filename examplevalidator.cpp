#include "namevalidator.h"

NameValidator::NameValidator(QObject* parent)
    : QValidator (parent)
{

}
NameValidator::~NameValidator()
{

}
// Валидатор для строки с названием (Из базового задания)
QValidator::State NameValidator::validate(QString &string, int &pos) const
{
    QString allowed("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 ");
    QString alph("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm");

    for (int i = 0; i < string.length(); i++){
        if (!allowed.contains(string[i])){
            return QValidator::Invalid;
        }
    }
    if (!string.isEmpty() && !(alph.contains(string[0]))){
        return QValidator::Invalid;
    }
}
