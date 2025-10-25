/*
 * initializer_controls.c
 *
 *  Created on: 18 de ago de 2021
 *      Author: Luiza
 */

#include "dynamic_controls/initializer_controls.h"

#include "dynamic_controls/PID.h"
#include "dynamic_controls/lateral_control.h"
#include "dynamic_controls/longitudinal_control.h"
#include "util/global_definitions.h"

void init_controls() {

    // Lateral Control
    init_lateral_control();
    // Longitudinal Control
    init_longitudinal_control();
}
