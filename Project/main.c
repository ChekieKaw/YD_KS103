/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    ：主要是完成对eeprom的写操作和读操作
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-04-20
 * 调试方式：J-Link-OB
**********************************************************************************/

//头文件
#include "stm32f10x.h"
#include "led.h"
#include "SYSTICK.h"
#include "usart.h"
#include "i2c.h"
#include <stdio.h>
#include "ks103.h"

#define KS103_Address 0xd0
#define detect 0xbc
#define oldaddress 0xe8
#define ks103_1_address 0xd0 //done
#define ks103_2_address 0xd2 //done
#define ks103_3_address 0xd4 
#define ks103_4_address 0xd6

//void I2C_Test(void);
void KS_103_Test(u8 address, u8 command);


/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	u16 distance;
    SYSTICK_Init();
    USART1_Config();
    LED_GPIO_Config();	
    I2C_Configuration();
//    printf("\r\n 这是一个I2C外设(AT24C08)读写测试例程 \r\n");
//    I2C_Test();
//    while (1)
//    {
//    LEDXToggle(LED2);
//    delay_ms(500);
//    }
	
//    while(1)
//	{
//		delay_ms(1000);
//		distance=KS103_Demo(KS103_Address,0x02,detect);
//		//KS_103_Test(KS103_Address,detect);
//		printf("%d",distance);
//	}
	
	KS103_SetAddress(oldaddress,ks103_4_address);
	
}

void KS_103_Test(u8 address, u8 command)
{
	I2C_Start();
	I2C_SendByte(address);
	I2C_WaitAck();
	I2C_SendByte(0x02);
	I2C_WaitAck();
	I2C_SendByte(command);
	I2C_Stop();
	delay_us(50);
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





