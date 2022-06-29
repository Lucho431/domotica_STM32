/*
 * funciones_tablero.h
 *
 *  Created on: 29 jun. 2022
 *      Author: Luciano Salvatore
 */

#ifndef INC_FUNCIONES_TABLERO_H_
#define INC_FUNCIONES_TABLERO_H_

typedef enum{
	PROG_VOID,
	PROG_CHECK,
	PROG_RUN,
	PROG_STOP,
}T_PROG_CMD;


uint8_t setProg_hidro (void);
uint8_t setProg_llenado (void);
uint8_t setProg_skimmer (void);
uint8_t setProg_luz (void);

uint8_t runProg_hidro (void);
uint8_t runProg_llenado (void);
uint8_t runProg_skimmer (void);
uint8_t runProg_luz (void);

#endif /* INC_FUNCIONES_TABLERO_H_ */
