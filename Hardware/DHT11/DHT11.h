#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

/* DHT11 数据引脚定义 (PB0) */
#define DHT11_GPIO_PORT      GPIOB
#define DHT11_GPIO_PIN       GPIO_Pin_0
#define DHT11_GPIO_RCC       RCC_APB2Periph_GPIOB

/* DHT11 数据结构体 */
typedef struct
{
    uint8_t Humidity;       /* 湿度整数部分 (0~100%RH) */
    uint8_t Temp;           /* 温度整数部分 (0~50°C) */
    uint8_t CheckSum;       /* 校验和 (前4字节累加) */
    uint8_t State;          /* 0=成功, 1=无响应, 2=校验失败 */
} DHT11_Data;

/* 函数声明 */
void DHT11_Init(void);
uint8_t DHT11_ReadData(DHT11_Data *data);

#endif
