// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include "main.h"

int estado=0;
int sentido;

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	inicializa_timer();
	SysTick_Wait1ms(1000);
	int poten;
	char comando;
	int controlador;
	transmitir_string("\r\ndigite '*' para comecar\r\n");
	while (1)
	{
		switch(estado){
			case 0:
				while(comando != '*'){
					comando= escutar();
				}
				estado =1;
				transmitir_string("\r\ndigite 'p' para controlar o motor pelo potenciomentro ou 't' pelo terminal\r\n");
				break;
				
			case 1:
				while(comando != 'p' && comando!='t'){
					comando= escutar();
				}
				if(comando =='p'){
					estado=2;
					transmitir_string("\r\ncontrole pelo potenciometro!\r\n");
				}
				else{
					estado=3;
					transmitir_string("\r\ndigite 'h' para rodar no sentido horario 'a' para anti-horario\r\n");
				}
				
				break;
			
			case 2:		
				SysTick_Wait1ms(10);
				poten=ler_potenciometro();
				if(poten==2048){
					sentido=0;
					velocidade_alvo=0;
				}
				else if(poten>2048){
					sentido=0;
					velocidade_alvo=(poten-2048)*100/2047;
				}
				else{
					sentido=1;
					velocidade_alvo=(2048-poten)*100/2048;
				}

				comando = escutar();
				if(comando=='s'){
					estado=1;
					transmitir_string("\r\nTchau\r\n");
					transmitir_string("\r\n");
					transmitir_string("\r\ndigite 'p' para controlar o motor pelo potenciomentro ou 't' pelo terminal\r\n");
				}
				break;
			case 3:
				while(comando != 'h' && comando!='a'){
				 comando= escutar();
				}
				if(comando=='h'){
					sentido=0;
				}
				else{
					sentido=1;
				}
				transmitir_string("\r\ndigite a velocidade de 5 a 9 e zero para a velocidade maxima\r\n");
				while(!((comando >= '5' && comando <= '9') || comando == '0')) {
						comando = escutar();
				}
				if (comando=='0'){
					velocidade_alvo =100; 
				}
				else{
					velocidade_alvo = (comando-'0')*10;
				}
				estado=4;
				transmitir_string("\r\nMotor inicializado\r\n");
				break;
			case 4:
				comando =escutar();
				if(comando == 'h' || comando =='a'){
					if(comando=='h'){
						sentido=0;
					}
					else if (comando=='a'){
						sentido=1;
					}
					transmitir_string("\r\nSentido atualizado\r\n");
				}
				if((comando >= '5' && comando <= '9') || comando == '0'){
					if (comando=='0'){
						velocidade_alvo =100; 
					}
					else{
						velocidade_alvo = (comando-'0')*10;
					}
					transmitir_string("\r\nVelocidade atualizada\r\n");
				}
				if(comando=='s'){
					estado=1;
					transmitir_string("\r\nTchau\r\n");
					transmitir_string("\r\n");
					transmitir_string("\r\ndigite 'p' para controlar o motor pelo potenciomentro ou 't' pelo terminal\r\n");
				}
		}
		

	}
}
