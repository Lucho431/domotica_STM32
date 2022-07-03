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


//variables menu
T_MENU* menuActual;
T_MENU* menuAux;
//variables tomas
uint8_t flag_tomas = 0;
//variables funciones
T_POS_OUTPUT aux_progOutput;
uint8_t pulsoLargo_skimmer = 0;
uint8_t pulsoLargo_luz = 0;


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
		{MENU_SET_LLENADO, NULL, init_llenado, acc_llenado}, //MENU_LLENADO
		{MENU_SKIMMER, NULL, init_skimmer, acc_skimmer}, //MENU_SKIMMER
		{MENU_HIDRO, NULL, init_hidro, acc_hidro}, //MENU_HIDRO
		{MENU_LUCES_EXT, NULL, init_lucesExt, acc_lucesExt}, //MENU_LUCES_EXT
		{MENU_LUCES_LED, NULL, init_lucesLed, acc_lucesLed}, //MENU_LUCES_LED
		{MENU_CONFIG, NULL, init_config, acc_config}, //MENU_CONFIG
};

T_STATUS_LLENADO status_menuLlenado = PREGUNTA_SENSOR;
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
		menuActual = &menu[MENU_SET_LLENADO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
	} //end if getStatBoton(IN_napa)...

	if (getStatBoton(IN_tomas) == FALL){
		if (!flag_tomas){
			setOutput(OUT_rele_tomas, 1); //logica positiva
			set_led(OUT_led_tomas, PRENDIDO);
			flag_tomas = 0;
		}else{
			setOutput(OUT_rele_tomas, 0); //logica positiva
			set_led(OUT_led_tomas, APAGADO);
			flag_tomas = 1;
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


	}
	if (pulsoLargo_skimmer != 0){
		if (getStatBoton(IN_pileta) == HIGH_L){ //pulso corto
			runProg_skimmer(PROG_SET1); //con programa
		}else{
			pulsoLargo_skimmer--;
			if (!pulsoLargo_skimmer){ //pulso largo
				runProg_skimmer(PROG_SET2); //sin programa
			}
		}
	}
	if (getStatBoton(IN_pileta) == LOW_L){

	}

}

/////////////////////////////////////////
//          INICIALIZADORES            //
/////////////////////////////////////////

void init_menuPrincipal (void){
	set_pantalla("Estoy en el menu principal");
//	menuActual = &menu[MENU_PRINCIPAL];
}


void init_setLlenado (void){

//	switch (status_menuLlenado) {
//		case PREGUNTA_SENSOR:
//			set_pantalla("Sensor conectado?");
//		break;
//		default:
//			set_pantalla("Llenando. terminar?");
//			//status_menuLlenado = 90;
//		break;
//	} //end switch status_menuTablero
	set_pantalla("elije tiempo on u off");

} //end init_llenado()


void init_llenado (void){

	switch (status_menuLlenado) {
		case PREGUNTA_SENSOR:
			set_led(OUT_led_napa, PRENDIDO);
			set_pantalla("Sensor conectado?");
		break;
		default:
			set_pantalla("Llenando. terminar?");
			//status_menuLlenado = 90;
		break;
	} //end switch status_menuTablero

} //end init_llenado()


void init_skimmer (void){
	set_pantalla("COMPRUEBE EL ESTADO DE LA BOMBA");
}


void init_hidro (void){
	set_pantalla("LIMITE DE TIEMPO");
}


void init_lucesExt (void){
	set_pantalla("LIMITE DE TIEMPO");
}


void init_lucesLed (void){

}


void init_config (void){

}


/////////////////////////////////////////
//            ACCIONES                 //
/////////////////////////////////////////

