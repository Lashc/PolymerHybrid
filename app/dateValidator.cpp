#include "dateValidator.h"
#include <QDate>

DateValidator::DateValidator(QObject* parent)
    : QValidator (parent)
{

}

QValidator::State DateValidator::validate(QString& input, int& pos) const
{
    const int inputLength = input.length();
    QDate today = QDate::currentDate();

    if (input.isNull() || input.isEmpty())
        return Intermediate;
    else {
        bool digitsOK = false;
        if (input[0] == '-' || input[0] == '+' || input.contains(' '))
            return Invalid;
        int inputYear = -1;
        if (inputLength >= 1) {
            int digitsChecked = inputLength > 4 ? 4 : inputLength;
            inputYear = input.mid(0, digitsChecked).toInt(&digitsOK);
            if ((inputYear < 1 && inputLength == 4) ||
                inputYear > today.year() ||
                !digitsOK)
                    return Invalid;
        }
        if (inputLength >= 5)
            if (input[4] != '-')
                return Invalid;
        int inputMonth = -1;
        if (inputLength >= 6) {
            int digitsChecked = inputLength > 7 ? 2 : inputLength - 5;
            inputMonth = input.mid(5, digitsChecked).toInt(&digitsOK);
            if ((inputMonth < 1 && inputLength == 7) ||
                inputMonth > 12 ||
                (inputYear == today.year() && inputMonth > today.month()) ||
                input[5] == '+' ||
                !digitsOK)
                     return Invalid;
        }
        if (inputLength >= 8)
            if (input[7] != '-')
                return Invalid;
        int inputDay;
        if (inputLength >= 9) {
            int digitsChecked = inputLength > 10 ? 2 : inputLength - 8;
            inputDay = input.mid(8, digitsChecked).toInt(&digitsOK);
            if ((inputLength == 10 && !QDate::isValid(inputYear, inputMonth, inputDay)) ||
                (inputYear == today.year() && inputMonth == today.month() && inputDay > today.day()) ||
                input[8] == '+' ||
                !digitsOK)
                    return Invalid;
        }
        if (inputLength == 10)
            return Acceptable;
        else if (inputLength > 10)
            return Invalid;
        return Intermediate;
    }
}
