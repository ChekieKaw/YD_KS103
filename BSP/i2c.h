#ifndef __I2C_H
#define __I2C_H			 
#include "stm32f10x.h"
#include "SYSTICK.h"

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/

#define EEPROM_I2C_SCL_PIN      					 	GPIO_Pin_6   
#define EEPROM_I2C_SCL_GPIO_PORT   	       	GPIOB  
#define EEPROM_I2C_SCL_GPIO_RCC      			  RCC_APB2Periph_GPIOB  
 
#define EEPROM_I2C_SDA_PIN      					 	GPIO_Pin_7   
#define EEPROM_I2C_SDA_GPIO_PORT   	       	GPIOB  
#define EEPROM_I2C_SDA_GPIO_RCC      			  RCC_APB2Periph_GPIOB  

#define KS103_I2C_SCL_PIN      					 	GPIO_Pin_6   
#define KS103_I2C_SCL_GPIO_PORT   	       	GPIOB  
#define KS103_I2C_SCL_GPIO_RCC      			  RCC_APB2Periph_GPIOB  
 
#define KS103_I2C_SDA_PIN      					 	GPIO_Pin_7   
#define KS103_I2C_SDA_GPIO_PORT   	       	GPIOB  
#define KS103_I2C_SDA_GPIO_RCC      			  RCC_APB2Periph_GPIOB  




#define SCL_H         	 GPIO_SetBits(KS103_I2C_SCL_GPIO_PORT , KS103_I2C_SCL_PIN)   /*GPIOB->BSRR = GPIO_Pin_6*/
#define SCL_L            GPIO_ResetBits(KS103_I2C_SCL_GPIO_PORT , KS103_I2C_SCL_PIN) /*GPIOB->BRR  = GPIO_Pin_6 */
   
#define SDA_H         	 GPIO_SetBits(KS103_I2C_SDA_GPIO_PORT, KS103_I2C_SDA_PIN)   /*GPIOB->BSRR = GPIO_Pin_7*/
#define SDA_L         	 GPIO_ResetBits(KS103_I2C_SDA_GPIO_PORT , KS103_I2C_SDA_PIN) /*GPIOB->BRR  = GPIO_Pin_7*/

#define SCL_read       	GPIO_ReadInputDataBit(KS103_I2C_SCL_GPIO_PORT , KS103_I2C_SCL_PIN)/* GPIOB->IDR  & GPIO_Pin_6   */
#define SDA_read       	GPIO_ReadInputDataBit(KS103_I2C_SDA_GPIO_PORT , KS103_I2C_SDA_PIN)/*GPIOB->IDR  & GPIO_Pin_7	  */

#define I2C_PageSize  8  

#define ADDR_24C08		0xA0

/* Private function prototypes -----------------------------------------------*/
void I2C_Test(void);
void I2C_Configuration(void);
FunctionalState I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress);
FunctionalState I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress,  uint8_t DeviceAddress);
FunctionalState I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
FunctionalState I2C_WaitAck(void);
void I2C_SendByte(uint8_t SendByte);
u8 I2C_ReceiveByte(void);

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
