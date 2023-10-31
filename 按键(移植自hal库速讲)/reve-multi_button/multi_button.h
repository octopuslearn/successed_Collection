/*********************以下为原版git修改而来，只不过去掉了**************************/
/***原版git void button_init(struct Button* handle, uint8_t(*pin_level)(uint8_t), uint8_t active_level, uint8_t button_id)
修改版      void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
去掉了一个参数********************************************************************/

/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS_START,
	LONG_PRESS_HOLD,
	number_of_event,
	NONE_PRESS
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  button_id;
	uint8_t  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif






/***********************以下为教程原版**************************/
///*
// * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
// * All rights reserved
// */

//#ifndef _MULTI_BUTTON_H_
//#define _MULTI_BUTTON_H_

//#include "stdint.h"
//#include "string.h"

////According to your need to modify the constants.
//#define TICKS_INTERVAL    5	//ms
//#define DEBOUNCE_TICKS    3	//MAX 8
//#define SHORT_TICKS       (300 /TICKS_INTERVAL)
//#define LONG_TICKS        (1000 /TICKS_INTERVAL)


//typedef void (*BtnCallback)(void*);

//typedef enum {
//	PRESS_DOWN = 0,
//	PRESS_UP,
//	PRESS_REPEAT,
//	SINGLE_CLICK,
//	DOUBLE_CLICK,
//	LONG_PRESS_START,
//	LONG_PRESS_HOLD,
//	number_of_event,
//	NONE_PRESS
//}PressEvent;

//typedef struct Button {
//	uint16_t ticks;
//	uint8_t  repeat : 4;
//	uint8_t  event : 4;
//	uint8_t  state : 3;
//	uint8_t  debounce_cnt : 3;
//	uint8_t  active_level : 1;
//	uint8_t  button_level : 1;
//	uint8_t  (*hal_button_Level)(void);
//	BtnCallback  cb[number_of_event];
//	struct Button* next;
//}Button;

//#ifdef __cplusplus
//extern "C" {
//#endif

//void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
//void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
//PressEvent get_button_event(struct Button* handle);
//int  button_start(struct Button* handle);
//void button_stop(struct Button* handle);
//void button_ticks(void);

//#ifdef __cplusplus
//}
//#endif

//#endif
/***********************以上为教程原版**************************/

