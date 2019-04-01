#ifndef DATABASECOLUMN_H
#define DATABASECOLUMN_H

#include <QObject>

class QValidator;

// Class for grouping information about table fields
class DatabaseColumn : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseColumn(QString, QObject *parent = nullptr);

private:
public:
    QString field;
    QString label;
    bool required;
    QValidator* validator;

signals:

public slots:
};

#endif // DATABASECOLUMN_H
