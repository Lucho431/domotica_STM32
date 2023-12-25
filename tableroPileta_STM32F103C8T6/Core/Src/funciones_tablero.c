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
uint8_t statusTiempoHidro = 99;
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
int16_t tiempoSkimmerAux = 0;
int16_t tiempoSkimmer_ON = 0;
int16_t tiempoSkimmer_OFF = 0;
uint8_t statusTiempoSkimmer = 99; //son los pasos intermedios para la entrada numerica de la programacion
RTC_TimeTypeDef hora_skimmerOn;
RTC_DateTypeDef fecha_skimmerOn;
RTC_TimeTypeDef hora_skimmerOff;
RTC_DateTypeDef fecha_skimmerOff;
int16_t progSkimmerAux = 0;
int16_t progSkimmerAux2 = 0;
int16_t prog1_Skimmer_ON = 0; // almancena en formato HH:MM la hora ON
int16_t prog1_Skimmer_OFF = 0; // almancena en formato HH:MM la hora OFF
int16_t prog2_Skimmer_ON = 0; // almancena en formato HH:MM la hora ON
int16_t prog2_Skimmer_OFF = 0; // almancena en formato HH:MM la hora OFF
//uint8_t statusProgSkimmer = 99;
RTC_TimeTypeDef hora_skimmerProg1_On;
RTC_DateTypeDef fecha_skimmerProg1_On;
RTC_TimeTypeDef hora_skimmerProg1_Off;
RTC_DateTypeDef fecha_skimmerProg1_Off;
RTC_TimeTypeDef hora_skimmerProg2_On;
RTC_DateTypeDef fecha_skimmerProg2_On;
RTC_TimeTypeDef hora_skimmerProg2_Off;
RTC_DateTypeDef fecha_skimmerProg2_Off;
uint8_t progSkimmer_activo = 0; //indica cual programa se encuentra en la franja horaria actual. 0: ninguno; 1: prog1; 2: prog2; 3: ambos.
uint8_t flag_skimmer = 0;

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

	int8_t newNumber = getNumber();

	switch (statusTiempoSkimmer) {
		case 99: //imprime al entrar al sub-menu
			switch (cmd) {
				case PROG_SET1:
					tiempoSkimmerAux = tiempoSkimmer_ON;

					sprintf(texto, "%d min.  ", tiempoSkimmerAux);
					setTexto_pantalla(texto);
					if (!tiempoSkimmerAux){
						statusTiempoSkimmer = 0;
						break;
					}else if (tiempoSkimmerAux < 10){
						statusTiempoSkimmer = 1;
						break;
					}else{
						statusTiempoSkimmer = 2;
						break;
					} //end if !tiempoSkimmerAux

				break;
				case PROG_SET2:
					tiempoSkimmerAux = tiempoSkimmer_OFF;

					sprintf(texto, "%d min.  ", tiempoSkimmerAux);
					setTexto_pantalla(texto);
					if (!tiempoSkimmerAux){
						statusTiempoSkimmer = 0;
						break;
					}else if (tiempoSkimmerAux < 10){
						statusTiempoSkimmer = 1;
						break;
					}else{
						statusTiempoSkimmer = 2;
						break;
					} //end if !tiempoSkimmerAux

				break;
				case PROG_SET3:
					progSkimmerAux = prog1_Skimmer_ON;

					sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
					setTexto_pantalla(texto);
					if (!progSkimmerAux){
						statusTiempoSkimmer = 10;
						break;
					}else if (progSkimmerAux < 10){
						statusTiempoSkimmer = 11;
						break;
					}else{
						statusTiempoSkimmer = 12;
						break;
					} //end if !tiempoSkimmerAux

				break;
				case PROG_SET4:
					progSkimmerAux = prog1_Skimmer_OFF;

					sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
					setTexto_pantalla(texto);
					if (!progSkimmerAux){
						statusTiempoSkimmer = 10;
						break;
					}else if (progSkimmerAux < 10){
						statusTiempoSkimmer = 11;
						break;
					}else{
						statusTiempoSkimmer = 12;
						break;
					} //end if !tiempoSkimmerAux

				break;
				case PROG_SET5:
					progSkimmerAux = prog2_Skimmer_ON;

					sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
					setTexto_pantalla(texto);
					if (!progSkimmerAux){
						statusTiempoSkimmer = 10;
						break;
					}else if (progSkimmerAux < 10){
						statusTiempoSkimmer = 11;
						break;
					}else{
						statusTiempoSkimmer = 12;
						break;
					} //end if !tiempoSkimmerAux

				break;
				case PROG_SET6:
					progSkimmerAux = prog2_Skimmer_OFF;

					sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
					setTexto_pantalla(texto);
					if (!progSkimmerAux){
						statusTiempoSkimmer = 10;
						break;
					}else if (progSkimmerAux < 10){
						statusTiempoSkimmer = 11;
						break;
					}else{
						statusTiempoSkimmer = 12;
						break;
					} //end if !tiempoSkimmerAux

				default:
				break;
			} //end switch cmd

		break;
		case 0: //tiempo en 0.
			if (getStatBoton(IN_AST) == FALL) {
				statusTiempoSkimmer = 99;
				return PROG_IDLE;
			}

			if (newNumber > 0) {
				tiempoSkimmerAux = newNumber;
				sprintf(texto, "%d min.  ", tiempoSkimmerAux);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 1;
			}
		break;
		case 1: //lote entre 1 y 9.
			if (getStatBoton(IN_AST) == FALL) {
				tiempoSkimmerAux = 0;
				sprintf(texto, "%d min.  ", tiempoSkimmerAux);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 0;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 3;
				break;
			}

			if (newNumber != -1) {
				tiempoSkimmerAux *= 10;
				tiempoSkimmerAux += newNumber;
				sprintf(texto, "%d min.  ", tiempoSkimmerAux);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 2;
				break;
			}

		break;
		case 2: //lote en 10 o más...
			if (getStatBoton(IN_AST) == FALL) {
				tiempoSkimmerAux /= 10;
				sprintf(texto, "%d min.  ", tiempoSkimmerAux);
				setTexto_pantalla(texto);
				if (tiempoSkimmerAux < 10) {
					statusTiempoSkimmer = 1;
				}
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 3;
				break;
			}

			switch (cmd) {
				case PROG_SET1:
					if (tiempoSkimmerAux < 30) {
						if (newNumber != -1) {
							tiempoSkimmerAux *= 10;
							tiempoSkimmerAux += newNumber;
							sprintf(texto, "%d min.  ", tiempoSkimmerAux);
							setTexto_pantalla(texto);
							break;
						} //end if newNumber...
					} //end if tiempoSkimmerAux...
				break;
				case PROG_SET2:
					if (tiempoSkimmerAux < 15) {
						if (newNumber != -1) {
							tiempoSkimmerAux *= 10;
							tiempoSkimmerAux += newNumber;
							sprintf(texto, "%d min.  ", tiempoSkimmerAux);
							setTexto_pantalla(texto);
							break;
						} //end if newNumber...
					} //end iftiempoSkimmerAux...
				break;
				default:
				break;
			} //end switch cmd

		break;
		case 3: //retorna y graba el valor

			switch (cmd) {
				case PROG_SET1:
					tiempoSkimmer_ON = tiempoSkimmerAux;
				break;
				case PROG_SET2:
					tiempoSkimmer_OFF = tiempoSkimmerAux;
				default:
				break;
			} //end switch cmd

			statusTiempoSkimmer = 99;
			return PROG_FINISHED;

		break;
		case 10: //hora en 0. Introduce la decena de hora
			if (getStatBoton(IN_AST) == FALL) {
				statusTiempoSkimmer = 99;
				return PROG_IDLE;
			}

			if (newNumber > -1 && newNumber < 3) {
				progSkimmerAux = newNumber * 1000;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 11;
			}
		break;
		case 11: //introduce la unidad de hora
			if (getStatBoton(IN_AST) == FALL) {
				progSkimmerAux = 0;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 10;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 33;
				break;
			}

			if (newNumber > -1) {
				progSkimmerAux += (newNumber * 100);
				if (progSkimmerAux > 2300) progSkimmerAux = 2300;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 12;
				break;
			}

		break;
		case 12: //introduce la decena de minutos
			if (getStatBoton(IN_AST) == FALL) {
				progSkimmerAux = (progSkimmerAux / 1000) * 1000;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 11;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 33;
				break;
			}

			if (newNumber != -1 && newNumber < 6) {
				progSkimmerAux += (newNumber * 10);
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 13;
				break;
			}
		break;
		case 13: //introduce la unidad de minutos
			if (getStatBoton(IN_AST) == FALL) {
				progSkimmerAux = (progSkimmerAux / 100) * 100;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 12;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 33;
				break;
			}

			if (newNumber != -1) {
				progSkimmerAux += newNumber;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 14;
				break;
			}
		break;
		case 14: //minutos completos
			if (getStatBoton(IN_AST) == FALL) {
				progSkimmerAux = (progSkimmerAux / 10) * 10;
				sprintf(texto, "   %02d:%02d", progSkimmerAux / 100, progSkimmerAux % 100);
				setTexto_pantalla(texto);
				statusTiempoSkimmer = 13;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				statusTiempoSkimmer = 33;
				break;
			}
		break;
		case 33: //retorna y graba el valor

			switch (cmd) {
				case PROG_SET3:
					prog1_Skimmer_ON = progSkimmerAux;
				break;
				case PROG_SET4:
					prog1_Skimmer_OFF = progSkimmerAux;
				break;
				case PROG_SET5:
					prog2_Skimmer_ON = progSkimmerAux;
				break;
				case PROG_SET6:
					prog2_Skimmer_OFF = progSkimmerAux;
				break;
				default:
				break;
			} //end switch cmd

			statusTiempoSkimmer = 99;
			return PROG_FINISHED;

		break;
		default:
			return PROG_ERROR;
		break;
	} //end switch statusTiempoLlenado.

	return PROG_BUSY;

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
				break;
			}

			if (hora_hidro.Hours < auxHora.Hours){
				setOutput(OUT_rele_jet, 0); //logica positiva
				set_led(OUT_led_jet, APAGADO);

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
				break;
			}

			if (hora_hidro.Minutes < auxHora.Minutes){
				setOutput(OUT_rele_jet, 0); //logica positiva
				set_led(OUT_led_jet, APAGADO);

//				return PROG_FINISHED;
				status_progHidro = PROG_IDLE;
				break;
			}

			if (hora_hidro.Minutes == auxHora.Minutes){
				if (hora_hidro.Seconds < auxHora.Seconds){
					setOutput(OUT_rele_jet, 0); //logica positiva
					set_led(OUT_led_jet, APAGADO);

//					return PROG_FINISHED;
					status_progHidro = PROG_IDLE;
					break;
				}
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

					if (hora_llenadoOn.Minutes == auxHora.Minutes) {
						if (hora_llenadoOn.Seconds < auxHora.Seconds) {
							setOutput(OUT_rele_napa, 0); //logica positiva
							set_led(OUT_led_napa, PRENDIDO);
							flag_bombaNapa = 0;
						}
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

					if (hora_llenadoOff.Minutes == auxHora.Minutes) {
						if (hora_llenadoOff.Seconds < auxHora.Seconds){
							setOutput(OUT_rele_napa, 1); //logica positiva
							set_led(OUT_led_napa, TITILA_LENTO);
							flag_bombaNapa = 1;
						}
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

//			__NOP();
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

			flag_skimmer = 0;
			fecha_skimmerOff = get_fecha();
			hora_skimmerOff = get_hora();

//			hora_skimmerOff.Seconds++;
//			if (hora_skimmerOff.Seconds > 59) {
//				hora_skimmerOff.Seconds -= 59;
//				hora_skimmerOff.Minutes++;
//				if (hora_skimmerOff.Minutes > 59) {
//					hora_skimmerOff.Minutes -= 59;
//					hora_skimmerOff.Hours++;
//					if (hora_skimmerOff.Hours > 23) {
//						hora_skimmerOff.Hours -= 23;
//						fecha_skimmerOff.Date++;
//					}
//				}
//			} //end if hora_skimmerOff...

			setOutput(OUT_rele_pileta, 0); //LOGICA POSITIVA
			set_led(OUT_led_pileta, PRENDIDO);
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
					setOutput(OUT_rele_pileta, 0); //logica positiva
					break;
				case 1:
					auxFecha = get_fecha();
					auxHora = get_hora();

					//compruebo estar en el prog1
					progSkimmer_activo = 0;
					if ( (prog1_Skimmer_ON / 100) <= auxHora.Hours){
						if ( (prog1_Skimmer_ON % 100) <= auxHora.Minutes){
							progSkimmer_activo = 1;
						} //end if auxHora.Minutes
					} //end if auxHora.Hours
					if ( (prog1_Skimmer_OFF / 100) <= auxHora.Hours){
						if ( (prog1_Skimmer_OFF % 100) <= auxHora.Minutes){
							progSkimmer_activo = 0;
						} //end if auxHora.Minutes
					} //end if auxHora.Hours

					//compruebo estar en el prog2
					if ( (prog2_Skimmer_ON / 100) <= auxHora.Hours){
						if ( (prog2_Skimmer_ON % 100) <= auxHora.Minutes){
//							progSkimmer_activo += 2;
							progSkimmer_activo |= 0b10;
						} //end if auxHora.Minutes
					} //end if auxHora.Hours
					if ( (prog2_Skimmer_OFF / 100) <= auxHora.Hours){
						if ( (prog2_Skimmer_OFF % 100) <= auxHora.Minutes){
//							progSkimmer_activo -= 2;
							progSkimmer_activo &= (uint8_t) (~0b10);
						} //end if auxHora.Minutes
					} //end if auxHora.Hours

					//defino si prendo o apago la salida
					if (progSkimmer_activo != 0){

						auxFecha = get_fecha();
						auxHora = get_hora();

						switch (flag_skimmer){
							case 0:
								if (fecha_skimmerOff.Date < auxFecha.Date) {
									setOutput(OUT_rele_pileta, 1); //logica positiva
									set_led(OUT_led_pileta, TITILA_LENTO);
									flag_skimmer = 1;
								}

								if (hora_skimmerOff.Hours < auxHora.Hours) {
									setOutput(OUT_rele_pileta, 1); //logica positiva
									set_led(OUT_led_pileta, TITILA_LENTO);
									flag_skimmer = 1;
								}

								if (hora_skimmerOff.Minutes < auxHora.Minutes) {
									setOutput(OUT_rele_pileta, 1); //logica positiva
									set_led(OUT_led_pileta, TITILA_LENTO);
									flag_skimmer = 1;
								}

								if (hora_skimmerOff.Minutes == auxHora.Minutes) {
									if (hora_skimmerOff.Seconds < auxHora.Seconds){
										setOutput(OUT_rele_pileta, 1); //logica positiva
										set_led(OUT_led_pileta, TITILA_LENTO);
										flag_skimmer = 1;
									}
								}

								if (flag_skimmer != 0) {
									fecha_skimmerOn = auxFecha;
									hora_skimmerOn = auxHora;

									hora_skimmerOn.Minutes += tiempoSkimmer_ON;
									if (hora_skimmerOn.Minutes > 59) {
										hora_skimmerOn.Minutes -= 59;
										hora_skimmerOn.Hours++;
										if (hora_skimmerOn.Hours > 23) {
											hora_skimmerOn.Hours -= 23;
											fecha_skimmerOn.Date++;
										}
									} //end if hora_skimmerOn...
								} //end if flag_skimmer...

							break;
							case 1:
								if (fecha_skimmerOn.Date < auxFecha.Date) {
									setOutput(OUT_rele_pileta, 0); //logica positiva
									set_led(OUT_led_pileta, PRENDIDO);
									flag_skimmer = 0;
								}

								if (hora_skimmerOn.Hours < auxHora.Hours) {
									setOutput(OUT_rele_pileta, 0); //logica positiva
									set_led(OUT_led_pileta, PRENDIDO);
									flag_skimmer = 0;
								}

								if (hora_skimmerOn.Minutes < auxHora.Minutes) {
									setOutput(OUT_rele_pileta, 0); //logica positiva
									set_led(OUT_led_pileta, PRENDIDO);
									flag_skimmer = 0;
								}

								if (hora_skimmerOn.Minutes == auxHora.Minutes) {
									if (hora_skimmerOn.Seconds < auxHora.Seconds) {
										setOutput(OUT_rele_pileta, 0); //logica positiva
										set_led(OUT_led_pileta, PRENDIDO);
										flag_skimmer = 0;
									}
								}

								if (!flag_skimmer) {
									fecha_skimmerOff = auxFecha;
									hora_skimmerOff = auxHora;

									hora_skimmerOff.Minutes += tiempoSkimmer_OFF;
									if (hora_skimmerOff.Minutes > 59) {
										hora_skimmerOff.Minutes -= 59;
										hora_skimmerOff.Hours++;
										if (hora_skimmerOff.Hours > 23) {
											hora_skimmerOff.Hours -= 23;
											fecha_skimmerOff.Date++;
										}
									} //end if hora_skimmerOff...
								} //end if flag_skimmer...

							break;
							default:
							break;
						} //end switch flag_skimmer

					}else{
						setOutput(OUT_rele_pileta, 0); //logica positiva
						set_led(OUT_led_pileta, PRENDIDO);
						flag_skimmer = 0;
					} //end if progSkimmer_activo

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
