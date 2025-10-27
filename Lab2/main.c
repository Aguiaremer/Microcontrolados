// main.c
// Desenvolvido para a placa EK-TM4C1294XL
// Verifica o estado das chaves USR_SW1 e USR_SW2, acende os LEDs 1 e 2 caso estejam pressionadas independentemente
// Caso as duas chaves estejam pressionadas ao mesmo tempo pisca os LEDs alternadamente a cada 500ms.
// Prof. Guilherme Peron

#include <stdint.h>

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortN_Output(uint32_t leds);
void Pisca_leds(void);
void escreve_lcd(char);
void escreve_lcd_string(const char *string);
void limpa_lcd();
char varrer_teclado();

int main(void)
{
	PLL_Init();
	SysTick_Init();
	GPIO_Init();
	int estado =0;
	char digitado[5]="****";
	char senha[5];
	int head=0;
	char tecla;
	int valido;
	int correto;
	int tentativa=0;
	while (1)
	{
		switch(estado){
			case 0:
				//cofre aberto
				escreve_lcd_string("Cofre aberto");
				tecla=varrer_teclado();
				if(tecla=='A' || tecla=='B' || tecla=='C' || tecla=='D' || tecla=='F'){}
				else if(tecla=='#') {
					valido=1;
					for(int i=0; i<4; i++){
						if (digitado[i]=='*'){
							valido = 0;
						}
					}
					if(valido){
						for (int i=1; i>=4; i--){
							senha[i]=digitado[(head+i)%4];
						}
						//roda motor
						tentativa=0;
						estado=1;
					}	
					else{
						escreve_lcd_string("erro");
						SysTick_Wait1us(100);
					}
				}
				else{
					digitado[head]= tecla;
					head=(head+1)%4;
				}
				break;
			case 1:
				//cofre fechado
				escreve_lcd_string("Cofre fechado");
				tecla=varrer_teclado();
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
						for (int i=1; i>=4; i--){
							if (senha[i-1]!=digitado[head+i]){
								correto=0;
							}		
						}
						if(correto){
							escreve_lcd_string("Cofre abrindo");
							//roda motor
							estado=0;
						}
						else{
							escreve_lcd_string("incorreto");
							tentativa++;
							if(tentativa>=3){
								estado=2;
							}
						}
					}	
					else{
						escreve_lcd_string("erro");
						SysTick_Wait1us(100);
					}
				}
				else{
					digitado[head]= tecla;
					head=(head+1)%4;
				}
				break;
			case 2:
					escreve_lcd_string("Cofre Travado");
					//pisca leds da pat
		}		
	}
}

void Pisca_leds(void)
{
	PortN_Output(0x2);
	SysTick_Wait1ms(250);
	PortN_Output(0x1);
	SysTick_Wait1ms(250);
}

