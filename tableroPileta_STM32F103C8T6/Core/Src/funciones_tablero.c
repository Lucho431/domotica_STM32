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
#include "leds_tablero.h"

//variables texto
char texto[20];

//variables hora y fecha
RTC_DateTypeDef auxFecha;
RTC_TimeTypeDef auxHora;

//variables hidro
T_PROG_OUTPUT status_progHidro = PROG_IDLE;
int16_t tiempoHidroAux = 0;
int16_t tiempoHidro = 0;
uint8_t statusTiempoHidro = 0;
RTC_TimeTypeDef hora_hidro;
RTC_DateTypeDef fecha_hidro;

//variables llenado
T_PROG_OUTPUT status_progLlenado = PROG_IDLE;
int16_t tiempoLlenadoAux = 0;
int16_t tiempoLlenado_ON = 0;
int16_t tiempoLlenado_OFF = 0;
uint8_t statusTiempoLlenado = 99;
RTC_TimeTypeDef hora_llenadoOn;
RTC_DateTypeDef fecha_llenadoOn;
RTC_TimeTypeDef hora_llenadoOff;
RTC_DateTypeDef fecha_llenadoOff;
uint8_t flag_bombaNapa = 0;

//variables pileta/skimmer
T_PROG_OUTPUT status_progPileta = PROG_IDLE;
uint8_t status_modoPileta = 0; //0: apagado; 1: con programa; 2: sin programa.

/////////////////////////////////////////
//          INICIALIZADORES            //
/////////////////////////////////////////

T_PROG_OUTPUT setProg_hidro (T_PROG_CMD cmd){

	int8_t newNumber = getNumber();

	switch (statusTiempoHidro) {
		case 99: //imprime al entrar al sub-menu

			tiempoHidroAux = tiempoHidro;
			sprintf(texto, "%d min.", tiempoHidroAux);
			setTexto_pantalla(texto);
			if (!tiempoHidroAux){
				statusTiempoHidro = 0;
				break;
			}else if (tiempoHidroAux < 10){
				statusTiempoHidro = 1;
				break;
			}else{
				statusTiempoHidro = 2;
				break;
			}
		break;
		case 0: //tiempo en 0.
			if (getStatBoton(IN_AST) == FALL) {
				statusTiempoHidro = 99;
				return PROG_IDLE;
			}

			if (newNumber > 0) {
				tiempoHidroAux = newNumber;
				sprintf(texto, "%d min.", tiempoHidroAux);
				setTexto_pantalla(texto);
				statusTiempoHidro = 1;
			}
		break;
		case 1: //lote entre 1 y 9.
			if (getStatBoton(IN_AST) == FALL) {
				tiempoHidroAux = 0;
				sprintf(texto, "%d min.", tiempoHidroAux);
				setTexto_pantalla(texto);
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
				setTexto_pantalla(texto);
				statusTiempoHidro = 2;
				break;
			}

		break;
		case 2: //lote en 10 o más...
			if (getStatBoton(IN_AST) == FALL) {
				tiempoHidroAux /= 10;
				sprintf(texto, "%d min.", tiempoHidroAux);
				setTexto_pantalla(texto);
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
					setTexto_pantalla(texto);
					break;
				}
			}

		break;
		case 3: //retorna y graba el valor

			tiempoHidro = tiempoHidroAux;
			statusTiempoHidro = 99;

			return PROG_FINISHED;
		break;

		default:
			return PROG_ERROR;
		break;
	} //end switch statusTiempoHidro.

	return PROG_BUSY;

} //end setProg_hidro()


