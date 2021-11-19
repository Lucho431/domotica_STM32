/*
 * botones_lfs.h
 *
 *  Created on: Nov 3, 2021
 *      Author: Luciano Salvatore
 */

#ifndef INC_IOPORTS_LFS_H_
#define INC_IOPORTS_LFS_H_

#include "main.h"

typedef enum{
    LOW_L,
    HIGH_L,
    FALL,
    RISE,
}T_INPUT;

typedef enum{
	IN_0,
	IN_1,
	IN_2,
	IN_3,
	IN_4,
	IN_5,
	IN_6,
	IN_7,
	IN_8,
	IN_9,
	IN_A,
	IN_B,
	IN_C,
	IN_D,
	IN_AST,
	IN_HASH,
	IN_tomas,
	IN_luces,
	IN_leds,
	IN_napa,
	IN_jet,
	IN_pileta,
	SIZEOF_POS_INPUT,
}T_POS_INPUT;

typedef enum{
	OUT_rele_tomas,
	OUT_rele_luces,
	OUT_rele_leds,
	OUT_rele_napa,
	OUT_rele_jet,
	OUT_rele_pileta,
	OUT_led_tomas,
	OUT_led_luces,
	OUT_led_leds,
	OUT_led_napa,
	OUT_led_jet,
	OUT_led_pileta,
	SIZEOF_POS_OUTPUT,
}T_POS_OUTPUT;



//void __turnONFila (uint8_t);
//uint8_t __lecturaColumna (uint8_t);
void lecturaTeclas(void);
void update_teclas (void);
T_INPUT getStatBoton (T_POS_INPUT);
//int8_t getNumber (void);
//void botonEXTI_handler(uint16_t);

void setOutput (T_POS_OUTPUT, uint8_t);

#endif /* INC_IOPORTS_LFS_H_ */
