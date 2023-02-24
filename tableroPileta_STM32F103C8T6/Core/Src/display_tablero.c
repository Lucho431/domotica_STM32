/*
 * display_tablero.c
 *
 *  Created on: 10 abr. 2022
 *      Author: Luciano Salvatore
 */

#include "display_tablero.h"
#include "ST7920_SERIAL.h"
#include "delay.h"

uint8_t flag_pantalla = 0;

void init_pantalla (void){
	delay_init();
	ST7920_Init();
}

//void set_pantalla (char* texto){
//	ST7920_Clear();
//	ST7920_SendString(0, 0, texto);
//	//flag_pantalla = 1;
//}

void set_pantalla (T_PANTALLA p){
	ST7920_Clear();
	switch (p){
		case PANT_init_menuPrincipal:
			ST7920_SendString(0, 0, "Estoy en el menu");
			ST7920_SendString(1, 0, "principal.");
		break;
		case PANT_sensor_conectado:
			ST7920_SendString(0, 0, "Sensor");
			ST7920_SendString(1, 0, "conectado?");
		break;
		case PANT_llenando_terminar:
			ST7920_SendString(0, 0, "Llenando.");
			ST7920_SendString(1, 0, "Terminar?");
		break;
		case PANT_init_skimmer:
			ST7920_SendString(0, 0, "COMPRUEBE EL");
			ST7920_SendString(1, 0, "ESTADO DE LA");
			ST7920_SendString(2, 0, "BOMBA");
		break;
		case PANT_init_hidro:
			ST7920_SendString(0, 0, "defina el tiempo");
			ST7920_SendString(1, 0, "del hidromasaje:");
		break;
		case PANT_init_lucesExt:
			ST7920_SendString(0, 0, "LIMITE DE TIEMPO");
		break;
		case PANT_init_setLlenado:
			ST7920_SendString(0, 0, "Config. llenado");
			ST7920_SendString(1, 0, "1: duracion ON");
			ST7920_SendString(2, 0, "2: duracion OFF");
		break;
		case PANT_PERIODO_ON_LLENADO:
			ST7920_SendString(0, 0, "defina el tiempo");
			ST7920_SendString(1, 0, "de duracion ON:");
		break;
		case PANT_PERIODO_OFF_LLENADO:
			ST7920_SendString(0, 0, "defina el tiempo");
			ST7920_SendString(1, 0, "de duracion OFF:");
		break;
		case PANT_COMPRUEBE_SENSOR:
			ST7920_SendString(0, 0, "COMPRUEBE EL");
			ST7920_SendString(1, 0, "SENSOR Y PULSE");
			ST7920_SendString(2, 0, "EL BOTON...");
		break;
		case PANT_CONECTE_SENSOR:
			ST7920_SendString(0, 0, "CONECTE EL");
			ST7920_SendString(1, 0, "SENSOR");
		break;
		case PANT_PREGUNTA_SENSOR:
			ST7920_SendString(0, 0, "Sensor");
			ST7920_SendString(1, 0, "conectado?");
		break;
		case PANT_LLENANDO:
			ST7920_SendString(0, 0, "Llenando...");
		break;
		case PANT_PILETA_LLENA:
			ST7920_SendString(0, 0, "Pileta llena.");
		break;
		case PANT_OPCIONES_SKIMMER:
			ST7920_SendString(0, 0, "Elija freecuecia");
			ST7920_SendString(1, 0, "u horario");
		break;
		case PANT_ELIJE_FRECUENCIA_SKIMMER:
			ST7920_SendString(0, 0, "Elijo timepo ON");
			ST7920_SendString(1, 0, "o timepo OFF");
		break;
		case PANT_ELIJE_PROGRAMA_SKIMMER:
			ST7920_SendString(0, 0, "Elijo programa 1");
			ST7920_SendString(1, 0, "o programa 2");
		break;
		case PANT_PERIODO_ON_SKIMMER:
			ST7920_SendString(0, 0, "Defina el tiempo");
			ST7920_SendString(1, 0, "de duracion ON:");
		break;
		case PANT_PERIODO_OFF_SKIMMER:
			ST7920_SendString(0, 0, "Defina el tiempo");
			ST7920_SendString(1, 0, "de duracion OFF:");
		break;
		case PANT_ELIJE_ON_OFF_HORARIO_SKIMMER:
			ST7920_SendString(0, 0, "Elije horario ON");
			ST7920_SendString(1, 0, "u horario OFF");
		break;
		case PANT_HORARIO_ON_SKIMMER:
			ST7920_SendString(0, 0, "Defina el");
			ST7920_SendString(1, 0, "horario ON");
		break;
		case PANT_HORARIO_OFF_SKIMMER:
			ST7920_SendString(0, 0, "Defina el");
			ST7920_SendString(1, 0, "horario OFF");
		break;

	} //end switch p
//	flag_pantalla = 1;
} //end set_pantalla()

void setTexto_pantalla (char* texto){
	//ST7920_Clear();
	ST7920_SendString(2, 0, texto);
	//flag_pantalla = 1;
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