T_PROG_OUTPUT setProg_llenado (T_PROG_CMD cmd){

	int8_t newNumber = getNumber();

	switch (statusTiempoLlenado) {
		case 99: //imprime al entrar al sub-menu
			switch (cmd) {
				case PROG_SET1:
					tiempoLlenadoAux = tiempoLlenado_ON;
				break;
				case PROG_SET2:
					tiempoLlenadoAux = tiempoLlenado_OFF;
				default:
				break;
			} //end switch cmd
			sprintf(texto, "%d min.", tiempoLlenadoAux);
			setTexto_pantalla(texto);
			if (!tiempoLlenadoAux){
				statusTiempoLlenado = 0;
				break;
			}else if (tiempoLlenadoAux < 10){
				statusTiempoLlenado = 1;
				break;
			}else{
				statusTiempoLlenado = 2;
				break;
			}
		break;
		case 0: //tiempo en 0.
			if (getStatBoton(IN_AST) == FALL) {
				statusTiempoLlenado = 99;
				return PROG_IDLE;
			}

			if (newNumber > 0) {
				tiempoLlenadoAux = newNumber;
				sprintf(texto, "%d min.", tiempoLlenadoAux);
				setTexto_pantalla(texto);
				statusTiempoLlenado = 1;
			}
		break;
		case 1: //lote entre 1 y 9.
			if (getStatBoton(IN_AST) == FALL) {
				tiempoLlenadoAux = 0;
				sprintf(texto, "%d min.", tiempoLlenadoAux);
				setTexto_pantalla(texto);
				statusTiempoLlenado = 0;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoLlenado = 3;
				break;
			}

			if (newNumber != -1) {
				tiempoLlenadoAux *= 10;
				tiempoLlenadoAux += newNumber;
				sprintf(texto, "%d min.", tiempoLlenadoAux);
				setTexto_pantalla(texto);
				statusTiempoLlenado = 2;
				break;
			}

		break;
		case 2: //lote en 10 o más...
			if (getStatBoton(IN_AST) == FALL) {
				tiempoLlenadoAux /= 10;
				sprintf(texto, "%d min.", tiempoLlenadoAux);
				setTexto_pantalla(texto);
				if (tiempoLlenadoAux < 10) {
					statusTiempoLlenado = 1;
				}
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoLlenado = 3;
				break;
			}

			switch (cmd) {
				case PROG_SET1:
					if (tiempoLlenadoAux < 30) {
						if (newNumber != -1) {
							tiempoLlenadoAux *= 10;
							tiempoLlenadoAux += newNumber;
							sprintf(texto, "%d min.", tiempoLlenadoAux);
							setTexto_pantalla(texto);
							break;
						} //end if newNumber...
					} //end iftiempoLlenadoAux...
				break;
				case PROG_SET2:
					if (tiempoLlenadoAux < 15) {
						if (newNumber != -1) {
							tiempoLlenadoAux *= 10;
							tiempoLlenadoAux += newNumber;
							sprintf(texto, "%d min.", tiempoLlenadoAux);
							setTexto_pantalla(texto);
							break;
						} //end if newNumber...
					} //end iftiempoLlenadoAux...
				break;
				default:
				break;
			} //end switch cmd

		break;
		case 3: //retorna y graba el valor

			switch (cmd) {
				case PROG_SET1:
					tiempoLlenado_ON = tiempoLlenadoAux;
				break;
				case PROG_SET2:
					tiempoLlenado_OFF = tiempoLlenadoAux;
				default:
				break;
			} //end switch cmd

			statusTiempoLlenado = 99;
			return PROG_FINISHED;

		break;
		default:
			return PROG_ERROR;
		break;
	} //end switch statusTiempoLlenado.

	return PROG_BUSY;

} //end setProg_llenado()


T_PROG_OUTPUT setProg_skimmer (T_PROG_CMD cmd){
	__NOP();
} //end setProg_skimmer()


T_PROG_OUTPUT setProg_luz (T_PROG_CMD cmd){
	__NOP();
} //end setProg_luz()

/////////////////////////////////////////
//            ACCIONES                 //
/////////////////////////////////////////

T_PROG_OUTPUT runProg_hidro (T_PROG_CMD cmd){

	switch (cmd) {
		case PROG_RUN:

			setOutput(OUT_rele_jet, 1); //logica positiva

			set_led(OUT_led_jet, TITILA_LENTO);

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
				set_led(OUT_led_jet, APAGADO);

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}

			if (hora_hidro.Hours < auxHora.Hours){
				setOutput(OUT_rele_jet, 0); //logica positiva
				set_led(OUT_led_jet, APAGADO);

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}

			if (hora_hidro.Minutes < auxHora.Minutes){
				setOutput(OUT_rele_jet, 0); //logica positiva
				set_led(OUT_led_jet, APAGADO);

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
			}
		break;
		case PROG_STOP:
			setOutput(OUT_rele_jet, 0); //logica positiva
			set_led(OUT_led_jet, APAGADO);

//			return PROG_FINISHED;
			status_progHidro = PROG_IDLE;
		break;

		default:
		break;
	} //end switch progCmd_hidro

	return status_progHidro;
} //end runProg_hidro()

