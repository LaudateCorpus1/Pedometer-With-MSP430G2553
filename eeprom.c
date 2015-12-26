#include "eeprom.h"
#include "timing.h"

const unsigned char s_byte = sizeof(t_byte);

//��ʼ��EEPROM��������
void EE_Init()
{
  PeSEL &= ~(eeSCK + eeSI + eeCS + eeSO);
  PeSEL2 &= ~(eeSCK + eeSI + eeCS + eeSO);
  PeDIR |= eeSCK + eeSI + eeCS;
  PeDIR &= ~eeSO;
}
//ˢʱ���ź�һ��
void send_SCK()
{
  PeOUT &= ~eeSCK;      // SCK=0
  PeOUT |= eeSCK;       // SCK=1
  PeOUT &= ~eeSCK;      // SCK=0

}
//д�ֽ�
void send_SI_byte(unsigned int a)
{
  t_byte i,b;
  for(i=0;i<8;++i)
  {
    b=a<<i;
    switch(b&0x80){
    case 0x80: PeOUT=PeOUT|eeSI; break; // a�е�(8-i)λΪ1����SI=1.
    case 0: PeOUT=PeOUT&(~eeSI); break;   // a�е�(8-i)λΪ0����SI=0.
    default:;
    }
    send_SCK();                           // ͨ��SCK���һ��clock
  }
}
//���ֽ�
t_byte read_SO_byte()
{
  t_byte i,a=0;
  for(i=0;i<8;++i)
  {
    a=a<<1;                              // ��a����һλ
    if((PeIN&eeSO)==0)       a=a&254;   // ��aĩλΪ0
    else a=a|1;                         // ��aĩλΪ1
    send_SCK();                          // ͨ��SCK���һ��clock
  }
  return a;
}
//��EEPROM״̬
t_byte ReadStatus()
{
  t_byte a;
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_RDSR);                  // ���Ͷ�״̬����
  a=read_SO_byte();                      // ��ȡ״ֵ̬
  PeOUT=PeOUT|eeCS;                      // CS=1 
  return a;
}
//�ȴ�ֱ����д��������
void CheckWIP()
{
  t_byte k;
  while(1){
  k=ReadStatus();;
  if((k&WIP)==0) break;                    // ��WIPλΪ0����æʱ����������ѭ��
  }
}
//����д����
void WriteEnable()
{
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_WREN);                  // д��д��������
  PeOUT=PeOUT|eeCS;                      // CS=1
}
//��ֹд����
void WriteDisable()
{
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_WRDI);                  // д��д��ֹ����
  PeOUT=PeOUT|eeCS;                      // CS=1  
}
//��ȡָ����ַ����
t_byte EE_ReadByte(t_byte address)
{
  t_byte a;
  CheckWIP();                             // ��ѯ״̬
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_READ);                  // ���Ͷ�����
  send_SI_byte(address);                 // ���Ͷ���Ԫ��ַ
  a=read_SO_byte();                      // ��ȡָ����Ԫ������
  PeOUT=PeOUT|eeCS;                      // CS=1 
  return a;
}
//��ָ����ַд������
void EE_WriteByte(t_byte address,t_byte data)
{
  CheckWIP();                             // ��ѯ״̬
  WriteEnable();                              // ����д����
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_WRITE);                 // ����д����
  send_SI_byte(address);                 // ����д��Ԫ��ַ
  send_SI_byte(data);                    // ����д�뵥Ԫ������
  PeOUT=PeOUT|eeCS;                      // CS=1
}
//��ָ����ַ��ʼд����������
void EE_WriteArray(t_byte address,unsigned char* data,t_byte length)
{
  t_byte i=0;
  for(;i<length;i++)
    EE_WriteByte(address+i, *(data+i*s_byte));
}
//��ָ����ַ��ʼ��ȡ���鲢�洢��data��
void EE_ReadArray(t_byte address,unsigned char* data,t_byte length)
{
  t_byte i=0;
  for(;i<length;i++)
    *(data+i*s_byte) = EE_ReadByte(address+i);
}
//д��״̬
void WriteStatus(t_byte data)
{
  CheckWIP();                             // ��ѯ״̬
  WriteEnable();                              // ����д����
  PeOUT=PeOUT&(~eeCS);                   // CS=0
  send_SI_byte(c_WRSR);                  // д״̬�Ĵ�������
  send_SI_byte(data);                    // ����Ҫд�������
  PeOUT=PeOUT|eeCS;                      // CS=1
}

//��������ʱ�䡢�Ʋ���ֵ�����ֵ��У���ַ�
void SaveData()
{
  EE_WriteByte(0x80, STdsec);
  EE_WriteByte(0x90, STsec);
  EE_WriteByte(0x20, sec);
  EE_WriteByte(0x21, min);
  EE_WriteByte(0x22, hour);
  EE_WriteByte(0x30, (steps&0xFF000000)>> 24);
  EE_WriteByte(0x31, (steps&0x00FF0000)>> 16);
  EE_WriteByte(0x32, (steps&0x0000FF00)>> 8);
  EE_WriteByte(0x33, (steps&0x000000FF));
  EE_WriteByte(0x10, MTimeSuspFLG);
  EE_WriteByte(0x11, STimeSuspFLG);
  EE_WriteArray(0xA0, CheckContent,CheckLength);
}

//��ȡ��������,У��ɹ�����0,У�������1
t_byte ReadData()
{
  t_byte checkd[CheckLength];
  t_byte checkt[CheckLength] = CheckContent;
  EE_ReadArray(0xA0, checkd, CheckLength);
  t_byte j=0;
  for(;j<CheckLength;j++)
    if(checkd[j]-checkt[j])
      return 1;
  STdsec = EE_ReadByte(0x80);
  STsec = EE_ReadByte(0x90);
  sec = EE_ReadByte(0x20);
  min = EE_ReadByte(0x21);
  hour = EE_ReadByte(0x22);
  steps = (unsigned long)EE_ReadByte(0x30) << 24;
  steps += (unsigned long)EE_ReadByte(0x31) << 16;
  steps += (unsigned long)EE_ReadByte(0x32) << 8;
  steps += (unsigned long)EE_ReadByte(0x33);
  MTimeSuspFLG = EE_ReadByte(0x10); 
  STimeSuspFLG = EE_ReadByte(0x11);
  return 0;
}