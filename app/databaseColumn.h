#ifndef DATABASECOLUMN_H
#define DATABASECOLUMN_H

#include <QObject>

class QValidator;

// Class for grouping information about table fields
class DatabaseColumn : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseColumn(const QString&, const QString&, QObject *parent = nullptr);

private:
public:
    QString table;
    QString field;
    QString label;
    bool required;
    QString validatorType;
    QValidator* validator;
    QStringList validatorArgs;

signals:

public slots:
};

#endif // DATABASECOLUMN_H
