#include "query.h"
#include "types.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QVector>

namespace polyDB {
    QSqlError insertInto(const int dataID, QStringList fields, QStringList data)
    {
        // Formulate and execute 'INSERT INTO' queries
        const int numData = data.length();
        const int numFields = fields.length();
        QSqlQuery q;
        QString insertQuery = "INSERT INTO ";

        if (dataID == printID || dataID == defectID) {
            // Simply insert all data into the correct table
            QString table = dataID == printID ? "prints" : "defects";
            insertQuery.append(table + " (" + fields.join(", ") + ") VALUES (");
            for (int i = 0; i < numFields - 1; i++)
                insertQuery.append(":" + fields[i] + ", ");
            insertQuery.append(":" + fields.last() + ");");
            q.prepare(insertQuery);
            for (int i = 0; i < numData; i++)
                q.bindValue(i, data[i].isEmpty() ? QVariant(QVariant::String) : data[i]);
            q.exec();
        }

        else if (dataID == testID) {
            // Get the tensile and tolerance fields and put them in lists
            const int tensileIdx = fields.indexOf("tolerance_id");
            QStringList toleranceFields = fields.mid(0, tensileIdx);
            QStringList tensileFields = fields.mid(tensileIdx);

            // Form and execute query for 'tolerances' table
            insertQuery.append(" tolerances (" + toleranceFields.join(", ") + ") VALUES (");
            const int numTolerances = toleranceFields.length();
            for (int i = 0; i < numTolerances - 1; i++)
                insertQuery.append(":" + toleranceFields[i] + ", ");
            insertQuery.append(":" + toleranceFields[numTolerances - 1] + ");");
            q.prepare(insertQuery);
            for (int i = 0; i < numTolerances; i++)
                q.bindValue(i, data[i].isEmpty() ? QVariant(QVariant::String) : data[i]);
            if (!q.exec())
                return q.lastError();

            // Get the tolerance ID from the previous insertion
            q.exec("SELECT last_insert_rowid();");
            q.first();
            const int toleranceID = q.value(0).toInt();

            // Form query for 'tensiles' table
            const int numTensiles = tensileFields.length();
            insertQuery = "INSERT INTO tensiles (" + tensileFields.join(", ") + ")" + "VALUES (";
            for (int i = 0; i < numTensiles - 1; i++)
                insertQuery.append(":" + tensileFields[i] + ", ");
            insertQuery.append(":" + tensileFields[numTensiles - 1] + ");");
            q.prepare(insertQuery);

            // Gather tensile data and execute a query for each coupon
            QVector<QStringList> tensiles;
            for (int i = numTolerances; i < data.length(); i++)
                tensiles.append(data[i].split(", "));
            for (int c = 0; c < NUM_COUPONS; c++) {
                q.bindValue(0, toleranceID);
                q.bindValue(1, c + 1);
                for (int t = 0; t < NUM_TENSILE_TESTS; t++)
                    q.bindValue(t + 2, tensiles[c][t].isEmpty() ? QVariant(QVariant::String) : tensiles[c][t]);
                if (!q.exec())
                    return q.lastError();
            }
        }
        return q.lastError();
    }

    QSqlError update(const int dataID, QStringList fields, QStringList data, const int rowID)
    {
        // Formulate and execute 'UPDATE' queries
        const int numData = data.length();
        const int numFields = fields.length();
        QSqlQuery q;
        QString updateQuery = "UPDATE ";

        if (dataID == printID || dataID == defectID) {
            // Overwrite all data for the row in the correct table
            QString table = dataID == printID ? "prints" : "defects";
            updateQuery.append(table + " SET ");
            for (int i = 0; i < numFields - 1; i++)
                updateQuery.append(fields[i] + "=:" + fields[i] + ", ");
            updateQuery.append(fields.last() + "=:" + fields.last() + " WHERE id=" + QString::number(rowID) + ";");
            q.prepare(updateQuery);
            for (int i = 0; i < numData; i++)
                q.bindValue(i, data[i].isEmpty() ? QVariant(QVariant::String) : data[i]);
            q.exec();
        }

        else if (dataID == testID) {
            // Get the tensile and tolerance fields and put them in lists
            const int tensileIdx = fields.indexOf("tolerance_id");
            QStringList toleranceFields = fields.mid(0, tensileIdx);
            QStringList tensileFields = fields.mid(tensileIdx);

            // Form and execute query for 'tolerances' table
            updateQuery.append("tolerances SET ");
            const int numTolerances = toleranceFields.length();
            for (int i = 0; i < numTolerances - 1; i++)
                updateQuery.append(toleranceFields[i] + "=:" + toleranceFields[i] + ", ");
            updateQuery.append(toleranceFields.last() + "=:" + toleranceFields.last() + " WHERE id=" + QString::number(rowID) + ";");
            q.prepare(updateQuery);
            for (int i = 0; i < numTolerances; i++)
                q.bindValue(i, data[i].isEmpty() ? QVariant(QVariant::String) : data[i]);
            if (!q.exec())
                return q.lastError();

            // Form query for 'tensiles' table
            updateQuery = "UPDATE tensiles SET ";
            const int numTensiles = tensileFields.length();
            for (int i = 2; i < numTensiles - 1; i++)
                updateQuery.append(tensileFields[i] + "=:" + tensileFields[i] + ", ");
            updateQuery.append(tensileFields.last() + "=:" + tensileFields.last()
                               + " WHERE tolerance_id=" + QString::number(rowID) + " AND coupon=:coupon;");
            q.prepare(updateQuery);

            // Gather tensile data and execute a query for each coupon
            QVector<QStringList> couponTensiles;
            for (int i = numTolerances; i < data.length(); i++)
                couponTensiles.append(data[i].split(", "));
            for (int c = 0; c < NUM_COUPONS; c++) {
                for (int t = 0; t < NUM_TENSILE_TESTS; t++)
                    q.addBindValue(couponTensiles[c][t].isEmpty() ? QVariant(QVariant::String) : couponTensiles[c][t]);
                q.addBindValue(c + 1);
                if (!q.exec())
                    return q.lastError();
            }
        }
        return q.lastError();
    }
}
