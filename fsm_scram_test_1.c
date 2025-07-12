#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para a função sleep() em sistemas UNIX-like (Linux, macOS)
#include <time.h>   // Para inicializar o gerador de números aleatórios

//scram_test_3.c
// Este é um simulador de controle de bombas de reator PWR (Pressurized Water Reactor)
// Implementa uma Máquina de Estados Finitos (FSM) para gerenciar o estado do sistema
// e simular o comportamento das bombas e parâmetros do reator.

// no windows
// #include <windows.h>

//no linux da pra usar os acentos nas palavras

// Definindo os estados possíveis da nossa FSM
typedef enum {
    DESLIGADO,
    INICIANDO,
    OPERANDO,
    FALHA,
    EMERGENCIA_SCRAM,
    RESETANDO
} EstadoBomba;

// Definindo os comandos que o operador pode enviar ao sistema
typedef enum {
    CMD_NULO,
    CMD_INICIAR,
    CMD_PARAR,
    CMD_RESETAR
} ComandoOperador;

// Estrutura para armazenar o estado atual do sistema de controle
typedef struct {
    EstadoBomba estado_atual;
    double pressao_primario;      // em bar
    double temperatura_refrigerante; // em Celsius
} SistemaControlePWR;

// --- Protótipos das Funções ---
void inicializarSistema(SistemaControlePWR *sistema);
void simularParametros(SistemaControlePWR *sistema);
void imprimirStatus(const SistemaControlePWR *sistema);
const char* getNomeEstado(EstadoBomba estado);
void maquinaDeEstados(SistemaControlePWR *sistema, ComandoOperador comando);

// --- Função Principal ---
int main() {
    // Inicializa o gerador de números aleatórios para simulação
    srand(time(NULL));

    SistemaControlePWR sistema;
    inicializarSistema(&sistema);

    char entrada_usuario[10];
    ComandoOperador comando = CMD_NULO;

    printf("--- Simulador de Controle de Bombas de Reator PWR ---\n");
    printf("Comandos disponíveis: 'iniciar', 'parar', 'resetar', 'sair'\n\n");

    while (1) {
        imprimirStatus(&sistema);

        // Simula a leitura de um comando do operador (não bloqueante)
        // Em um sistema real, isso viria de uma interface de controle.
        printf("Digite um comando: ");
        scanf("%9s", entrada_usuario);

        if (strcmp(entrada_usuario, "iniciar") == 0) {
            comando = CMD_INICIAR;
        } else if (strcmp(entrada_usuario, "parar") == 0) {
            comando = CMD_PARAR;
        } else if (strcmp(entrada_usuario, "resetar") == 0) {
            comando = CMD_RESETAR;
        } else if (strcmp(entrada_usuario, "sair") == 0) {
            break;
        } else {
            comando = CMD_NULO;
        }

        // Processa o estado atual da FSM
        maquinaDeEstados(&sistema, comando);

        // Simula a passagem do tempo e a flutuação dos parâmetros
        if (sistema.estado_atual == OPERANDO || sistema.estado_atual == INICIANDO) {
            simularParametros(&sistema);
        }

        // Limpa o buffer de entrada para evitar leituras indesejadas
        while (getchar() != '\n');

        printf("\n----------------------------------------------------\n");
        // Em um sistema windows, use Sleep(2000);
        sleep(2);
    }

    printf("Simulação encerrada.\n");

    printf("\n\n");
    return -23;
}

// --- Implementação das Funções ---

/**
 Inicializa o sistema de controle para o estado inicial.
 */
void inicializarSistema(SistemaControlePWR *sistema) {
    sistema->estado_atual = DESLIGADO;
    sistema->pressao_primario = 10.0;      // Pressão ambiente
    sistema->temperatura_refrigerante = 25.0; // Temperatura ambiente
}

/**
 Retorna o nome do estado como uma string para impressão
 */
const char* getNomeEstado(EstadoBomba estado) {
    switch (estado) {
        case DESLIGADO: return "DESLIGADO";
        case INICIANDO: return "INICIANDO";
        case OPERANDO: return "OPERANDO";
        case FALHA: return "FALHA DETECTADA";
        case EMERGENCIA_SCRAM: return "EMERGENCIA (SCRAM ATIVO)";
        case RESETANDO: return "RESETANDO SISTEMA";
        default: return "ESTADO DESCONHECIDO";
    }
}

/**
Imprime o status atual do sistema no console.
 */
