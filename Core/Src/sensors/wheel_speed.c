/*
 * speed_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "sensors/wheel_speed.h"

#include "cmsis_os.h"
#include "datalogging/datalog_handler.h"
#include "math.h"
#include "stm32h7xx.h"
#include "string.h"
#include "util/CMSIS_extra/global_variables_handler.h"
/*
 * distancia    = circuferencia dividido pela quantidade de dentes
 *              = 2*pi*raio/quant.dentes (m)
 * tempo        = valor do timer multiplicado pelo prescaler dividido pela frequencia
 *              = timer*prescaler/frequencia (s)
 * velocidade   = distancia/tempo
 *              = (2*pi*raio/quant.dentes)*freq/(presc*timer) (m/s)
 *              = (10*3.6*2*pi*raio/quant.dentes)*freq/(presc*timer) (10*km/s)
 */

void reset_speed_all();
void reset_speed_single(speed_message_t* message, speed_message_t* last_messages,
                        uint32_t min_count);
void log_speed(WHEEL_SPEEDS_t* wheel_speeds);
static inline uint32_t get_tim2_freq();
static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc);
static inline uint32_t calculate_timeout(uint32_t speed);

extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;

void speed_calc(void) {
    speed_message_t message;
    speed_message_t last_messages[4];
    // inicializa com 0 buffer de ultimas mensagens
    // NOLINTNEXTLINE
    memset(&last_messages, 0, sizeof(speed_message_t) * 4);

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

    uint32_t d_tim_count;
    uint32_t speed;

    for (;;) {
#ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
#endif

        // espera ate alguma mensagem chegar ou timeout estourar
        switch (osMessageQueueGet(q_speed_messageHandle, &message, NULL, min_timeout)) {

            // caso a tarefa tenha sido chamada por timeout
            case osErrorTimeout:
                reset_speed_all(); // zera a velocidade de todas as rodas
                break;

            // caso a tarefa tenha sido chamada pela queue
            default:
                // verifica se alguma roda esta a muito tempo sem receber interrupcao,
                // caso sim a velocidade dessa roda eh zerada
                reset_speed_single(&message, last_messages, min_count);

                // diferenca entre timestamp da mensagem atual e da anterior
                d_tim_count = message.tim_count - last_messages[message.pin].tim_count;

                // caso d_tim_count calcule uma velocidade maior do que eh possivel ela
                // sera descartada
                if (d_tim_count < max_count) {
                    continue;
                }

                speed = calculate_speed(d_tim_count, tim_freq, tim_presc);
                WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
                // seta velocidade especifica da roda recebida
                wheel_speeds.speed[message.pin] = (float)speed;
                set_global_var(WHEEL_SPEEDS, &wheel_speeds);
                // guarda mensagem ate a proxima interacao
                last_messages[message.pin] = message;
                log_speed(&wheel_speeds);
                break;
        }
    }
}

void log_speed(WHEEL_SPEEDS_t* wheel_speeds) {
    log_data(ID_SPEED_FR, (uint16_t)wheel_speeds->speed[FRONT_RIGHT]);
    log_data(ID_SPEED_FL, (uint16_t)wheel_speeds->speed[FRONT_LEFT]);
    log_data(ID_SPEED_RR, (uint16_t)wheel_speeds->speed[REAR_RIGHT]);
    log_data(ID_SPEED_RL, (uint16_t)wheel_speeds->speed[REAR_LEFT]);
}

void reset_speed_all() {
    WHEEL_SPEEDS_t wheel_speeds = {.speed = {0, 0, 0, 0}};
    set_global_var(WHEEL_SPEEDS, &wheel_speeds);
    log_speed(&wheel_speeds);
}

void reset_speed_single(speed_message_t* message, speed_message_t* last_messages,
                        uint32_t min_count) {
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
    for (speed_pin_e i = FIRST_WHEEL; i <= LAST_WHEEL; i++) {
        if ((message->tim_count - last_messages[i].tim_count) > min_count) {
            wheel_speeds.speed[i] = 0;
        }
    }
    set_global_var(WHEEL_SPEEDS, &wheel_speeds);
}

// obtem a frequencia do tim2 a partir APB1, considerando que ele pode ter um prescaler
// que dobra a frequencia
static inline uint32_t get_tim2_freq() {
    // Get PCLK1 prescaler
    if (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) {
        // PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1
        return 2 * HAL_RCC_GetPCLK1Freq();
    }
    // PCLK1 prescaler equal to 1 => TIMCLK = PCLK1
    return HAL_RCC_GetPCLK1Freq();
}

static inline uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN)
                      * ((float)freq / ((float)presc)) / speed);
}

static inline uint32_t calculate_timeout(uint32_t speed) {
    return (uint32_t)((10 * 3.6 * 2 * M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN)
                      * 1000 / speed);
}
