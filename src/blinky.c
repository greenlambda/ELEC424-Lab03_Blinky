#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32_eval.h"

GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
       
  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PD0 or PD2 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */
  while (1)
  {
    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;

    /* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;
  }
}
