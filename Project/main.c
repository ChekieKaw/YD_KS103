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

#include "mavlink_avoid_errors.h"
#include "stm32f10x.h"
#include "led.h"
#include "SYSTICK.h"
#include "usart.h"
#include "i2c.h"
#include <stdio.h>
#include "ks103.h"
#include "mavlink.h"
#include "common.h"
#include "checksum.h"
#include "protocol.h"
#include "string.h"
#include "math.h"



#define KS103_Address 0xd0
#define detect 0xb8
#define oldaddress 0xe8
#define ks103_1_address 0xd0 //done
#define ks103_2_address 0xd2 //done
#define ks103_3_address 0xd4 //done
#define ks103_4_address 0xd6 //done

#define distance_y_1 distance[0]//y+
#define distance_x_0 distance[1]//x-
#define distance_y_0 distance[2]//y-
#define distance_x_1 distance[3]//x+

#define Gravity 9.85
#define system_ID 0x00
#define component_ID 0x00
#define PI 3.14159

typedef struct sonar_distance
{
	u8 address;
	u16 distance;
	int position;//This is the falg of sonar position (or its direction)
}Sonar_Distance;

typedef struct attitute
{
	float pitch_angel;
	float roll_angel;
	float yaw_angel;
	float thrust;
}ATTITUTE;

/*Function definition*/
void KS_103_Test(u8 address, u8 command);
//void I2C_Test(void);
void mavlink_init(mavlink_system_t* mavlink_system);
float distance2angle(float distance);




//static uint16_t cnt_1000ms;
Sonar_Distance distance[4];
/* mavlink*/
mavlink_system_t mavlink_system;
mavlink_message_t msg_send;
mavlink_message_t msg_recieve;
ATTITUTE current_attitute;
ATTITUTE set_attitute;
double distance_x;
double distance_y;
mavlink_set_roll_pitch_yaw_thrust_t set_attitue_msg;


#define distance_init() {distance[0].address=ks103_1_address;distance[0].position=1;distance[0].distance=12000;\
						distance[1].address=ks103_2_address;distance[1].position=-1;distance[1].distance=12000;\
						distance[2].address=ks103_3_address;distance[2].position=-1;distance[2].distance=12000;\
						distance[3].address=ks103_4_address;distance[3].position=1;distance[3].distance=12000;}

/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	int i=0,j=0;
	unsigned short len=0;
	u8 buf[MAVLINK_MAX_PACKET_LEN];
	//distance init.
	distance[0].address=ks103_1_address;distance[0].position=1;
	distance[1].address=ks103_2_address;distance[1].position=-1;
	distance[2].address=ks103_3_address;distance[2].position=-1;
	distance[3].address=ks103_4_address;distance[3].position=1;
	//
    SYSTICK_Init();
    USART1_Config();
    LED_GPIO_Config();	
    I2C_Configuration();
	mavlink_init(&mavlink_system);
	
//    printf("\r\n 这是一个I2C外设(AT24C08)读写测试例程 \r\n");
//    I2C_Test();
//    while (1)
//    {
//    LEDXToggle(LED2);
//    delay_ms(500);
//    }
	memset(&current_attitute,0,sizeof(current_attitute));
	memset(&set_attitute,0,sizeof(set_attitute));
	
    while(1)
	{
		distance_x=0;
		distance_y=0;
		KS103_WriteOneByte(0x00,0x02,0xb8);//boardcast a detecting command.
		while(!SCL_read);
		delay_ms(5);//this could replace a get attitute code, as a delay NVIC status;
		do
		{
			for(i=0;i<4;i++)
			{
				distance[i].distance=KS103_GetData(distance[i].address,0x02);
			}
		}
		while(distance[0].distance>11000&&
			  distance[1].distance>11000&&
			  distance[2].distance>11000&&
			  distance[3].distance>11000);//get distance.
		/*culculate roll,pitch angle*/
		distance_x=(distance_x_1.distance*distance_x_1.position+distance_x_0.distance*distance_x_0.position)*
					(1/sqrt(1+pow(tan(current_attitute.roll_angel/360*2*PI),2)));
		distance_y=(distance_y_1.distance*distance_y_1.position+distance_y_0.distance*distance_y_0.position)*
					(1/sqrt(1+pow(tan(current_attitute.pitch_angel/360*2*PI),2)));
		set_attitute.pitch_angel=(float)atan(-distance2angle(
														(fabs(distance_x)*fabs(distance_y))/
														sqrt(pow(distance_x,2)+pow(distance_y,2))
													)*distance_x/
									(Gravity*sqrt(pow(distance_x,2)+pow(distance_y,2)))
									)/(2*PI)*360;
		set_attitute.roll_angel=(float)atan(-distance2angle(
														(fabs(distance_x)*fabs(distance_y))/
														sqrt(pow(distance_x,2)+pow(distance_y,2))
													)*distance_y/
									(Gravity*sqrt(pow(distance_x,2)+pow(distance_y,2)))
									)/(2*PI)*360;
		/*set setangle as current angle( current scheme)*/
		set_attitute.yaw_angel=0;
		set_attitute.thrust=0.7;
		current_attitute.pitch_angel=set_attitute.pitch_angel;
		current_attitute.roll_angel=set_attitute.roll_angel;
		current_attitute.thrust=set_attitute.thrust;
		current_attitute.yaw_angel=set_attitute.yaw_angel;
		/*use mavlink*/
		set_attitue_msg.pitch=set_attitute.pitch_angel;
		set_attitue_msg.roll=set_attitute.roll_angel;
		set_attitue_msg.thrust=set_attitute.thrust;
		set_attitue_msg.yaw=set_attitute.yaw_angel;
		set_attitue_msg.target_system=system_ID;
		set_attitue_msg.target_component=component_ID;
		mavlink_msg_set_roll_pitch_yaw_thrust_encode(system_ID,component_ID,&msg_send,&set_attitue_msg);
		len=mavlink_msg_to_send_buffer(buf,&msg_send);
		for(j=0;j<len;j++)
		{
			USART_SendData(USART1,buf[j]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		}
	}
	
	//KS103_SetAddress(oldaddress,ks103_4_address);
	
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

void mavlink_init(mavlink_system_t* mavlink_system)
{
	mavlink_system->sysid=20;
	mavlink_system->compid=MAV_COMP_ID_IMU;
	mavlink_system->type=MAV_TYPE_QUADROTOR;
}

float distance2angle(float distance)
{
	if (distance>=0&&distance <=1) return 5;
	else if (distance >1 && distance <= 6) return (float)(-1+exp(-0.358*distance +2.148));
	else if (distance >6 )return 0;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





