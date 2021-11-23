/*
 * botones_lfs.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Luciano Salvatore
 */

#include <IOports_lfs.h>
#include "74HC165_SPI_lfs.h"


//macros
#define GETBUTTONSTATUS(BUTTON,EDGE) (EDGE & (0B1 << (BUTTON)))



//variables
uint8_t flag_lecturas = 1;
//uint8_t read_teclas [SIZEOF_POS_BOTONES];
//uint8_t last_teclas [SIZEOF_POS_BOTONES];
T_INPUT status_teclas [SIZEOF_POS_INPUT];

uint8_t read_teclas, last_teclas;
uint16_t read_teclado, last_teclado;
uint32_t read_input, last_input, fall_input, rise_input;

void turnONFila (uint8_t filaNum){

	switch (filaNum){
		case 0:
			//pongo en 0 la fila 0; resto en 1 (LOGICA NEGATIVA).
			HAL_GPIO_WritePin(OUT_fila0_GPIO_Port, OUT_fila0_Pin, 0);
			HAL_GPIO_WritePin(OUT_fila1_GPIO_Port, OUT_fila1_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila2_GPIO_Port, OUT_fila2_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila3_GPIO_Port, OUT_fila3_Pin, 1);
		break;
		case 1:
			//pongo en 0 la fila 1; resto en 1 (LOGICA NEGATIVA).
			HAL_GPIO_WritePin(OUT_fila0_GPIO_Port, OUT_fila0_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila1_GPIO_Port, OUT_fila1_Pin, 0);
			HAL_GPIO_WritePin(OUT_fila2_GPIO_Port, OUT_fila2_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila3_GPIO_Port, OUT_fila3_Pin, 1);
		break;
		case 2:
			//pongo en 0 la fila 2; resto en 1 (LOGICA NEGATIVA).
			HAL_GPIO_WritePin(OUT_fila0_GPIO_Port, OUT_fila0_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila1_GPIO_Port, OUT_fila1_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila2_GPIO_Port, OUT_fila2_Pin, 0);
			HAL_GPIO_WritePin(OUT_fila3_GPIO_Port, OUT_fila3_Pin, 1);
		break;
		case 3:
			//pongo en 0 la fila 3; resto en 1 (LOGICA NEGATIVA).
			HAL_GPIO_WritePin(OUT_fila0_GPIO_Port, OUT_fila0_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila1_GPIO_Port, OUT_fila1_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila2_GPIO_Port, OUT_fila2_Pin, 1);
			HAL_GPIO_WritePin(OUT_fila3_GPIO_Port, OUT_fila3_Pin, 0);
		default:
		break;
	}//end witch
}//end turnONFila

uint8_t lecturaColumna (uint8_t columNum){
	//acá se lee el pin correspondiente a la columna.
	//se devuelve un 1 o 0 según corresponda.
	switch (columNum){
		case 0:
			return HAL_GPIO_ReadPin(IN_colum0_GPIO_Port, IN_colum0_Pin);
		break;
		case 1:
			return HAL_GPIO_ReadPin(IN_colum1_GPIO_Port, IN_colum1_Pin);
		break;
		case 2:
			return HAL_GPIO_ReadPin(IN_colum2_GPIO_Port, IN_colum2_Pin);
		break;
		case 3:
			return HAL_GPIO_ReadPin(IN_colum3_GPIO_Port, IN_colum3_Pin);
		default:
		break;
	}//end witch
}//end lecturaColumna()

void lecturaTeclas (void){


	//pulsadores:
	last_teclas = read_teclas;
	spi_74HC165_receive(&read_teclas, 1);

	//teclado matricial
	last_teclado = read_teclado;
	read_teclado = 0;
	for (uint8_t i = 0; i < 4; i++){
		turnONFila(i);
		for (uint8_t j = 0; j < 4; j++){
			read_teclado += (lecturaColumna(j) << (i*4+j));
		}//end for j
	}//end for i

	read_input = read_teclado + (read_teclas << 16);
	last_input = last_teclado + (read_teclas << 16);

} //end lecturaTeclas()


void update_teclas(void){ //ESTADO DE LAS TECLAS CON LOGICA NEGATIVA


	fall_input = last_input & ~read_input;
	rise_input = ~last_input & read_input;



} //end teclas


T_INPUT getStatBoton (T_POS_INPUT b){

	if ( (fall_input & (1 << b)) != 0)
		return FALL;

	if (rise_input & (1 << b) != 0)
		return RISE;

	if (read_input & (1 << b) != 0)
		return HIGH_L;

	return LOW_L;

} //end getStatBoton()


int8_t getNumber (void){

	if (fall_input & (1 << IN_0))
			return 0;
	if (fall_input & (1 << IN_1))
			return 1;
	if (fall_input & (1 << IN_2))
			return 2;
	if (fall_input & (1 << IN_3))
			return 3;
	if (fall_input & (1 << IN_4))
			return 4;
	if (fall_input & (1 << IN_5))
			return 5;
	if (fall_input & (1 << IN_6))
			return 6;
	if (fall_input & (1 << IN_7))
			return 7;
	if (fall_input & (1 << IN_8))
			return 8;
	if (fall_input & (1 << IN_9))
			return 9;

	return -1;
} //end getNumber()
