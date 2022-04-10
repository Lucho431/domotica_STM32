/*
 * leds_tablero.h
 *
 *  Created on: 10 abr. 2022
 *      Author: lucho
 */

#ifndef INC_LEDS_TABLERO_H_
#define INC_LEDS_TABLERO_H_

#include "IOports_lfs.h"

typedef enum{
	APAGADO,
	PRENDIDO,
	TITILA_LENTO,
	TITILA_RAPIDO,
}T_STATUS_LED;

void update_leds (void);
void set_led (T_POS_OUTPUT, T_STATUS_LED);

#endif /* INC_LEDS_TABLERO_H_ */
