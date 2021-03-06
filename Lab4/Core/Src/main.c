/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * FILE          : main.c
  * PROJECT       : COE4DS4 - Lab Assignment #4
  * PROGRAMMER    : Uday Sharma, Vivek Patel
  * FIRST VERSION : 2021-04-03
  * DESCRIPTION   :
  *		The functions in this file are used to simulate a credit card reader that processes a transaction
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
/*
 * FILE          : main.c
 * PROJECT       : 4DS4 - Lab Assignment #4
 * PROGRAMMER    : Uday Sharma, Vivek Patel
 * FIRST VERSION : 2021-04-3
 * DESCRIPTION   :
 *   The functions in this file are used to create a state machine for a credit card machine */
/*
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debounce.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
char buttonVal(void);
int arrayIn(int pin, int numbers[], int size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int passwords[] = {1111, 1121, 1133, 1213, 2111, 2331, 311, 3211, 3333};
	int state = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  int8_t mode = 1;

  /* USER CODE BEGIN Init */
  deBounceInit(GPIO_PIN_3, 'A', mode);
  deBounceInit(GPIO_PIN_4, 'A', mode);
  deBounceInit(GPIO_PIN_7, 'A', mode);
  deBounceInit(GPIO_PIN_8,'A', mode);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HD44780_Init();
  /* USER CODE END 2 */
  /* Infinite loop */
  while (1)
  {
	 /* USER CODE BEGIN WHILE */
	 switch(state){
	 	 case 1:
	 		 HD44780_ClrScr();
	 		 HD44780_PutStr("Welcome");
	 		 printf("%s\r\n","Welcome");
	 		 HAL_Delay(2000);
	 		 if(deBounceReadPin(GPIO_PIN_8, 'A', mode) == 0) {
	 			 state = 2;
	 		 }
	 	 break;

	 	 case 2:
	 		HD44780_ClrScr();
	 		HD44780_PutStr("$35.20");
	 		HD44780_GotoXY(0, 1);
	 		HD44780_PutStr("Ok:1 or Cancel:2");

	 		printf("%s\r\n", "Ok: 1 or Cancel: 2");
	 		HAL_Delay(2000);
	 		if(deBounceReadPin(GPIO_PIN_3, 'A', mode) == 0) {
	 			state = 3;
	 		} else if (deBounceReadPin(GPIO_PIN_4, 'A', mode) == 0) {
	 			state = 6;
	 		}
	 	 break;

	 	 case 3:
	 		HD44780_ClrScr();
	 		HD44780_PutStr("Chequing:1");
	 		HD44780_GotoXY(0, 1);
	 		HD44780_PutStr("Savings:2");

	 		printf("%s\r\n", "Press 1 for chequing, and 2 for savings");
	 		HAL_Delay(5000);
	 		state = 4;
	 	 break;

	 	 case 4:
	 		HD44780_ClrScr();
	 		HD44780_PutStr("Enter PIN");
	 		printf("%s\r\n", "Enter PIN");
	 		HD44780_GotoXY(0, 1);
	 		char pin[5] = {'0', '0', '0', '0', '\0'};
	 		for(int i = 0; i < 4; i++) {
	 			pin[i] = buttonVal();
	 		}
	 		HAL_Delay(500);
	 		int enteredPin = atoi(pin);
	 		state = 5;
	 	 break;

	 	 case 5:
	 		HD44780_ClrScr();
	 		HD44780_PutStr("Processing...");
	 		printf("%s\r\n", "Processing...");
	 		HAL_Delay(10000);
	 		HD44780_ClrScr();
	 		if(arrayIn(enteredPin, passwords, 9) == 1) {
		 		HD44780_PutStr("Transaction");
		 		HD44780_GotoXY(0, 1);
		 		HD44780_PutStr("Complete");
		 		HAL_Delay(500);

	 			printf("%s\r\n", "Transaction Complete, Thank you!");
	 			state = 1;
	 		} else if (arrayIn(enteredPin, passwords, 9) == 2) {
	 			HD44780_PutStr("Insufficent");
		 		HD44780_GotoXY(0, 1);
		 		HD44780_PutStr("Funds");
		 		HAL_Delay(500);

	 			printf("%s\r\n", "Insufficent Funds");
	 			state = 6;
	 		} else {
	 			HD44780_PutStr("Invalid Pin");
		 		HAL_Delay(500);

	 			printf("%s\r\n", "Invalid Pin");
	 			state = 6;
	 		}
	 	 break;

	 	 case 6:
	 		HD44780_ClrScr();
	 		HD44780_PutStr("Transaction");
	 		HD44780_GotoXY(0, 1);
	 		HD44780_PutStr("Cancelled");

	 		printf("%s\r\n", "Transaction Cancelled, Goodbye!");
	 		HAL_Delay(2000);
			state = 1;
		 break;
	 }
  }
  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/*
 * FUNCTION     : buttonVal
 * DESCRIPTION  : This function reads the button number entered by the user
 * PARAMETERS   : void
 * RETURNS      : char: the character value of the button number
 */

char buttonVal(void) {
	uint8_t mode = 1;
	int flag = 0;
	while(flag == 0) {
		if(deBounceReadPin(GPIO_PIN_3, 'A', mode) == 0) {
			HAL_Delay(1000);
			flag = 1;
			printf("%c", '*');
			//Put char onto display
	 		HD44780_PutChar('*');
			return('1');
		} else if(deBounceReadPin(GPIO_PIN_4, 'A', mode) == 0) {
			HAL_Delay(1000);
			flag = 1;
			printf("%c", '*');
			HD44780_PutChar('*');
			return('2');
		} else if(deBounceReadPin(GPIO_PIN_7, 'A', mode) == 0) {
			HAL_Delay(1000);
			flag = 1;
			printf("%c\r\n", '*');
			HD44780_PutChar('*');
			return('3');
		}
	}
	return('w');
}

/*
 * FUNCTION     : arrayIn
 *
 * DESCRIPTION  : This function checks the array initialized at the beginning of the file and scans for the inputted pin
 *
 * PARAMETERS   : int pin         : entered pin
 * 				  int passwords[] : passwords array
 * 				  int size        : size of the passwords array
 *
 * RETURNS      : int: 1 if valid, 2 if valid but not enough money, 0 if invalid
 */
int arrayIn(int pin, int passwords[], int size) {

	for (int i = 0; i < size; i++) {
		if (passwords[i] == pin) {
			return 1;
		}
	}

	return 0;

  	if (pin == 3333) {
		return 2;
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA3 PA4 PA7 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 LD3_Pin PB4
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
