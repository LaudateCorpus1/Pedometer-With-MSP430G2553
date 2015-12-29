#include "timing.h"
#include "button.h"
#include "digitaltube.h"
#include "eeprom.h"

t_byte sec = 00;  //��������
t_byte min = 00;  //���������
t_byte hour = 00; //����Сʱ��

t_byte STsec = 00;  //�������
t_byte STdsec = 00; //��������

t_byte TA0FLG = 0;  //�������־
t_byte TA1SFLG = 0; //������־
t_byte BeepFLG = 0; //��������ʱ��־
t_byte SleepFLG = Sleeptime;  //������ʱ��־
t_byte MTimeSuspFLG = 0;      //��ʱ��ͣ��־
t_byte STimeSuspFLG = 1;      //�����ͣ��־
#ifdef IsAutoSave
t_byte delaySave = AutoSavetime; //�Զ�������ʱ��־
#endif
void TimerInit()
{  
  //ʱ������  
  BCSCTL3 |= LFXT1S1;
  //TA0��ʱ�����ã�1s
  TA0CTL |= TACLR;
  TA0CTL |= TASSEL_1+ MC_1;
  TA0CCR0 = TARv; 
  TA0CCTL0 |= CCIE; 
  //TA1��ʱ�����ã�0.01s
  TA1CTL |= TACLR;
  TA1CTL |= TASSEL_1+ MC_1;
  TA1CCR0 = TARv/100;
  TA1CCTL0 |= CCIE; 
}

void TAFlagProcess()
{
  if(TA0FLG)
  {
    sec++;
    if(sec==60)
    {
      sec=0;
      min++;
      if(min==60)
      {
        min=0;
        hour++;
      }
    } 
    if(mode==0 || mode == 3 )
      ContentFresh();
    TA0FLG=0;
  }
  if(TA1SFLG)
  {
    STdsec++;
    if(STdsec >= 100)
    {
      STdsec=0;
      STsec++;
    }
    if(mode == 2)
      ContentFresh();
    TA1SFLG=0;
  }
}

//�������ģʽ
void Sleep()
{
#ifdef IsSleep
  SendData(~0x80);//digidis[31]);
  P1OUT |= 0x0F; //��ʾ��....����ʾ����ģʽ
  LPM4;
#endif
}

void SleepFlagProcess()
{
  if(!SleepFLG)
    Sleep();
}

//�˳�����ģʽ
void Wake()
{ /*���ڲ��������ж��е��ú����˳��͹���ģʽ,������������Ч*/ }

//TA0 : 1s�ж�,��ʱ��
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0Intr()
{
  //�������û����ͣ���ô����־
  if(!MTimeSuspFLG)
    TA0FLG = 1;
  //������ʱ��־����
  if(SleepFLG && STimeSuspFLG)
    --SleepFLG;
#ifdef IsAutoSave
  if(!(--delaySave))
  {
    delaySave = AutoSavetime;
    SaveData();
  }
#endif
}

//TA1 : 0.01s�ж�,��ʱ��
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1Intr()
{
  //����������ʱ����
  if(delayFLG1)
    delayFLG1--;
  if(delayFLG2)
    delayFLG2--;
  if(delayFLG3)
    delayFLG3--;
  //��������ʱ����
  if(BeepFLG)
    if(!(--BeepFLG))
      P1OUT |= BIT4;//ʱ�䵽��رշ�����
  //����������ʱ��־����
  if(delayHold1)
    if(!(--delayHold1))
       K1Hold();
  if(delayHold2)
    if(!(--delayHold2))
       K2Hold(); 
  //������û����ͣ���ô����־
  if(!STimeSuspFLG)
    TA1SFLG = 1; 
  //ContentFresh();
}

//�������
void Beep()
{
  P1OUT &= ~BIT4;
  BeepFLG = Beeptime;
}