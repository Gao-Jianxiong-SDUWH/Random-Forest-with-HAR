
#include	"delay.h"
//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int  i;
		 do
		 {
	      i = MAIN_Fosc / 13000;
				while(--i)	;   //14T per loop
     }
		 while(--ms);
}

void Delay5us(void)
{
	uint16  j,i;   
	for(j=0;j<2;j++)   
	{    
		for(i=0;i<7;i++);   
	}  
}

