/*
 * menu_tablero.c
 *
 *  Created on: Jan 30, 2022
 *      Author: Luciano Salvatore
 */

#include "menu_tablero.h"
#include "IOports_lfs.h"
#include "display_tablero.h"
#include "funciones_tablero.h"
#include "leds_tablero.h"
#include "hora_tablero.h"


//variables menu
T_MENU* menuActual;
T_MENU* menuAux;
char texto[20];
//variables tomas
uint8_t flag_tomas = 0;
//variables funciones
T_PROG_OUTPUT aux_progOutput;
uint8_t pulsoLargo_skimmer = 0;
uint8_t pulsoLargo_luz = 0;
//variables hora y fecha (PRUEBA)
RTC_DateTypeDef auxFecha;
RTC_TimeTypeDef auxHora;
RTC_DateTypeDef lastFecha;
RTC_TimeTypeDef lastHora;

void acc_menuPrincipal (void);
void acc_setLlenado (void);
void acc_llenado (void);
void acc_skimmer (void);
void acc_hidro (void);
void acc_lucesExt (void);
void acc_lucesLed (void);
void acc_config (void);

void init_menuPrincipal (void);
void init_setLlenado (void);
void init_llenado (void);
void init_skimmer (void);
void init_hidro (void);
void init_lucesExt (void);
void init_lucesLed (void);
void init_config (void);

T_MENU menu[SIZE_MENU_NOMBRE] = {
		{MENU_PRINCIPAL, NULL, init_menuPrincipal, acc_menuPrincipal}, //MENU_PRINCIPAL
		{MENU_LLENADO, NULL, init_llenado, acc_llenado}, //MENU_LLENADO
		{MENU_SET_LLENADO, NULL, init_setLlenado, acc_setLlenado}, //MENU_SET_LLENADO
		{MENU_SKIMMER, NULL, init_skimmer, acc_skimmer}, //MENU_SKIMMER
		{MENU_HIDRO, NULL, init_hidro, acc_hidro}, //MENU_HIDRO
		{MENU_LUCES_EXT, NULL, init_lucesExt, acc_lucesExt}, //MENU_LUCES_EXT
		{MENU_LUCES_LED, NULL, init_lucesLed, acc_lucesLed}, //MENU_LUCES_LED
		{MENU_CONFIG, NULL, init_config, acc_config}, //MENU_CONFIG
};

T_STATUS_LLENADO status_menuLlenado = PREGUNTA_SENSOR;
T_STATUS_LLENADO status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;
T_STATUS_SKIMMER status_menuSkimmer = COMPRUEBE_BOMBA;
T_STATUS_HIDRO status_menuHidro = LIMITE_TIEMPO_HIDRO;
T_STATUS_LUCESEXT status_menuLucesExt = LIMITE_TIEMPO_LUCESEXT;

/////////////////////////////////////////
//         FUNCIONES PUBLICAS          //
/////////////////////////////////////////
void start_menu (void){
	init_pantalla();
	init_menuPrincipal();
	menuActual = &menu[MENU_PRINCIPAL];
}

void check_menu (void){
	menuActual->accion();
}

void check_duracionPulsadores (void){

	if (pulsoLargo_skimmer != 0){
		pulsoLargo_skimmer--;
	}

	if (pulsoLargo_luz != 0){
		pulsoLargo_luz--;
	}
} //end check_duracionPulsadores ()

