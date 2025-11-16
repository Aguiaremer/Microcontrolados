#ifndef MAIN_H__
#define MAIN_H_

#include <stdint.h>
#include <string.h>



/* Flag setada pela ISR quando USR_SW1 é pressionada (falling edge) */
extern volatile int usr_sw1_event;

/* Protótipo da função de inicialização da interrupção */

int usr_sw1_pressed(void);

void pisca_led();

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);

uint32_t PortJ_Input(void);

void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void Pisca_leds(void);
void escreve_lcd(char);
void escreve_lcd_string(const char *string);
void limpa_lcd();
char varrer_teclado();
void move_cursor_lcd(uint8_t pos);

extern uint8_t sentido;
extern uint8_t velocidade;

void stepper_close(void);
void stepper_open(void);

void stepper_move(void);

void GPIOPortJ_Handler(void);


#endif // MAIN_H__