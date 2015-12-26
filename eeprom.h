#ifndef EEPROM
#define EEPROM

#include "universal.h"

// �÷��Ŷ���25LC020A ������
#define  c_READ         3       //������           0000 0011B
#define  c_WRITE        2       //д����           0000 0010B
#define  c_WRDI         4       //д��ֹ           0000 0100B
#define  c_WREN         6       //д����           0000 0110B
#define  c_RDSR         5       //��״̬�Ĵ���     0000 0101B
#define  c_WRSR         1       //д״̬�Ĵ���     0000 0001B

// �÷��Ŷ���״̬�Ĵ����е�״̬λ
#define	 BP1    BIT3
#define	 BP0    BIT2
#define	 WEL    BIT1
#define	 WIP    BIT0		   
 
// �÷��Ŷ���ʹ�õĶ˿ں͹ܽţ��ɾݾ�������߽����޸�
#define  PeSEL   P2SEL
#define  PeSEL2  P2SEL2
#define  PeDIR   P2DIR
#define  PeOUT   P2OUT
#define  PeIN  	 P2IN
#define  eeSCK   BIT3	//SCK -> P2.3	
#define  eeSI  	 BIT4    //SI -> P2.4
#define  eeCS  	 BIT2    //CS -> P2.2
#define  eeSO 	 BIT1    //SO -> P2.1

//У�����ݵ�����
#define CheckContent    "DtCheck"
#define CheckLength     8

void EE_Init();
t_byte ReadData();
void SaveData();

#endif