/*
 * botones_lfs.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Luciano Salvatore
 */

#include <IOports_lfs.h>

//variables
uint8_t flag_lecturas = 1;
uint8_t read_teclas [SIZEOF_POS_INPUT];
uint8_t last_teclas [SIZEOF_POS_INPUT];
T_INPUT status_teclas [SIZEOF_POS_INPUT];

void lecturaTeclas (void){

//	read_teclas[IN_pileta] = HAL_GPIO_ReadPin(IN_pileta_GPIO_Port, IN_pileta_Pin);
//	read_teclas[IN_jet] = HAL_GPIO_ReadPin(IN_jet_GPIO_Port, IN_jet_Pin);
//	read_teclas[IN_napa] = HAL_GPIO_ReadPin(IN_napa_GPIO_Port, IN_napa_Pin);
//	read_teclas[IN_leds] = HAL_GPIO_ReadPin(IN_leds_GPIO_Port, IN_leds_Pin);
//	read_teclas[IN_luces] = HAL_GPIO_ReadPin(IN_luces_GPIO_Port, IN_luces_Pin);
//	read_teclas[IN_tomas] = HAL_GPIO_ReadPin(IN_tomas_GPIO_Port, IN_tomas_Pin);

}


void update_teclas(void){ //ESTADO DE LAS TECLAS CON LOGICA NEGATIVA

	for (uint8_t i = IN_tomas; i < SIZEOF_POS_INPUT; i++){
		if(!read_teclas[i]){
			if(last_teclas[i]){
				status_teclas[i] = FALL;
			}else{
				status_teclas[i] = LOW_L;
			}
		}else{
			if(last_teclas[i]){
				status_teclas[i] = HIGH_L;
			}else{
				status_teclas[i] = RISE;
			}
		}//end if !read_teclas[]
		last_teclas[i] = read_teclas[i];
	}//end for i
} //end teclas


T_INPUT getStatBoton (T_POS_INPUT b){

	return status_teclas[b];
} //end getStatBoton()


//int8_t getNumber (void){
//
//	for (uint8_t i = IN_1; i < IN_AST; i++){
//		if (status_teclas[i] == FALL)
//			return (i+1);
//	}
//
//	if (status_teclas[IN_0] == FALL)
//		return (0);
//
//	return (-1);
//}
