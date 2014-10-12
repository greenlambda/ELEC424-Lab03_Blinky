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
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "blinky.h"


void InitializePWMChannel()
{
    TIM_OCInitTypeDef outputChannelInit;
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = TIM2PERIOD / 8 * 7;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
}

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

	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* set bit; LED off */
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
	TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
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
	InitializePWMChannel();

	/* Loop. Forever. */
	for (;;) {
	}
}

/*
 * Interrupt service routine.  This function handles toggling the LED
 * when the timer throws an update flag.
 */
void TIM2_IRQHandler() {
	ITStatus updateSig,compareSig;
	updateSig = TIM_GetITStatus(TIM2, TIM_IT_Update);
	compareSig = TIM_GetITStatus(TIM2, TIM_IT_CC1);
//	int timerValue1 = TIM_GetCounter(TIM2);
	static unsigned char stateLED = 0;
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET &&
//		TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
//		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
//	}
	/* Overflow event */
	if (updateSig != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); /* clear flag */
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); /* Turn on LED */
		//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
	/* Capture Control event */
	else if (compareSig != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); /* clear flag */
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
		//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
//	int timerValue2 = TIM_GetCounter(TIM2);

}
