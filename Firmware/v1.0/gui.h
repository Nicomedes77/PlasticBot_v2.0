#ifndef H_GUI
#define H_GUI

#include "v1.0.h"

void PresentacionLCD(void);
void updateDataGUI(int *_indexes , int *_btn , PETfilConv *_petFilConv , int *currentScreen , int *_vel_col);
void updateLCD(PETfilConv *_petFilConv , int *currentScreen);
void printScreenSettingTemp(PETfilConv *_petFilConv);
void printSettingTemp(PETfilConv *_petFilConv);
void printscreenWorking(PETfilConv *_petFilConv);
void printCurrentTemp(PETfilConv *_petFilConv);
void updateCursor(int *_indexes , PETfilConv *_petFilConv , int *currentScreen , int *_vel_col);
void printX(uint8_t i);
void printCursor(uint8_t index);
void clearAllCursor(void);
void printScreen(char *screen[]);
void clearScreen(void);

#endif // H_GUI
