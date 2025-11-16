// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>
#include "main.h"



volatile int usr_sw1_event = 0;

uint8_t sentido=0;
uint8_t velocidade=0;

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	int estado =0;
	char digitado[4]="****";
	char senha[4];
	int head=0;
	char tecla;
	int valido;
	int correto;
	int tentativa=0;
	escreve_lcd_string("Cofre aberto");
	int teste=0;
	char empr[4];
	char senha_mestra[5]="1234";
	while (1)
	{
		if(usr_sw1_event){
			usr_sw1_event = 0;
			estado=3;
			limpa_lcd();
			escreve_lcd_string("chave mestra:");
		}
		switch(estado){
			case 0:
				//cofre aberto
				tecla=varrer_teclado();
				while(varrer_teclado()!='F'){
					teste=0;
				}
				if(tecla=='A' || tecla=='B' || tecla=='C' || tecla=='D' || tecla=='F'){}
				else if(tecla=='#') {
					valido=1;
					for(int i=0; i<4; i++){
						if (digitado[i]=='*'){
							valido = 0;
						}
					}
					if(valido){
						for (int i=0; i<=3; i++){
							senha[i]=digitado[(head+i)%4];
						}
						SysTick_Wait1ms(5000);
						stepper_close();
						tentativa=0;
						limpa_lcd();
						estado=1;
						escreve_lcd_string("Cofre fechado");
					}	
					else{
						limpa_lcd();
						escreve_lcd_string("erro");
						SysTick_Wait1ms(1000);
						limpa_lcd();
						escreve_lcd_string("Cofre aberto");
					}
					for(int i=0; i<4; i++){
							digitado[i]='*';
							head=0;
					}
				}
				else{
					digitado[head]= tecla;
					head=(head+1)%4;
					SysTick_Wait1ms(1000);
				}
				break;
			case 1:
				//cofre fechado
				tecla=varrer_teclado();
				while(varrer_teclado()!='F'){
				}
				if(tecla=='A' || tecla=='B' || tecla=='C' || tecla=='D' || tecla=='F'){}
				else if(tecla=='#') {
					valido=1;
					for(int i=0; i<4; i++){
						if (digitado[i]=='*'){
							valido = 0;
						}
					}
					if(valido){
						correto=1;
						for (int i=0; i<=3; i++){
							if (senha[i]!=digitado[head+i]){
								correto=0;
							}		
						}
						if(correto){
							limpa_lcd();
							escreve_lcd_string("Cofre abrindo");
							stepper_open();
							estado=0;
							SysTick_Wait1ms(1000);
							limpa_lcd();
							escreve_lcd_string("Cofre aberto");
						}
						else{
							limpa_lcd();
							escreve_lcd_string("incorreto");
							tentativa++;
							if(tentativa>=3){
								estado=2;
								limpa_lcd();
								escreve_lcd_string("Cofre Travado");
							}
							else{
								SysTick_Wait1ms(1000);
								limpa_lcd();
								escreve_lcd_string("Cofre fechado");
							}
						}
						for(int i=0; i<4; i++){
							digitado[i]='*';
							head=0;
						}
					}	
					else{
						limpa_lcd();
						escreve_lcd_string("erro");
						SysTick_Wait1ms(1000);
						limpa_lcd();
						escreve_lcd_string("Cofre fechado");
					}
				}
				else{
					digitado[head]= tecla;
					head=(head+1)%4;
					
				}
				break;
			case 2:
					pisca_led();
			case 3:
					tecla=varrer_teclado();
				while(varrer_teclado()!='F'){
				}
				if(tecla=='A' || tecla=='B' || tecla=='C' || tecla=='D' || tecla=='F'){}
				else if(tecla=='#') {
					valido=1;
					for(int i=0; i<4; i++){
						if (digitado[i]=='*'){
							valido = 0;
						}
					}
					if(valido){
						correto=1;
						for (int i=0; i<=3; i++){
							if (senha_mestra[i]!=digitado[head+i]){
								correto=0;
							}		
						}
						if(correto){
							limpa_lcd();
							escreve_lcd_string("Cofre abrindo");
							stepper_open();
							estado=0;
							SysTick_Wait1ms(1000);
							limpa_lcd();
							escreve_lcd_string("Cofre aberto");
						}
						else{
							limpa_lcd();
							escreve_lcd_string("incorreto");
							SysTick_Wait1ms(1000);
							limpa_lcd();
							escreve_lcd_string("chave mestra:");
						}
						for(int i=0; i<4; i++){
							digitado[i]='*';
							head=0;
						}
					}	
					else{
						limpa_lcd();
						escreve_lcd_string("erro");
						SysTick_Wait1ms(1000);
						limpa_lcd();
						escreve_lcd_string("chave mestra:");
					}
				}
				else{
					digitado[head]= tecla;
					head=(head+1)%4;
				}
		}		
	}
}

void stepper_close(void) {
    // aguarda 1s antes de girar (conforme enunciado)
    SysTick_Wait1ms(1000);
    velocidade = 1;
    sentido = 1;
    stepper_move();
    SysTick_Wait1ms(1000);
}

void stepper_open(void) {
    // aguarda 1s antes de girar (conforme enunciado)
    SysTick_Wait1ms(1000);
    velocidade = 2;
    sentido = 0;
    stepper_move();
    SysTick_Wait1ms(1000);
}

