/************************************************
 *	Robot3 - main.c
 *	The main code.
 *	See README for more details.
 ***********************************************/

// The main STM32 library
#include "stm32f10x_lib.h"

#include "drive.h"

//Function prototypes
void Clock_Config();
void GPIO_Config();
void NVIC_Config();
void TIM_Config();
void ADC_Config();
void USART_Config();

int main();

void Delay( unsigned long delay );
void USART_Send(char* str);

// Variables for initialising peripherals
// Stores configuration options that are then applied
// to a specific peripheral.
GPIO_InitTypeDef	    GPIO_InitStructure;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
NVIC_InitTypeDef        NVIC_InitStructure;
ADC_InitTypeDef         ADC_InitStructure;
USART_InitTypeDef       USART_InitStructure;

//Hold clock startup error status
ErrorStatus HSEStartUpStatus;

int main() {

	Clock_Config();
	GPIO_Config();
    NVIC_Config();
    TIM_Config();
    ADC_Config();
    USART_Config();
    
    Delay(0xFFFFF);
    
    USART_Send("\r\n\r\n\r\n");
    USART_Send(" Robot3 initialised.\r\n");
    USART_Send("======CONTROLS========\r\n");
    USART_Send("| W  DRIVE FORWARDS  |\r\n");
    USART_Send("| S  DRIVE BACKWARDS |\r\n");
    USART_Send("|        ---         |\r\n");
    USART_Send("| A  STEER LEFT      |\r\n");
    USART_Send("| D  STEER RIGHT     |\r\n");
    USART_Send("======================\r\n");
    
	// Main loop
	for(;;);
    
}


void Delay( unsigned long delay ) {
	for(; delay; --delay );
}

void USART_Send(char* str) {
    unsigned int i;
    for(i=0; str[i] != 0x00; i++) {
        USART_SendData(USART1, str[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

void GPIO_Config() {

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void NVIC_Config() {
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    #ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif
}

void TIM_Config() {
    
    // TIM1: Generate PWM signals to the motor controller
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 4095;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_Cmd(TIM1, ENABLE);
    
    // TIM2: Generate timed interrupt events, clocked at 1kHz
    TIM_TimeBaseStructure.TIM_Period = 1024;
    TIM_TimeBaseStructure.TIM_Prescaler = 35999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 192;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
}

void ADC_Config() {
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_71Cycles5);
    
    ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x8CCC, 0x0000);
    ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_4);
    ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
    
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void USART_Config() {
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init( USART1, &USART_InitStructure );
    
    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void Clock_Config() {

	// Reset RCC
	RCC_DeInit();

	// Enable HSE (High-Speed External oscillator)
	RCC_HSEConfig( RCC_HSE_ON );

	// Wait for the HSE to be ready
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if( HSEStartUpStatus == SUCCESS ) {
		// Enable Prefetch Buffer
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		// Flash 2 wait state
		FLASH_SetLatency(FLASH_Latency_2);

		// HCLK = SYSCLK
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		// PCLK2 = HCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);

		// PCLK1 = HCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);

		// PLLCLK = 8MHz * 9 = 72 MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		// Enable PLL
		RCC_PLLCmd(ENABLE);

		// Wait till PLL is ready
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}

		// Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// Wait till PLL is used as system clock source
		while(RCC_GetSYSCLKSource() != 0x08) {}
	}
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    
	// Initialise clock to peripherals
    
    RCC_APB1PeriphClockCmd(
        RCC_APB1Periph_TIM2
        , ENABLE);
    
	RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
		RCC_APB2Periph_GPIOB |
        RCC_APB2Periph_TIM1  |
        RCC_APB2Periph_ADC1  |
        RCC_APB2Periph_USART1
		, ENABLE);

}
