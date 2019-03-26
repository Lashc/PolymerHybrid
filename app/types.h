#ifndef TYPES_H
#define TYPES_H

#include <QString>

// Structure for grouping information about table fields
struct DatabaseColumn
{
    QString field;
    QString label;
};

// IDs for buttons on the main menu
enum MainMenuButtonID {
    databaseID = 1,
    cameraID,
    manualID
};

// IDs for indexing groups of data in the database
enum DataID {
    printID,
    testID,
    defectID,
    allID
};

// Constants for loop logic
const int NUM_COUPONS = 12;
const int NUM_TENSILE_TESTS = 5;

#endif // TYPES_H
