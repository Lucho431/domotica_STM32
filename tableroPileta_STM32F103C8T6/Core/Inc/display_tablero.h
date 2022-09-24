/*
 * display_tablero.h
 *
 *  Created on: 10 abr. 2022
 *      Author: Luciano Salvatore
 */

#ifndef INC_DISPLAY_TABLERO_H_
#define INC_DISPLAY_TABLERO_H_

#include "stdio.h"

typedef enum{
	PANT_init_menuPrincipal,
	PANT_sensor_conectado,
	PANT_llenando_terminar,
	PANT_init_skimmer,
	PANT_init_hidro,
	PANT_init_lucesExt,
	PANT_init_setLlenado,
	PANT_PERIODO_ON_LLENADO,
	PANT_PERIODO_OFF_LLENADO,
	PANT_COMPRUEBE_SENSOR,
	PANT_CONECTE_SENSOR,
	PANT_PREGUNTA_SENSOR,
	PANT_LLENANDO,
	PANT_PILETA_LLENA,
	PANT_OPCIONES_SKIMMER,
	PANT_ELIJE_FRECUENCIA_SKIMMER,
	PANT_ELIJE_PROGRAMA_SKIMMER,
	PANT_PERIODO_ON_SKIMMER,
	PANT_PERIODO_OFF_SKIMMER,
	PANT_ELIJE_ON_OFF_HORARIO_SKIMMER,
	PANT_HORARIO_ON_SKIMMER,
	PANT_HORARIO_OFF_SKIMMER,
}T_PANTALLA;

void init_pantalla (void);
//void set_pantalla (char* texto);
void set_pantalla (T_PANTALLA);
void setTexto_pantalla (char*);
void pantallaLista(void);
void pantallaUpdate(void);

#endif /* INC_DISPLAY_TABLERO_H_ */