void check_pulsadores (void){

	if (getStatBoton(IN_jet) == FALL){
		if (runProg_hidro(PROG_CHECK) == PROG_BUSY){
			runProg_hidro(PROG_STOP);
		}else if(runProg_hidro(PROG_CHECK) == PROG_IDLE){
			runProg_hidro(PROG_RUN);
		}
	} //end if getStatBoton IN_jet

	if (getStatBoton(IN_napa) == FALL){

		menuAux = menuActual;
		menuActual = &menu[MENU_LLENADO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
	} //end if getStatBoton(IN_napa)...

	if (getStatBoton(IN_tomas) == FALL){
		if (!flag_tomas){
			setOutput(OUT_rele_tomas, 1); //logica positiva
			set_led(OUT_led_tomas, PRENDIDO);
			flag_tomas = 1;
		}else{
			setOutput(OUT_rele_tomas, 0); //logica positiva
			set_led(OUT_led_tomas, APAGADO);
			flag_tomas = 0;
		}
	} //end if IN_tomas

	if (getStatBoton(IN_pileta) == FALL){
		aux_progOutput = runProg_skimmer(PROG_CHECK);

		if (aux_progOutput == PROG_IDLE){
			pulsoLargo_skimmer = 150; //en 10 * ms.
		}else if(aux_progOutput == PROG_BUSY){
			pulsoLargo_skimmer = 0;
			runProg_skimmer(PROG_STOP);
		}


	} //end if getStatBoton(IN_pileta)

	if (pulsoLargo_skimmer != 0){

		if (getStatBoton(IN_pileta) == HIGH_L){ //pulso corto
			runProg_skimmer(PROG_SET1); //con programa
			pulsoLargo_skimmer = 0;
		}

		if (pulsoLargo_skimmer == 1){ //pulso largo, justo 10 ms antes de terminar
			if (getStatBoton(IN_pileta) == LOW_L){ //doble check
				runProg_skimmer(PROG_SET2); //sin programa
			}
		} //end if pulsoLargo_skimmer

	}else{

		if (getStatBoton(IN_pileta) == LOW_L){

		}

	} //end if pulsoLargo_skimmer != 0


	if (getStatBoton(IN_pileta) == LOW_L){
		__NOP();
	}

} //end check_pulsadores ()

/////////////////////////////////////////
//          INICIALIZADORES            //
/////////////////////////////////////////

void init_menuPrincipal (void){

	auxFecha = get_fecha();
	auxHora = get_hora();

	lastFecha = auxFecha;
	lastHora = auxHora;

	sprintf(texto, "%02i  %02i:%02i:%02i", auxFecha.Date, auxHora.Hours, auxHora.Minutes, auxHora.Seconds );

	set_pantalla(PANT_init_menuPrincipal);
//	menuActual = &menu[MENU_PRINCIPAL];

	setTexto_pantalla(texto);
}


void init_setLlenado (void){

	set_pantalla(PANT_init_setLlenado);
	status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;

} //end init_llenado()


void init_llenado (void){

	switch (status_menuLlenado) {
		case PREGUNTA_SENSOR:
			set_led(OUT_led_napa, PRENDIDO);
			set_pantalla(PANT_sensor_conectado);
		break;
		case LLENANDO_CHECK:
			set_pantalla(PANT_llenando_terminar);
			//status_menuLlenado = 90;
		break;
		case CONECTE_SENSOR:
			set_pantalla(PANT_CONECTE_SENSOR);
			set_led(OUT_led_napa, PRENDIDO);
		default:
		break;
	} //end switch status_menuTablero

} //end init_llenado()


void init_skimmer (void){
	set_pantalla(PANT_init_skimmer);
}


void init_hidro (void){
	set_pantalla(PANT_init_hidro);
	status_menuHidro = ELIJE_TIEMPO_HIDRO;
}


void init_lucesExt (void){
	set_pantalla(PANT_init_lucesExt);
}


void init_lucesLed (void){

}


void init_config (void){

}


/////////////////////////////////////////
//            ACCIONES                 //
/////////////////////////////////////////

void acc_menuPrincipal (void){

//	if (getStatBoton(IN_napa)==FALL){
//		menuActual = &menu[MENU_SET_LLENADO];
//		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
//		menuActual->inicia_menu();
//		return;
//	}

	if (getStatBoton(IN_jet)==FALL){
		__NOP();
	}

	if (getStatBoton(IN_A)==FALL){
		menuAux = menuActual;
		menuActual = &menu[MENU_SET_LLENADO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
	} //end if getStatBoton(IN_A)

	if (getStatBoton(IN_B)==FALL){
		menuActual = &menu[MENU_HIDRO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
		return;
	} //end if getStatBoton(IN_B)

	if (getStatBoton(IN_C)==FALL){

	}

	if (getStatBoton(IN_D)==FALL){
		menuActual = &menu[MENU_LUCES_EXT];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
		return;
	}

	if (runProg_llenado(PROG_CHECK) == PROG_FINISHED){
		menuActual = &menu[MENU_LLENADO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
	}


	//prueba hora

	auxFecha = get_fecha();
	auxHora = get_hora();

	if (lastFecha.Date < auxFecha.Date) {
		lastFecha = auxFecha;
		lastHora = auxHora;
		sprintf(texto, "%02i  %02i:%02i:%02i", auxFecha.Date, auxHora.Hours, auxHora.Minutes, auxHora.Seconds );
		setTexto_pantalla(texto);
	}

	if (lastHora.Hours < auxHora.Hours) {
		lastFecha = auxFecha;
		lastHora = auxHora;
		sprintf(texto, "%02i  %02i:%02i:%02i", auxFecha.Date, auxHora.Hours, auxHora.Minutes, auxHora.Seconds );
		setTexto_pantalla(texto);
	}

	if (lastHora.Minutes < auxHora.Minutes) {
		lastFecha = auxFecha;
		lastHora = auxHora;
		sprintf(texto, "%02i  %02i:%02i:%02i", auxFecha.Date, auxHora.Hours, auxHora.Minutes, auxHora.Seconds);
		setTexto_pantalla(texto);
	}

	if (lastHora.Seconds < auxHora.Seconds) {
		lastFecha = auxFecha;
		lastHora = auxHora;
		sprintf(texto, "%02i  %02i:%02i:%02i", auxFecha.Date, auxHora.Hours, auxHora.Minutes, auxHora.Seconds);
		setTexto_pantalla(texto);
	}


} //end acc_menuPrincipal()


void acc_setLlenado (void){

	switch (status_menuSetLlenado) {

		case ELIJE_ON_OFF_LLENADO:
			if (getStatBoton(IN_1) == FALL) {
				set_pantalla(PANT_PERIODO_ON_LLENADO);
				status_menuSetLlenado = PERIODO_ON_LLENADO;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla(PANT_PERIODO_OFF_LLENADO);
				status_menuSetLlenado = PERIODO_OFF_LLENADO;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				status_menuSetLlenado = PREGUNTA_SENSOR;
			}
		break;

		case PERIODO_ON_LLENADO:
			//funcion de seteo de periodo on
			aux_progOutput = setProg_llenado(PROG_SET1);

			switch (aux_progOutput) {
				case PROG_IDLE:
					status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;
					set_pantalla(PANT_init_setLlenado);
				break;
				case PROG_FINISHED:
					status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;
					set_pantalla(PANT_init_setLlenado);
				default:
				break;
			} //end switch aux_progOutput
		break;
		case PERIODO_OFF_LLENADO:
			//funcion de seteo de periodo off
			aux_progOutput = setProg_llenado(PROG_SET2);

			switch (aux_progOutput) {
				case PROG_IDLE:
					status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;
					set_pantalla(PANT_init_setLlenado);
				break;
				case PROG_FINISHED:
					status_menuSetLlenado = ELIJE_ON_OFF_LLENADO;
					set_pantalla(PANT_init_setLlenado);
				default:
				break;
			} //end switch aux_progOutput
		default:
		break;
	} //end switch status_menuLlenado

} //end acc_setLlenado ()


void acc_llenado (void){

	switch (status_menuLlenado) {
		case PREGUNTA_SENSOR:

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				set_pantalla(PANT_COMPRUEBE_SENSOR);
				status_menuLlenado = COMPRUEBE_SENSOR;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //niego
				set_pantalla(PANT_CONECTE_SENSOR);
				status_menuLlenado = CONECTE_SENSOR;
				break;
			}

		break;
		case CONECTE_SENSOR:

			if (getStatBoton(IN_AST) == FALL) { //volver
				//vuelve al menu principal
				set_led(OUT_led_napa, APAGADO);
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				set_pantalla(PANT_PREGUNTA_SENSOR);
				status_menuLlenado = PREGUNTA_SENSOR;
				break;
			}
		break;
		case COMPRUEBE_SENSOR:

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				set_pantalla(PANT_LLENANDO);
				runProg_llenado(PROG_RUN);
				status_menuLlenado = LLENANDO;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //volver
				set_pantalla(PANT_PREGUNTA_SENSOR);
				status_menuLlenado = PREGUNTA_SENSOR;
				break;
			}

		break;
		case LLENANDO:
			if (runProg_llenado(PROG_CHECK) == PROG_FINISHED ) { //LOGICA NEGATIVA
				set_pantalla(PANT_PILETA_LLENA);
				set_led(OUT_led_napa, APAGADO);
				status_menuLlenado = PILETA_LLENA;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				//vuelve al menu principal
				status_menuLlenado = LLENANDO_CHECK;
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //volver
				//vuelve al menu principal
				status_menuLlenado = LLENANDO_CHECK;
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}
		break;
		case LLENANDO_CHECK:
			if (runProg_llenado(PROG_CHECK) == PROG_FINISHED ) { //LOGICA NEGATIVA
				set_pantalla(PANT_PILETA_LLENA);
				status_menuLlenado = PILETA_LLENA;
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				status_menuLlenado = PREGUNTA_SENSOR;
				runProg_llenado(PROG_STOP);
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //volver
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}
		break;
		case PILETA_LLENA:
			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				status_menuLlenado = PREGUNTA_SENSOR;
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //volver
				status_menuLlenado = PREGUNTA_SENSOR;
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

		default:
		break;
	} //end switch status_menuLlenado

} //end acc_llenado ()


void acc_skimmer (void){
	switch (status_menuSkimmer) {
		case COMPRUEBE_BOMBA:
			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
			}

			if (getStatBoton(IN_HASH) == FALL) {
				set_pantalla(PANT_OPCIONES_SKIMMER);
				status_menuSkimmer = OPCIONES_SKIMMER;
				break;
			}
		break;
		case OPCIONES_SKIMMER:
			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
			}

			if (getStatBoton(IN_1) == FALL) {
				set_pantalla(PANT_ELIJE_FRECUENCIA_SKIMMER);
				status_menuSkimmer = ELIJE_FRECUENCIA_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla(PANT_ELIJE_PROGRAMA_SKIMMER);
				status_menuSkimmer = ELIJE_PROGRAMA_SKIMMER;
				break;
			}
		break;
		case ELIJE_FRECUENCIA_SKIMMER:
			if (getStatBoton(IN_1) == FALL) {
				set_pantalla(PANT_PERIODO_ON_SKIMMER);
				status_menuSkimmer = PERIODO_ON_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla(PANT_PERIODO_OFF_SKIMMER);
				status_menuSkimmer = PERIODO_OFF_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla(PANT_OPCIONES_SKIMMER);
				status_menuSkimmer = OPCIONES_SKIMMER;
				break;
			}
		break;
		case PERIODO_ON_SKIMMER:
			//funcion de seteo de periodo on
		break;
		case PERIODO_OFF_SKIMMER:
			//funcion de seteo de periodo off
		break;
		case ELIJE_PROGRAMA_SKIMMER:
			if (getStatBoton(IN_1) == FALL) {
				//levanta el flag de modificar el programa 1
				set_pantalla(PANT_ELIJE_ON_OFF_HORARIO_SKIMMER);
				status_menuSkimmer = ELIJE_ON_OFF_HORARIO_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				//levanta el flag de modificar el programa 2
				set_pantalla(PANT_ELIJE_ON_OFF_HORARIO_SKIMMER);
				status_menuSkimmer = ELIJE_ON_OFF_HORARIO_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla(PANT_OPCIONES_SKIMMER);
				status_menuSkimmer = OPCIONES_SKIMMER;
				break;
			}
		break;
		case ELIJE_ON_OFF_HORARIO_SKIMMER:
			if (getStatBoton(IN_1) == FALL) {
				//levanta el flag de modificar el horario ON
				set_pantalla(PANT_HORARIO_ON_SKIMMER);
				status_menuSkimmer = HORARIO_ON_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				//levanta el flag de modificar el horario OFF
				set_pantalla(PANT_HORARIO_OFF_SKIMMER);
				status_menuSkimmer = HORARIO_OFF_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla(PANT_ELIJE_PROGRAMA_SKIMMER);
				status_menuSkimmer = ELIJE_PROGRAMA_SKIMMER;
				break;
			}
		break;
		case HORARIO_ON_SKIMMER:
			//funcion de seteo de horario on
		break;
		case HORARIO_OFF_SKIMMER:
			//funcion de seteo de horario off
		break;
	} //end switch status_menuSkimmer
} //end acc_skimmer()


