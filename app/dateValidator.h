#ifndef DATEVALIDATOR_H
#define DATEVALIDATOR_H

#include <QValidator>

// Class for verifying dates are possible and conform to ISO 8601 standard
// (except without negative years)
class DateValidator : public QValidator
{
    Q_OBJECT
public:
    explicit DateValidator(QObject* parent = nullptr);
    QValidator::State validate(QString&, int&) const override;
};

#endif // DATEVALIDATOR_H
