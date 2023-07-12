/*成功*/
//必须使用ps类似软件，保存成.bmp格式的，再用PCtoLCD2002-阳码，逆向，逐行式，逆向
//完美解决

#include <U8g2lib.h>
#include <Arduino.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// u8g2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(u8g2_R0,U8X8_PIN_NONE);

const unsigned char bitmap_icon_wushousign [] PROGMEM = 
{
0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x40,0x02,0x20,0x04,0x0C,0x30,0x48,0x12,0x08,0x10,0x80,0x01,0x48,0x12,0x20,0x04,0x24,0x24,0x22,0x44,0x12,0x48,0x00,0x00,/*"C:\Users\zy\Desktop\摊手图标.bmp",0*/
/* (16 X 16 )*/

};


const unsigned char bitmap_item_sel_outline[] PROGMEM = //所选项中框 
{
0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,
0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"C:\Users\zy\Desktop\中框 (1).bmp",0*/
/* (128 X 21 )*/

};


const unsigned char bitmap_scrollbar_background[] PROGMEM = //滚动列表背景
{
0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,0xFE,0xFC,/*"C:\Users\zy\Desktop\滚动列表线.bmp",0*/
/* (2 X 64 )*/

};


int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one  //上一项 - 在菜单屏幕中用于在所选项目之前绘制项目
int item_sel_next; // next item - used in the menu screen to draw next item after the selected one  //下一项 - 在菜单屏幕中用于在所选项目之后绘制下一项
int item_selected = 0;//选择了菜单中的那个项目，即但前按键项目

int current_screen = 0; //屏幕，如截屏，二维码

int demo_mode_delay = 0;//屏幕切换速度，即屏幕延时时间
int demo_mode_state = 0;//屏幕数  //即直接模式下，界面的个数



int demo_mode = 0;//屏幕模式选择



int button_up_clicked = 0;//向上，单击按键，等到再次按下，即只按一次
int button_down_clicked = 0;//向下，单击按键，等到再次按下，即只按一次
int button_select_clicked = 0;//确定


const unsigned char* bitmap_icons[3]={
  bitmap_icon_wushousign,       //握手标志
  bitmap_item_sel_outline,      //所选项中框
  bitmap_scrollbar_background,  //滚动列表背景
};


const int NUM_ITEMS = 4;      //列表中的项目数，即行数
const int MAX_ITEM_LENGTH = 20;//项目名称的最大数组，即menu_items列数

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  // array with item names
  { "hd_value" }, 
  { "t_lr90" }, 
  { "car_O/C" }, 
  { "wring!" }, 
  // { "GPS Speed" }, 
  // { "Big Knob" },   
  // { "Park Sensor" }, 
  // { "Turbo Gauge" }
 };


#define hy1   22     //OUT5
#define hy2   23     //OUT4
#define hy3   24     //OUT3
#define hy4   25     //OUT2
#define hy5   26    //OUT1
#define hy6   27    //OUT6
int hw_in[8];


/*函数声明*/
void read_hd_value_setup();
void read_hd_value();


void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //mode
  pinMode(3, INPUT_PULLUP); //down
  pinMode(4, INPUT_PULLUP); //up
  pinMode(5, INPUT_PULLUP); //enter
  u8g2.begin();//初始化演示器
  u8g2.setColorIndex(1);  // set the color to white//设置颜色为白色
  read_hd_value_setup();
  // put your setup code here, to run once:

}