void acc_menuPrincipal (void){

	if (getStatBoton(IN_napa)==FALL){
		menuActual = &menu[MENU_SET_LLENADO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
		return;
	}

	//RE VER: debería ejecutar el programa y no configurarlo...
	if (getStatBoton(IN_jet)==FALL){
		menuActual = &menu[MENU_HIDRO];
		menuActual->menuAnterior = &menu[MENU_PRINCIPAL];
		menuActual->inicia_menu();
		return;
	}
}


void acc_setLlenado (void){

//	switch (status_menuLlenado) {
//		case PREGUNTA_SENSOR:
//
//			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
//				set_pantalla("COMPRUEBE EL SENSOR Y PULSE EL BOTON...");
//				status_menuLlenado = COMPRUEBE_SENSOR;
//				break;
//			}
//
//			if (getStatBoton(IN_AST) == FALL) { //niego
//				set_pantalla("CONECTE EL SENSOR");
//				status_menuLlenado = CONECTE_SENSOR;
//				break;
//			}
//
//		break;
//		case CONECTE_SENSOR:
//
//			if (getStatBoton(IN_AST) == FALL) { //volver
//				//vuelve al menu principal
//				menuActual = &menu[MENU_PRINCIPAL];
//				menuActual->inicia_menu();
//				break;
//			}
//
//			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
//				set_pantalla("¿Sensor conectado?");
//				status_menuLlenado = PREGUNTA_SENSOR;
//				break;
//			}
//		break;
//		case COMPRUEBE_SENSOR:
//
//			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
//				set_pantalla("llenando...");
//				status_menuLlenado = LLENANDO;
//				//funcion de llenado de pileta (biblio de funciones automaticas)
//				setProg_llenado(PROG_RUN);
//				break;
//			}
//
//			if (getStatBoton(IN_AST) == FALL) { //volver
//				set_pantalla("¿Sensor conectado?");
//				status_menuLlenado = PREGUNTA_SENSOR;
//				break;
//			}
//
//		break;
//		case LLENANDO:
//
//			if (runProg_llenado(PROG_CHECK) == PROG_FINISHED ) { //LOGICA NEGATIVA
//				set_pantalla("pileta llena.");
//				status_menuLlenado = PILETA_LLENA;
//				break;
//			}
//		case PILETA_LLENA:
//			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
//				//vuelve al menu principal
//				menuActual = &menu[MENU_PRINCIPAL];
//				menuActual->inicia_menu();
//				break;
//			}
//
//			if (getStatBoton(IN_AST) == FALL) { //volver
//				//vuelve al menu principal
//				menuActual = &menu[MENU_PRINCIPAL];
//				menuActual->inicia_menu();
//				break;
//			}
//
//		default:
//		break;
//	} //end switch status_menuLlenado

	switch (status_menuLlenado) {

		case ELIJE_ON_OFF_LLENADO:
			if (getStatBoton(IN_1) == FALL) {
				set_pantalla("defina el tiempo de duracion ON:");
				status_menuLlenado = PERIODO_ON_LLENADO;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla("defina el tiempo de duracion OFF:");
				status_menuLlenado = PERIODO_OFF_LLENADO;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
				menuActual = &menu[MENU_PRINCIPAL];
				menuActual->inicia_menu();
			}
		break;

		case PERIODO_ON_LLENADO:
			//funcion de seteo de periodo on
			aux_progOutput = setProg_llenado(PROG_SET1);

			switch (aux_progOutput) {
				case PROG_IDLE:
					status_menuLlenado = ELIJE_ON_OFF_LLENADO;
				break;
				case PROG_FINISHED:
					status_menuHidro = ELIJE_ON_OFF_LLENADO;
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
				set_pantalla("COMPRUEBE EL SENSOR Y PULSE EL BOTON...");
				status_menuLlenado = COMPRUEBE_SENSOR;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //niego
				set_pantalla("CONECTE EL SENSOR");
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
				set_pantalla("¿Sensor conectado?");
				status_menuLlenado = PREGUNTA_SENSOR;
				break;
			}
		break;
		case COMPRUEBE_SENSOR:

			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
				set_pantalla("llenando...");
				runProg_llenado(PROG_RUN);
				status_menuLlenado = LLENANDO;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) { //volver
				set_pantalla("¿Sensor conectado?");
				status_menuLlenado = PREGUNTA_SENSOR;
				break;
			}

		break;
		case LLENANDO:

			if (runProg_llenado(PROG_CHECK) == PROG_FINISHED ) { //LOGICA NEGATIVA
				set_pantalla("pileta llena.");
				status_menuLlenado = PILETA_LLENA;
				break;
			}
		case PILETA_LLENA:
			if (getStatBoton(IN_HASH) == FALL) { //CONFIRMO
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
				set_pantalla("elija freecuecia u horario");
				status_menuSkimmer = OPCIONES_SKIMMER;
				break;
			}
		break;
		case OPCIONES_SKIMMER:
			if (getStatBoton(IN_AST) == FALL) {
				//vuelve al menu principal
			}

			if (getStatBoton(IN_1) == FALL) {
				set_pantalla("elijo timepo ON o timepo OFF");
				status_menuSkimmer = ELIJE_FRECUENCIA_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla("elijo programa 1 o programa 2");
				status_menuSkimmer = ELIJE_PROGRAMA_SKIMMER;
				break;
			}
		break;
		case ELIJE_FRECUENCIA_SKIMMER:
			if (getStatBoton(IN_1) == FALL) {
				set_pantalla("defina el tiempo de duracion ON:");
				status_menuSkimmer = PERIODO_ON_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				set_pantalla("defina el tiempo de duracion OFF:");
				status_menuSkimmer = PERIODO_OFF_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla("elija freecuecia u horario");
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
				set_pantalla("elije horario ON u horario OFF");
				status_menuSkimmer = ELIJE_ON_OFF_HORARIO_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				//levanta el flag de modificar el programa 2
				set_pantalla("elije horario ON u horario OFF");
				status_menuSkimmer = ELIJE_ON_OFF_HORARIO_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla("elija freecuecia u horario");
				status_menuSkimmer = OPCIONES_SKIMMER;
				break;
			}
		break;
		case ELIJE_ON_OFF_HORARIO_SKIMMER:
			if (getStatBoton(IN_1) == FALL) {
				//levanta el flag de modificar el horario ON
				set_pantalla("defina el horario ON");
				status_menuSkimmer = HORARIO_ON_SKIMMER;
				break;
			}

			if (getStatBoton(IN_2) == FALL) {
				//levanta el flag de modificar el horario OFF
				set_pantalla("defina el horario OFF");
				status_menuSkimmer = HORARIO_OFF_SKIMMER;
				break;
			}

			if (getStatBoton(IN_AST) == FALL) {
				set_pantalla("elijo programa 1 o programa 2");
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
					status_menuHidro = LIMITE_TIEMPO_HIDRO;
				break;
				case PROG_FINISHED:
					status_menuHidro = LIMITE_TIEMPO_HIDRO;
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
