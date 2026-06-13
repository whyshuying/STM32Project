#ifndef __ALERT_H
#define __ALERT_H

#include "stm32f10x.h"

/* 有源蜂鸣器引脚定义 (PB1) — 温度报警 */
#define BUZZER_GPIO_PORT      GPIOB
#define BUZZER_GPIO_PIN       GPIO_Pin_1
#define BUZZER_GPIO_RCC       RCC_APB2Periph_GPIOB

/* LED 报警灯引脚定义 (PB2) — 湿度报警 */
#define LED_ALARM_GPIO_PORT   GPIOB
#define LED_ALARM_GPIO_PIN    GPIO_Pin_14
#define LED_ALARM_GPIO_RCC    RCC_APB2Periph_GPIOB

/* 温度/湿度报警阈值 */
#define TEMP_ALARM_THRESHOLD   35
#define HUMI_ALARM_THRESHOLD   85

/* 模块初始化 */
void Alert_Init(void);

/* 蜂鸣器控制（温度报警）*/
void Alert_Buzzer_ON(void);
void Alert_Buzzer_OFF(void);

/* LED 控制（湿度报警）*/
void Alert_LED_ON(void);
void Alert_LED_OFF(void);

#endif
