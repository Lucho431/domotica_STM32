/*
 * menu_tablero.c
 *
 *  Created on: Jan 30, 2022
 *      Author: Luciano Salvatore
 */

#include "menu_tablero.h"
#include "IOports_lfs.h"

T_MENU* menuActual;
T_MENU* menuAux;

void acc_menuPrincipal (void);
void acc_llenado (void);
void acc_skimmer (void);
void acc_hidro (void);
void acc_lucesExt (void);
void acc_lucesLed (void);
void acc_config (void);

void init_menuPrincipal (void);
void init_llenado (void);
void init_skimmer (void);
void init_hidro (void);
void init_lucesExt (void);
void init_lucesLed (void);
void init_config (void);

T_MENU menu[SIZE_MENU_NOMBRE] = {
		{MENU_PRINCIPAL, NULL, init_menuPrincipal, acc_menuPrincipal}, //MENU_PRINCIPAL
		{MENU_LLENADO, NULL, init_llenado, acc_llenado}, //MENU_LLENADO
		{MENU_SKIMMER, NULL, init_skimmer, acc_skimmer}, //MENU_SKIMMER
		{MENU_HIDRO, NULL, init_hidro, acc_hidro}, //MENU_HIDRO
		{MENU_LUCES_EXT, NULL, init_lucesExt, acc_lucesExt}, //MENU_LUCES_EXT
		{MENU_LUCES_LED, NULL, init_lucesLed, acc_lucesLed}, //MENU_LUCES_LED
		{MENU_CONFIG, NULL, init_config, acc_config}, //MENU_CONFIG
};


/////////////////////////////////////////
//          INICIALIZADORES            //
/////////////////////////////////////////

void init_menuPrincipal (void){

}

void init_llenado (void){

}

void init_skimmer (void){

}

void init_hidro (void){

}

void init_lucesExt (void){

}

void init_lucesLed (void){

}

void init_config (void){

}


/////////////////////////////////////////
//            ACCIONES                 //
/////////////////////////////////////////

void acc_menuPrincipal (void){

}

void acc_llenado (void){

}

void acc_skimmer (void){

}

void acc_hidro (void){

}

void acc_lucesExt (void){

}

void acc_lucesLed (void){

}

void acc_config (void){

}
