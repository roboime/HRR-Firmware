/*
 * dynamixel.hpp
 *
 *  Created on: Mar 8, 2020
 *      Author: Gabriel
 */

#ifndef INC_DYNAMIXEL_H_
#define INC_DYNAMIXEL_H_

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

int transmit (uint8_t id, uint8_t instruction, uint8_t numParam, uint8_t* param);
int receive (uint8_t id, uint8_t instruction, uint8_t first, uint8_t size, uint8_t* buffer);

#endif /* INC_DYNAMIXEL_H_ */
