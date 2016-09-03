#include "stm32f4xx.h"
#include "I2C.h"
#define DEVICE_ADDRESS 0x68 //bit 7 : 0b1101000 = ox68 AD=0;

void I2C_Config(GPIO_InitTypeDef *GPIO_InitStructure_t, I2C_InitTypeDef  *I2C_InitStructure_t)
{
 
	
  /* RCC Configuration */
  /*I2C Peripheral clock enable */
  RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
  
  /*SDA GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, ENABLE);
  
  /*SCL GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, ENABLE);
  
  
  
  /* GPIO Configuration */
  /*Configure I2C SCL pin */
  GPIO_InitStructure_t->GPIO_Pin = I2Cx_SCL_PIN;
  GPIO_InitStructure_t->GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure_t->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_t->GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure_t->GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(I2Cx_SCL_GPIO_PORT, GPIO_InitStructure_t);
  
  /*Configure I2C SDA pin */
  GPIO_InitStructure_t->GPIO_Pin = I2Cx_SDA_PIN;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, GPIO_InitStructure_t);
    
  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
  
  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);
	
	  /* I2C Struct Initialize */
  I2C_InitStructure_t->I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure_t->I2C_DutyCycle = I2C_DUTYCYCLE;
  I2C_InitStructure_t->I2C_OwnAddress1 = 0x0A;
  I2C_InitStructure_t->I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure_t->I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStructure_t->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	  /* I2C Initialize */
  I2C_Cmd(I2Cx, ENABLE);
  I2C_Init(I2Cx, I2C_InitStructure_t);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	
	
	
	
	
	

//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

//GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
//GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

//GPIO_InitStructure_t->GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//GPIO_InitStructure_t->GPIO_Mode = GPIO_Mode_AF;
//GPIO_InitStructure_t->GPIO_OType = GPIO_OType_OD;
//GPIO_InitStructure_t->GPIO_Speed = GPIO_Speed_50MHz;
//GPIO_InitStructure_t->GPIO_PuPd = GPIO_PuPd_NOPULL;
//GPIO_Init(GPIOB, GPIO_InitStructure_t);

//I2C_DeInit(I2C1);
//I2C_InitStructure_t->I2C_Mode = I2C_Mode_I2C;
//I2C_InitStructure_t->I2C_DutyCycle = I2C_DutyCycle_2;
//I2C_InitStructure_t->I2C_OwnAddress1 = 0x30; 
//I2C_InitStructure_t->I2C_Ack = I2C_Ack_Enable; 
//I2C_InitStructure_t->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//I2C_InitStructure_t->I2C_ClockSpeed = 100000;
//I2C_Cmd(I2C1, ENABLE);
//I2C_Init(I2C1, I2C_InitStructure_t);
////I2C_AcknowledgeConfig(I2C_BMP085, ENABLE);




}

uint8_t I2C_Read(I2C_TypeDef *I2Cx_t,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t len)
{  
if(len==0)
return 1;

  
//while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

I2C_AcknowledgeConfig(I2Cx, ENABLE);
 
I2C_GenerateSTART(I2Cx, ENABLE);
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2Cx,  I2C_Addr, I2C_Direction_Transmitter);
while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

I2C_SendData(I2Cx, addr);
while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTART(I2Cx, ENABLE);
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); 


while (len)
{
if(len==1)
{
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
  I2C_GenerateSTOP(I2Cx, ENABLE);
}
 
while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
 *buf = I2C_ReceiveData(I2Cx);
 buf++;
 /* Decrement the read bytes counter */
 len--;
}  
I2C_AcknowledgeConfig(I2Cx, ENABLE);
return 0;
} 


uint8_t I2C_Write(I2C_TypeDef *I2Cx_t,uint8_t I2C_Addr,uint8_t addr,uint8_t data)
{

while(I2C_GetFlagStatus(I2Cx_t, I2C_FLAG_BUSY))

I2C_GenerateSTART(I2Cx_t, ENABLE);
while(!I2C_CheckEvent(I2Cx_t, I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2Cx_t, I2C_Addr, I2C_Direction_Transmitter);
while(!I2C_CheckEvent(I2Cx_t, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

I2C_SendData(I2Cx_t, addr);
while(!I2C_CheckEvent(I2Cx_t, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_SendData(I2Cx_t, data);

while(!I2C_CheckEvent(I2Cx_t, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTOP(I2Cx_t, ENABLE);
I2C_AcknowledgeConfig(I2Cx_t, DISABLE);

return 0;
}


