#ifndef MIR_DEVICE_INIT_H_
#define MIR_DEVICE_INIT_H_
#include "stm32f10x.h"
#define KEY_START_PERIPH                RCC_APB2Periph_GPIOA
#define KEY_START_GPIO                  GPIOA
#define KEY_START_PIN                   GPIO_Pin_15
#define KEY_SPURT_PERIPH                RCC_APB2Periph_GPIOA
#define KEY_SPURT_GPIO                  GPIOA
#define KEY_SPURT_PIN                   GPIO_Pin_14
#define LED1_PERIPH                     RCC_APB2Periph_GPIOA
#define LED1_GPIO                       GPIOA
#define LED1_PIN                        GPIO_Pin_15
#define BELL_PERIPH                     RCC_APB2Periph_GPIOC
#define BELL_GPIO                       GPIOC
#define BELL_PIN                        GPIO_Pin_11
#ifdef DEBUG
#define DEBUG_LED_ON                    GPIO_SetBits(LED1_GPIO,LED1_PIN);
#define DEBUG_LED_OFF                   GPIO_ResetBits(LED1_GPIO,LED1_PIN);
#else
#define DEBUG_LED_ON                    
#define DEBUG_LED_OFF                   
#endif
void RCC_Config(void);
void NVIC_Configuration(void);
void Init_Moter_PWM(void);
void Init_Moter_ENC(void);
void Init_KeyBoard(void);
void Check_PowerVoltage(void);
#endif

