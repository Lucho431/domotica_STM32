/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IN_nivelAgua_Pin GPIO_PIN_2
#define IN_nivelAgua_GPIO_Port GPIOA
#define OUT_PL_Pin GPIO_PIN_12
#define OUT_PL_GPIO_Port GPIOB
#define OUT_CE1_Pin GPIO_PIN_8
#define OUT_CE1_GPIO_Port GPIOA
#define OUT_CE2_Pin GPIO_PIN_9
#define OUT_CE2_GPIO_Port GPIOA
#define OUT_ST_Pin GPIO_PIN_10
#define OUT_ST_GPIO_Port GPIOA
#define OUT_fila3_Pin GPIO_PIN_15
#define OUT_fila3_GPIO_Port GPIOA
#define OUT_fila2_Pin GPIO_PIN_3
#define OUT_fila2_GPIO_Port GPIOB
#define OUT_fila1_Pin GPIO_PIN_4
#define OUT_fila1_GPIO_Port GPIOB
#define OUT_fila0_Pin GPIO_PIN_5
#define OUT_fila0_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
