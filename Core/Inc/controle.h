/*
 * controle.h
 *
 *  Created on: May 11, 2020
 *      Author: renanmoreira
 */

#ifndef INC_CONTROLE_H_
#define INC_CONTROLE_H_

typedef enum estado_veiculo {
	S_DISABLE_E = 0,
	S_BRAKE_E = 1,
	S_ACCELERATE_E = 2,
	S_NEUTER_E = 3,
	S_REVERSE_E = 4
} vehicle_state_e;



#endif /* INC_CONTROLE_H_ */
