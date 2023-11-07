#include "reg51.h"
#include "lcd.h"

#include "math.h"
#define PI 3.14159


#define  Key_Net  	P1	////��
#define Key_P2 		P2
//uchar code num[]="0123456789ABCDEF" ;
//uchar code number[]={7,8,9,(x),4,5,6,(-),1,2,3,(+),0,off,=,/};//������ʾ������
//uchar code number[]={1,2,3,0x2b-0x30(+), 4,5,6,0x2d-0x30(-), 7,8,9,0x2a-0x30(*), 0,0x01-0x30(clr),0x3d-0x30(=),0x2b-0x30(/)};//������ʾ������
//uchar code number[]={
//1,2,3,0x2b-0x30, 
//4,5,6,0x2d-0x30,
//7,8,9,0x2a-0x30,
//0,0x01-0x30,0x3d-0x30,0x2f-0x30 };

//uchar code number[19]={
//	1,2,3,0,	'+','-','*','/',
//	4,5,6,'^','s','c','=','e',		
//	7,8,9,	
//};

uchar code number[19]={
	1,2,3,0,	0x2b-0x30,0x2d-0x30,0x2a-0x30,0x2f-0x30,
	4,5,6,0x5e-0x30,0x73-0x30,0x73-0x30,0x3d-0x30,0x01-0x30,		
	7,8,9,	
};
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
	Key_Net=0x0ff;	////�г�ʼֵ		Key_Net - P1
	Key_P2 = 0x00;
	if(Key_Net!=0xff)	////��������
	{
		delay10ms();
		if(Key_Net!= 0xff)	////��������
		{
			Key_P2 = 0x00;
			switch(Key_Net)
			{
				case(0x7f) : value = 7 ;break;  //0111 1111	��һ��
				case(0xbf) : value = 6 ;break;	//1011 1111
				case(0xdf) : value = 5 ;break;	//1101 1111
				case(0xef) : value = 4 ;break;	//1110 1111
				case(0xf7) : value = 3 ;break;	//1111 0111
				case(0xfb) : value = 2 ;break;	//1111 1011
				case(0xfd) : value = 1 ;break;	//1111 1101
				case(0xfe) : value = 0 ;break;	//1111 1110
			}				
			
			Key_Net = 0x00;
			Key_P2 = 0xff;
			delay10ms();
			switch(Key_P2)
			{
				case(0xfe) : value = value+0 ;break;	//1111 1110	//��+�� 0+0
				case(0xfd) : value = value+8 ;break;	//1111 1101	
				case(0xfb) : value = value+16 ;break;	//1111 1011
			}
			while((a<100)&&(Key_P2!=0xff))	//���ּ��
			{
				delay10ms();
				a++;
			}
				a=0;
		}					
		

		  if((0<=value)&&(value<=2)||(8<=value)&&(value<=10)||(16<=value)&&(value<=18)||value==3)	//���̰���0-9
			{
				TABE=0;	////TABE = 0;��ʾ������
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
			
			
			
			
			
			if(value==4)  //  ���̰���+		Ҳ����д��SWITCH���
			{
					TABE=1;	////TABE = 1;��ʾ����
				fuhao=1;  // +�ű�־
				Lcd1602_Write_dat(0x30+number[value]);
			}

			if(value==5)  //  ���̰���-
			{
			  	TABE=1;
				fuhao=2;  // -�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==6)  //  ���̰���*
			{
			  	TABE=1;
				fuhao=3;  // -�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==7)  //  ���̰���/
			{
			  	TABE=1;
				fuhao=4;  // /�ű�־
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==15)  //  ���̰�e
			{
			  	TABE=1;
				fuhao=5;  // e��־
				Lcd1602_Write_com(0x30+number[value]);
				fuhao=0;
				TABE=0;
				Table_A=0;
				Table_B=0;			
			}
			
			
			
			
//################################			
			if(value==12)  //  ���̰�sin
			{
					TABE=1;
				fuhao=6;  // sin��־
				Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==13)  //  ���̰�cos
			{
					TABE=1;
				fuhao=7;  // cos��־
				Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==11)  //  ���̰�^---�׳�
			{
					TABE=1;
				fuhao=8;  // ^�׳˱�־
				Lcd1602_Write_dat(0x30+number[value]);
			}					
//################################	

			
			
			
			
			
			
			if(value==14) //  ���̰���=��ֱ����ʾ
			{
//--------------------------------------------------------------------�ӷ�------------------------------------------------
				if(fuhao==1)	//�ӷ�	////fuhao=1;  // +�ű�־
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
				if(fuhao==2)	//����	////fuhao=2;  // -�ű�־
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
				if(fuhao==3)	//�˷�	////fuhao=3;  // -�ű�־
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
				if(fuhao==4)	//����	////fuhao=4;  // /�ű�־
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
				
//#####################################################				
//-----------------------------------------------------------------------------sin-------------------------------------------
//				if(fuhao==6)	//sin	////fuhao=6;  // sin��־
//				{	
//					SUM=(Table_A)*(Table_B);
//					Lcd1602_Write_com(0Xcf);
//					Lcd1602_Write_com(0X04);
//					while(SUM!=0)	 		//һλһλ��ʾ
//							{
//								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
//								SUM=SUM/10;							//ȡǰ��Ľ������	
//							}
//					Lcd1602_Write_dat(0x3d);						//д���ں�
//				}


//				if(fuhao==7)	//cos	////fuhao=7;  // cos��־
//				{	
//					SUM=(Table_A)*(Table_B);
//					Lcd1602_Write_com(0Xcf);
//					Lcd1602_Write_com(0X04);
//					while(SUM!=0)	 		//һλһλ��ʾ
//							{
//								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
//								SUM=SUM/10;							//ȡǰ��Ľ������	
//							}
//					Lcd1602_Write_dat(0x3d);						//д���ں�
//				}				


				if(fuhao==8)	//^�׳�	////fuhao=8;  // ^�׳˱�־
				{	
//					SUM=(Table_A)*(Table_B);
					SUM=pow(Table_A,Table_B);
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					while(SUM!=0)	 		//һλһλ��ʾ
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//��ʾ��������һλ��0x4f��λ��
								SUM=SUM/10;							//ȡǰ��Ľ������	
							}
					Lcd1602_Write_dat(0x3d);						//д���ں�
				}								
				
//#####################################################						
				
				
				
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
	
