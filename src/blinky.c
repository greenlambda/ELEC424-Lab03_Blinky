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
    outputChannelInit.TIM_Pulse = 0; /* Set to 0% */
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM4, &outputChannelInit);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM4, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM4, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
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
 * Initialize the GPIO which controls the Motors
 */
static void InitializeMotors() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
							 GPIO_Pin_8 | GPIO_Pin_9;
	gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	//GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* set bit; LED off */
}

/*
 * Initialize the timer and set the clock to the external oscillator
 */
static void InitializeTimer() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); /* set clock to HSE */

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM4PRESCALER;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = TIM4PERIOD;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1, ENABLE);
}

/*
 * Enable the interrupt for the timer (TIM4)
 */
static void EnableTimerInterrupt() {
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM4_IRQn;
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
	InitializeMotors();
	InitializeTimer();
	EnableTimerInterrupt();
	InitializePWMChannel();

	TIM_SetCompare3(TIM4, 0);
	TIM_SetCompare1(TIM4, TIM4PERIOD / 20);
	/* Loop. */
	int i;
	for (i = 0;i < 1000000; i++) {
		//wait
	}
	TIM_SetCompare1(TIM4, TIM4PERIOD / 40);
	for (;;);

}

/*
 * Set a motor to be on or off
 * @param  m : Select the motor to set the state for
 * 		Can be one of the following values
 * 		Motor1, Motor2, Motor3, Motor4
 * @param  s : Select the state for the motor
 * 		Can be either ENABLE or DISABLE
 */
void setMotor(motor_t m, FunctionalState s) {
	BitAction b = (s == ENABLE) ? Bit_SET : Bit_RESET;
	switch (m)
	{
	case Motor1 :
		GPIO_WriteBit(GPIOB, MOTOR1_PIN, b);
		break;
	case Motor2 :
		GPIO_WriteBit(GPIOB, MOTOR2_PIN, b);
		break;
	case Motor3 :
		GPIO_WriteBit(GPIOB, MOTOR3_PIN, b);
		break;
	case Motor4 :
		GPIO_WriteBit(GPIOB, MOTOR4_PIN, b);
		break;
	}
}

/*
 * Interrupt service routine.  This function handles toggling the LED
 * when the timer throws an update flag.
 */
void TIM4_IRQHandler() {
	ITStatus updateSig,compareSig;
	updateSig = TIM_GetITStatus(TIM4, TIM_IT_Update);
	compareSig = TIM_GetITStatus(TIM4, TIM_IT_CC1);
//	int timerValue1 = TIM_GetCounter(TIM4);
	static unsigned char stateLED = 0;
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET &&
//		TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
//		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
//	}
	/* Overflow event */
	if (updateSig != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* clear flag */
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); /* Turn on LED */
		setMotor(Motor1, ENABLE); /* Turn on Motor */
		setMotor(Motor2, ENABLE);
		//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
	/* Capture Control event */
	else if (compareSig != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); /* clear flag */
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
		setMotor(Motor1, DISABLE); /* Turn off Motor */
		setMotor(Motor2, DISABLE);
		//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
//	int timerValue2 = TIM_GetCounter(TIM4);

}
