; Exemplo.s
; Desenvolvido para a placa EK-TM4C1294XL
; Prof. Guilherme Peron
; 12/03/2018

; -------------------------------------------------------------------------------
        THUMB                        ; Instru��es do tipo Thumb-2
; -------------------------------------------------------------------------------
; Declara��es EQU - Defines
;<NOME>         EQU <VALOR>
; -------------------------------------------------------------------------------
; �rea de Dados - Declara��es de vari�veis
		AREA  DATA, ALIGN=2
		; Se alguma vari�vel for chamada em outro arquivo
		;EXPORT  <var> [DATA,SIZE=<tam>]   ; Permite chamar a vari�vel <var> a 
		                                   ; partir de outro arquivo
;<var>	SPACE <tam>                        ; Declara uma vari�vel de nome <var>
                                           ; de <tam> bytes a partir da primeira 
                                           ; posi��o da RAM		

; -------------------------------------------------------------------------------
; �rea de C�digo - Tudo abaixo da diretiva a seguir ser� armazenado na mem�ria de 
;                  c�digo
        AREA    |.text|, CODE, READONLY, ALIGN=2

		; Se alguma fun��o do arquivo for chamada em outro arquivo	
        EXPORT Start                ; Permite chamar a fun��o Start a partir de 
			                        ; outro arquivo. No caso startup.s
									
		; Se chamar alguma fun��o externa	
        ;IMPORT <func>              ; Permite chamar dentro deste arquivo uma 
									; fun��o <func>

; -------------------------------------------------------------------------------
; Fun��o main()
vetor1 EQU 0x20000400
vetor2 EQU 0x20000600
Start  
; Comece o c�digo aqui <======================================================
BubbleSort2
	ldr r0, =vetor1
	ldr r10, =0
	ldr r11, =0
BubbleSort1
	ldrh r1, [r0]
	ldrh r2, [r0,#2]
	cmp r1,r2 
	ittt hi
	strhhi r2, [r0]
	strhhi r1, [r0,#2]
	ldrhi r11, =1
	add r10, #1
	add r0, #2
	cmp r10, #28
	bls BubbleSort1
	cmp r11, #0
	bne BubbleSort2
	
;============================================================

    ldr   r0, =vetor1      
    ldr   r1, =0           ; �ndice externo
    ldrh  r12, [r0]        
	
loop_externo
    mov   r2, r1          ; zera �ndice interno
    mov   r10, r0          ; r10 = ponteiro tempor�rio pro loop interno
    ldrh  r11, [r10,#2]       ; pega o valor atual pro interno

loop_interno
    udiv  r3, r11, r12
    mls   r4, r3, r12, r11
    cmp   r4, #0           ; testa se � m�ltiplo exato
    beq   func1            ; achou algo v�lido
exit_func1

    add   r2, r2, #1
    add   r10, r10, #2     ; avan�a no vetor interno
    ldrh  r11, [r10]
    cmp   r2, #29
    blt   loop_interno

    ; terminou o loop interno, avan�a externo
    add   r1, r1, #1
    add   r0, r0, #2 ; n�o precisa verificar oque j� verificou
    ldrh  r12, [r0]
    cmp   r1, #29
    blt   loop_externo
	b exit
	
;==================================================================
func1
	ldr r5,=2 ; os 2 primeiros termos da termo da pg
	push {r12}; coloca o primeiro termo na pilha
	push {r11}; coloca o segundo termo na pilha

	mov r7, r10
	mov r8, r2
	ldrh r9, [r7]
	mul r6,r9,r3
loop1_func1
	add r8, r8, #1
	add r7, r7, #2
	ldrh r9, [r7]
	
	cmp r6,r9
	ittt eq
	addeq r5,r5,#1 ; achou mais um termo
	pusheq {r9}; coloca o termo na pilha
	muleq r6,r9,r3; o proximo termo para buscar
	
	cmp r8,#29
	blt loop1_func1
	
	cmp r5, #5
	bhs copy_vetor

limpa_pilha
	cmp r5, #0
	itt gt
	popgt {r6}
	subgt r5, r5, #1
	bgt limpa_pilha
	
	b exit_func1
	
;==================================================================

copy_vetor
	ldr r0, =vetor2
	ldr r1, =2
	mul r6,r5,r1
	add r0, r0, r6
loop_copy_vetor
	sub r5, r5, #1
	sub r0,r0,#2
	pop {r7} 
	strh r7, [r0]
	cmp r5, #0
	bne loop_copy_vetor

exit
    ALIGN                           ; garante que o fim da se��o est� alinhada 
    END                             ; fim do arquivo
