#include "DHT11.h"
#include <stddef.h>
#include "Delay.h"

/**
  * @brief  将 DHT11 数据引脚配置为推挽输出模式
  * @param  无
  * @retval 无
  */
static void DHT11_Pin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  将 DHT11 数据引脚配置为上拉输入模式
  * @param  无
  * @retval 无
  */
static void DHT11_Pin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  读取 DHT11 数据引脚电平
  * @param  无
  * @retval 0 或 1
  */
static uint8_t DHT11_ReadBit(void)
{
    return GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

/**
  * @brief  主机拉低并释放总线，获取 DHT11 响应
  * @param  无
  * @retval 0=响应正常, 1=无响应
  */
static uint8_t DHT11_Start(void)
{
    uint16_t timeout;

    /* 主机拉低至少 18ms，表示起始信号 */
    DHT11_Pin_Output();
    GPIO_WriteBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN, Bit_RESET);
    Delay_ms(20);

    /* 主机拉高 30us，释放总线 */
    GPIO_WriteBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN, Bit_SET);
    Delay_us(30);

    /* 切换为输入模式，等待 DHT11 拉低（响应信号） */
    DHT11_Pin_Input();

    /* 等待 DHT11 拉低总线，超时约 100us */
    timeout = 100;
    while (DHT11_ReadBit() && timeout) { Delay_us(1); timeout--; }
    if (timeout == 0) return 1;

    /* DHT11 拉低约 80us，等待其释放 */
    timeout = 120;
    while (!DHT11_ReadBit() && timeout) { Delay_us(1); timeout--; }
    if (timeout == 0) return 1;

    /* DHT11 再拉高约 80us，准备发送数据 */
    timeout = 120;
    while (DHT11_ReadBit() && timeout) { Delay_us(1); timeout--; }
    if (timeout == 0) return 1;

    return 0;
}

/**
  * @brief  读取 DHT11 的一个字节数据
  * @param  无
  * @retval 读取到的字节
  */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t data = 0;
    uint16_t timeout;
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        timeout = 100;
        while (!DHT11_ReadBit() && timeout) { Delay_us(1); timeout--; }
        if (timeout == 0) return 0;

        Delay_us(35);

        data <<= 1;
        if (DHT11_ReadBit())
            data |= 0x01;

        timeout = 80;
        while (DHT11_ReadBit() && timeout) { Delay_us(1); timeout--; }
        if (timeout == 0) return 0;
    }

    return data;
}

/**
  * @brief  DHT11 初始化
  * @param  无
  * @retval 无
  */
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT11_GPIO_RCC, ENABLE);
    DHT11_Pin_Output();
    GPIO_WriteBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN, Bit_SET);
    Delay_ms(1500);
}

/**
  * @brief  读取 DHT11 的温湿度数据
  * @param  data 指向 DHT11_Data 结构体的指针
  * @retval 0=成功, 1=无响应, 2=校验失败
  */
uint8_t DHT11_ReadData(DHT11_Data *data)
{
    uint8_t buf[5];
    uint8_t sum;
    uint8_t i;

    if (data == NULL) return 1;

    if (DHT11_Start())
    {
        data->State = 1;
        return 1;
    }

    for (i = 0; i < 5; i++)
        buf[i] = DHT11_ReadByte();

    sum = buf[0] + buf[1] + buf[2] + buf[3];
    if (sum != buf[4])
    {
        data->State = 2;
        return 2;
    }

    data->Humidity = buf[0];
    data->Temp     = buf[2];
    data->CheckSum = buf[4];
    data->State    = 0;
    return 0;
}
