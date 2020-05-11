/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "fake_op.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	P_Core = 0x1, S_Core = 0x2, Reboot = 0x3, Sleep = 0x4, Killed = 0x5 
}States;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int trigger = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display_LED(States *state);
void goodnight();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	States state = P_Core;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	SystemPower_Config();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	HAL_InitTick(0xF);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_SET);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(state){
			case P_Core:
				if (trigger == 1) { state = S_Core; }
				if (my_op() == 7) {
					HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_SET);
				}
				break;
			case S_Core:
				if (trigger == 1) { state = Reboot; }
				break;
			case Reboot:
				//FLASH LEDS for REBOOT
				state = Sleep;
				break;
			case Sleep:
				//if (trigger == 1) { state = P_Core; }
				state = P_Core;
				//HAL_GPIO_WritePin(GPIOA, Green_LED_Pin, GPIO_PIN_SET);
				goodnight();
				//HAL_GPIO_WritePin(GPIOA, Green_LED_Pin, GPIO_PIN_SET);
				break;
			case Killed:
				state = Killed;
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_RESET);
				break;
		}
			//RESET TRIGGER
		trigger = 0;	
		display_LED(&state);
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Green_LED_Pin|State1_Pin|State2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Power_GPIO_Port, Power_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 Trigger_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_13|Trigger_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : Green_LED_Pin */
  GPIO_InitStruct.Pin = Green_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Green_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : State1_Pin State2_Pin */
  GPIO_InitStruct.Pin = State1_Pin|State2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Power_Pin */
  GPIO_InitStruct.Pin = Power_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Power_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	trigger = 1;
}		

void display_LED(States *state){
		switch(*state){
			case P_Core:
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_SET);
				break;
			case S_Core:
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_RESET);					
				break;
			case Reboot:
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_SET);
				break;
			case Sleep:
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_RESET);
				break;
			case Killed:
				HAL_GPIO_WritePin(GPIOC, Power_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, State1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, State2_Pin, GPIO_PIN_SET);
				break;
		}
}

void goodnight() {

		/* Enable the power down mode during Sleep mode */
    __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

    /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
    /* Otherwise the Systick interrupt will wake up the device within 1ms     */
    /* (HAL time base).                                                       */
    HAL_SuspendTick();

    /* Enter Sleep Mode , wake up is done once Key push button is pressed */
    HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

    /* Resume Tick interrupt if disabled prior to sleep mode entry */
    HAL_ResumeTick();
    
    /* Small delay to be sure that another button press is not detected */
    HAL_Delay(100);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/