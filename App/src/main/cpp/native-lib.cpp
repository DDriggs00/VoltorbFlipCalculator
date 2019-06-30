#include <jni.h>
#include <string>
#include <list>

#include "MainCppClass.h"
#include "board.h"
#include "cellStats.h"
#include "Game.h"


//Create Class
extern "C" JNIEXPORT jlong JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_CreateCppClass(JNIEnv *env, jobject instance) {
    return (long)(new MainCppClass());
}

//Delete Class
extern "C" JNIEXPORT void JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_DeleteCppClass(JNIEnv *env, jobject instance, jlong cppClassPtr) {
    delete (MainCppClass*)(cppClassPtr);
}

//Verify Inputs
extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_VerifyInputs
        (JNIEnv *env, jobject instance, jintArray rowTotals_, jintArray rowVoltorbs_, jintArray colTotals_, jintArray colVoltorbs_, jlong cppClassPtr) {

    jint *rowTotals = env->GetIntArrayElements(rowTotals_, NULL);
    jint *rowVoltorbs = env->GetIntArrayElements(rowVoltorbs_, NULL);
    jint *colTotals = env->GetIntArrayElements(colTotals_, NULL);
    jint *colVoltorbs = env->GetIntArrayElements(colVoltorbs_, NULL);

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    bool GoodInputs = mainClassPtr->VerifyInputs(rowTotals, rowVoltorbs, colTotals, colVoltorbs);

    env->ReleaseIntArrayElements(rowTotals_, rowTotals, 0);
    env->ReleaseIntArrayElements(rowVoltorbs_, rowVoltorbs, 0);
    env->ReleaseIntArrayElements(colTotals_, colTotals, 0);
    env->ReleaseIntArrayElements(colVoltorbs_, colVoltorbs, 0);

    return (jboolean)GoodInputs;
}

// Do Initial Calculations
extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_InitialCalculations
        (JNIEnv *env, jobject instance, jintArray rowTotals_, jintArray rowVoltorbs_, jintArray colTotals_, jintArray colVoltorbs_, jlong cppClassPtr) {

    jint *rowTotals = env->GetIntArrayElements(rowTotals_, NULL);
    jint *rowVoltorbs = env->GetIntArrayElements(rowVoltorbs_, NULL);
    jint *colTotals = env->GetIntArrayElements(colTotals_, NULL);
    jint *colVoltorbs = env->GetIntArrayElements(colVoltorbs_, NULL);

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;
	bool good = mainClassPtr->Calculate(rowTotals, rowVoltorbs, colTotals, colVoltorbs);


    env->ReleaseIntArrayElements(rowTotals_, rowTotals, 0);
    env->ReleaseIntArrayElements(rowVoltorbs_, rowVoltorbs, 0);
    env->ReleaseIntArrayElements(colTotals_, colTotals, 0);
    env->ReleaseIntArrayElements(colVoltorbs_, colVoltorbs, 0);

	return (jboolean)good;
}

// Get the value of a cell to print
extern "C" JNIEXPORT jstring JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_GetCellValue
        (JNIEnv *env, jobject instance, jint row, jint col, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    std::string cellVal = mainClassPtr->getDisplayCell(row, col);

    char temp[12];
    strcpy(temp, cellVal.c_str());
    return (*env).NewStringUTF(temp);
}

//Flip a cell
extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_FlipCell
        (JNIEnv *env, jobject instance, jint row, jint col, jint val, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    return (jboolean)(mainClassPtr->FlipCell(row, col, val));


}

// Returns how many layouts have the specific cell as the specific value
extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_GetNumValues
        (JNIEnv *env, jobject instance, jint row, jint col, jint val, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    return mainClassPtr->getMatchingLayouts(row, col, val);
}

// Returns the total number of layouts
extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_GetTotalLayouts(JNIEnv *env, jobject instance, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    return mainClassPtr->getTotalLayouts();
}

// Returns if the selected cell is the best cell, but still has risk.
extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_IsBestCell(JNIEnv *env, jobject instance, jint row, jint col, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    return (jboolean)mainClassPtr->isBestCell(row, col);

}

extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_MainActivity_IsKnown(JNIEnv *env, jobject instance, jint row, jint col, jlong cppClassPtr) {

    MainCppClass *mainClassPtr = (MainCppClass*)cppClassPtr;

    return (jboolean)mainClassPtr->IsKnown(row, col);
}

// Below here is for the game part of the app
extern "C" JNIEXPORT jlong JNICALL
Java_jolteon_voltorbflipcalculator_Game_createGamePointer(JNIEnv *env, jobject instance) {
    long gameptr = (long)(new Game());
    jlong  test = gameptr;
    return test;
}

extern "C" JNIEXPORT void JNICALL
Java_jolteon_voltorbflipcalculator_Game_deleteGamePointer(JNIEnv *env, jobject instance, jlong gamePtr) {
    delete (Game*)(gamePtr);
}

extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_Game_gameFlipCell(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return gameClassPtr->flipCell(row, col);
}

extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_Game_gameGetCell(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return gameClassPtr->getCellVal(row,col);
}

extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_Game_getTotal(JNIEnv *env, jobject instance, jboolean isVoltorb, jboolean isCol, jint index, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return gameClassPtr->getTotal((bool)isVoltorb,(bool)isCol,index);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_Game_gameIsFlipped(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return (jboolean)(gameClassPtr->isFlipped(row,col));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_Game_isWin(JNIEnv *env, jobject instance, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return (jboolean)(gameClassPtr->isWin());


}

extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_Game_getPoints(JNIEnv *env, jobject instance, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return gameClassPtr->getPoints();
}


extern "C" JNIEXPORT void JNICALL
Java_jolteon_voltorbflipcalculator_Game_startNextLevel(JNIEnv *env, jobject instance, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    gameClassPtr->nextLevel();
}

extern "C" JNIEXPORT jint JNICALL
Java_jolteon_voltorbflipcalculator_Game_getCurrentLevel(JNIEnv *env, jobject instance, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return gameClassPtr->getLevel();
}

extern "C" JNIEXPORT void JNICALL
Java_jolteon_voltorbflipcalculator_Game_flag(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    gameClassPtr->flag(row, col);
    return;
}

extern "C" JNIEXPORT void JNICALL
Java_jolteon_voltorbflipcalculator_Game_unflag(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    gameClassPtr->unflag(row, col);
    return;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_jolteon_voltorbflipcalculator_Game_isFlagged(JNIEnv *env, jobject instance, jint row, jint col, jlong gamePtr) {
    Game *gameClassPtr = (Game*)(gamePtr);
    return (jboolean)(gameClassPtr->isFlagged(row, col));
}