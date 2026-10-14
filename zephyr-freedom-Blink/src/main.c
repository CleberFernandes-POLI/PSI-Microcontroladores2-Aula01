#include <zephyr/kernel.h>             // Funcoes basicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saida (GPIO)
#include <pwm_z42.h>                   // Biblioteca personalizada com funcoes de controle do TPM (Timer/PWM Module)

#define TEMPO 500 // meio segundo
// Define o valor do registrador MOD do TPM para configurar o periodo do PWM
#define TPM_MODULE 1000         // Define a frequencia do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t duty_50  = TPM_MODULE/2;       // 50% de duty cycle

int main(void)
{
    // Inicializa o modulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operacao EDGE_PWM
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOB_18
    // - modo TPM_PWM_H (nivel alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18); // Vermelho
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19); // Verde

    // Loop infinito
    while(1)
    {
        pwm_tpm_CnV(TPM2, 0, duty_50); 
        pwm_tpm_CnV(TPM2, 1, duty_50);
        k_msleep(TEMPO); // Acima ele fica amarelo
        pwm_tpm_CnV(TPM2, 0, 1000);
        k_msleep(TEMPO); // Vermelho apaga e fica verde
        pwm_tpm_CnV(TPM2, 1, 1000);
        pwm_tpm_CnV(TPM2, 0, duty_50);
        k_msleep(TEMPO); // Vermelho acende e verde apaga, fica vermelho.
        pwm_tpm_CnV(TPM2, 0, 1000);
        k_msleep(TEMPO); // Vermelho apaga, fica apagado total
    }

    return 0;
}   