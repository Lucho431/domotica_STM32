/*
 * funciones_tablero.h
 *
 *  Created on: 29 jun. 2022
 *      Author: Luciano Salvatore
 */

#ifndef INC_FUNCIONES_TABLERO_H_
#define INC_FUNCIONES_TABLERO_H_

#include "main.h"

typedef enum{
	PROG_VOID,
	PROG_CHECK,
	PROG_RUN,
	PROG_STOP,
}T_PROG_CMD;

typedef enum{
	PROG_ERROR,
	PROG_IDLE,
	PROG_BUSY,
	PROG_FINISHED,
}T_PROG_OUTPUT;




T_PROG_OUTPUT setProg_hidro (T_PROG_CMD);
T_PROG_OUTPUT setProg_llenado (T_PROG_CMD);
T_PROG_OUTPUT setProg_skimmer (T_PROG_CMD);
T_PROG_OUTPUT setProg_luz (T_PROG_CMD);

T_PROG_OUTPUT runProg_hidro (T_PROG_CMD);
T_PROG_OUTPUT runProg_llenado (T_PROG_CMD);
T_PROG_OUTPUT runProg_skimmer (T_PROG_CMD);
T_PROG_OUTPUT runProg_luz (T_PROG_CMD);

#endif /* INC_FUNCIONES_TABLERO_H_ */
