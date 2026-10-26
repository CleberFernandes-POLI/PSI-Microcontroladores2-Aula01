#include <zephyr/kernel.h>              // Funcoes basicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>              // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>        // API para controle de pinos de entrada/saida (GPIO)
#include <pwm_z42.h>                    // Biblioteca personalizada com funcoes de controle do TPM (Timer/PWM Module)

#define TEMPO 500 // meio segundo
#define TPM_MODULE 1000                 // Define a frequencia do PWM
#define STACK_SIZE 512                  // Tamanho da pilha da thread
#define PRIORITY 5                      // Prioridade da thread

uint16_t duty_50 = TPM_MODULE / 2;      // 50% de duty cycle

// --- Função da Thread para controlar os LEDs ---
void led_thread_code(void *p1, void *p2, void *p3)
{
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18); // Vermelho
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19); // Verde

    while (1)
    {
        pwm_tpm_CnV(TPM2, 0, duty_50); 
        pwm_tpm_CnV(TPM2, 1, duty_50);
        k_msleep(TEMPO); // Amarelo
        pwm_tpm_CnV(TPM2, 0, 1000);
        k_msleep(TEMPO); // Vermelho apaga, fica verde
        pwm_tpm_CnV(TPM2, 1, 1000);
        pwm_tpm_CnV(TPM2, 0, duty_50);
        k_msleep(TEMPO); // Vermelho acende, verde apaga (fica vermelho)
        pwm_tpm_CnV(TPM2, 0, 1000);
        k_msleep(TEMPO); // Vermelho apaga (totalmente apagado)
    }
}
// Aqui ele declara e cria a thread automaticamente pelo Zephyr
K_THREAD_DEFINE(led_tid, STACK_SIZE, led_thread_code, NULL, NULL, NULL, PRIORITY, 0, 0);

int main(void)
{
    // Main apenas inicializa e dorme permanentemente para liberar a CPU
    while(1)
    {
        k_sleep(K_FOREVER);
    }
    return 0;
}