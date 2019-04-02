#ifndef FOREIGNKEYVALIDATOR_H
#define FOREIGNKEYVALIDATOR_H

#include <QValidator>

// Class for validating foreign keys in a database
class ForeignKeyValidator : public QValidator
{
    Q_OBJECT
public:
    explicit ForeignKeyValidator(const QString&, const QString&, QObject* parent = nullptr);
    QValidator::State validate(QString&, int&) const override;

private:
    QString table;
    QString foreignTable;
};

#endif // FOREIGNKEYVALIDATOR_H
