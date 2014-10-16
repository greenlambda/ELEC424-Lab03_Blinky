#define TIM_PRESCALER 	0
#define TIM_PERIOD 		200

typedef enum {
	Motor1, Motor2, Motor3, Motor4
} motor_t;

#define MOTOR1_PIN		1
#define MOTOR2_PIN		0
#define MOTOR3_PIN		9
#define MOTOR4_PIN		8

#define MOTOR1_TIM		TIM3
#define MOTOR2_TIM		TIM3
#define MOTOR4_TIM		TIM4
#define MOTOR3_TIM		TIM4

//Motor 1: Tim3 channel 4
//Motor 2: Tim3 channel 3
//Motor 3: Tim4 channel 4
//Motor 4: Tim4 channel 3
#define MOTOR1_SET_COMPARE(a, b)	TIM_SetCompare4(a, b);
#define MOTOR2_SET_COMPARE(a, b)	TIM_SetCompare3(a, b);
#define MOTOR3_SET_COMPARE(a, b)	TIM_SetCompare4(a, b);
#define MOTOR4_SET_COMPARE(a, b)	TIM_SetCompare3(a, b);

#define MULT			TIM_PERIOD / 100
