#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QLabel>

// Class for database menu
class Database : public QWidget
{
    Q_OBJECT
public:
    explicit Database(QWidget *parent = nullptr);
    ~Database();

private:
    QLabel* title; // Temporary widget to test with

signals:

public slots:
};

#endif // DATABASE_H
