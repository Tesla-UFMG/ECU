/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "DynamicControls/initializer_controls.h"
#include "DynamicControls/lateral_control.h"
#include "global_definitions.h"
#include "DynamicControls/longitudinal_control.h"
#include "DynamicControls/PID.h"


void init_controls() {

    //Lateral Control
    init_lateral_control();
    //Longitudinal Control
    init_longitudinal_control();

}
