#pragma once
#include "stm32f10x_lib.h"

extern TIM_OCInitTypeDef TIM_OCInitStructure;

void drive_forwards(unsigned int speed);
void drive_backwards(unsigned int speed);
void stop();
void steer_left();
void steer_right();
void steer_straight();