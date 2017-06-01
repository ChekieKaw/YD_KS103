#include "ks103.h"
#include "i2c.h"
#include "systick.h"
#include "stm32f10x.h"

/**
 *@file KS103ReadByte
 *@brief Read one byte data from ks103 0x02/0x03
 *@param u8 address (address of ks103); u8 reg (address of register)
 *@retval u8 data(hex)
*/
u8 KS103_ReadOneByte(u8 address, u8 reg)
{
	u8 buffer=0;
	
	I2C_Start();	
	I2C_SendByte(address);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(address+1);
	I2C_WaitAck();
	delay_us(100);
	buffer=I2C_ReceiveByte();//ÎÞackÓ¦´ð
	I2C_Stop();
	return buffer;
	
}

void KS103_WriteOneByte(u8 address, u8 reg, u8 command)
{
	I2C_Start();
	I2C_SendByte(address);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_SendByte(command);
	I2C_WaitAck();
	I2C_Stop();
	delay_us(50);
}

u16 KS103_GetData(u8 address, u8 reg)
{
	u16 data=0;
	data=KS103_ReadOneByte(address,reg);
	data<<=8;
	data+=KS103_ReadOneByte(address,reg);
	return data;
}

void KS103_SetAddress(u8 oldADD, u8 newADD)
{
	delay_ms(2000);
	KS103_WriteOneByte(oldADD,0x02,0x9a);
	delay_ms(2);
	
	KS103_WriteOneByte(oldADD,0x02,0x92);
	delay_ms(2);
	
	KS103_WriteOneByte(oldADD,0x02,0x9e);
	delay_ms(2);
	
	KS103_WriteOneByte(oldADD,0x02,newADD);
	delay_ms(150);
}

u16 KS103_Demo(u8 address, u8 reg, u8 command)
{
	u16 distance=0x0000;
	KS103_WriteOneByte(address,reg,command);
	delay_ms(100);
	distance=KS103_GetData(address,reg);
	return distance;
}

