#include "namevalidator.h"


NameValidator::NameValidator(QObject* parent)
    : QValidator (parent)
{

}
NameValidator::~NameValidator()
{

}

QValidator::State NameValidator::validate(QString &string, int &pos) const
{

}
