// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include "tm4c1294ncpdt.h"
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
void PortN_Output(uint32_t leds);
void transmitir(char dado);
char escutar();


int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	SysTick_Wait1ms(1000);
	char palavra;
	transmitir('o');
	transmitir('l');
	transmitir('a');
	transmitir(' ');
	transmitir('m');
	transmitir('u');
	transmitir('n');
	transmitir('d');
	transmitir('o');
	
	while (1)
	{
	}
}


