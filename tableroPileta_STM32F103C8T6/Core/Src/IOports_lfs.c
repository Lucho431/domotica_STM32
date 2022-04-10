/*
 * botones_lfs.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Luciano Salvatore
 */

#include <IOports_lfs.h>
#include "74HC165_SPI_lfs.h"
#include "74_HC595_SPI_lfs.h"

//macros
#define GETBUTTONSTATUS(BUTTON,EDGE) (EDGE & (0B1 << (BUTTON)))



//variables
uint8_t flag_lecturas = 1;
uint8_t flag_salidas = 1;

//uint8_t read_teclas [SIZEOF_POS_BOTONES];
//uint8_t last_teclas [SIZEOF_POS_BOTONES];
T_INPUT status_teclas [SIZEOF_POS_INPUT];

uint8_t read_teclas;
uint8_t read_teclado[4];

uint32_t read_input, last_input, fall_input, rise_input;

uint16_t write_output;


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


void lecturaTeclas (void){

	last_input = read_input;

	//pulsadores:
	spi_74HC165_receiveBotones (&read_teclas, 1);

	//teclado matricial:

	for (uint8_t i = 0; i < 4; i++){
		turnONFila(i);
		spi_74HC165_receiveTeclado(&read_teclado[i], 1);
	}//end for i

	read_input = (uint32_t) (read_teclas |
							(read_teclado[0] << 6) |
							(read_teclado[1] << 10) |
							(read_teclado[2] << 14) |
							(read_teclado[3] << 18));

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


void setOutput (T_POS_OUTPUT s, uint8_t val){

	if (val != 0){
		write_output |= (uint16_t)(1 << s);
		return;
	}else{
		write_output &= ~( (uint16_t)(1 << s) );
	}

} //end setOutput()

void toggleOutput (T_POS_OUTPUT s){

	write_output ^= (uint16_t)(1 << s);

} //end toggleOutput()


void update_outputs (void){

	spi_74HC595_Transmit( (uint8_t*)&write_output, 2); // 2 bytes.

}
