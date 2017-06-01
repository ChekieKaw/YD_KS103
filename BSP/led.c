/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��led.c
 * ����    ��LED����������     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-04-20
 * Ӳ������: LED->Pb9
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "led.h"

 /**
  * @file   LED_GPIO_Config
  * @brief  LED����������
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{	
    //����һ��GPIO_InitTypeDef ���͵Ľṹ��
    GPIO_InitTypeDef  GPIO_InitStructure;	
    RCC_APB2PeriphClockCmd(LED2_GPIO_RCC,ENABLE);//ʹ��GPIO������ʱ��
    /*LED*/
    GPIO_InitStructure.GPIO_Pin =LED2_GPIO_PIN;//ѡ��Ҫ�õ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��������ģʽΪ�������ģʽ						 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ         
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
}

 /**
  * @file   LEDXToggle
  * @brief  LED����ת
  * @param  ledx:1--PC13;2--PB0;1--PB1
  * @retval ��
  */
void LEDXToggle(uint8_t ledx)
{
  if(ledx==1)
  {
    LED1_GPIO_PORT->ODR^=LED1_GPIO_PIN;
  }
  else if(ledx==2)
  {
    LED2_GPIO_PORT->ODR^=LED2_GPIO_PIN;
  }
  else
  {
    LED3_GPIO_PORT->ODR^=LED3_GPIO_PIN;
  }


}

