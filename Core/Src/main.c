/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include "GMG12864_lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define rx_uart_buffer_size 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rx_uart_buffer[rx_uart_buffer_size] = {0, };
uint16_t rx_uart_buffer_len = 0;
bool rx_uart_buffer_ready = 0;
extern char tx_buffer[128];
extern uint8_t Frame_buffer[1024];
uint8_t counter = 0;
bool flag_led = 0;
uint8_t status, status_old;
uint8_t number_str = 0;
bool stage_menu = 0;
bool update_display = 1;
unsigned long t_led = 0;
uint8_t tim4_counter = 0;
bool tim4_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void print_first_line(void);
void print_second_line(void);
void print_third_line(void);
void print_fourth_line(void);
void print_fifth_line(void);
void print_sixth_line(void);
void pointer_write(void);
void upd_disp(void);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_IT(&huart2, rx_uart_buffer, rx_uart_buffer_size);
  HAL_TIM_Base_Start_IT(&htim4);
  t_led = HAL_GetTick();
  GMG12864_Init();
  GPIOC -> BSRR = GPIO_PIN_3;
  sprintf(tx_buffer, "Hello world");
  GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
  GMG12864_Update();
  GMG12864_Clean_Frame_buffer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(rx_uart_buffer_ready)
	  {
		  char tx_uart_buffer[] = "Interrupt!\r\n";
		  HAL_UART_Transmit_IT(&huart2, tx_uart_buffer, (sizeof(tx_uart_buffer) / sizeof(tx_uart_buffer[0])));
		  rx_uart_buffer_ready = 0;
	  }
	  if(stage_menu == 0)
	  {
		  upd_disp();
		  print_first_line();
		  print_second_line();
		  print_third_line();
		  print_fourth_line();
		  print_fifth_line();
		  print_sixth_line();
		  pointer_write();
	  }
	  else if(stage_menu)
	  {
		  switch(number_str)
		  {
		  case 0:
				GMG12864_Clean_Frame_buffer();
				if (!flag_led)
				{
					sprintf(tx_buffer, "User led is on");
					GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
					GMG12864_Update();
				}
				else
				{
					sprintf(tx_buffer, "User led is off");
					GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
					GMG12864_Update();
				}
				break;
		  case 10:
				if(tim4_flag == 0)
				{
					GMG12864_Clean_Frame_buffer();
					sprintf(tx_buffer, "tim4_counter is %d", tim4_counter);
					GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
					GMG12864_Update();
				}
				if(tim4_counter >= 252)
				{
					tim4_flag = 1;
					GMG12864_Clean_Frame_buffer();
					sprintf(tx_buffer, "Ok!");
					GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
					GMG12864_Update();
				}
				break;
		  case 20:
				GMG12864_Clean_Frame_buffer();
				sprintf(tx_buffer, "UART speed %d", (huart2.Init.BaudRate));
				GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
				GMG12864_Update();
				break;
		  case 30:
				GMG12864_Clean_Frame_buffer();
				sprintf(tx_buffer, "Parameters fourth str");
				GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
				GMG12864_Update();
				break;
		  case 40:
				GMG12864_Clean_Frame_buffer();
				sprintf(tx_buffer, "Parameters fifht str");
				GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
				GMG12864_Update();
				break;
		  case 50:
				GMG12864_Clean_Frame_buffer();
				sprintf(tx_buffer, "Parameters sixth str");
				GMG12864_Decode_UTF8(0, 0, 1, 0, tx_buffer);
				GMG12864_Update();
				break;
		  }
	  }
	  /*if(!flag_led)
	  {
		  GPIOA -> BSRR = user_led_Pin;
	  }
	  else
	  {
		  GPIOA -> BSRR = (uint32_t)user_led_Pin << 16;
	  }*/
	  if(HAL_GetTick() - t_led > 1000)
	  {
		  t_led = HAL_GetTick();
		  flag_led = !flag_led;
		  if(!flag_led)
		  {
			  GPIOA -> BSRR = user_led_Pin;
		  }
		  else
		  {
			  GPIOA -> BSRR = (uint32_t)user_led_Pin << 16;
		  }
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM34;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Tim34ClockSelection = RCC_TIM34CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		if (!(GPIOC->IDR & GPIO_PIN_0) && !(GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x00;
		} else if ((GPIOC->IDR & GPIO_PIN_0) && !(GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x10;
		} else if (!(GPIOC->IDR & GPIO_PIN_0) && (GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x01;
		} else if ((GPIOC->IDR & GPIO_PIN_0) && (GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x11;
		}
		if ((status_old == 0x10) && (status == 0x11)) {
			counter += 1;
			if (number_str < 50) {
				number_str += 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 50;
			}
		} else if ((status_old == 0x01) && (status == 0x00)) {
			counter += 1;
			if (number_str < 50) {
				number_str += 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 50;
			}
		}
		if (status_old == 0x11 && status == 0x10) {
			status_old = 0x10;

		} else if (status_old == 0x00 && status == 0x01) {
			status_old = 0x01;
		} else if (status_old == 0x10 && status == 0x00) {
			counter -= 1;
			if (number_str > 0) {
				number_str -= 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 0;
			}
		} else if (status_old == 0x01 && status == 0x11) {
			counter -= 1;
			if (number_str > 0) {
				number_str -= 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 0;
			}
		}
		status_old = status;
	}
	if(GPIO_Pin == GPIO_PIN_1)
	{
		if (!(GPIOC->IDR & GPIO_PIN_0) && !(GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x00;
		} else if ((GPIOC->IDR & GPIO_PIN_0) && !(GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x10;
		} else if (!(GPIOC->IDR & GPIO_PIN_0) && (GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x01;
		} else if ((GPIOC->IDR & GPIO_PIN_0) && (GPIOC->IDR & GPIO_PIN_1)) {
			status = 0x11;
		}
		if ((status_old == 0x10) && (status == 0x11)) {
			counter += 1;
			if (number_str < 50) {
				number_str += 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 50;
			}
		} else if ((status_old == 0x01) && (status == 0x00)) {
			counter += 1;
			if (number_str < 50) {
				number_str += 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 50;
			}
		}
		if (status_old == 0x11 && status == 0x10) {
			status_old = 0x10;

		} else if (status_old == 0x00 && status == 0x01) {
			status_old = 0x01;
		} else if (status_old == 0x10 && status == 0x00) {
			counter -= 1;
			if (number_str > 0) {
				number_str -= 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 0;
			}
		} else if (status_old == 0x01 && status == 0x11) {
			counter -= 1;
			if (number_str > 0) {
				number_str -= 10;
				GMG12864_Clean_Frame_buffer();
			} else {
				number_str = 0;
			}
		}
		status_old = status;

	}
	if(GPIO_Pin == GPIO_PIN_2)
	{
		flag_led = !flag_led;
		stage_menu = !stage_menu;
		update_display = 1;
		tim4_flag = 0;
	}
}

void print_first_line()
{
	sprintf(tx_buffer, "User led param.");
	GMG12864_Decode_UTF8(15, 0, 1, 0, tx_buffer);
	GMG12864_Update();
}

void print_second_line()
{
	sprintf(tx_buffer, "TIM_4 param");
	GMG12864_Decode_UTF8(15, 10, 1, 0, tx_buffer);
	GMG12864_Update();
}

void print_third_line()
{
	sprintf(tx_buffer, "Uart param");
	GMG12864_Decode_UTF8(15, 20, 1, 0, tx_buffer);
	GMG12864_Update();
}

void print_fourth_line()
{
	sprintf(tx_buffer, "Fourth line");
	GMG12864_Decode_UTF8(15, 30, 1, 0, tx_buffer);
	GMG12864_Update();
}

void print_fifth_line()
{
	sprintf(tx_buffer, "Fifth line");
	GMG12864_Decode_UTF8(15, 40, 1, 0, tx_buffer);
	GMG12864_Update();
}

void print_sixth_line()
{
	sprintf(tx_buffer, "Sixth line");
	GMG12864_Decode_UTF8(15, 50, 1, 0, tx_buffer);
	GMG12864_Update();
}

void pointer_write()
{
	sprintf(tx_buffer, "->");
	GMG12864_Decode_UTF8(0, number_str, 1, 0, tx_buffer);
	GMG12864_Update();
}

void upd_disp()
{
	if(update_display)
	{
		GMG12864_Clean_Frame_buffer();
		update_display = 0;
	}
}

void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
		rx_uart_buffer_len = rx_uart_buffer_size - huart->RxXferCount;
		HAL_UART_AbortReceive_IT(&huart2);
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
		HAL_UART_Receive_IT(&huart2, rx_uart_buffer, rx_uart_buffer_size);
		rx_uart_buffer_ready = 1;
	}
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
