#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // Para usleep()
#include <stdlib.h> // Para system()

//cscram_top_5.c

// Estados da FSM
typedef enum {
    STATE_NORMAL,
    STATE_SCRAM
} SystemState;

// Parâmetros do reator
typedef struct {
    float temperature;     // °C
    float pressure;        // MPa sist internacional
    float waterLevel;      // %
} ReactorParams;

// Limites seguros
#define MAX_TEMP 350.0
#define MAX_PRESSURE 15.5
#define MIN_WATER_LEVEL 90.0

// Função para verificar SCRAM
bool checkScram(ReactorParams params) {
    return (params.temperature > MAX_TEMP || 
            params.pressure > MAX_PRESSURE || 
            params.waterLevel < MIN_WATER_LEVEL);
}

// Função para atualizar barras de status
void updateBars(ReactorParams params) {
    system("clear"); // Limpa o terminal "clear" Linux/macOS "cls" windows

    printf("Controle do Reator PWR - Aguardando anomalias...\n");
    printf("(SCRAM será ativado se qualquer parâmetro exceder o limite)\n\n");
    
    // Barra de Temperatura (30 caracteres de largura)
    int tempBar = (int)(params.temperature / MAX_TEMP * 30);
    printf("TEMPERATURA: [");
    for (int i = 0; i < 30; i++) 
        printf(i < tempBar ? "█" : " ");
    printf("] %.1f°C\n", params.temperature);

    // Barra de Pressão (30 caracteres)
    int pressureBar = (int)(params.pressure / MAX_PRESSURE * 30);
    printf("PRESSÃO:     [");
    for (int i = 0; i < 30; i++)
        printf(i < pressureBar ? "\x1B[31m█\x1B[0m" : " ");
    //        printf("\n\x1B[31m[SCRAM ATIVADO!] REATOR DESLIGADO.\x1B[0m\n");


    printf("] %.1f MPa\n", params.pressure);

    // Barra de Nível de Água (30 caracteres)
    int waterBar = (int)(params.waterLevel / 100.0 * 30);
    printf("NÍVEL ÁGUA:  [");
    for (int i = 0; i < 30; i++)
        printf(i < waterBar ? "█" : " ");
    printf("] %.1f%%\n", params.waterLevel);
}

int main() {


    ReactorParams reactor = {
        .temperature = 300.0,
        .pressure = 12.0,
        .waterLevel = 95.0
    };



    printf("Controle do Reator PWR - Aguardando anomalias...\n");
    printf("(SCRAM será ativado se qualquer parâmetro exceder o limite)\n\n");

    while (true) {
        updateBars(reactor);

        // Verifica SCRAM
        if (checkScram(reactor)) {
            printf("\n\x1B[31m[SCRAM ATIVADO!] REATOR DESLIGADO.\x1B[0m\n");
            printf("Motivo: ");
            if (reactor.temperature > MAX_TEMP) printf("TEMPERATURA CRÍTICA (%.1f°C > %.1f°C)\n", reactor.temperature, MAX_TEMP);
            if (reactor.pressure > MAX_PRESSURE) printf("PRESSÃO CRÍTICA (%.1f MPa > %.1f MPa)\n", reactor.pressure, MAX_PRESSURE);
            if (reactor.waterLevel < MIN_WATER_LEVEL) printf("NÍVEL DE ÁGUA CRÍTICO (%.1f%% < %.1f%%)\n", reactor.waterLevel, MIN_WATER_LEVEL);
            break;
        }

        // Atualiza parâmetros aleatoriamente (simulação)
        reactor.temperature += (rand() % 100) / 20.0 - 2.5;  // Variação controlada
        reactor.pressure += (rand() % 100) / 30.0 - 1.5;
        reactor.waterLevel += (rand() % 100) / 50.0 - 1.0;

        // Garante limites físicos
        if (reactor.temperature < 0) reactor.temperature = 0;
        if (reactor.pressure < 0) reactor.pressure = 0;
        if (reactor.waterLevel < 0) reactor.waterLevel = 0;
        if (reactor.waterLevel > 100) reactor.waterLevel = 100;

        usleep(500000); // Atualiza a cada 0.5s (500ms)
    }

    printf("\n\n");
    return -23;
}
