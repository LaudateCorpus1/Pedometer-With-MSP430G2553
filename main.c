#include "universal.h"
#include "eeprom.h"
#include "digitaltube.h"
#include "timing.h"
#include "button.h"

unsigned long steps = 0; //123456789UL;
t_byte conts[4]={20,23,24,22} ;//�������λ���ݣ�Ĭ����ʾ"Proc"����ʾ�ڶ�ȡ�洢��
t_byte mode = 0; //0:��ʾ����ʱ��  1:��ʾ����  2:��� 3:��ʾ�ٶ� 4:����ģʽ
t_byte StepOffset = 0;//��ʾ����ʱ��������λƫ��,ʹ��long�����������9λ�Ĳ���
t_byte TimeOffset = 0;//0:��ʾmm.ss  1:��ʾhh.mm

//��ʱ��delay����
void delay()
{
  int i = 0;
  for(i=0;i<0xfff;i++);
}

//�������ж�����
void SensorInit()
{
  SetOpFalse(P2DIR,0);
  SetOpTrue(P2IE,0);
  SetOpFalse(P2IFG,0);
  SetOpTrue(P2IES,0);
  SetOpTrue(P2OUT,0);
  SetOpTrue(P2REN,0);
}

//��ʼ��
void MainInit()
{
  DigiTubeInit();
  ButtonInit();
  SensorInit();
  TimerInit();
  EE_Init();
  _EINT();
}

int main( void )
{
  //�رտ��Ź�
  WDTCTL = WDTPW + WDTHOLD;
  MainInit();
  ReadData(); //����ʱ��ȡ�洢���е�����
  ContentFresh();
  while(1)
  {
      TAFlagProcess();
      SensorFlagProcess();
      DigiTubeProcess();
      ButtonHoldProcess();
      SleepFlagProcess(); //���ڲ������жϹ����н���͹���ģʽ,����GIE���ر��޷��˳�,��˽���͹���ģʽ�Ĺ��̷ŵ���ѭ����
  }
  //return 0;
}



