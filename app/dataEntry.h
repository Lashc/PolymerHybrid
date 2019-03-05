#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

// Dialog abstract base class for adding new records to the database
class DataEntry : public QDialog
{
    Q_OBJECT
public:
    DataEntry(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DataEntry();
    virtual QHash<QString, QString> getData() const;

protected:
    QList<QLabel *> labelList;
    QList<QLineEdit *> lineEditList;
    QList<QString> dbFields;
    QLabel* title;
    QPushButton* submitBtn;
    QPushButton* cancelBtn;

private:
    virtual void configureLayout();
    // These two functions may be modified to eliminate the need for pure virtual
    virtual QList<QString> createLabels() = 0;
    virtual void setDBFields() = 0;
};

#endif // DATAENTRY_H
