/************************************************
 * Name: blinky.c
 * Authors: Jeremy Hunt and Christopher Buck
 * Date: 9-30-14
 * Description: Blinks the green LED on the crazieflie
 * 				using timers and interrupts
 * The structure of this program was based off a reference
 * describing how to use timers on an STM chip.  You may
 * find their website here:
 * http://visualgdb.com/tutorials/arm/stm32/timers/
 ************************************************/
#include "blinky.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

/*
 * Initialize the GPIO which controls the LED
 */
static void InitializeLEDs() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_5;
	gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); /* clear bit; LED on */
}

/*
 * Initialize the timer and set the clock to the external oscillator
 */
static void InitializeTimer() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); /* set clock to HSE */

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM2PRESCALER;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = TIM2PERIOD;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/*
 * Enable the interrupt for the timer (TIM2)
 */
static void EnableTimerInterrupt() {
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

/*
 * Main function.  Initializes the GPIO, Timers, and
 */
int main() {
	InitializeLEDs();
	InitializeTimer();
	EnableTimerInterrupt();

	/* Loop. Forever. */
	for (;;) {
	}
}

/*
 * Interrupt service routine.  This function handles toggling the LED
 * when the timer throws an update flag.
 */
void TIM2_IRQHandler() {
	static unsigned char stateLED = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); /* clear flag */
		if (stateLED)
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
		else
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
		stateLED = 1 - stateLED; /* flip the state for next operation */
	}
}
