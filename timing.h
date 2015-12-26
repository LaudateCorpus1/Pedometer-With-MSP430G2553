#ifndef TIMING
#define TIMING

#include "universal.h"

#define TARv 11430 //1s��Ӧ��TA0R��������ʵ�ʵ�ѹ��Ҫ����  //MSP430VCC����ʱ��VCLOCK ʵ��Ϊ11430Hz

#define Beeptime 32  //���������ʱ�� (x0.01s)
#define Sleeptime 10  //����Ӧ������ʱ�� (x1s)
#define AutoSavetime 30 //�Զ������ʱ�� (x1s)

extern t_byte sec;
extern t_byte min;
extern t_byte hour;
extern t_byte SleepFLG;   
extern t_byte MTimeSuspFLG;  
extern t_byte STimeSuspFLG;  

extern t_byte STsec;
extern t_byte STdsec;
#ifdef IsAutoSave
extern t_byte delaySave;
#endif

void TAFlagProcess();
void SleepFlagProcess();
void TimerInit();
void Beep();
void Wake();

#endif