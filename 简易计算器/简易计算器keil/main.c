#include "reg51.h"
#include "lcd.h"
#define Key_Net  P1
sbit key_In1 = P2^0;
sbit key_In2 = P2^1;
sbit key_In3 = P2^2;
//uchar code num[]="0123456789ABCDEF" ;
//uchar code number[]={7,8,9,(x),4,5,6,(-),1,2,3,(+),0,off,=,/};//������ʾ������
//uchar code number[]={1,2,3,0x2b-0x30(+), 4,5,6,0x2d-0x30(-), 7,8,9,0x2a-0x30(*), 0,0x01-0x30(clr),0x3d-0x30(=),0x2b-0x30(/)};//������ʾ������
uchar code number[]={
1,2,3,0x2b-0x30, 
4,5,6,0x2d-0x30,
7,8,9,0x2a-0x30,
0,0x01-0x30,0x3d-0x30,0x2f-0x30 };
uchar value;							  
uchar fuhao,TABE;	 //fuhao ����+��-��*��/��clr��=	��TABE ��ʶ   1�Ƿ�  0����
float Table_A,Table_B;     //Table_A,Table_B �����������������
long SUM,YU;
//=======================================================================��ʱ����===========================================	
void delay10ms()   //��� 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}
//=========================================================================��ʼ��=============================================
void Init()
{ 
	fuhao=0;
	TABE=0;
	Table_A=0;
	Table_B=0;
}
//=========================================================================ɨ�躯��===========================================
void Keydown()
{
	char a,i;
	i=0;
	Key_Net=0x0f;
	if(Key_Net!=0x0f)
	{
		delay10ms();
		if(Key_Net!= 0x0f)
		{
			Key_Net=0x0f;
			switch(Key_Net)
			{
				case(0x07) : value = 0 ;break; 
				case(0x0b) : value = 1 ;break;
				case(0x0d) : value = 2 ;break;
				case(0x0e) : value = 3 ;break;
			}	
		
			Key_Net = 0xf0;
			delay10ms();
			switch(Key_Net)
			{
				case(0x70) : value = value+0 ;break;
				case(0xb0) : value = value+4 ;break;
				case(0xd0) : value = value+8 ;break;
				case(0xe0) : value = value+12 ;break;	
			}
			while((a<100)&&(Key_Net!=0xf0))
			{
				delay10ms();
				a++;
			}
				a=0;
		}					
		    if((0<=value)&&(value<=2)||(4<=value)&&(value<=6)||(8<=value)&&(value<=10)||value==12)	//���̰���0-9
			{
				TABE=0;
			    //SHUZI=0;
				if(fuhao==0)
				{
					Table_A=Table_A*10+number[value];	 //����һλ�����ڸ�λ���ٰ���һλԭ������*10���ϸհ��µ�ֵ

				}
				if((fuhao>0))
				{
					Table_B=Table_B*10+number[value];	 //���з���λ���º����ֵ������Table_B�� 

				}
					Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==3)  //  ���̰���+		Ҳ����д��SWITCH���
			{
			  	TABE=1;
				fuhao=1;  // +�ű�־
				Lcd1602_Write_dat(0x30+number[value]);
			}

			if(value==7)  //  ���̰���-
			{
			  	TABE=1;
				fuhao=2;  // -�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==11)  //  ���̰���*
			{
			  	TABE=1;
				fuhao=3;  // -�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==15)  //  ���̰���*
			{
			  	TABE=1;
				fuhao=4;  // /�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==13)  //  ���̰���*
			{
			  	TABE=1;
				fuhao=5;  // RST��־
				Lcd1602_Write_com(0x30+number[value]);
				fuhao=0;
				TABE=0;
				Table_A=0;
				Table_B=0;			
			}

			if(value==14) //  ���̰���=��ֱ����ʾ
			{
//--------------------------------------------------------------------�ӷ�------------------------------------------------
				if(fuhao==1)	//�ӷ�
				{	
					SUM=Table_A+Table_B;
					Lcd1602_Write_com(0Xcf); //�ӵڶ������ұ߿�ʼ��ʾ
					Lcd1602_Write_com(0X04); //ָ������
					while(SUM!=0)			 //һλһλ��ʾ
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
								SUM=SUM/10;							//ȡǰ��Ľ������	
							}
					Lcd1602_Write_dat(0x3d);						//д���ں�
				}
//-----------------------------------------------------------------------����-----------------------------------------------
				if(fuhao==2)	//����
				{	
					if(Table_A>Table_B)
					{
					 	SUM=Table_A - Table_B;
					}
					else
					{
						SUM=Table_B - Table_A;
					}
						Lcd1602_Write_com(0Xcf);
						Lcd1602_Write_com(0X04);
						while(SUM!=0)	 		//һλһλ��ʾ
								{
									Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
									SUM=SUM/10;							//ȡǰ��Ľ������	
								}
						if(Table_A < Table_B)
						{
							Lcd1602_Write_dat(0x2d);
						}
						Lcd1602_Write_dat(0x3d);						//д���ں�
				}
//-----------------------------------------------------------------------------�˷�-------------------------------------------
				if(fuhao==3)	//�˷�
				{	
					SUM=(Table_A)*(Table_B);
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					while(SUM!=0)	 		//һλһλ��ʾ
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
								SUM=SUM/10;							//ȡǰ��Ľ������	
							}
					Lcd1602_Write_dat(0x3d);						//д���ں�
				}
//----------------------------------------------------------------------------����--------------------------------------------
				if(fuhao==4)	//����
				{
					uchar i;
					SUM=((Table_A/Table_B)*10000);		  
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					if((Table_A < Table_B)&&SUM<100)	  //����������100��ʱ
					{
					while(SUM!=0)	 		//һλһλ��ʾ
							{
								i++;
								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
								SUM=SUM/10;
								if(i==2)	
								{
								Lcd1602_Write_dat(0x30);
								Lcd1602_Write_dat(0x2e);
								i=0;
								}				
							}
					}
					while(SUM!=0)	 		//��������10��ʱSUMΪ3λ��
							{
								i++;
								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
								SUM=SUM/10;
								
								if(i==3)							//�ı�num�ı�����i��ֵ���Ըı䱣��С���Ĵ�С
								{
								Lcd1602_Write_dat(0x2e);
								i=0;
								}
							}	
					if(Table_A < Table_B)
					{
					Lcd1602_Write_dat(0x30);
					}
					Lcd1602_Write_dat(0x3d);						//д���ں�
				
				}
			}	 

	}	
}
//========================================================������===================================================================
void main()
{
// 	uchar i;
	Lcd_Init();		//��ʼ��LCD
//	for(i=0;i<16;i++)
//	{
//		Lcd1602_Write_dat(display[i]);
//	}
    while(1)
	{
		Keydown();
//		Lcd1602_Write_com(0X80);
//		Lcd1602_Write_dat(0x30+number[value]);
	}
}
	
