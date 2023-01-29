/*
 * menu_tablero.h
 *
 *  Created on: Jan 30, 2022
 *      Author: Luciano Salvatore
 */

#ifndef INC_MENU_TABLERO_H_
#define INC_MENU_TABLERO_H_

#include "stm32f1xx_hal.h"

typedef struct t_menu {
	int8_t nombre;
	struct t_menu* menuAnterior;
	void (*inicia_menu) (void);
	void (*accion) (void);
} T_MENU;

typedef enum{
	MENU_PRINCIPAL,
	MENU_SET_LLENADO,
	MENU_SKIMMER,
	MENU_HIDRO,
	MENU_LUCES_EXT,
	MENU_LUCES_LED,
	MENU_CONFIG,
	SIZE_MENU_NOMBRE,
} T_MENU_NOMBRE;


typedef enum{
	PREGUNTA_SENSOR,
	CONECTE_SENSOR,
	COMPRUEBE_SENSOR,
	LLENANDO,
	LLENANDO_CHECK,
	PILETA_LLENA,

	ELIJE_ON_OFF_LLENADO,
	PERIODO_ON_LLENADO,
	PERIODO_OFF_LLENADO,
} T_STATUS_LLENADO;

typedef enum{
	COMPRUEBE_BOMBA,
	OPCIONES_SKIMMER,
	ELIJE_FRECUENCIA_SKIMMER,
	PERIODO_ON_SKIMMER,
	PERIODO_OFF_SKIMMER,
	ELIJE_PROGRAMA_SKIMMER,
	ELIJE_ON_OFF_HORARIO_SKIMMER,
	HORARIO_ON_SKIMMER,
	HORARIO_OFF_SKIMMER,
} T_STATUS_SKIMMER;

typedef enum{
	LIMITE_TIEMPO_HIDRO,
	ELIJE_TIEMPO_HIDRO,
} T_STATUS_HIDRO;

typedef enum{
	LIMITE_TIEMPO_LUCESEXT,
	ELIJE_TIEMPO_LUCESEXT,
} T_STATUS_LUCESEXT; //LUCES AMARILLAS


//funciones publicas
void start_menu (void);
void check_menu (void);
void check_pulsadores (void);
void check_duracionPulsadores (void);

#endif /* INC_MENU_TABLERO_H_ */
