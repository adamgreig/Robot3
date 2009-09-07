/************************************************
 *	FollowingRobot - main.c
 *	The main code.
 *	See README for more details.
 ***********************************************/

// The main STM32 library
#include "stm32f10x_lib.h"

//Function prototypes
void Clock_Config();	// Starts the HSE, clocks the system and enables peripheral clocks
void GPIO_Config();	// Configures all the input/output pins

int main();		// Code entry point

// Wait a set number of iterations, used as a very rough delay (busy wait loop)
void Delay( unsigned long delay );

// Variables for initialising peripherals
// Stores configuration options that are then applied
// to a specific peripheral.
GPIO_InitTypeDef	GPIO_InitStructure;
USART_InitTypeDef	USART_InitStructure;
I2C_InitTypeDef		I2C_InitStructure;
SPI_InitTypeDef		SPI_InitStructure;
DMA_InitTypeDef		DMA_InitStructure;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
TIM_OCInitTypeDef	TIM_OCInitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;
EXTI_InitTypeDef	EXTI_InitStructure_HD;
EXTI_InitTypeDef	EXTI_InitStructure_VD;

//Hold clock startup error status
ErrorStatus HSEStartUpStatus;

int main() {

	// Configure clock
	Clock_Config();

	// Configure peripherals
	GPIO_Config();
    
    #ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif

	// Main loop
	for(;;) {
        
        //Set everything off
        GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
        
        //Turn left
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        Delay(0xFFFFF);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        Delay(0xAFFFF);
        
        //Turn right
        GPIO_SetBits(GPIOB, GPIO_Pin_13);
        Delay(0xFFFFF);
        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        Delay(0xAFFFF);
        
        //Go forward
        GPIO_SetBits(GPIOB, GPIO_Pin_15);
        Delay(0xAFFFF);
        GPIO_ResetBits(GPIOB, GPIO_Pin_15);
        Delay(0xAFFFF);
        
        //Go backward
        GPIO_SetBits(GPIOB, GPIO_Pin_14);
        Delay(0xAFFFF);
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
        Delay(0xAFFFF);
        
	}

}


void Delay( unsigned long delay ) {
	for(; delay; --delay );
}

void GPIO_Config() {

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void EXTI15_10_IRQHandler() {
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
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		// Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// Wait till PLL is used as system clock source
		while(RCC_GetSYSCLKSource() != 0x08) {}
	}

	// Initialise clock to peripherals
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_GPIOB
		, ENABLE );

}
