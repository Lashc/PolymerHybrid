#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

// Dialog abstract base class for adding new records to the database
class DataEntry : public QDialog
{
    Q_OBJECT
public:
    DataEntry(QList<QString>, QList<QString>, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DataEntry();
    virtual QHash<QString, QString> getData() const = 0;

protected:
    QList<QString> dbFields;
    QList<QString> labelTexts;

private:
    QLabel* title;
    QPushButton* submitBtn;
    QPushButton* cancelBtn;
};

#endif // DATAENTRY_H
