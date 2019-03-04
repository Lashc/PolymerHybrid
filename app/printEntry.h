#ifndef PRINTENTRY_H
#define PRINTENTRY_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

class PrintEntry : public QDialog
{
    Q_OBJECT
public:
    explicit PrintEntry(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QList<QString> getData() const;

private:
    QList<QLineEdit *> inputList;
    QList<QString> inputLabels;
    QList<QString> data;
    QPushButton* submitBtn;
    QPushButton* cancelBtn;

    void initLineEdits(QGridLayout*);

public slots:
    void populateMap();
};

#endif // PRINTENTRY_H
