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
int ler_potenciometro();



int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	SysTick_Wait1ms(1000);
	int poten;
	int estado =0;
	char comando;
	int controlador;
	int sentido;
	int velocidade;
	while (1)
	{
		switch(estado){
			case 0:
				//transmite_string("digite '*' para comecar")
				while(comando != '*'){
					comando= escutar();
				}
				estado =1;
				break;
				
			case 1:
				//transmite_string("'p' para potenciomentro 't' para terminal")
				while(comando != 'p' && comando!='t'){
					comando= escutar();
				}
				if(comando =='p'){
					estado=2;
				}
				else{
					estado=3;
				}
				break;
			
			case 2:		
				SysTick_Wait1ms(10);
				poten=ler_potenciometro();
				if(poten==2048){
					sentido=0;
					velocidade=0;
				}
				else if(poten>2048){
					sentido=0;
					velocidade=(poten-2048)*100/2047;
				}
				else{
					sentido=1;
					velocidade=(2048-poten)*100/2048;
				}
				comando = escutar();
				if(comando=='s'){
					estado=1;
				}
				break;
			case 3:
				//transmite_string("'h' para horario 'a' para anti-horario")
				while(comando != 'h' && comando!='a'){
				 comando= escutar();
				}
				if(comando=='h'){
					sentido=0;
				}
				else{
					sentido=1;
				}
				//transmite_string("digite a velocidade de 5 a 0")
				while((comando < '5' || comando>'9') && comando!='0'){
					comando = escutar();
				}
				if (comando=='0'){
					velocidade =100; 
				}
				else{
					velocidade = (comando-'0')*10;
				}
				estado=4;
				break;
			case 4:
				comando =escutar();
				if(comando != 'h' && comando!='a' && (comando < '5' || comando>'9') && comando!='0'){
					if(comando=='h'){
						sentido=0;
					}
					else if (comando=='a'){
						sentido=1;
					}
					else if (comando=='0'){
						velocidade =100; 
					}
					else{
						velocidade = (comando-'0')*10;
					}
				}
			 if(comando=='s'){
					estado=1;
			 }
		}
		

	}
}


