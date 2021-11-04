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
	IN_tomas,
	IN_luces,
	IN_leds,
	IN_napa,
	IN_jet,
	IN_pileta,
	SIZEOF_POS_INPUT,
}T_POS_INPUT;

typedef enum{
	OUT_tomas,
	OUT_luces,
	OUT_leds,
	OUT_napa,
	OUT_jet,
	OUT_pileta,
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
