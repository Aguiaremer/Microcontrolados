; main.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 15/03/2018
; Este programa espera o usuário apertar a chave USR_SW1 e/ou a chave USR_SW2.
; Caso o usuário pressione a chave USR_SW1, acenderá o LED2. Caso o usuário pressione 
; a chave USR_SW2, acenderá o LED1. Caso as duas chaves sejam pressionadas, os dois 
; LEDs acendem.

; -------------------------------------------------------------------------------
        THUMB                        ; Instruções do tipo Thumb-2
; -------------------------------------------------------------------------------

; -------------------------------------------------------------------------------
; Área de Dados - Declarações de variáveis
		AREA  DATA, ALIGN=2
		; Se alguma variável for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a variável <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma variável de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posição da RAM		

; -------------------------------------------------------------------------------
; Área de Código - Tudo abaixo da diretiva a seguir será armazenado na memória de 
;                  código
        AREA    |.text|, CODE, READONLY, ALIGN=2

		; Se alguma função do arquivo for chamada em outro arquivo	
        EXPORT Start                ; Permite chamar a função Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma função externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; função <func>
		IMPORT  PLL_Init
		IMPORT  SysTick_Init
		IMPORT  SysTick_Wait1ms										
		IMPORT  GPIO_Init
        IMPORT  PortN_Output
		IMPORT 	PortA_Output			
		IMPORT 	PortQ_Output			
		IMPORT 	PortB_Output			
		IMPORT 	PortP_Output			

; -------------------------------------------------------------------------------
; Função main()
Start  		
	BL PLL_Init                 ;Chama a subrotina para alterar o clock do microcontrolador para 80MHz
	BL SysTick_Init             ;Chama a subrotina para inicializar o SysTick
	BL GPIO_Init                ;Chama a subrotina que inicializa os GPIO
	BL Tabela_Display_7seg
	
	LDR R10, =15 				;Temperatura atual inicial
	LDR R11, =25				;Temperatura alvo inicial
	LDR R4,	 =10				;divisor
	LDR R6,	=167				;contador de um segundo
MainLoop

    UDIV R5, R10, R4      		;Dezenas da temperatura atual R5 = dezenas (R10 / R4)
	LDRB R0, [R12,R5]			;Carrega as configurações do pino para o display de 7 segmentos de acordo com o numero da tabela na memoria RAM
	BL PortA_Output				;Chama a função para habilitar o pinos PA7 a PA4
	BL PortQ_Output				;Chama a função para habilitar o pinos PQ3 a PQ0
	
	LDR R0,=2_00010000			;Seta R0 como parametro de entrada de função para habilitar transistor Q2
	BL PortB_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
	LDR R0,=2_00000000			;Seta R0 como parametro de entrada de função para desabilitar transistor Q2
	BL PortB_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
    MLS R5, R5, R4, R10  		;Unidades da temepratura atual R5 = unidades (R10 - (R5 * R4))
	LDRB R0, [R12,R5]			;Carrega as configurações do pino para o display de 7 segmentos de acordo com o numero da tabela na memoria RAM
	BL PortA_Output				;Chama a função para habilitar o pinos PA7 a PA4
	BL PortQ_Output				;Chama a função para habilitar o pinos PQ3 a PQ0
	
	LDR R0,=2_00100000			;Seta R0 como parametro de entrada de função para habilitar transistor Q1
	BL PortB_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
	LDR R0,=2_00000000			;Seta R0 como parametro de entrada de função para desabilitar transistor Q1
	BL PortB_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
	MOV R0, R11
	BL PortA_Output				;Chama a função para habilitar o pinos PA7 a PA4
	BL PortQ_Output				;Chama a função para habilitar o pinos PQ3 a PQ0
	
	LDR R0,=2_00100000			;Seta R0 como parametro de entrada de função para habilitar transistor Q1
	BL PortP_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
	LDR R0,=2_00000000			;Seta R0 como parametro de entrada de função para desabilitar transistor Q1
	BL PortP_Output				;Chama a função da porta B 
	LDR R0,=1					;Seta R0 como parametro de entrada de função para esperar 1 ms
	BL SysTick_Wait1ms			;Chama a função de espera
	
	SUB R6,R6,#1
	CMP R6,#0
	BLS Passou_1s
	
	B MainLoop
	
; -------------------------------------------------------------------------------
; Função Passou_1s
Passou_1s
	LDR R6,=167
	CMP	R10,R11
	BLT	T_atual_menor_que
	BHI T_atual_maior_que
	BEQ T_atual_igual
T_atual_menor_que
	ADD R10,R10,#1
	LDR R0, =2_01
	BL PortN_Output
	B MainLoop
T_atual_maior_que
	SUB R10,R10,#1
	LDR R0, =2_10
	BL PortN_Output
	B MainLoop
T_atual_igual
	LDR R0, =2_11
	BL PortN_Output
	B MainLoop
	
; -------------------------------------------------------------------------------
; Função Tabela_Display_7seg
; Parâmetro de entrada: R5 --> numero
; Parâmetro de saída: R0 -> saida dos pinos
Tabela_Display_7seg
	LDR  R12, =0x20000000  ; base da tabela
    MOV  R1, #2_00111111   ; valor p/ dígito 0
    STRB R1, [R12,#0]       ; guarda na posição 0
	
	MOV  R1, #2_00000110   ; valor p/ dígito 1
    STRB R1, [R12,#1]
	
	MOV  R1, #2_01011011   ; valor p/ dígito 2
    STRB R1, [R12,#2]
	
	MOV  R1, #2_01001111   ; valor p/ dígito 3
    STRB R1, [R12,#3]
	
	MOV  R1, #2_01100110   ; valor p/ dígito 4
    STRB R1, [R12,#4]
	
	MOV  R1, #2_01101101   ; valor p/ dígito 5
    STRB R1, [R12,#5]
	
	MOV  R1, #2_01111101   ; valor p/ dígito 6
    STRB R1, [R12,#6]
	
	MOV  R1, #2_00000111   ; valor p/ dígito 7
    STRB R1, [R12,#7]
	
	MOV  R1, #2_01111111   ; valor p/ dígito 8
    STRB R1, [R12,#8]
	
	MOV  R1, #2_01101111   ; valor p/ dígito 9
    STRB R1, [R12,#9]
	
	BX LR






    ALIGN                        ;Garante que o fim da seção está alinhada 
    END                          ;Fim do arquivo
