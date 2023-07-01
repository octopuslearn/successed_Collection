#include "car.h"

/*前进*/
void go()
{
	lef1_go();//*左前前*/
	rig1_go();//*右前前*/
	lef2_go();//*左后前*/
	rig2_go();//*右后前*/
}
/*后退*/
void back()
{
	lef1_back();//*左前后*/
	rig1_back();//*右前后*/
	lef2_back();//*左后后*/
	rig2_back();//*右后后*/
}
/*左转*/
void turnleft()
{
	rig1_go();//*右前前*/
	rig2_go();//*右后前*/
	lef1_stop();//*左前停止*/
	lef2_stop();//*左后停止*/
}
/*右转*/
void turnright()
{
	lef1_go();//*左前前*/
	lef2_go();//*左后前*/
	rig1_stop();//*右前停止*/
	rig2_stop();//*右后停止*/
}
/*停止*/
void stop()
{
	lef1_stop();//*左前停止*/
	rig1_stop();//*右前停止*/
	lef2_stop();//*左后停止*/
	rig2_stop();//*右后停止*/
}