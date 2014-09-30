#define STM32F10X_MD

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
       
  /* GPIOB Periph clock enable */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  /* Configure PB5 in output pushpull mode */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PD0 or PD2 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */
  while (1)
  {
    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;

    /* Set PB5 */
    GPIOB->BSRR = 0x00000010;
    /* Reset PB5 */
    GPIOB->BRR  = 0x00000010;
  }
}
