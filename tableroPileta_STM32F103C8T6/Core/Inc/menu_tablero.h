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
	MENU_LLENADO,
	MENU_SKIMMER,
	MENU_HIDRO,
	MENU_LUCES_EXT,
	MENU_LUCES_LED,
	MENU_CONFIG,
	SIZE_MENU_NOMBRE,
} T_MENU_NOMBRE;



#endif /* INC_MENU_TABLERO_H_ */
