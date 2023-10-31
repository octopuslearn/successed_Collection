#include "motor.h"

/*左前前*/
void lef1_go()
{
	In1=1;//左前前
	In2=0;
}
/*左前后*/
void lef1_back()
{
	In1=0;//左前后
	In2=1;
}
/*右前前*/
void rig1_go()
{
	In3=1;//右前前
	In4=0;
}
/*右前后*/
void rig1_back()
{
	In3=0;//右前后
	In4=1;
}
/*左后前*/
void lef2_go()
{
	In5=0;//左后前
	In6=1;
}
/*左后后*/
void lef2_back()
{
	In5=1;//左后后
	In6=0;
}
/*右后前*/
void rig2_go()
{
	In7=1;//右后前
	In8=0;
}
/*右后后*/
void rig2_back()
{
	In7=0;//右后后
	In8=1;
}
/*左前停止*/
void lef1_stop()
{
	In1=1;
	In2=1;
}
/*右前停止*/
void rig1_stop()
{
	In3=1;
	In4=1;
}
/*左后停止*/
void lef2_stop()
{
	In5=1;
	In6=1;
}
/*右后停止*/
void rig2_stop()
{
	In7=1;
	In8=1;
}