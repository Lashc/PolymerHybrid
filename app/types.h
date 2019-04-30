#ifndef TYPES_H
#define TYPES_H

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
const int NUM_TENSILE_TESTS = 4;

#endif // TYPES_H
