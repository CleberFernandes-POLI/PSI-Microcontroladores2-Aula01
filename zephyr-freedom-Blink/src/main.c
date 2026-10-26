#include <zephyr/kernel.h>              // Funcoes basicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>              // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>        // API para controle de pinos de entrada/saida (GPIO)
#include <pwm_z42.h>                    // Biblioteca personalizada com funcoes de controle do TPM (Timer/PWM Module)

#define TEMP1 1000                     // 1s
#define TEMP2 350                       // 350ms
#define TPM_MODULE 1000                 // Define a frequencia do PWM
#define STACK_SIZE 512                  // Tamanho da pilha da thread
#define PRIORITY_RED   5
#define PRIORITY_GREEN 6

uint16_t duty_50 = TPM_MODULE / 2;      // 50% de duty cycle

void red_led_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        pwm_tpm_CnV(TPM2, 0, duty_50); // Vermelho acende
        k_msleep(TEMP2); // 350ms
        pwm_tpm_CnV(TPM2, 0, 1000);    // Vermelho apaga
        k_msleep(TEMP2); // 350ms
    }
}

void green_led_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        pwm_tpm_CnV(TPM2, 1, duty_50); // Verde acende
        k_msleep(TEMP1); // 1s
        pwm_tpm_CnV(TPM2, 1, 1000);    // Verde apaga
        k_msleep(TEMP1); // 1s
    }
}

K_THREAD_DEFINE(red_tid,   STACK_SIZE, red_led_thread,   NULL, NULL, NULL, PRIORITY_RED,   0, 0);
K_THREAD_DEFINE(green_tid, STACK_SIZE, green_led_thread, NULL, NULL, NULL, PRIORITY_GREEN, 0, 0);

int main(void)
{
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18); // Vermelho
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19); // Verde

    while (1) {
        k_sleep(K_FOREVER);
    }
    return 0;
}