void loop() {
/*直接进入界面or手动进入界面模式*/
  if(digitalRead(2) == 0) //mode  //自动进入界面模式
  {
    demo_mode = 0;
  }
  else  //按键进入界面模式
  {
    demo_mode = 1;
  }

/*up,down跳转到上一个，下一个项*/ //直接进入模式
  if(demo_mode == 0)
  { 
    demo_mode_delay++;//延时
    if(demo_mode_delay>15)
    {
      demo_mode_delay = 0;//延时清零
      demo_mode_state++;//增加计数器，即一页中项数累加
      if(demo_mode_state >= NUM_ITEMS*3)  //此处为何要*3???
      {
        demo_mode_state = 0;//跳回到第一个屏幕
      }
      if(demo_mode_state%3 == 0)      {current_screen = 0; item_selected = demo_mode_state/3;}//跳回到主菜单
      else if(demo_mode_state%3 == 1) {current_screen = 1; item_selected = demo_mode_state/3;}//直接跳转到hd_value
      else if(demo_mode_state%3 == 2) {current_screen = 2; item_selected = demo_mode_state/3;}//直接跳转到t_lr90  //没有数对三取余数是3的！！！！！！！
    }
  }

  if(demo_mode == 1)
  {
    if((digitalRead(3)==0) && (button_down_clicked==0))//down按下&&？  单击向下按钮 // && (button_down_clicked==0)-我认为是避免误触的方法
    {
      button_down_clicked = 1;
      item_selected++;//当前按键项目++
      /*调试*/
      Serial.print("down按下: "); Serial.println(item_selected);
      if(item_selected>=NUM_ITEMS)  //划到低了
      {
        item_selected = 0;
      }
    }
    else if((digitalRead(4)==0) && (button_up_clicked==0))//up按下&&？  单击向下按钮
    {
      button_up_clicked = 1;
      item_selected--;//当前按键项目++
      /*调试*/
      Serial.print("up按下: "); Serial.println(item_selected);
      if(item_selected < 0)  //划到顶了 //为啥不是<=1???
      {
        item_selected = NUM_ITEMS-1;  //我认为是跳转到最后一项 NUM_ITEMS-1  //引用下标
      }
    }
 
    if((digitalRead(4)==HIGH) && (button_up_clicked==1))   {button_up_clicked = 0;};  //取消点击  //up

    if((digitalRead(3)==HIGH) && (button_down_clicked==1)) {button_down_clicked = 0;};  //down
  }

/*按下enter,在屏幕之间跳转*/
  if((digitalRead(5)==LOW) && (button_select_clicked==0)) //enter
  {
    delay(50);
    if((digitalRead(5)==LOW) && (button_select_clicked==0)) //enter
    {
      button_select_clicked = 1;
      if(current_screen==0) 
      {       
        if(item_selected == 0)
        {
          current_screen = 1;/*调试*/ Serial.println("1级菜单");
        }
        if(item_selected == 1)
        {
          current_screen = 2;/*调试*/ Serial.println("1级菜单");
        }
        if(item_selected == 2)
        {
          current_screen = 3;/*调试*/ Serial.println("1级菜单");
        }
        if(item_selected == 3)
        {
          current_screen = 4;/*调试*/ Serial.println("1级菜单");
        }
      }
      else 
        {current_screen=0;Serial.println("主菜单");} /*就缺了个这！！！*/
    }
  } 

  /*以下，把这里注释了就回不去了*/
    //主菜单-1级菜单 //到达子菜单  /*只要只设定一次，就可以不用按4下才能回到主菜单*/
  // else  {current_screen=0;Serial.println("主菜单");}                   //-主菜单
  /*以上，把这里注释了就回不去了*/

  if((digitalRead(5)==HIGH) && (button_select_clicked==1))
  {
    button_select_clicked = 0;//取消点击
  }
  
/*确定上一项，下一项的值*/
  item_sel_previous = item_selected-1;//上一项=当前项-1
  if(item_sel_previous < 0)          {item_sel_previous = NUM_ITEMS-1;}
  item_sel_next = item_selected+1;
  if(item_sel_next >= NUM_ITEMS) {item_sel_next = 0;}


/*分页模式*/
    u8g2.firstPage();
  do {

/*全缓存模式*/
  u8g2.clearBuffer(); 
  u8g2.setCursor(0,0);//清除内部缓冲区

      if(current_screen == 0) //菜单屏幕
      {
        u8g2.drawXBMP(0, 22, 128, 21, bitmap_item_sel_outline);  //绘制所选项中框

        /*将上一项设定为淡色*/
        u8g2.setFont(u8g_font_7x14); //设置字体
        u8g2.drawStr(25, 15, menu_items[item_sel_previous]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
        u8g2.drawXBMP(4, 2, 16, 16, bitmap_icons[0]);  //绘制握手图标
        /*调试*/ Serial.print("上一项，图标："); Serial.println(item_sel_previous);

        /*将当前项设定为深色*/
        u8g2.setFont(u8g_font_7x14B); //设置字体 //B代表粗体
        u8g2.drawStr(25, 15+20+2, menu_items[item_selected]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
        u8g2.drawXBMP(4, 24, 16, 16, bitmap_icons[0]);  //绘制握手图标
        /*调试*/  Serial.print("当前项，图标："); Serial.println(item_selected);
        /*将下一项设定为浅色*/
        u8g2.setFont(u8g_font_7x14); //设置字体 //B代表粗体
        u8g2.drawStr(25, 15+20+20+2+2, menu_items[item_sel_next]);  //绘制选项中框中的选项文字 //二维数组为啥这样使用？？？  //drawStr绘制字符
        u8g2.drawXBMP(4, 46, 16, 16, bitmap_icons[0]);  //绘制握手图标
        /*调试*/  Serial.print("下一项，图标："); Serial.println(item_sel_next);

        u8g2.drawXBMP(128-2, 0, 2, 64, bitmap_scrollbar_background);  //绘制滚动列表背景
        u8g2.drawBox(125, 64/NUM_ITEMS * item_selected, 3, 64/NUM_ITEMS);  //绘制滚动条 //drawBox是绘制啥的，为啥绘制滚动条的时候要用？？？
      }
      else if(current_screen == 1)  //直接跳转到hd_value
      {
       
          u8g2.drawStr(0,10,"hd:");
          read_hd_value();
          u8g2.setCursor((10*3), 10);   u8g2.print(hw_in[0]);
          u8g2.setCursor((10*4.5), 10); u8g2.print(hw_in[1]);
          u8g2.setCursor((10*6), 10);   u8g2.print(hw_in[2]);
          u8g2.setCursor((10*7.5), 10); u8g2.print(hw_in[3]);
          u8g2.setCursor((10*9), 10);   u8g2.print(hw_in[4]);
          u8g2.setCursor((10*10.5), 10);u8g2.print(hw_in[5]);
          // while(1);//直接进入不了


      }
      else if(current_screen == 2)  //直接跳转到t_lr90
      {
        u8g2.drawStr(25, 25, "sss");
      }
      else if(current_screen == 3)  //直接跳转到car_O/C
      {
        u8g2.drawStr(25, 25, "mmm");
      }
      else if(current_screen == 4)  //直接跳转到car_O/C
      {
        u8g2.drawStr(25, 25, "wring!");
      }

  // u8g2.sendBuffer();                 //绘制缓冲区的内容

  } while ( u8g2.nextPage() );
}


void read_hd_value_setup()
{
  pinMode(hy1, INPUT);//寻迹模块引脚初始化
  pinMode(hy2, INPUT);
  pinMode(hy3, INPUT);
  pinMode(hy4, INPUT);
  pinMode(hy5, INPUT);
  pinMode(hy6, INPUT);
}

void read_hd_value()
{
  hw_in[0] = digitalRead(hy1);  //OUT5
  hw_in[1] = digitalRead(hy2);  //OUT4
  hw_in[2] = digitalRead(hy3);  //OUT3
  hw_in[3] = digitalRead(hy4);  //OUT2
  hw_in[4] = digitalRead(hy5);  //OUT1
  hw_in[5] = digitalRead(hy6);  //OUT6
  /*调试*/
  // Serial.print("hw_in[0]:");Serial.print(hw_in[0]); Serial.print('\t');
  //  Serial.print("hw_in[1]:");Serial.print(hw_in[1]); Serial.print('\t');
  //   Serial.print("hw_in[2]:");Serial.print(hw_in[2]); Serial.print('\t');
  //    Serial.print("hw_in[3]:");Serial.print(hw_in[3]); Serial.print('\t');
  //     Serial.print("hw_in[4]:");Serial.print(hw_in[4]); Serial.print('\t');
  //       Serial.print("hw_in[5]:");Serial.print(hw_in[5]); Serial.println('\t'); 
}