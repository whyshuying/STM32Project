#include "Alert.h"
#include "Delay.h"

/**
  * @brief  报警模块初始化（蜂鸣器 + LED）
  * @param  无
  * @retval 无
  */
void Alert_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOB 时钟 */
    RCC_APB2PeriphClockCmd(BUZZER_GPIO_RCC | LED_ALARM_GPIO_RCC, ENABLE);

    /* 配置蜂鸣器引脚为推挽输出 */
    GPIO_InitStructure.GPIO_Pin   = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);

    /* 配置 LED 引脚为推挽输出 */
    GPIO_InitStructure.GPIO_Pin   = LED_ALARM_GPIO_PIN;
    GPIO_Init(LED_ALARM_GPIO_PORT, &GPIO_InitStructure);

    /* 初始状态：关闭 */
    Alert_Buzzer_OFF();
    Alert_LED_OFF();
}

/**
  * @brief  打开蜂鸣器
  * @param  无
  * @retval 无
  */
void Alert_Buzzer_ON(void)
{
    GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_RESET);
}

/**
  * @brief  关闭蜂鸣器
  * @param  无
  * @retval 无
  */
void Alert_Buzzer_OFF(void)
{
    GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_SET);
}

/**
  * @brief  打开 LED 报警灯
  * @param  无
  * @retval 无
  */
void Alert_LED_ON(void)
{
    GPIO_WriteBit(LED_ALARM_GPIO_PORT, LED_ALARM_GPIO_PIN, Bit_SET);
}

/**
  * @brief  关闭 LED 报警灯
  * @param  无
  * @retval 无
  */
void Alert_LED_OFF(void)
{
    GPIO_WriteBit(LED_ALARM_GPIO_PORT, LED_ALARM_GPIO_PIN, Bit_RESET);
}
