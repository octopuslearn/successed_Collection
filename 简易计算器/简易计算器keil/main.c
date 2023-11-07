#include "reg51.h"
#include "lcd.h"

#include "math.h"
#define PI 3.14159


#define  Key_Net  	P1	////列
#define Key_P2 		P2
//uchar code num[]="0123456789ABCDEF" ;
//uchar code number[]={7,8,9,(x),4,5,6,(-),1,2,3,(+),0,off,=,/};//保存显示的数据
//uchar code number[]={1,2,3,0x2b-0x30(+), 4,5,6,0x2d-0x30(-), 7,8,9,0x2a-0x30(*), 0,0x01-0x30(clr),0x3d-0x30(=),0x2b-0x30(/)};//保存显示的数据
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
	Key_Net=0x0ff;	////列初始值		Key_Net - P1
	Key_P2 = 0x00;
	if(Key_Net!=0xff)	////按键按下
	{
		delay10ms();
		if(Key_Net!= 0xff)	////按键消抖
		{
			Key_P2 = 0x00;
			switch(Key_Net)
			{
				case(0x7f) : value = 7 ;break;  //0111 1111	第一列
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
				case(0xfe) : value = value+0 ;break;	//1111 1110	//行+列 0+0
				case(0xfd) : value = value+8 ;break;	//1111 1101	
				case(0xfb) : value = value+16 ;break;	//1111 1011
			}
			while((a<100)&&(Key_P2!=0xff))	//松手检测
			{
				delay10ms();
				a++;
			}
				a=0;
		}					
		

		  if((0<=value)&&(value<=2)||(8<=value)&&(value<=10)||(16<=value)&&(value<=18)||value==3)	//键盘按下0-9
			{
				TABE=0;	////TABE = 0;表示是数字
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
			
			
			
			
			
			if(value==4)  //  键盘按下+		也可以写成SWITCH语句
			{
					TABE=1;	////TABE = 1;表示符号
				fuhao=1;  // +号标志
				Lcd1602_Write_dat(0x30+number[value]);
			}

			if(value==5)  //  键盘按下-
			{
			  	TABE=1;
				fuhao=2;  // -号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==6)  //  键盘按下*
			{
			  	TABE=1;
				fuhao=3;  // -号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==7)  //  键盘按下/
			{
			  	TABE=1;
				fuhao=4;  // /号标志
				Lcd1602_Write_dat(0x30+number[value]);			
			}

			if(value==15)  //  键盘按e
			{
			  	TABE=1;
				fuhao=5;  // e标志
				Lcd1602_Write_com(0x30+number[value]);
				fuhao=0;
				TABE=0;
				Table_A=0;
				Table_B=0;			
			}
			
			
			
			
//################################			
			if(value==12)  //  键盘按sin
			{
					TABE=1;
				fuhao=6;  // sin标志
				Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==13)  //  键盘按cos
			{
					TABE=1;
				fuhao=7;  // cos标志
				Lcd1602_Write_dat(0x30+number[value]);
			}
			
			if(value==11)  //  键盘按^---阶乘
			{
					TABE=1;
				fuhao=8;  // ^阶乘标志
				Lcd1602_Write_dat(0x30+number[value]);
			}					
//################################	

			
			
			
			
			
			
			if(value==14) //  键盘按下=后直接显示
			{
//--------------------------------------------------------------------加法------------------------------------------------
				if(fuhao==1)	//加法	////fuhao=1;  // +号标志
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
				if(fuhao==2)	//减法	////fuhao=2;  // -号标志
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
				if(fuhao==3)	//乘法	////fuhao=3;  // -号标志
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
				if(fuhao==4)	//除法	////fuhao=4;  // /号标志
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
				
//#####################################################				
//-----------------------------------------------------------------------------sin-------------------------------------------
//				if(fuhao==6)	//sin	////fuhao=6;  // sin标志
//				{	
//					SUM=(Table_A)*(Table_B);
//					Lcd1602_Write_com(0Xcf);
//					Lcd1602_Write_com(0X04);
//					while(SUM!=0)	 		//一位一位显示
//							{
//								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
//								SUM=SUM/10;							//取前面的结果数据	
//							}
//					Lcd1602_Write_dat(0x3d);						//写等于号
//				}


//				if(fuhao==7)	//cos	////fuhao=7;  // cos标志
//				{	
//					SUM=(Table_A)*(Table_B);
//					Lcd1602_Write_com(0Xcf);
//					Lcd1602_Write_com(0X04);
//					while(SUM!=0)	 		//一位一位显示
//							{
//								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
//								SUM=SUM/10;							//取前面的结果数据	
//							}
//					Lcd1602_Write_dat(0x3d);						//写等于号
//				}				


				if(fuhao==8)	//^阶乘	////fuhao=8;  // ^阶乘标志
				{	
//					SUM=(Table_A)*(Table_B);
					SUM=pow(Table_A,Table_B);
					Lcd1602_Write_com(0Xcf);
					Lcd1602_Write_com(0X04);
					while(SUM!=0)	 		//一位一位显示
							{
								Lcd1602_Write_dat(0x30+SUM%10);		//显示结果的最后一位在0x4f的位置
								SUM=SUM/10;							//取前面的结果数据	
							}
					Lcd1602_Write_dat(0x3d);						//写等于号
				}								
				
//#####################################################						
				
				
				
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
	
