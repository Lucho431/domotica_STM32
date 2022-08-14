/*
 * 74HC165_SPI_lfs.c
 *
 *  Created on: 24 jun. 2021
 *      Author: Luciano Salvatore
 */

#include "74HC165_SPI_lfs.h"

SPI_HandleTypeDef* spi_handler;
GPIO_TypeDef* portPL;

GPIO_TypeDef* portCE;
uint16_t pinPL, pinCE;


/*
 * La función de inicialización está modificada para trabajar con dos
 * integrados 74HC165 a la vez, tratándolos como distintos esclavos SPI.
 * Cada integrado contará con su propia función de lectura, así como sus
 * propias señales CE.
 */
void spi_74HC165_init (SPI_HandleTypeDef* hspi, GPIO_TypeDef* PLport, uint16_t PLpin, GPIO_TypeDef* CEport, uint16_t CEpin){

	spi_handler=hspi;

	portPL = PLport;
	pinPL = PLpin;

	portCE = CEport;
	pinCE = CEpin;

}


void spi_74HC165_receiveBotones (uint8_t* pdata, uint16_t sizeData){

	//carga paralela
	HAL_GPIO_WritePin(portPL, pinPL, 0);
	HAL_GPIO_WritePin(portPL, pinPL, 1);

	//clock enable
	HAL_GPIO_WritePin(portCE, pinCE, 0);

	//SPI
	HAL_SPI_Receive(spi_handler, pdata, sizeData, 100);

	//clock disable
	HAL_GPIO_WritePin(portCE, pinCE, 1);
}


//void spi_74HC165_receiveTeclado (uint8_t* pdata, uint16_t sizeData){
//
//	//carga paralela
//	HAL_GPIO_WritePin(portPL, pinPL, 0);
//	HAL_GPIO_WritePin(portPL, pinPL, 1);
//
//	//clock enable
//	HAL_GPIO_WritePin(portCE2, pinCE2, 0);
//
//	//SPI
//	HAL_SPI_Receive(spi_handler, pdata, sizeData, 100);
//
//	//clock disable
//	HAL_GPIO_WritePin(portCE2, pinCE2, 1);
//}

void spi_74HC165_receiveTeclado (uint8_t* pdata, uint16_t sizeData){

	//carga paralela
	HAL_GPIO_WritePin(portPL, pinPL, 0);
	HAL_GPIO_WritePin(portPL, pinPL, 1);

	//clock enable
	HAL_GPIO_WritePin(portCE, pinCE, 0);

	uint8_t auxData[2];

	//SPI
	HAL_SPI_Receive(spi_handler, auxData, 2, 100);

	//clock disable
	HAL_GPIO_WritePin(portCE, pinCE, 1);

	*pdata = auxData[1];
}
