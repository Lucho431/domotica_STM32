/*
 * display_tablero.c
 *
 *  Created on: 10 abr. 2022
 *      Author: Luciano Salvatore
 */

#include "display_tablero.h"
#include "ST7920_SERIAL.h"

uint8_t flag_pantalla = 0;

void set_pantalla (char* texto){
	ST7920_SendString(0, 0, texto);
	flag_pantalla = 1;
}

void pantallaLista(void){
	flag_pantalla = 1;
}

void pantallaUpdate(void){
	if (flag_pantalla != 0){
		ST7920_Update();
		flag_pantalla = 0;
	}
} //end pantallaUpdate()