T_PROG_OUTPUT runProg_llenado (T_PROG_CMD cmd){

	switch (cmd) {
		case PROG_RUN:

			setOutput(OUT_rele_napa, 1); //logica positiva
			set_led(OUT_led_napa, TITILA_LENTO);
			flag_bombaNapa = 1;

			fecha_llenadoOn = get_fecha();
			hora_llenadoOn = get_hora();

			hora_llenadoOn.Minutes += tiempoLlenado_ON;
			if (hora_llenadoOn.Minutes > 59) {
				hora_llenadoOn.Minutes -= 59;
				hora_llenadoOn.Hours++;
				if (hora_llenadoOn.Hours > 23) {
					hora_llenadoOn.Hours -= 23;
					fecha_llenadoOn.Date++;
				}
			} //end if hora_llenadoOn...

			status_progLlenado = PROG_BUSY;

		break;
		case PROG_CHECK:

			if (status_progLlenado == PROG_IDLE) {
				//return PROG_IDLE;
				break;
			}

			auxFecha = get_fecha();
			auxHora = get_hora();
/*
			switch (flag_bombaNapa) {
				case 1:
					if (fecha_llenadoOn.Date < auxFecha.Date) {
						setOutput(OUT_rele_napa, 0); //logica positiva
						set_led(OUT_led_napa, PRENDIDO);
						flag_bombaNapa = 0;
					}

					if (hora_llenadoOn.Hours < auxHora.Hours) {
						setOutput(OUT_rele_napa, 0); //logica positiva
						set_led(OUT_led_napa, PRENDIDO);
						flag_bombaNapa = 0;
					}

					if (hora_llenadoOn.Minutes < auxHora.Minutes) {
						setOutput(OUT_rele_napa, 0); //logica positiva
						set_led(OUT_led_napa, PRENDIDO);
						flag_bombaNapa = 0;
					}

					if (!flag_bombaNapa) {
						fecha_llenadoOff = get_fecha();
						hora_llenadoOff = get_hora();

						hora_llenadoOff.Minutes += tiempoLlenado_OFF;
						if (hora_llenadoOff.Minutes > 59) {
							hora_llenadoOff.Minutes -= 59;
							hora_llenadoOff.Hours++;
							if (hora_llenadoOff.Hours > 23) {
								hora_llenadoOff.Hours -= 23;
								fecha_llenadoOff.Date++;
							}
						} //end if hora_llenadoOff...
					} //end if bombaNapa...
				break;
				case 0:
					if (fecha_llenadoOff.Date < auxFecha.Date) {
						setOutput(OUT_rele_napa, 1); //logica positiva
						set_led(OUT_led_napa, TITILA_LENTO);
						flag_bombaNapa = 1;
					}

					if (hora_llenadoOff.Hours < auxHora.Hours) {
						setOutput(OUT_rele_napa, 1); //logica positiva
						set_led(OUT_led_napa, TITILA_LENTO);
						flag_bombaNapa = 1;
					}

					if (hora_llenadoOff.Minutes < auxHora.Minutes) {
						setOutput(OUT_rele_napa, 1); //logica positiva
						set_led(OUT_led_napa, TITILA_LENTO);
						flag_bombaNapa = 1;
					}

					if (flag_bombaNapa != 0) {
						fecha_llenadoOn = get_fecha();
						hora_llenadoOn = get_hora();

						hora_llenadoOn.Minutes += tiempoLlenado_ON;
						if (hora_llenadoOn.Minutes > 59) {
							hora_llenadoOn.Minutes -= 59;
							hora_llenadoOn.Hours++;
							if (hora_llenadoOn.Hours > 23) {
								hora_llenadoOn.Hours -= 23;
								fecha_llenadoOn.Date++;
							}
						} //end if hora_llenadoOn...
					} //end if bombaNapa...
				default:
				break;
			} //end switch flag_bombaNapa...
*/
			__NOP();
		break;
		case PROG_STOP:
			setOutput(OUT_rele_napa, 0); //logica positiva
			set_led(OUT_led_napa, APAGADO);

			status_progLlenado = PROG_IDLE;
		break;

		default:
		break;
	} //end switch cmd

	if (!HAL_GPIO_ReadPin(IN_nivelAgua_GPIO_Port, IN_nivelAgua_Pin)) { //LOGICA NEGATIVA
		setOutput(OUT_rele_napa, 0); //logica positiva
		set_led(OUT_led_napa, APAGADO);
		return PROG_FINISHED;
	}
	return PROG_BUSY;

} //end runProg_llenado()

T_PROG_OUTPUT runProg_skimmer (T_PROG_CMD cmd){
	__NOP();

	switch (cmd) {
		case PROG_SET1: //con programa
			status_modoPileta = 1;
			status_progPileta = PROG_BUSY;
			set_led(OUT_led_pileta, TITILA_LENTO);
			break;
		case PROG_SET2: //sin programa
			status_modoPileta = 2;
			status_progPileta = PROG_BUSY;
			set_led(OUT_led_pileta, TITILA_RAPIDO);
			break;
		case PROG_STOP:
			setOutput(OUT_rele_pileta, 0); //LOGICA POSITIVA
			set_led(OUT_led_pileta, APAGADO);
			status_modoPileta = 0;
			status_progPileta = PROG_IDLE;
			break;
		case PROG_CHECK:
			if (status_progPileta == PROG_IDLE){
				//return PROG_IDLE;
				break;
			}

			switch (status_modoPileta) {
				case 0:

					break;
				case 1:
					setOutput(OUT_rele_pileta, 1); //logica positiva
					break;
				case 2:
					setOutput(OUT_rele_pileta, 1); //logica positiva
					break;
				default:
					break;
			}

		default:
			break;
	} //end switch cmd

	return status_progPileta;
}

T_PROG_OUTPUT runProg_luz (T_PROG_CMD cmd){
	__NOP();
}