void acc_hidro (void){

	switch (status_menuHidro) {
		case LIMITE_TIEMPO_HIDRO:

			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
				break;
			}

			if (getStatBoton(IN_HASH) == FALL) {
				status_menuHidro = ELIJE_TIEMPO_HIDRO;
				break;
			}
		break;
		case ELIJE_TIEMPO_HIDRO:

			//funcion de seteo de timepo de hidro
			aux_progOutput = setProg_hidro(PROG_RUN);

			switch (aux_progOutput) {
				case PROG_IDLE:
				case PROG_FINISHED:
					menuActual = &menu[MENU_PRINCIPAL];
					menuActual->inicia_menu();
				default:
				break;
			} //end switch status_menuHidro

		break;
	} //end switch status_menuHidro

} //end acc_hidro()


void acc_lucesExt (void){ //luces Amarillas
	switch (status_menuLucesExt) {
			case LIMITE_TIEMPO_LUCESEXT:
				if (getStatBoton(IN_AST) == FALL) {
					//vuelve al menu principal
					menuActual = &menu[MENU_PRINCIPAL];
					menuActual->inicia_menu();
				}

				if (getStatBoton(IN_HASH) == FALL) {
					status_menuLucesExt = ELIJE_TIEMPO_LUCESEXT;
					break;
				}
			break;
			case ELIJE_TIEMPO_LUCESEXT:
				//funcion de seteo de timepo de hidro
				setProg_luz(PROG_RUN);
			break;
		} //end switch status_menuLucesExt
} //end acc_lucesExt()


void acc_lucesLed (void){

}


void acc_config (void){

}
