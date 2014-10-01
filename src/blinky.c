#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;

int main(void) {
	/* GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	volatile int i = 0;

	while (1) {
		/* Set PB5 */
		GPIOB->BSRR = GPIO_Pin_5;
		for (i = 0; i < 10000000; i++);
		/* Reset PB5 */
		GPIOB->BRR = GPIO_Pin_5;
		for (i = 0; i < 10000000; i++);
	}
}
