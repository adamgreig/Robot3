#include "drive.h"

void drive_forwards(unsigned int speed) {
    
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = speed;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
    
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
}

void drive_backwards(unsigned int speed) {
    
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = speed;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
    
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
}

void stop() {
    
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
        
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 4095;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
    
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    
}

void steer_left() {
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    
}

void steer_right() {
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    
}

void steer_straight() {
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
    
}