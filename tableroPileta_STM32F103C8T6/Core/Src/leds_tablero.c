/*
 * leds_tablero.c
 *
 *  Created on: 10 abr. 2022
 *      Author: Luciano Salvatore
 */

#include "leds_tablero.h"

T_STATUS_LED status_led [6] = {
		APAGADO, //tomas
		APAGADO, //luces ext (amarillas)
		APAGADO, //leds (pileta, supongo)
		APAGADO, //napa
		APAGADO, //jets
		APAGADO //pileta
};

uint8_t demora_rapida = 19; //equivale a 200 ms: (valor + 1)*10ms
uint8_t demora_lenta = 49; //equivale a 500 ms: (valor + 1)*10ms


void update_leds (void){ //LOGICA NEGATIVA

	for (uint8_t i=0; i < 6; i++){

		switch (status_led[i]) {
			case APAGADO:
				setOutput(OUT_led_tomas + i, 1); //APAGA LED
			break;
			case PRENDIDO:
				setOutput(OUT_led_tomas + i, 0); //PRENDE LED
			case TITILA_LENTO:
				if (!demora_lenta){
					toggleOutput (OUT_led_tomas + i);
					break;
				}
			break;
			case TITILA_RAPIDO:
				if (!demora_rapida){
					toggleOutput (OUT_led_tomas + i);
					break;
				}
			break;
			default:
			break;
		} //end switch status_led

	} //end for uint8_t i...

	if (demora_lenta != 0){
		demora_lenta--;
	}else{
		demora_lenta = 49;
	}

	if (demora_rapida != 0){
		demora_rapida--;
	}else{
		demora_rapida = 19;
	}

} //end update_leds()


void set_led (T_POS_OUTPUT l, T_STATUS_LED val){

	if (l < OUT_led_tomas) return;

	status_led [l - OUT_led_tomas] = val;

} //end set_led()
