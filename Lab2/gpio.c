// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

#define GPIO_PORTJ  (0x0100) //bit 8
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTK  (0x0200) //bit 9
#define GPIO_PORTM  (0x0800) //bit 11
#define GPIO_PORTL 	(0x0400)

void Timer2A_Handler();
void SysTick_Wait1ms(uint32_t delay);
void inicializa_lcd();
void inicializa_timer();

// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void GPIO_Init(void)
{
	// 1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTJ | GPIO_PORTN | GPIO_PORTK | GPIO_PORTM | GPIO_PORTL);
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & 	(GPIO_PORTJ | GPIO_PORTN | GPIO_PORTK | GPIO_PORTM | GPIO_PORTL) ) != 
														(GPIO_PORTJ | GPIO_PORTN | GPIO_PORTK | GPIO_PORTM | GPIO_PORTL) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTJ_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTK_AMSEL_R = 0x00;
	GPIO_PORTM_AMSEL_R = 0x00;
	GPIO_PORTL_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTJ_AHB_PCTL_R = 0x00;
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTK_PCTL_R = 0x00;
	GPIO_PORTM_PCTL_R = 0x00;
	GPIO_PORTL_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTJ_AHB_DIR_R = 0x00;
	GPIO_PORTN_DIR_R = 0x03; //BIT0 | BIT1
	GPIO_PORTK_DIR_R = 0xFF;
	GPIO_PORTM_DIR_R = 0x07;
	GPIO_PORTL_DIR_R = 0x00;
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTJ_AHB_AFSEL_R = 0x00;
	GPIO_PORTN_AFSEL_R = 0x00;
	GPIO_PORTK_AFSEL_R = 0x00;
	GPIO_PORTM_AFSEL_R = 0x00;
	GPIO_PORTL_AFSEL_R = 0x00;
		
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTJ_AHB_DEN_R = 0x03;   //Bit0 e bit1
	GPIO_PORTN_DEN_R = 0x03; 		   //Bit0 e bit1
	GPIO_PORTK_DEN_R = 0xFF;
	GPIO_PORTM_DEN_R = 0xF7;
	GPIO_PORTL_DEN_R = 0x0F;
	
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1
	GPIO_PORTJ_AHB_PUR_R = 0x03;   //Bit0 e bit1
	GPIO_PORTL_PUR_R = 0x0F;

	inicializa_timer();
	inicializa_lcd();
}	

void inicializa_timer(){
	SYSCTL_RCGCTIMER_R = 0x4;
	
	while(SYSCTL_PRTIMER_R != 0x4){
		
	}
	
	TIMER2_CTL_R = 0x00;
	
	TIMER2_CFG_R = 0x00;
	
	TIMER2_TAMR_R =0x00;
	
	TIMER2_TAILR_R = 55999999;
	
	TIMER2_TAPR_R =0;
	
	TIMER2_ICR_R=1;
	
	TIMER2_IMR_R=1;
	
	NVIC_PRI5_R = 4 << 29;
	
	NVIC_EN0_R = 1 << 23;
	
	TIMER2_CTL_R=1;
}

void set_LCD_RS(uint8_t bool) {
  uint32_t temp = GPIO_PORTM_DATA_R;
  temp = temp & 0xFFFFFFFE;
  if (bool) {
    temp = temp | 0x00000001;
  }
  GPIO_PORTM_DATA_R = temp;
}

void set_LCD_RW(uint8_t bool) {
  uint32_t temp = GPIO_PORTM_DATA_R;
  temp = temp & 0xFFFFFFFD;
  if (bool) {
    temp = temp | 0x00000002;
  }
  GPIO_PORTM_DATA_R = temp;
}

void set_LCD_Enable(uint8_t bool) {
  uint32_t temp = GPIO_PORTM_DATA_R;
  temp = temp & 0xFFFFFFFB;
  if (bool) {
    temp = temp | 0x00000004;
  }
  GPIO_PORTM_DATA_R = temp;
}

void escreve_lcd(char letra) {
  set_LCD_RS(1);
  GPIO_PORTK_DATA_R = letra;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);
  set_LCD_RS(0);
}

void escreve_lcd_string(const char *string) {
  while (*string != '\0') {
    escreve_lcd(*string);
    string++;
  }
}

void move_cursor_lcd(uint8_t pos) {
  GPIO_PORTK_DATA_R = pos;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);
}

