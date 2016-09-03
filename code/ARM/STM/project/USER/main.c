#include "main.h"

GPIO_InitTypeDef  GPIO_InitStructure;
GPIO_InitTypeDef  GPIO_InitStructure_I2C;
I2C_InitTypeDef  I2C_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;
void I2C_Config(GPIO_InitTypeDef *GPIO_InitStructure_t, I2C_InitTypeDef  *I2C_InitStructure_t);
uint8_t I2C_Read(I2C_TypeDef *I2Cx_t,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t len);


int main(void)
{
  uint8_t buf[8];

  /* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = LED1_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
  I2C_Config(&GPIO_InitStructure_I2C,&I2C_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PG6 or PG8 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */  
  while (1)
  {
		I2C_Read(I2C1,0x68,0x75,buf,1);

		
    /* Set PB9 */
    GPIOB->BSRRL = LED1_PIN;
    /* Reset PB9 */
    GPIOB->BSRRH = LED1_PIN;
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
