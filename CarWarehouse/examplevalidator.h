#ifndef NAMEVALIDATOR_H
#define NAMEVALIDATOR_H

#include <QValidator>

class NameValidator : public QValidator
{
    Q_OBJECT
public:
    NameValidator(QObject *parent);
    ~NameValidator();

    QValidator::State validate(QString & string, int & pos) const;
};

#endif // NAMEVALIDATOR_H