void limpa_lcd() {
  GPIO_PORTK_DATA_R = 0x01;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);
}

void inicializa_lcd() {
  GPIO_PORTK_DATA_R = 0x38;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);

  GPIO_PORTK_DATA_R = 0x06;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);

  GPIO_PORTK_DATA_R = 0x0E;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);

  GPIO_PORTK_DATA_R = 0x01;
  set_LCD_Enable(1);
  SysTick_Wait1ms(2);
  set_LCD_Enable(0);
}

void encontrou() {
  GPIO_PORTM_DATA_R = GPIO_PORTM_DATA_R & 0xFFFFFF0F;
  GPIO_PORTM_DIR_R = 0x07;
}

char varrer_teclado() {
  GPIO_PORTM_DIR_R = 0x17;
  SysTick_Wait1ms(2);
  GPIO_PORTM_DATA_R = GPIO_PORTM_DATA_R & 0xFFFFFFEF;
  SysTick_Wait1ms(2);
  switch (GPIO_PORTL_DATA_R & 0xF) {
  case 0xE:
    encontrou();
    return '1';
    break;
  case 0xD:
    encontrou();
    return '4';
    break;
  case 0xB:
    encontrou();
    return '7';
    break;
  case 0x7:
    encontrou();
    return '*';
    break;
  }

  GPIO_PORTM_DIR_R = 0x27;
  SysTick_Wait1ms(2);
  GPIO_PORTM_DATA_R = GPIO_PORTM_DATA_R & 0xFFFFFFDF;
  SysTick_Wait1ms(2);
  switch (GPIO_PORTL_DATA_R & 0xF) {
  case 0xE:
    encontrou();
    return '2';
    break;
  case 0xD:
    encontrou();
    return '5';
    break;
  case 0xB:
    encontrou();
    return '8';
    break;
  case 0x7:
    encontrou();
    return '0';
    break;
  }

  GPIO_PORTM_DIR_R = 0x47;
  SysTick_Wait1ms(2);
  GPIO_PORTM_DATA_R = GPIO_PORTM_DATA_R & 0xFFFFFFBF;
  SysTick_Wait1ms(2);
  switch (GPIO_PORTL_DATA_R & 0xF) {
  case 0xE:
    encontrou();
    return '3';
    break;
  case 0xD:
    encontrou();
    return '6';
    break;
  case 0xB:
    encontrou();
    return '9';
    break;
  case 0x7:
    encontrou();
    return '#';
    break;
  }

  GPIO_PORTM_DIR_R = 0x87;
  SysTick_Wait1ms(2);
  GPIO_PORTM_DATA_R = GPIO_PORTM_DATA_R & 0xFFFFFF7F;
  SysTick_Wait1ms(2);
  switch (GPIO_PORTL_DATA_R & 0xF) {
  case 0xE:
    encontrou();
    return 'A';
    break;
  case 0xD:
    encontrou();
    return 'B';
    break;
  case 0xB:
    encontrou();
    return 'C';
    break;
  case 0x7:
    encontrou();
    return 'D';
    break;
  }
  encontrou();
  return 'F';
}

// -------------------------------------------------------------------------------
// Função PortJ_Input
// Lê os valores de entrada do port J
// Parâmetro de entrada: Não tem
// Parâmetro de saída: o valor da leitura do port
uint32_t PortJ_Input(void)
{
	return GPIO_PORTJ_AHB_DATA_R;
}

// -------------------------------------------------------------------------------
// Função PortN_Output
// Escreve os valores no port N
// Parâmetro de entrada: Valor a ser escrito
// Parâmetro de saída: não tem
void PortN_Output(uint32_t valor)
{
    uint32_t temp;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | valor;
    GPIO_PORTN_DATA_R = temp; 
}

void inverteled()
{
		uint32_t temp;
		uint32_t bit1;
    //vamos zerar somente os bits menos significativos
    //para uma escrita amigável nos bits 0 e 1
    temp = GPIO_PORTN_DATA_R & 0xFC;
		bit1=GPIO_PORTN_DATA_R & 1;
    //agora vamos fazer o OR com o valor recebido na função
    temp = temp | !bit1 ;
    GPIO_PORTN_DATA_R = temp; 
}

void Timer2A_Handler()
{
		TIMER2_ICR_R = 0x01;
		inverteled();
}



