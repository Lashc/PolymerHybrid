#ifndef ENTRYVIEW_H
#define ENTRYVIEW_H

#include <QDialog>

class QSqlRecord;

// Dialog class for viewing existing database entries in read-only mode
class EntryView : public QDialog
{
    Q_OBJECT
public:
    explicit EntryView(int, int, QVector<QString>, const QSqlRecord&, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // ENTRYVIEW_H
