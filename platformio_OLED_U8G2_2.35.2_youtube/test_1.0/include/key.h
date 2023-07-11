#ifndef __bsp_key_H__
#define __bsp_key_H__
#include "arduino.h"
#include "multi_button.h"

#define key0_Pin        2
#define KEY0_DOWN       0
extern int key_to_u8g2;


void MX_GPIO_key_Init(void);
bool HAL_key0_ReadPin();

#endif
