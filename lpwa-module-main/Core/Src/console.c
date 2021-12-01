/*
 * console.c
 *
 *  Created on: 2021/06/04
 *      Author: kazuaki
 */

#include <stdarg.h>
#include <stdio.h>

#include "main.h"
#include "console.h"
#include "stm32l0xx_hal.h"

void SerialOutput(char *argv, ...)
{
	va_list list;
	va_start(list, argv);

	char buffer[100];
	int size = vsprintf(buffer, argv, list);
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, size, 1000);

	va_end( list );
}


void LedOutput(enum LEDs led, int value)
{
	GPIO_TypeDef *GPIOx = GPIOB;  // default GPIOB
	int pin;
	switch(led){
	case LED_BLUE:
		pin = GPIO_PIN_6;
		break;
	case LED_RED:
		pin = GPIO_PIN_7;
		break;
	case LED_GREEN:
		pin = GPIO_PIN_5;
		break;
	case LED_AUX:
		pin = GPIO_PIN_5;
		GPIOx = GPIOA;
		break;
	default:
		return;
	}
    HAL_GPIO_WritePin(GPIOx, pin, value);
}



