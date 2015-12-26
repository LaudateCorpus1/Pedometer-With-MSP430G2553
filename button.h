#ifndef BUTTON
#define BUTTON

#include "universal.h"

#define ButtonDelay 32 //���������ӳ�ʱ�� (x0.01s)
#define ButtonHold 255 //���������ж�ʱ�� (x0.01s)

extern t_byte delayFLG1;
extern t_byte delayFLG2;
extern t_byte delayFLG3;
extern t_byte delayHold1;
extern t_byte delayHold2;
extern t_byte ClrDisp;

void K1Hold();
void K2Hold();
void ButtonInit();
void SensorFlagProcess();
void ButtonHoldProcess();

#endif