#include "reg51.h"
#include "lcd.h"
#define Key_Net  P1
sbit key_In1 = P2^0;
sbit key_In2 = P2^1;
sbit key_In3 = P2^2;
//uchar code num[]="0123456789ABCDEF" ;
//uchar code number[]={7,8,9,(x),4,5,6,(-),1,2,3,(+),0,off,=,/};//保存显示的数据
//uchar code number[]={1,2,3,0x2b-0x30(+), 4,5,6,0x2d-0x30(-), 7,8,9,0x2a-0x30(*), 0,0x01-0x30(clr),0x3d-0x30(=),0x2b-0x30(/)};//保存显示的数据
uchar code number[]={
1,2,3,0x2b-0x30, 
4,5,6,0x2d-0x30,
7,8,9,0x2a-0x30,
0,0x01-0x30,0x3d-0x30,0x2f-0x30 };
uchar value;							  
uchar fuhao,TABE;	 //fuhao 储存+，-，*，/，clr，=	；TABE 标识   1是符  0是数
float Table_A,Table_B;     //Table_A,Table_B 储存输入的两个数字
long SUM,YU;
//=======================================================================延时函数===========================================	
void delay10ms()   //误差 0us
{
    unsigned char a,b,c;
    for(c=5;c>0;c--)
        for(b=4;b>0;b--)
            for(a=248;a>0;a--);
}
//=========================================================================初始化=============================================
void Init()
{ 
	fuhao=0;
	TABE=0;
	Table_A=0;
	Table_B=0;
}
//=========================================================================扫描函数===========================================
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
		    if((0<=value)&&(value<=2)||(4<=value)&&(value<=6)||(8<=value)&&(value<=10)||value==12)	//键盘按下0-9
			{
				TABE=0;
			    //SHUZI=0;
				if(fuhao==0)
				{
					Table_A=Table_A*10+number[value];	 //按下一位储存在个位，再按下一位原来的数*10加上刚按下的值

				}
				if((fuhao>0))
				{
					Table_B=Table_B*10+number[value];	 //当有符号位按下后面的值储存在Table_B中 

				}
					Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==3)  //  键盘按下+		也可以写成SWITCH语句
			{
			  	TABE=1;
				fuhao=1;  // +号标志
				Lcd1602_Write_dat(0x30+number[value]);
			}

			if(value==7)  //  键盘按下-
			{
			  	TABE=1;
				fuhao=2;  // -号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==11)  //  键盘按下*
			{
			  	TABE=1;
				fuhao=3;  // -号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==15)  //  键盘按下*
			{
			  	TABE=1;
				fuhao=4;  // /号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==13)  //  键盘按下*
			{
			  	TABE=1;
				fuhao=5;  // RST标志
				Lcd1602_Write_com(0x30+number[value]);
				fuhao=0;
				TABE=0;
				Table_A=0;
				Table_B=0;			
			}

			if(value==14) //  键盘按下=后直接显示
			{
//--------------------------------------------------------------------加法------------------------------------------------
				if(fuhao==1)	//加法
				{	
					SUM=Table_A+Table_B;
					Lcd1602_Write_com(0Xcf); //从第二行最右边开始显示
					Lcd1602_Write_com(0X04); //指针左移
					while(SUM!=0)			 //一位一位显示
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
								SUM=SUM/10;							//取前面的结果数据	
							}
					Lcd1602_Write_dat(0x3d);						//写等于号
				}
//-----------------------------------------------------------------------减法-----------------------------------------------
				if(fuhao==2)	//减法
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
						while(SUM!=0)	 		//一位一位显示
								{
									Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
									SUM=SUM/10;							//取前面的结果数据	
								}
						if(Table_A < Table_B)
						{
							Lcd1602_Write_dat(0x2d);
						}
						Lcd1602_Write_dat(0x3d);						//写等于号
				}
//-----------------------------------------------------------------------------乘法-------------------------------------------
				if(fuhao==3)	//乘法
				{	
					SUM=(Table_A)*(Table_B);
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					while(SUM!=0)	 		//一位一位显示
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
								SUM=SUM/10;							//取前面的结果数据	
							}
					Lcd1602_Write_dat(0x3d);						//写等于号
				}
//----------------------------------------------------------------------------除法--------------------------------------------
				if(fuhao==4)	//除法
				{
					uchar i;
					SUM=((Table_A/Table_B)*10000);		  
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					if((Table_A < Table_B)&&SUM<100)	  //当两个数差100倍时
					{
					while(SUM!=0)	 		//一位一位显示
							{
								i++;
								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
								SUM=SUM/10;
								if(i==2)	
								{
								Lcd1602_Write_dat(0x30);
								Lcd1602_Write_dat(0x2e);
								i=0;
								}				
							}
					}
					while(SUM!=0)	 		//两个数差10倍时SUM为3位数
							{
								i++;
								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
								SUM=SUM/10;
								
								if(i==3)							//改变num的倍数和i的值可以改变保留小数的大小
								{
								Lcd1602_Write_dat(0x2e);
								i=0;
								}
							}	
					if(Table_A < Table_B)
					{
					Lcd1602_Write_dat(0x30);
					}
					Lcd1602_Write_dat(0x3d);						//写等于号
				
				}
			}	 

	}	
}
//========================================================主函数===================================================================
void main()
{
// 	uchar i;
	Lcd_Init();		//初始化LCD
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
	
