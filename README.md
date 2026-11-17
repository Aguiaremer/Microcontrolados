# Microcontrolados

Repositório de atividades da disciplina de Microcontrolados. Os projetos foram preparados para a placa **EK-TM4C1294XL**, com o microcontrolador **TM4C1294NCPDT**, e usam projetos do Keil µVision. Os laboratórios avançam de exercícios em Assembly para aplicações com GPIO, temporização, teclado, LCD, ADC, UART e controle de motores.

## Visão geral

| Pasta | Tema | O que o programa faz |
|---|---|---|
| [`Lab0`](Lab0/) | Vetores e Assembly | Ordena 30 valores de 16 bits e procura no vetor sequências de pelo menos cinco termos com razão inteira, copiando as encontradas para outra região da RAM. |
| [`Lab1`](Lab1/) | GPIO e display | Simula a aproximação de uma temperatura atual a uma temperatura-alvo, mostra os valores em displays de sete segmentos multiplexados e indica a relação entre eles pelos LEDs da placa. |
| [`Lab2`](Lab2/) | Cofre eletrônico | Usa teclado matricial, LCD, interrupção de botão e motor de passo para fechar/abrir um cofre por senha; inclui bloqueio após tentativas inválidas e acesso por senha mestra. |
| [`Lab3`](Lab3/) | Controle de motor | Permite escolher por UART entre definir sentido/velocidade pelo terminal ou ajustar o alvo com um potenciômetro lido pelo ADC. A rotina de timer calcula uma rampa de velocidade e um sinal PWM. |

## Lab0 — ordenação e busca em Assembly

O arquivo [`Lab0/exemplo.s`](Lab0/exemplo.s) contém o algoritmo principal. Ele primeiro aplica Bubble Sort ao vetor em `0x20000400` e, depois, percorre os valores procurando uma progressão geométrica de razão inteira. Quando encontra pelo menos cinco termos, copia a sequência para `0x20000600`.

Os dados de teste ficam em [`Lab0/teste1.ini`](Lab0/teste1.ini) e [`Lab0/vetor.ini`](Lab0/vetor.ini); a configuração de depuração do projeto aponta para `teste1.ini`. O código usa operações de divisão e resto para validar a razão, compara os termos seguintes e usa a pilha para guardar os candidatos antes de copiá-los.

**Ponto para conferir:** há um caminho de busca que pode ler uma posição além do fim do vetor quando o índice externo chega ao último par. O algoritmo também pressupõe divisor diferente de zero. Vale revisar esses limites antes de usar outros vetores de entrada.

## Lab1 — temperatura simulada e display

O projeto Keil é [`Lab1/gpio.uvprojx`](Lab1/gpio.uvprojx). Apesar do nome do projeto e de comentários antigos sobre botões e LEDs, o programa atual em [`Lab1/main.s`](Lab1/main.s) implementa uma simulação de controle de temperatura: inicia a temperatura atual em 15 e a alvo em 25, mostra os dígitos usando uma tabela de segmentos e atualiza a temperatura atual em um grau por segundo até alcançar o alvo.

A multiplexação alterna rapidamente os dígitos usando portas GPIO e transistores de seleção. Os LEDs da porta N indicam se a temperatura está abaixo, acima ou igual ao alvo. [`Lab1/gpio.s`](Lab1/gpio.s) configura as portas e disponibiliza as rotinas de entrada/saída; [`Lab1/utils.s`](Lab1/utils.s) configura o PLL para 80 MHz e o SysTick usado nos atrasos. **Ponto para conferir:** a temperatura atual passa pela tabela decimal de segmentos; o alvo é enviado diretamente como valor binário para as portas do display, sem conversão pela tabela. Confira se essa visualização corresponde ao resultado esperado.

## Lab2 — cofre com senha

O projeto Keil é [`Lab2/exemplo.uvprojx`](Lab2/exemplo.uvprojx). A lógica principal está em [`Lab2/main.c`](Lab2/main.c), e a interface com os periféricos em [`Lab2/gpio.c`](Lab2/gpio.c).

O fluxo tem quatro estados: cofre aberto, cofre fechado, bloqueio após três senhas incorretas e entrada da chave mestra. No estado aberto, uma senha de quatro teclas seguida de `#` fecha o cofre e guarda a combinação. No estado fechado, uma combinação correta abre o cofre. A chave mestra está definida no código como `1234`; a USR_SW1 solicita sua entrada. O teclado é varrido por GPIO, as mensagens aparecem no LCD e o motor de passo gira para simular a trava. O SysTick fornece atrasos e o Timer 2 atende uma interrupção.

**Pontos para conferir:** as senhas e a lógica do cofre são voláteis e reiniciam com o programa; a senha mestra está fixa no código. Também há acessos à senha digitada que dependem do índice circular `head`; convém verificar os limites do vetor e testar sequências que preencham o buffer começando em posições diferentes.

## Lab3 — controle de motor por potenciômetro ou terminal

O projeto Keil é [`Lab3/exemplo.uvprojx`](Lab3/exemplo.uvprojx). [`Lab3/main.c`](Lab3/main.c) implementa o menu de operação via UART: `*` inicia, `p` seleciona o potenciômetro e `t` seleciona o terminal. No modo do terminal, `h`/`a` escolhem o sentido e as teclas de `5` a `9` ou `0` definem a velocidade; `s` retorna ao menu. No modo potenciômetro, a leitura do ADC define o sentido e a velocidade alvo, com ponto central de parada.

[`Lab3/gpio.c`](Lab3/gpio.c) configura UART, ADC e Timer 2. A interrupção do timer aproxima gradualmente a velocidade atual ao alvo e tenta gerar PWM. [`Lab3/utils.s`](Lab3/utils.s) oferece as rotinas de PLL e SysTick.

**Estado observado no código:** o tratamento do timer contém comentários `//motor(ligado)` e `//motor(desligado)` sem uma escrita correspondente aos pinos do motor; além disso, `timer_pwm` não é reiniciado no período. Assim, o projeto apresenta a interface e parte da lógica de controle, mas a geração efetiva do PWM e o acionamento do motor precisam ser conferidos/completados para operação física.

## Arquivos compartilhados e referências

- `startup.s`: inicialização e tabela de vetores de interrupção, adaptadas para cada projeto.
- `utils.s`: configuração do PLL e rotinas de atraso pelo SysTick, copiadas para os laboratórios que precisam delas.
- `tm4c1294ncpdt.h`: definições de registradores do microcontrolador usadas pelos projetos em C.
- [`UserGuide_EK-TM4C1294XL.pdf`](UserGuide_EK-TM4C1294XL.pdf): guia do kit de desenvolvimento e conexões da placa.
- [`Datasheet_TM4C1294NCPDT.pdf`](Datasheet_TM4C1294NCPDT.pdf): características e registradores do microcontrolador.

## Abrir e executar

1. Abra o arquivo `.uvprojx` dentro da pasta do laboratório desejado usando Keil µVision e o suporte ao dispositivo TM4C1294NCPDT.
2. Compile o projeto e carregue o `.axf` na placa EK-TM4C1294XL. O `Lab0` é um exercício em Assembly com dados de memória preparados para depuração; os demais interagem com os periféricos indicados em cada seção.
3. Consulte o guia da placa para confirmar conexões externas, especialmente teclado, LCD, potenciômetro e motores. Os nomes de sinais e mapeamentos nos fontes devem corresponder ao hardware montado no laboratório.

Os diretórios `Objects/` e `Listings/` contêm saídas geradas pelo Keil (objetos, mapas, listagens e logs), não são fontes principais para edição.

