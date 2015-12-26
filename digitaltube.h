#ifndef DIGTUB
#define DIGTUB

#include "universal.h"

void DigiTubeProcess();
void DigiTubeInit();
void ContentFresh();
void SendData(t_byte d);

extern t_byte Stepmode;

#define LongStep 1000 //��λmm
#define ShortStep 600 //��λmm

#endif