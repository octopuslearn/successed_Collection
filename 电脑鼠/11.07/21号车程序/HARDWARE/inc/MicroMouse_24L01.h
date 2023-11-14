#ifndef _MIRMOUSE_24L01_H_
#define _MIRMOUSE_24L01_H_
#include "stm32f10x.h"
#define READ_reg      0x00                                            
#define WRITE_reg     0x20                                            
#define RD_RX_PLOAD   0x61                                            
#define WR_TX_PLOAD   0xA0                                            
#define FLUSH_TX      0xE1                                            
#define FLUSH_RX      0xE2                                            
#define REUSE_TX_PL   0xE3                                            
#define NOP           0xFF                                            
#define CONFIG        0x00                                            
#define EN_AA         0x01                                           
#define EN_RXADDR     0x02                                            
#define SETUP_AW      0x03                                            
#define SETUP_RETR    0x04                                            
#define RF_CH         0x05                                            
#define RF_SETUP      0x06                                           
#define STATUS        0x07                                            
#define OBSERVE_TX    0x08                                            
#define CD            0x09                                            
#define RX_ADDR_P0    0x0A                                            
#define RX_ADDR_P1    0x0B                                            
#define RX_ADDR_P2    0x0C                                            
#define RX_ADDR_P3    0x0D                                            
#define RX_ADDR_P4    0x0E                                            
#define RX_ADDR_P5    0x0F                                           
#define TX_ADDR       0x10                                            
#define RX_PW_P0      0x11                                            
#define RX_PW_P1      0x12                                            
#define RX_PW_P2      0x13                                            
#define RX_PW_P3      0x14                                            
#define RX_PW_P4      0x15                                            
#define RX_PW_P5      0x16                                            
#define FIFO_STATUS   0x17                                           
#define w24L01_SPI                       SPI2
#define w24L01_SPI_CLK                   RCC_APB1Periph_SPI2
#define w24L01_SPI_SCK_PIN               GPIO_Pin_13                  
#define w24L01_SPI_SCK_GPIO_PORT         GPIOB                       
#define w24L01_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOB
#define w24L01_SPI_MISO_PIN              GPIO_Pin_14                  
#define w24L01_SPI_MISO_GPIO_PORT        GPIOB                       
#define w24L01_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOB
#define w24L01_SPI_MOSI_PIN              GPIO_Pin_15                  
#define w24L01_SPI_MOSI_GPIO_PORT        GPIOB                       
#define w24L01_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOB
#define w24L01_SPI_NSS_PIN               GPIO_Pin_12                  
#define w24L01_SPI_NSS_PORT              GPIOB                       
#define w24L01_SPI_NSS_CLK               RCC_APB2Periph_GPIOB
#define w24L01_CE_PIN                    GPIO_Pin_11                  
#define w24L01_CE_PORT                   GPIOB                       
#define w24L01_CE_CLK                    RCC_APB2Periph_GPIOB
#define w24L01_IRQ_PIN                   GPIO_Pin_10                  
#define w24L01_IRQ_PORT                  GPIOB                       
#define w24L01_IRQ_CLK                   RCC_APB2Periph_GPIOB
#define w24L01_USE_INTERUPUT
#ifdef w24L01_USE_INTERUPUT
#define w24L01_EXTI_LINE                EXTI_Line10
#define w24L01_NVIC_IRQCHANNEL          EXTI15_10_IRQn
#define w24L01_PIN_SOURCE               GPIO_PinSource10
#endif
#define w24L01_DATA_LEN                 (32)
#define w24L01_ADDR_LEN                 (5)
#define w24L01_ADDR_DEF                 0x0e,0x00,0x00,0x00,0x1f
#define w24L01_SET_BIT(PORT,PIN)               PORT->BSRR = PIN
#define w24L01_RESET_BIT(PORT,PIN)             PORT->BRR  = PIN
char w24L01_ReadReg(char Reg);
void Init_24L01(void);
void w24L01_WriteReg(char Addr,char Data);
void w24L01_WriteData(char Reg,char *Data,char DataLen);
void w24L01_ReadData(char Reg,char *Data,char DataLen);
void w24L01_SendData(char *SendBuffer,char DataSize);
void w24L01_ReadBuffer(char *Data,char DataLen);
void w24L01_SetTxMode(char RetryTimes);
void w24L01_SetCommonConfig(void);
#endif
