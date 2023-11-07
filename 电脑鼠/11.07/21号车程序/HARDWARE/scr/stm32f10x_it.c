/**
******************************************************************************
* @file    TIM/7PWM_Output/stm32f10x_it.c
* @author  MCD Application Team
* @version V3.5.0
* @date    08-April-2011
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and peripherals
*          interrupt service routine.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "MicroMouse_Device_Init.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
* @{
*/

/** @addtogroup TIM_7PWM_Output
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {}
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {}
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {}
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {}
}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{}

/**
* @brief  This function handles PendSV_Handler exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/

char      SendData[4]= {0};
extern  float     GYRO_ADCVALUE;
extern  float     Gyro_Angle;
extern  u16       ADC1ConvertedValue[6];
extern  u16       ADCValue[4];
extern  float     IRDistance[4];
float     IRDistanceTemp[4];
u16       ADCBasicValue[4];
extern  long      Time ;
extern  MOTER     GmRight;
extern  MOTER     GmLeft;
long      SpeedCorrect;
long      i=0;
long      Power_Voltage = 0;
extern  char      WallInf;
extern  MAZECOOR  GmcMouseCoor;
extern  char      GucMapBlock[MAZETYPE][MAZETYPE];
extern  long      SpeedChangeTime;
extern  char      Test;
extern  char      GucMapStep[MAZETYPE][MAZETYPE];
extern  SPURTSTEP SpurtStep[MAZETYPE*MAZETYPE];
extern  char      GucMouseDir;
extern  char      GucMouseTask;
extern  long      LeanTime;
void SysTick_Handler(void)
{
    u32 start = SysTick->VAL;
    Time ++;
    SpeedChangeTime ++;
    LeanTime ++;
    grc_irsensor_rf_adc = ADCValue[0];
    grc_irsensor_r30_adc = ADCValue[1];
    grc_irsensor_L30_adc = ADCValue[2];
    grc_irsensor_lf_adc = ADCValue[3];
    grc_gyro_adc = (u16)GYRO_ADCVALUE;
    IRCD( ADCValue,IRDistanceTemp);
    GCA(GYRO_ADCVALUE, &Gyro_Angle);
    if(GucMouseTask == INITPARAM)
    {
        DEBUG_LED_ON
        GC(GYRO_ADCVALUE,&GYRO_OFFSET,GYRO_MUL_PARAM);
        IRSPC(IRDistanceTemp,&GucMouseTask,MAZESEARCH);
#ifdef DEBUG
        if(GucMouseTask == MAZESEARCH)
            DEBUG_LED_OFF
#endif
        }
    IRDC();
    IRDistance[0] = IRDistanceTemp[0];
    IRDistance[1] = IRDistanceTemp[1];
    IRDistance[2] = IRDistanceTemp[2];
    IRDistance[3] = IRDistanceTemp[3];
    SpeedCorrect = PIDIRSC();
    SCL(SpeedCorrect);
    SCR(SpeedCorrect);
    grc_rf_dist = IRDistance[0];
    grc_r30_dist = ((IRDistance[1]));
    grc_l30_dist = (IRDistance[2]);
    grc_lf_dist = (IRDistance[3]);
    grc_gyro_angle = Gyro_Angle;
    PushOneDataFrame();
    TIM_Cmd(TIM6, ENABLE);
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
* @brief  This function handles PPP interrupt request.
* @param  None
* @retval None
*/
u16     ADCValueMin=0,ADCValueMax=0;
u16     ADCGryoValue = 0;
void TIM6_IRQHandler(void)
{
    static u8 State = 0;
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        switch(State)
        {
        case 0:
            ADCBasicValue[0] = ADC1ConvertedValue[0];
            ADCBasicValue[3] = ADC1ConvertedValue[3];
            GYRO_ADCVALUE = ADC1ConvertedValue[4];
            Power_Voltage = ADC1ConvertedValue[5];
            GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);
            ADCValueMin = (u16)GYRO_ADCVALUE;
            ADCValueMax = (u16)GYRO_ADCVALUE;
            break;
        case 1:
            ADCValue[0] = ADC1ConvertedValue[0] - ADCBasicValue[0];
            ADCValue[3] = ADC1ConvertedValue[3] - ADCBasicValue[3];
            ADCGryoValue = ADC1ConvertedValue[4];
            GYRO_ADCVALUE += ADCGryoValue;
            Power_Voltage += ADC1ConvertedValue[5];
            GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
            if(ADCValueMin > ADCGryoValue)
                ADCValueMin = ADCGryoValue;
            if(ADCValueMax < ADCGryoValue)
                ADCValueMax = ADCGryoValue;
            break;
        case 2:
            ADCBasicValue[1]= ADC1ConvertedValue[1];
            ADCBasicValue[2]= ADC1ConvertedValue[2];
            ADCGryoValue = ADC1ConvertedValue[4];
            GYRO_ADCVALUE += ADCGryoValue;
            Power_Voltage += ADC1ConvertedValue[5];
            GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);
            if(ADCValueMin > ADCGryoValue)
                ADCValueMin = ADCGryoValue;
            if(ADCValueMax < ADCGryoValue)
                ADCValueMax = ADCGryoValue;
            break;
        case 3:
            ADCValue[1] = ADC1ConvertedValue[1] - ADCBasicValue[1];
            ADCValue[2] = ADC1ConvertedValue[2] - ADCBasicValue[2];
            ADCGryoValue = ADC1ConvertedValue[4];
            GYRO_ADCVALUE += ADCGryoValue;
            Power_Voltage += ADC1ConvertedValue[5];
            GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);
            TIM_Cmd(TIM6, DISABLE);
            if(ADCValueMin > ADCGryoValue)
                ADCValueMin = ADCGryoValue;
            if(ADCValueMax < ADCGryoValue)
                ADCValueMax = ADCGryoValue;
            GYRO_ADCVALUE = (GYRO_ADCVALUE - ADCValueMin - ADCValueMax)*2;
            break;
        }
        State = (State +1)%4;
    }
}
void EXTI15_10_IRQHandler()
{
    if(EXTI_GetITStatus(w24L01_EXTI_LINE) != RESET)
    {
        EXTI_ClearITPendingBit(w24L01_EXTI_LINE);
    }
}
/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
