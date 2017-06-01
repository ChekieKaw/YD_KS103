#ifndef __KS103_H
#define __KS103_H

#include "stm32f10x.h"
#include "SYSTICK.h"
#include "i2c.h"

u8 KS103_ReadOneByte(u8 address, u8 reg);
void KS103_WriteOneByte(u8 address, u8 reg, u8 command);
u16 KS103_GetData(u8 address, u8 reg);
void KS103_SetAddress(u8 oldADD, u8 newADD);
u16 KS103_Demo(u8 address, u8 reg, u8 command);

#endif
