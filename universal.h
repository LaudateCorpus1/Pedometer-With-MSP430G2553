#ifndef UNIVERSAL
#define UNIVERSAL

#include "io430.h"
#include "in430.h"

typedef unsigned char t_byte; //���޷���char��ʾ��t_byte�����ڳ����д
extern unsigned long steps;
extern t_byte conts[];
extern t_byte mode;
extern t_byte StepOffset;
extern t_byte TimeOffset;

//�����������Ŷ�д��Ԥ����ָ��
#define SetPort1Out(data,pin) P1OUT = (data==1) ? P1OUT | (1<<pin) : P1OUT &~(1<<pin)
#define SetPort2Out(data,pin) P2OUT = (data==1) ? P2OUT | (1<<pin) : P2OUT &~(1<<pin)
#define SetOpTrue(reg,pin) reg |= (1<<pin)
#define SetOpFalse(reg,pin) reg &= ~(1<<pin)
#define SetOp(reg,pin,val) reg = (val==1) ? reg | (1<<pin) : reg & ~(1<<pin)  //δ�ɹ�ʵ��
#define ReadOp(reg,pin) (reg>>pin)&1

#define BIT67 (BIT6 + BIT7)

#define IsSleep //�Ƿ��Զ�����͹���״̬,ע�͵��򲻻����͹���ģʽ
#define IsAutoSave //�Ƿ��Զ����棬ע�͵��򲻻ᱣ��

#endif