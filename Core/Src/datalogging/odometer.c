/*
 * odometer.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */


/*
    Anotações e dúvidas
- No programa wheel_speed.c utiliza-se uma a detecção do recebimento de uma mensagem enviada para uma fila para que 
a thread seja executada. Quem é responsável por enviar a mensagem para a fila? O que identifica a mensagem, o handler? ( a partir sinal de interrupção da roda fônica)
- O RTOS utilizado na ECU é na realidade uma API do FreeRTOS para microprocessadores da arquitetura ARM CORTEX. Isso faz com que 
o processo de criação de tarefas/threads seja diferente da utilização do FreeRTOS vanilla? No programa wheel_speed.c não seria 
necessário criar uma tarefa para a execução da rotina? definindo prioridade e etc
 
*/
#include "datalogging/odometer.h"
#include "datalogging/datalog_handler.h"
#include "cmsis_os.h"
#include "util/util.h"
#include "math.h"

static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc);

void odometer_calc(void)
{
    dist_message_t message;
    dist_message_t last_messages;

    uint32_t total_dist = 0;
    uint32_t partial_dist = 0;
    //uint32_t speed = 0;
    uint32_t d_tim_count = 0;

    // pega frequencia que esta conectada ao tim2
    const uint32_t tim_freq = get_tim2_freq();
    // prescaler do tim2
    const uint32_t tim_presc = htim2.Init.Prescaler + 1;
    // valor em tempo do timer2 da velocidade maxima a ser calculada
    const uint32_t max_count = calculate_speed(MAX_SPEED, tim_freq, tim_presc);
    // valor em tempo do timer2 da velocidade minima a ser calculada
    const uint32_t min_count = calculate_speed(MIN_SPEED, tim_freq, tim_presc);
    // valor em tempo do timersys da velocidade minima a ser calculada
    const uint32_t min_timeout = calculate_timeout(MIN_SPEED);

    
        for (;;) 
    {
    #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
    #endif
    osDelay(100); //Suficiente para implementação utilizando RTOS e ainda contabilizar o tempo?

// diferenca entre timestamp da mensagem atual e da anterior
    d_tim_count = message.tim_count - last_messages.tim_count;

    // caso d_tim_count calcule uma velocidade maior do que eh possivel ela
    // sera descartada
    if (d_tim_count < max_count) {
        continue;
    }

    //Sem utilizar variável global
    partial_dist = dist_calc(d_tim_count, tim_freq, tim_presc);
    if (partial_dist >1)
    {
      total_dist += partial_dist;
    }

    //Utilizando variável global
    WHEEL_DIST_t wheel_dist = get_global_var value(WHEEL_DIST); // Possível criação de variável global para rastreamento da distância?
    wheel_dist.dist = partial_dist  //Sem utilização de vetor. Uma única roda seria suficiente?
    set_global_var(WHEEL_DIST, &wheel_dist);
    last_messages = message;
    log_dist(&wheel_dist);

    //Salva de alguma maneira na Memória Flash
    }
}

void log_dist(WHEEL_DIST_t* wheel_dist) 
{
    log_data(ID_DIST_FR, (uint16_t)wheel_dist); //Sem utilização de vetor. Adicionaria ID CAN para a distância?
}

static inline uint32_t get_tim2_freq() {
    // Get PCLK1 prescaler
    if (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) {
        // PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1
        return 2 * HAL_RCC_GetPCLK1Freq();
    }
    // PCLK1 prescaler equal to 1 => TIMCLK = PCLK1
    return HAL_RCC_GetPCLK1Freq();
}

static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc) 
{

    return (uint32_t)((2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN)
                      * ((float)freq / ((float)presc)) / speed);
}

static inline uint32_t dist_calc(uint32_t d_tim_count, uint32_t tim_freq,uint32_t tim_presc)
{
    //Considerando uma execução a cada 100ms?
   uint32_t speed = calculate_speed(d_tim_count, tim_freq, tim_presc);
   uint32_t dist = (uint32_t) (1000 * speed * (float)tim_freq/((float)tim_presc));
   return dist;
}


/*
void odometer_calc(void* argument) {
    UNUSED(argument);
    for (;;) {
        osDelay(100);

    }
    /*
    const static uint8_t TEST_DIST = 0;
    const static uint8_t OVERALL_DIST = 1;

     //calculates delta
    Dist_Calc();

    // verifica se o calculo da distancia ja deu 1 metro ou mais - valor retornado em
    dist_calc esta em decimetro

    if ((dist_calc / 10) >= 1) {
            dist_log[TEST_DIST] += (dist_calc / 10);		// updates test and
    overall distance dist_log[OVERALL_DIST] += (dist_calc / 10);		// updates
    distance already travelled

            // rounds to the nearest meter
            Record_Distance(dist_log);						// writes
    values in flash memory
    }*/

