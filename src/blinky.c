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

    //Motors 3&4
    TIM_OC3Init(TIM4, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM4, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    //Motors 1&2
    TIM_OC3Init(TIM3, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    //TIM_CtrlPWMOutputs(TIM3, ENABLE);
    //TIM_CtrlPWMOutputs(TIM4, ENABLE);

    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    //GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
}

/*
 * Initialize the GPIO which controls the LED
 */
static void InitializeLEDs() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_5;
	gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);

	gpioStructure.GPIO_Pin = GPIO_Pin_4;
	gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET); /* set bit; LED off */
}

/*
 * Initialize the GPIO which controls the Motors
 */
static void InitializeMotors() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	gpioStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);

	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	//GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* set bit; LED off */
}

/*
 * Initialize the timer and set the clock to the external oscillator
 */
static void InitializeTimer() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); /* set clock to HSE */

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM_PRESCALER;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = TIM_PERIOD;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	//TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = TIM_PRESCALER;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = TIM_PERIOD;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_Cmd(TIM3, ENABLE);

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
	int i;
	for (i = 0;i < 1500000; i++);
	InitializeLEDs();
	InitializeMotors();
	InitializeTimer();
	EnableTimerInterrupt();
	InitializePWMChannel();

	setMotor(Motor1, 25);
	for (i = 0;i < 500000; i++);
	setMotor(Motor2, 25);
	for (i = 0;i < 500000; i++);
	setMotor(Motor3, 25);
	for (i = 0;i < 500000; i++);
	setMotor(Motor4, 25);
	for (i = 0;i < 500000; i++);

	setMotor(Motor1, 0);
	for (i = 0;i < 500000; i++);
	setMotor(Motor2, 0);
	for (i = 0;i < 500000; i++);
	setMotor(Motor3, 0);
	for (i = 0;i < 500000; i++);
	setMotor(Motor4, 0);
	for (i = 0;i < 500000; i++);
	for (;;);
}

/*
 * Set a motor to be on or off
 * @param  m : Select the motor to set the state for
 * 		Can be one of the following values
 * 		Motor1, Motor2, Motor3, Motor4
 * @param  s : Select the speed for the motor
 * 		Can be between 0 and 100
 */
void setMotor(motor_t m, int duty) {
	//MULT: preprocessor define -
	//multiplier to convert speed to units in
	//terms of the timer period
	int pulse = duty * MULT;	//pulse width (in ticks) to achieve the
								//desired duty cycle
	switch (m)
	{
	case Motor1 :
		MOTOR1_SET_COMPARE(MOTOR1_TIM, pulse);
		break;
	case Motor2 :
		MOTOR2_SET_COMPARE(MOTOR2_TIM, pulse);
		break;
	case Motor3 :
		MOTOR3_SET_COMPARE(MOTOR3_TIM, pulse);
		break;
	case Motor4 :
		MOTOR4_SET_COMPARE(MOTOR4_TIM, pulse);
		break;
	}
}

/*
 * Interrupt service routine.  This function handles toggling the LED
 * when the timer throws an update flag.
 */
void TIM4_IRQHandler() {
	//ITStatus updateSig,compareSig;
	//updateSig = TIM_GetITStatus(TIM4, TIM_IT_Update);
	//compareSig = TIM_GetITStatus(TIM4, TIM_IT_CC1);
//	int timerValue1 = TIM_GetCounter(TIM4);
	static unsigned char stateLED = 0;
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET &&
//		TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
//		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
//	}
	/* Overflow event */
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* clear flag */
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET); /* Turn on LED */
		setMotor(Motor1, ENABLE); /* Turn on Motor */
		setMotor(Motor2, ENABLE);
		//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
	/* Capture Control event */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); /* clear flag */
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
			setMotor(Motor1, DISABLE); /* Turn off Motor */
	//		setMotor(Motor1, DISABLE);
			//stateLED = 1 - stateLED; /* flip the state for next operation */
	}
	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) {
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2); /* clear flag */
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET); /* Turn off LED */
			setMotor(Motor2, DISABLE); /* Turn off Motor */
	//		setMotor(Motor1, DISABLE);
			//stateLED = 1 - stateLED; /* flip the state for next operation */
		}
//	int timerValue2 = TIM_GetCounter(TIM4);

}
