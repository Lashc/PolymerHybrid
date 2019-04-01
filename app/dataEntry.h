#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QDialog>

class DatabaseColumn;
class QLabel;

// Dialog abstract base class for adding new records to the database
class DataEntry : public QDialog
{
    Q_OBJECT
public:
    explicit DataEntry(const QVector<DatabaseColumn*>&, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~DataEntry();
    virtual QStringList getData() const = 0;

protected:
    QVector<DatabaseColumn*> columns;

private:
    QLabel* title;
    QPushButton* submitBtn;
    QPushButton* cancelBtn;
};

#endif // DATAENTRY_H
