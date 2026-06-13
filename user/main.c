#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "DHT11.h"
#include "Alert.h"

/* ========== 时序参数 ========== */
#define TICK_MS             100     /* 主循环 tick 间隔 */
#define SAMPLE_TICKS        20      /* 每 20 ticks = 2000ms 采样一次 DHT11 */
#define BUZZER_ON_TICKS     3       /* 蜂鸣器持续 3 ticks = 300ms */
#define BUZZER_OFF_TICKS    7       /* 蜂鸣器静默 7 ticks = 700ms */
#define ALARM_CLEAR_TICKS   2       /* 连续 2 次正常读数后才解除报警（防抖动）*/

int main(void){

    DHT11_Data dht11;
    uint32_t tick = 0;
    uint8_t  temp_alarm = 0;            /* 温度报警标志（驱动蜂鸣器）*/
    uint8_t  humi_alarm = 0;            /* 湿度报警标志（驱动 LED）*/
    uint8_t  buzzer_tick = 0;           /* 蜂鸣器节奏计数器 */
    uint8_t  temp_clear_cnt = 0;        /* 温度报警防抖计数器 */
    uint8_t  humi_clear_cnt = 0;        /* 湿度报警防抖计数器 */

    OLED_Init();
    OLED_Clear();
    DHT11_Init();
    Alert_Init();

    /* OLED 静态标题 */
    OLED_ShowString(1, 1, "Temperature");
    OLED_ShowString(3, 1, "Humidity   ");

    while (1)
    {
        /* ========== 定时采样（每 20 ticks = 2s）========== */
        if ((tick % SAMPLE_TICKS) == 0)
        {
            uint8_t ret = DHT11_ReadData(&dht11);

            if (ret == 0)
            {
                /* --- 显示温度 --- */
                OLED_ShowString(2, 1, "       ");
                OLED_ShowNum(2, 1, dht11.Temp, 2);
                OLED_ShowString(2, 3, " C");

                /* --- 温度报警判断 --- */
                if (dht11.Temp > TEMP_ALARM_THRESHOLD)
                {
                    temp_alarm = 1;
                    temp_clear_cnt = 0;
                    OLED_ShowString(2, 8, "T-ALM");
                }
                else
                {
                    temp_clear_cnt++;
                    if (temp_clear_cnt >= ALARM_CLEAR_TICKS)
                    {
                        temp_alarm = 0;
                        OLED_ShowString(2, 8, "     ");
                    }
                }

                /* --- 显示湿度 --- */
                OLED_ShowString(4, 1, "       ");
                OLED_ShowNum(4, 1, dht11.Humidity, 2);
                OLED_ShowString(4, 3, " %");

                /* --- 湿度报警判断 --- */
                if (dht11.Humidity > HUMI_ALARM_THRESHOLD)
                {
                    humi_alarm = 1;
                    humi_clear_cnt = 0;
                    OLED_ShowString(4, 8, "H-ALM");
                }
                else
                {
                    humi_clear_cnt++;
                    if (humi_clear_cnt >= ALARM_CLEAR_TICKS)
                    {
                        humi_alarm = 0;
                        OLED_ShowString(4, 8, "     ");
                    }
                }

                OLED_ShowString(4, 14, "OK");
            }
            else
            {
                OLED_ShowString(4, 14, "ER");
                temp_alarm = 0;
                humi_alarm = 0;
                temp_clear_cnt = 0;
                humi_clear_cnt = 0;
                buzzer_tick = 0;
            }
        }

        /* ========== 蜂鸣器控制（温度报警，300ms 响 + 700ms 停循环）========== */
        if (temp_alarm)
        {
            if (buzzer_tick < BUZZER_ON_TICKS)
                Alert_Buzzer_ON();
            else
                Alert_Buzzer_OFF();

            buzzer_tick++;
            if (buzzer_tick >= (BUZZER_ON_TICKS + BUZZER_OFF_TICKS))
                buzzer_tick = 0;
        }
        else
        {
            Alert_Buzzer_OFF();
            buzzer_tick = 0;
        }

        /* ========== LED 控制（湿度报警，常亮）========== */
        if (humi_alarm)
            Alert_LED_ON();
        else
            Alert_LED_OFF();

        tick++;
        Delay_ms(TICK_MS);
    }
}
