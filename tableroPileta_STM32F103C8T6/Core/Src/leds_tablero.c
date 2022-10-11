/*
 * leds_tablero.c
 *
 *  Created on: 10 abr. 2022
 *      Author: Luciano Salvatore
 */

#include "leds_tablero.h"

T_STATUS_LED status_led [16] = {
		APAGADO, //leds (pileta, supongo)
		APAGADO, //napa
		APAGADO, //jets
		APAGADO, //pileta
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO

		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //DESUSO
		APAGADO, //luces ext (amarillas)
		APAGADO, //tomas
};

uint8_t demora_rapida = 19; //equivale a 200 ms: (valor + 1)*10ms
uint8_t demora_lenta = 49; //equivale a 500 ms: (valor + 1)*10ms


void update_leds (void){ //LOGICA NEGATIVA

	for (uint8_t i=0; i < 4; i++){

		switch (status_led[i]) {
			case APAGADO:
				setOutput(i, 1); //APAGA LED
			break;
			case PRENDIDO:
				setOutput(i, 0); //PRENDE LED
			break;
			case TITILA_LENTO:
				if (!demora_lenta){
					toggleOutput (i);
					break;
				}
			break;
			case TITILA_RAPIDO:
				if (!demora_rapida){
					toggleOutput (i);
					break;
				}
			break;
			default:
			break;
		} //end switch status_led

	} //end for uint8_t i...

	switch (status_led[14]) {
		case APAGADO:
			setOutput(OUT_led_luces, 1); //APAGA LED
			break;
		case PRENDIDO:
			setOutput(OUT_led_luces, 0); //PRENDE LED
		case TITILA_LENTO:
			if (!demora_lenta){
				toggleOutput (OUT_led_luces);
				break;
			}
			break;
		case TITILA_RAPIDO:
			if (!demora_rapida){
				toggleOutput (OUT_led_luces);
				break;
			}
			break;
		default:
			break;
	} //end switch status_led

	switch (status_led[15]) {
		case APAGADO:
			setOutput(OUT_led_tomas, 1); //APAGA LED
			break;
		case PRENDIDO:
			setOutput(OUT_led_tomas, 0); //PRENDE LED
		case TITILA_LENTO:
			if (!demora_lenta){
				toggleOutput (OUT_led_tomas);
				break;
			}
			break;
		case TITILA_RAPIDO:
			if (!demora_rapida){
				toggleOutput (OUT_led_tomas);
				break;
			}
			break;
		default:
			break;
	} //end switch status_led

	if (demora_lenta != 0){
		demora_lenta--;
	}else{
		demora_lenta = 4; // por 10 ms.
	}

	if (demora_rapida != 0){
		demora_rapida--;
	}else{
		demora_rapida = 1; // por 10 ms.
	}

} //end update_leds()


void set_led (T_POS_OUTPUT l, T_STATUS_LED val){

	status_led [l] = val;

} //end set_led()
