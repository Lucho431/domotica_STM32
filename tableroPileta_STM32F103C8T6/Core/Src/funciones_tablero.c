/*
 * funciones_tablero.c
 *
 *  Created on: 29 jun. 2022
 *      Author: Luciano Salvatore
 */

#include "funciones_tablero.h"
#include "menu_tablero.h"
#include "display_tablero.h"
#include "IOports_lfs.h"
#include "hora_tablero.h"

//variables texto
char texto[20];

//variables hora y fecha
RTC_DateTypeDef auxFecha;
RTC_TimeTypeDef auxHora;

//variables hidro
T_PROG_OUTPUT status_progHidro;
int16_t tiempoHidroAux = 0;
int16_t tiempoHidro = 0;
uint8_t statusTiempoHidro = 0;
RTC_TimeTypeDef hora_hidro;
RTC_DateTypeDef fecha_hidro;


/////////////////////////////////////////
//          INICIALIZADORES            //
/////////////////////////////////////////

T_PROG_OUTPUT setProg_hidro (T_PROG_CMD cmd){

	int8_t newNumber = getNumber();

	switch (statusTiempoHidro) {
		case 0: //tiempo en 0.
			if (getStatBoton(IN_AST) == FALL) {
//			menuActual = menuActual->menuPadre;
//			HMI_setPage(menuActual->nombre);
				return -1;
			}

			if (newNumber > 0) {
				tiempoHidroAux = newNumber;
				sprintf(texto, "%d min.", tiempoHidroAux);
				set_pantalla(texto);
				statusTiempoHidro = 1;
			}
		break;
		case 1: //lote entre 1 y 9.
			if (getStatBoton(IN_AST) == FALL) {
				tiempoHidroAux = 0;
				sprintf(texto, "%d min.", tiempoHidroAux);
				set_pantalla(texto);
				statusTiempoHidro = 0;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoHidro = 3;
				break;
			}

			if (newNumber != -1) {
				tiempoHidroAux *= 10;
				tiempoHidroAux += newNumber;
				sprintf(texto, "%d min.", tiempoHidroAux);
				set_pantalla(texto);
				statusTiempoHidro = 2;
				break;
			}

		break;
		case 2: //lote en 10 o más...
			if (getStatBoton(IN_AST) == FALL) {
				tiempoHidroAux /= 10;
				sprintf(texto, "%d min.", tiempoHidroAux);
				set_pantalla(texto);
				if (statusTiempoHidro < 10) {
					statusTiempoHidro = 1;
				}
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoHidro = 3;
				break;
			}

			if (tiempoHidroAux < 120) {
				if (newNumber != -1) {
					tiempoHidroAux *= 10;
					tiempoHidroAux += newNumber;
					sprintf(texto, "%d min.", tiempoHidroAux);
					set_pantalla(texto);
					break;
				}
			}

		break;
		case 3: //retorna y graba el valor

			tiempoHidro = tiempoHidroAux;

			return 1;
		break;

		default:
		break;
	} //end switch statusNumLote.


	return 0;
}

T_PROG_OUTPUT setProg_llenado (T_PROG_CMD cmd){

	if (cmd != PROG_RUN) return PROG_ERROR;

	if (!HAL_GPIO_ReadPin(IN_nivelAgua_GPIO_Port, IN_nivelAgua_Pin)) { //LOGICA NEGATIVA
		return PROG_ERROR;
	}

	setOutput(OUT_rele_napa, 1); //logica positiva
	//falta manejar los led

	return PROG_FINISHED;
}

T_PROG_OUTPUT setProg_skimmer (T_PROG_CMD cmd){
	__NOP();
}
T_PROG_OUTPUT setProg_luz (T_PROG_CMD cmd){
	__NOP();
}

/////////////////////////////////////////
//            ACCIONES                 //
/////////////////////////////////////////

T_PROG_OUTPUT runProg_hidro (T_PROG_CMD cmd){

	switch (cmd) {
		case PROG_RUN:

			setOutput(OUT_rele_jet, 1); //logica positiva

			//falta poner el led

			fecha_hidro = get_fecha();
			hora_hidro = get_hora();

			hora_hidro.Minutes += tiempoHidro;
			if (hora_hidro.Minutes > 59){
				hora_hidro.Minutes -= 59;
				hora_hidro.Hours++;
				if (hora_hidro.Hours > 23){
					hora_hidro.Hours -= 23;
					fecha_hidro.Date++;
				}
			} //end if hora_hidro...

			status_progHidro = PROG_BUSY;

		break;
		case PROG_CHECK:

			if (status_progHidro == PROG_IDLE){
				//return PROG_IDLE;
				break;
			}

			auxFecha = get_fecha();
			auxHora = get_hora();

			if (fecha_hidro.Date < auxFecha.Date){
				setOutput(OUT_rele_jet, 0); //logica positiva
				//falta manejar el led

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}

			if (hora_hidro.Hours < auxHora.Hours){
				setOutput(OUT_rele_jet, 0); //logica positiva
				//falta manejar el led

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}

			if (hora_hidro.Minutes < auxHora.Minutes){
				setOutput(OUT_rele_jet, 0); //logica positiva
				//falta manejar el led

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}
		break;
		case PROG_STOP:
			setOutput(OUT_rele_jet, 0); //logica positiva
			//falta manejar el led

//			return PROG_FINISHED;
			status_progHidro = PROG_IDLE;
		break;

		default:
		break;
	} //end switch progCmd_hidro

	return status_progHidro;
} //end runProg_hidro()

T_PROG_OUTPUT runProg_llenado (T_PROG_CMD cmd){

	if (!HAL_GPIO_ReadPin(IN_nivelAgua_GPIO_Port, IN_nivelAgua_Pin)) { //LOGICA NEGATIVA
		setOutput(OUT_rele_napa, 0); //logica positiva
		//falta manejar los led
		return PROG_FINISHED;
	}
	return PROG_BUSY;
}

T_PROG_OUTPUT runProg_skimmer (T_PROG_CMD cmd){
	__NOP();
}

T_PROG_OUTPUT runProg_luz (T_PROG_CMD cmd){
	__NOP();
}
