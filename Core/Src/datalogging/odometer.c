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

*main.c
    osMessageQueueId_t -> osMessageNew() (Processo de criação de fila)
    
*external_interrupt_handler.c
    osMessagePut() (Criação de mensagem a partir de Callback (Interrupção))

*main.c 
    MX_GPIO_INIT() -> GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING (Configuração de pinos e seus modos)

- O RTOS utilizado na ECU é na realidade uma API do FreeRTOS para microprocessadores da arquitetura ARM CORTEX. Isso faz com que 
o processo de criação de tarefas/threads seja diferente da utilização do FreeRTOS vanilla? No programa wheel_speed.c não seria 
necessário criar uma tarefa para a execução da rotina? definindo prioridade e etc
 
 *main.c
    osThreadId_t -> osThreadNew() (Processo de criação de tarefa)

*/

/*

Se utilizássemos osDelay(100) como referência de tempo
Considerando uma velocidade linear máxima de 150 km/h = 41,6667 m/s
Raio da roda fônica (gira na mesma velocidade angular que a roda) = 0.26 m
Calcula-se a velocidade angular: v = w*R -> w = 160.2565 rad/s
Distância entre dentes da roda fônica: 2piR/quantidade de dentes = 0.102 m
Considerando uma velocidade máxima de 160 rad/s e uma distância entre dentes de 0.1 m, 
o tempo entre as interrupções seria de = 6.25 x 10^-4

Com o delay de 100 ms muita informação seria perdida?

*/
#include "datalogging/odometer.h"
#include "datalogging/datalog_handler.h"
#include "cmsis_os.h"
#include "util/util.h"
#include "math.h"

static inline uint32_t calculate_distance(void *argument);

void odoemter_calc(void *argument)
{
    distance_message_t message;      //Criação de struct necessária 
    distance_message_t last_messages[4];
    // inicializa com 0 buffer de ultimas mensagens
    // NOLINTNEXTLINE
    memset(&last_messages, 0, sizeof(speed_message_t) * 4);

    // pega frequencia que esta conectada ao tim2
    const uint32_t tim_freq = get_tim2_freq();
    // prescaler do tim2
    const uint32_t tim_presc = htim2.Init.Prescaler + 1;
    // valor em tempo do timer2 da velocidade maxima a ser calculada
    const uint32_t max_count;
    // valor em tempo do timer2 da velocidade minima a ser calculada
    const uint32_t min_count;
    // valor em tempo do timersys da velocidade minima a ser calculada
    const uint32_t min_timeout;

    uint32_t d_tim_count;
    uint32_t distance;

    for (;;)
    {
        #ifndef DEBUG_ECU
                extern void brkpt();
                brkpt();
        #endif

        switch (osMessageQueueGet(q_distance_messageHandle, &message, NULL, min_timeout)) {    //Criacao de fila para distancia necessaria

            // caso a tarefa tenha sido chamada por timeout
            case osErrorTimeout:
               // reset_speed_all(); // Seria necessario resetar de alguma forma a distancia?
                break;

            // caso a tarefa tenha sido chamada pela queue
            default:
                // verifica se alguma roda esta a muito tempo sem receber interrupcao,
                // caso sim a velocidade dessa roda eh zerada
                reset_distance_single(&message, last_messages, min_count);

                // diferenca entre timestamp da mensagem atual e da anterior
                d_tim_count = message.tim_count - last_messages[message.pin].tim_count;

                // caso d_tim_count calcule uma distancia muito pequena 
                // sera descartada
                if (d_tim_count > min_count) {
                    continue;
                }

                //distance += calculate_distance(d_tim_count, tim_freq, tim_presc);
                distance += (DISTANCIA_ENTRE_DENTES)*RAIO_RODA;
                WHEEL_DISTANCE_t wheel_distances = get_global_var_value(WHEEL_DISTANCE); //Criação de variável global para distancia 
                // seta velocidade especifica da roda recebida
                wheel_distances.distance[message.pin] = (float)distance; //Utilizaria vetor ou somente aferição por uma roda seria suficiente?
                set_global_var(WHEEL_DISTANCE, &wheel_distances);
                // guarda mensagem ate a proxima interacao
                last_messages[message.pin] = message;
                log_distance(&wheel_distances);
                break;
        }
    }
}

/*
    
    De alguma maneira comunicar com a tarefa de wheel_speed, de forma que o odometro somente realize uma multiplicacao entre a velocidade e a diferenca de timestamps


*/
/*
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

*/
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

