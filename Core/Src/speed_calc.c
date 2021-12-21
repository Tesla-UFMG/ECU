/*
 * speed_calc.c
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#include "speed_calc.h"
#include "cmsis_os.h"
#include "stm32h7xx.h"
#include "math.h"
#include "string.h"
#include "datalog_handler.h"
#include "CMSIS_extra/global_variables_handler.h"

//distancia     = circuferencia dividido pela quantidade de dentes
//              = 2*pi*raio/quant.dentes (m)
//tempo         = valor do timer multiplicado pelo prescaler dividido pela frequencia
//              = timer*prescaler/frequencia (s)
//velocidade    = distancia/tempo
//              = (2*pi*raio/quant.dentes)*freq/(presc*timer) (m/s)
//              = (10*3.6*2*pi*raio/quant.dentes)*freq/(presc*timer) (10*km/s)

void reset_speed_all();
void reset_speed_single(speed_message_t* message, speed_message_t* last_messages, uint32_t min_count);
void log_speed(WHEEL_SPEEDS_t* wheel_speeds);
uint32_t get_tim2_freq();
uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc);
uint32_t calculate_timeout(uint32_t speed, uint32_t freq, uint32_t presc);

extern TIM_HandleTypeDef htim2;
extern osMessageQueueId_t q_speed_messageHandle;



void speed_calc(void *argument) {
    speed_message_t message;
    speed_message_t last_messages[4];
    memset(&last_messages, 0, sizeof(speed_message_t)*4);   //inicializa com 0 buffer de ultimas mensagens

    const uint32_t tim_freq = get_tim2_freq();          //pega frequência que está conectada ao tim2
    const uint32_t tim_presc = htim2.Init.Prescaler+1;  //prescaler do tim2
    //valor em tempo do timer2 da velocidade máxima a ser calculada
    const uint32_t max_count = calculate_speed(MAX_SPEED, tim_freq, tim_presc);
    //valor em tempo do timer2 da velocidade mínima a ser calculada
    const uint32_t min_count = calculate_speed(MIN_SPEED, tim_freq, tim_presc);
    //valor em tempo do timersys da velocidade mínima a ser calculada
    const uint32_t min_timeout = calculate_timeout(MIN_SPEED, tim_freq, tim_presc);
    uint32_t d_tim_count, speed;

    for(;;) {
        #ifdef DEBUG_ECU
        extern void brkpt();
        brkpt();
        #endif

        switch(osMessageQueueGet(q_speed_messageHandle, &message, NULL,  min_timeout)){     //espera até alguma mensagem chegar ou timeout estourar

        //caso a tarefa tenha sido chamada por timeout
        case osErrorTimeout:
            reset_speed_all();  //zera a velocidade de todas as rodas
            break;

        //caso a tarefa tenha sido chamada pela queue
        default:
            //verifica se alguma roda está a muito tempo sem receber interrupção, caso sim a velocidade dessa roda é zerada
            reset_speed_single(&message, last_messages, min_count);

            //diferenca entre timestamp da mensagem atual e da anterior
            d_tim_count = message.tim_count - last_messages[message.pin].tim_count;

            //caso d_tim_count calcule uma velocidade maior do que é possível o valor será descartado
            if(d_tim_count < max_count) {
                continue;
            }

            speed = calculate_speed(d_tim_count, tim_freq, tim_presc); //calcula a velocidade
            WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
            wheel_speeds.speed[message.pin] = speed;     //seta velocidade especifica da roda recebida
            set_global_var(WHEEL_SPEEDS, &wheel_speeds);
            last_messages[message.pin] = message;   //guarda mensagem até a próxima interacão
            log_speed(&wheel_speeds);
        break;
        }
    }
}

void log_speed(WHEEL_SPEEDS_t* wheel_speeds){
    log_data(ID_SPEED_FR, wheel_speeds->speed[FRONT_RIGHT]);
    log_data(ID_SPEED_FL, wheel_speeds->speed[FRONT_LEFT]);
    log_data(ID_SPEED_RR, wheel_speeds->speed[REAR_RIGHT]);
    log_data(ID_SPEED_RL, wheel_speeds->speed[REAR_LEFT]);
}

void reset_speed_all() {
    WHEEL_SPEEDS_t wheel_speeds = {.speed = {0, 0, 0, 0}};
    set_global_var(WHEEL_SPEEDS, &wheel_speeds);
    log_speed(&wheel_speeds);
}

void reset_speed_single(speed_message_t* message, speed_message_t* last_messages, uint32_t min_count) {
    WHEEL_SPEEDS_t wheel_speeds = get_global_var_value(WHEEL_SPEEDS);
    for(speed_pin_e i = FIRST_WHEEL; i <= LAST_WHEEL; i++) {
        if((message->tim_count - last_messages[i].tim_count) > min_count) {
            wheel_speeds.speed[i] = 0;
        }
    }
    set_global_var(WHEEL_SPEEDS, &wheel_speeds);
}

//obtem a frequencia do tim2 a partir APB1, considerando que ele pode ter um prescaler que dobra a frequencia
uint32_t get_tim2_freq() {
    if (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) {   // Get PCLK1 prescaler
        return 2*HAL_RCC_GetPCLK1Freq();    // PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1
    } else {
        return HAL_RCC_GetPCLK1Freq();      // PCLK1 prescaler equal to 1 => TIMCLK = PCLK1
    }
}


uint32_t calculate_speed(uint32_t speed, uint32_t freq, uint32_t presc){
    return ((10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * ((float)freq/((float)presc)) / speed);
}

uint32_t calculate_timeout(uint32_t speed, uint32_t freq, uint32_t presc){
    return ((10*3.6*2*M_PI * WHEEL_RADIUS / SPEED_SENSOR_TEETH_QUAN) * 1000 / speed);
}
