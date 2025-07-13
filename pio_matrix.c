// Inclusão das bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25  // Total de LEDs da matriz 5x5
#define OUT_PIN 7  // Pino de saída onde a matriz está conectada
#define DEBOUNCE_TIME_MS 200 // Tempo para ignorar múltiplos cliques (evita bouncing)

const uint button_B = 6; // Botão B - GPIO6
const uint button_A = 5; // Botão A - GPIO5

// Função para converter RGB normalizado em valor de 32 bits
uint32_t matrix_rgb(double b, double r, double g) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Função que envia os dados da matriz para o PIO
void desenho_pio(const double *desenho, PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t valor_led = matrix_rgb(desenho[NUM_PIXELS - 1 - i] * b, desenho[NUM_PIXELS - 1 - i] * r, desenho[NUM_PIXELS - 1 - i] * g); // Multiplica o valor do pixel pela cor desejada (permite ajustar tons)
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o PIO
    }
}

// Frames da animação do botão B (flecha)
const double frames_b[6][NUM_PIXELS] = {
     // Cada quadro é uma imagem 5x5 representada como vetor linearizado
    // Valores 1.0 representam LEDs com intesidade máxima, 0.0 apagados
    {
        0,0,0,0,0,
        0,0,0,0,0,
        1,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0
    },
    {
        0,0,0,0,0,
        0,1,0,0,0,
        1,1,0,0,0,
        0,1,0,0,0,
        0,0,0,0,0
    },
    {
        0,0,0,0,0,
        0,1,0,0,0,
        1,1,1,1,0,
        0,1,0,0,0,
        0,0,0,0,0
    },
    {
        0,0,0,0,0,
        0,1,0,0,0,
        1,1,1,1,0,
        0,1,0,0,0,
        0,0,0,0,0
    },
     {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0
    },
};

// Frames da animação do botão A (explosão)
const double frames_a[5][NUM_PIXELS] = {
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0
    },
    {
        0,0,1,0,0,
        0,0,1,0,0,
        1,1,1,1,1,
        0,0,1,0,0,
        0,0,1,0,0
    },
    {
        0,1,0.5,1,0,
        1,0.5,1,0.5,1,
        0.5,1,1,1,0.5,
        1,0.5,1,0.5,1,
        0,1,0.5,1,0
    },
    {
        0,0.5,0.3,0.5,0,
        0.5,0.3,0.5,0.3,0.5,
        0.3,0.5,0.5,0.5,0.3,
        0.5,0.3,0.5,0.3,0.5,
        0,0.5,0.3,0.5,0
    },
    {
        0,0.2,0.1,0.2,0,
        0.2,0.1,0.2,0.1,0.2,
        0.1,0.2,0.2,0.2,0.1,
        0.2,0.1,0.2,0.1,0.2,
        0,0.2,0.1,0.2,0
    }
};

// Função para executar a animação do botão B
void animacao_botao_B(PIO pio, uint sm) {
    for (int f = 0; f < 6; f++) {
        desenho_pio(frames_b[f], pio, sm, 1.0, 0.0, 0.0); // vermelho
        sleep_ms(300);
    }
}

// Função para executar a animação do botão A
void animacao_botao_A(PIO pio, uint sm) {
    for (int f = 0; f < 5; f++) {
        desenho_pio(frames_a[f], pio, sm, 1.0, 1.0, 0.0); // amarelo
        sleep_ms(300);
    }
}

int main() {
    PIO pio = pio0; // Usa o bloco PIO 0
    bool ok = set_sys_clock_khz(128000, false); // Ajusta o clock para 128 MHz
    stdio_init_all();  // Inicializa o sistema de E/S

    printf("Iniciando a transmissão PIO\n");
    if (ok) printf("Clock set to %ld\n", clock_get_hz(clk_sys));

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);

    absolute_time_t last_A = get_absolute_time();
    absolute_time_t last_B = get_absolute_time();

    // Loop principal
    while (true) {
        absolute_time_t now = get_absolute_time();

        // Se o botão A for pressionado e respeitar o tempo de debounce
        if (!gpio_get(button_A) && absolute_time_diff_us(last_A, now) > DEBOUNCE_TIME_MS * 1000) {
            animacao_botao_A(pio, sm);
            last_A = now; // Executa a animação A (explosão)

            // Se o botão B for pressionado e respeitar o tempo de debounce
        } else if (!gpio_get(button_B) && absolute_time_diff_us(last_B, now) > DEBOUNCE_TIME_MS * 1000) {
            animacao_botao_B(pio, sm);
            last_B = now; // Executa a animação B (flecha)
        }

        sleep_ms(10); // Pequeno delay para evitar uso excessivo da CPU
    }
}