void imprimirStatus(const SistemaControlePWR *sistema) {
    printf("Estado Atual: [%s]\n", getNomeEstado(sistema->estado_atual));
    printf("  -> Pressão Circuito Primário: %.2f bar\n", sistema->pressao_primario);
    printf("  -> Temperatura do Refrigerante: %.2f °C\n", sistema->temperatura_refrigerante);
}

/**
Simula a flutuação dos parâmetros do reator.
Há uma pequena chance de gerar um valor fora da faixa normal para testar o SCRAM.
*/
void simularParametros(SistemaControlePWR *sistema) {
    if (sistema->estado_atual == OPERANDO) {


        // Flutuação normal
        sistema->pressao_primario = 155.0 + ((rand() % 100 - 50) / 50.0) * 2.0; // Variação normal de +/- 2 bar
        sistema->temperatura_refrigerante = 310.0 + ((rand() % 100 - 50) / 50.0) * 5.0; // Variação normal de +/- 5 °C

        // Chance de 15% de gerar um evento anômalo para teste de SCRAM
        if (rand() % 100 < 15) {
            printf("\n*** SIMULANDO ANOMALIA NOS PARÂMETROS! ***\n");
            if (rand() % 2 == 0) {
                sistema->pressao_primario = 145.0; // Pressão baixa
            } else {
                sistema->temperatura_refrigerante = 335.0; // Temperatura alta
            }
        }
    }
}

/**
O coração do programa: a Máquina de Estados Finitos.
Processa o estado atual com base nos comandos e parâmetros.
 */
void maquinaDeEstados(SistemaControlePWR *sistema, ComandoOperador comando) {
    switch (sistema->estado_atual) {
        case DESLIGADO:
            if (comando == CMD_INICIAR) {
                printf(">>> Transição: DESLIGADO -> INICIANDO\n");
                printf("Iniciando sequência de partida das bombas...\n");
                sistema->estado_atual = INICIANDO;
                // Simula o aumento gradual de pressão/temperatura
                sistema->pressao_primario = 100.0;
                sistema->temperatura_refrigerante = 150.0;
            }
            break;

        case INICIANDO:
            printf("Verificando sistemas... Bombas entrando em velocidade nominal.\n");
            // Em uma simulação real, haveria um delay e checagens aqui.
            // Após a inicialização, o sistema passa a operar normalmente.
            printf(">>> Transição: INICIANDO -> OPERANDO\n");
            sistema->estado_atual = OPERANDO;
            break;

        case OPERANDO:
            // Condição de SCRAM: Verifica se os parâmetros estão fora da faixa de segurança
            if (sistema->pressao_primario < 150.0 || sistema->pressao_primario > 160.0 ||
                sistema->temperatura_refrigerante < 290.0 || sistema->temperatura_refrigerante > 325.0) {

                printf("\nALERTA! Parâmetros fora dos limites de segurança!\n");
                printf(">>> Transição: OPERANDO -> FALHA\n");
                sistema->estado_atual = FALHA;
            } else if (comando == CMD_PARAR) {
                printf("Comando de parada recebido. Desligando as bombas de forma controlada.\n");
                printf(">>> Transição: OPERANDO -> DESLIGADO\n");
                sistema->estado_atual = DESLIGADO;
                // Reseta parâmetros para valores de repouso
                inicializarSistema(sistema);
            }
            break;

        case FALHA:
            printf("\n!!! SCRAM INICIADO !!!\n");
            printf(">>> Inserindo barras de controle para desligamento imediato do reator.\n");
            printf(">>> Desligando todas as bombas do circuito primário.\n");
            printf(">>> Transição: FALHA -> EMERGENCIA_SCRAM\n");
            sistema->estado_atual = EMERGENCIA_SCRAM;
            // Zera a pressão e temperatura para simular o desligamento
            sistema->pressao_primario = 0.0;
            sistema->temperatura_refrigerante = 80.0; // Temperatura residual
            break;

        case EMERGENCIA_SCRAM:
            printf("SISTEMA EM ESTADO DE EMERGÊNCIA. Necessário reset manual.\n");
            if (comando == CMD_RESETAR) {
                printf("Reset manual autorizado. Limpando condição de falha...\n");
                printf(">>> Transição: EMERGENCIA_SCRAM -> RESETANDO\n");
                sistema->estado_atual = RESETANDO;
            }
            break;

        case RESETANDO:
            printf("Sistema resetado. Retornando para o estado seguro.\n");
            printf(">>> Transição: RESETANDO -> DESLIGADO\n");
            sistema->estado_atual = DESLIGADO;
            inicializarSistema(sistema);
            break;
    }
}
