#pragma once
#include "stm32f10x_lib.h"

void drive_forwards(unsigned int speed);
void drive_backwards(unsigned int speed);
void steer_left();
void steer_right();
void steer_straight();