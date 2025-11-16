// gpio.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa as portas J e N
// Prof. Guilherme Peron


#include <stdint.h>

#include "tm4c1294ncpdt.h"

  
#define GPIO_PORTA  (0x0001) //bit 0
#define GPIO_PORTN  (0x1000) //bit 12
#define GPIO_PORTE  (0x0010)
// -------------------------------------------------------------------------------
// Função GPIO_Init
// Inicializa os ports J e N
// Parâmetro de entrada: Não tem
// Parâmetro de saída: Não tem
void Timer2A_Handler();

int velocidade_alvo = 0;
int velocidade = 0;
int on_off = 0;
int timer_pwm = 0;
int timer_acelerador= 500;
int timer_velocidade=100000;

void GPIO_Init(void)
{
	SYSCTL_RCGCUART_R =0x01;
	 
	while(SYSCTL_PRUART_R != 0x01){
	}
	
	UART0_CTL_R =  !UART_CTL_UARTEN & UART0_CTL_R;
	

	UART0_IBRD_R =520;
	UART0_FBRD_R =33;
	
	UART0_LCRH_R  = UART0_LCRH_R  | UART_LCRH_WLEN_8;
	UART0_LCRH_R = UART0_LCRH_R | UART_LCRH_FEN;
	UART0_LCRH_R = UART0_LCRH_R | UART_LCRH_STP2;
	UART0_LCRH_R = UART0_LCRH_R | UART_LCRH_PEN;
	
	UART0_CC_R = 0x00;
	
	UART0_CTL_R = UART0_CTL_R | UART_CTL_RXE;
	UART0_CTL_R = UART0_CTL_R | UART_CTL_TXE;
	//UART0_CTL_R =UART0_CTL_R | UART_CTL_HSE;
	UART0_CTL_R = UART0_CTL_R | UART_CTL_UARTEN;
	
	
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R = (GPIO_PORTA | GPIO_PORTN | GPIO_PORTE);
	
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_PORTA | GPIO_PORTN | GPIO_PORTE) ) != (GPIO_PORTA | GPIO_PORTN | GPIO_PORTE) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica

	GPIO_PORTN_AMSEL_R = 0x00;
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTE_AHB_AMSEL_R =0x10;
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTN_PCTL_R = 0x00;
	GPIO_PORTA_AHB_PCTL_R =0x11;
	GPIO_PORTE_AHB_PCTL_R =0x10;
	// 4. DIR para 0 se for entrada, 1 se for saída
	GPIO_PORTN_DIR_R = 0x0f; //BIT0 | BIT1
	GPIO_PORTE_AHB_DIR_R =0x00;
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTN_AFSEL_R = 0x00; 
	GPIO_PORTA_AHB_AFSEL_R=0x03;
	GPIO_PORTE_AHB_AFSEL_R=0x10;
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTN_DEN_R = 0x0f; 		   //Bit0 e bit1
	GPIO_PORTA_AHB_DEN_R =0X03;
	GPIO_PORTE_AHB_DEN_R =0X00;
	// 7. Habilitar resistor de pull-up interno, setar PUR para 1

	SYSCTL_RCGCADC_R = 0x01;
	while((SYSCTL_PRADC_R & 0x01) != 0x01);
	
	ADC0_PC_R =0x7;
	ADC0_SSPRI_R =(ADC0_SSPRI_R & !0x3333) | 0x0123;//00 0001 0010 0011=0x0123
	ADC0_ACTSS_R =ADC0_ACTSS_R & !ADC_ACTSS_ASEN3;
  ADC0_EMUX_R=ADC0_EMUX_R | ADC_EMUX_EM3_M;	
	ADC0_SSMUX3_R=0x9;
	ADC0_SSCTL3_R=0x6;
	ADC0_ACTSS_R =ADC0_ACTSS_R | ADC_ACTSS_ASEN3;
}	
void inicializa_timer(){
	SYSCTL_RCGCTIMER_R = 0x4;
	
	while(SYSCTL_PRTIMER_R != 0x4){
		
	}
	
	TIMER2_CTL_R = 0x00;
	
	TIMER2_CFG_R = 0x00;
	
	TIMER2_TAMR_R =0x00;
	
	TIMER2_TAILR_R = 800;
	
	TIMER2_TAPR_R =0;
	
	TIMER2_ICR_R=1;
	
	TIMER2_IMR_R=1;
	
	NVIC_PRI5_R = 4 << 29;
	
	NVIC_EN0_R = 1 << 23;
	
	TIMER2_CTL_R=1;
}
char escutar(){

	return UART0_DR_R & 0xFF;
}

void transmitir(char dado){
	while(UART0_FR_R & UART_FR_TXFF);
	UART0_DR_R = dado;
}
void transmitir_string(const char *string) {
  while (*string != '\0') {
    transmitir(*string);
    string++;
  }
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

int ler_potenciometro(){
		int poten;
		ADC0_PSSI_R=0x08;
		while((ADC0_RIS_R & 0x8) != 0x8);
		poten=ADC0_SSFIFO3_R;
		ADC0_ISC_R  =0x0;
		return poten;
}
void altera_velocidade(int sentido, int velo){
	if(sentido==0){
		velocidade_alvo= velo;
	}
	else if(sentido == 1){
		velocidade_alvo= velo* -1;
	}
}

void Timer2A_Handler(){
	TIMER2_ICR_R = 0x01;
	char* texto="velocidade: 00"
	if(velocidade!= velocidade_alvo){
		timer_acelerador--;
		if(timer_acelerador<=0){
			timer_acelerador=500;
			if(velocidade>velocidade_alvo){
				velocidade--;
			}
			else{
				velocidade++;
			}
		}
	}
	timer_pwm++;
	if(on_off==1){
		if(timer_pwm>=velocidade){
			on_off=0;
			//motor(desligado)
		}
	}
	else{
		if(timer_pwm>=100-velocidade){
			on_off=1;
			//motor(ligado)
		}
	}
	
	timer_velocidade--;
	if(timer_velocidade<=0){
		timer_velocidade=100000;
		texto[12]='0'+(velocidade/10);
		texto[13]='0'+(velocidade%10);
		transmitir_string(texto);
	}
}




