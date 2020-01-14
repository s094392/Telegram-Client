/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32l4xx.h"
#include "stm32l4xx_nucleo.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EPD_Test.h"
#include "EPD_2IN13_V2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SET_REG(REG, SELECT, VAL) ((REG) = ((REG) & (~(SELECT))) | (VAL))
#define GET_REG(REG, SELECT) ((REG) & (SELECT))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void EPD_test(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void EXTI_config() {
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM0, EXTI_IMR1_IM0);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM1, EXTI_IMR1_IM1);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM2, EXTI_IMR1_IM2);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM3, EXTI_IMR1_IM3);

    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT0, EXTI_RTSR1_RT0);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT1, EXTI_RTSR1_RT1);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT2, EXTI_RTSR1_RT2);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT3, EXTI_RTSR1_RT3);

    SET_REG(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN, RCC_APB2ENR_SYSCFGEN);

    SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0, SYSCFG_EXTICR1_EXTI0_PC);
    SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI1, SYSCFG_EXTICR1_EXTI1_PC);
    SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI2, SYSCFG_EXTICR1_EXTI2_PC);
    SET_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI3, SYSCFG_EXTICR1_EXTI3_PC);
}

void NVIC_config() {
    NVIC->ISER[0] |= 0b1 << EXTI0_IRQn;
    NVIC->ISER[0] |= 0b1 << EXTI1_IRQn;
    NVIC->ISER[0] |= 0b1 << EXTI2_IRQn;
    NVIC->ISER[0] |= 0b1 << EXTI3_IRQn;
}

void EXTI0_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF0)) {
        int x = keypad_scan(0);
        SET_REG(EXTI->PR1, EXTI_PR1_PIF0, EXTI_PR1_PIF0);
    }
}

void EXTI1_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF1)) {
        int x = keypad_scan(1);
        SET_REG(EXTI->PR1, EXTI_PR1_PIF1, EXTI_PR1_PIF1);
    }
}

void EXTI2_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF2)) {
        int x = keypad_scan(2);
        SET_REG(EXTI->PR1, EXTI_PR1_PIF2, EXTI_PR1_PIF2);
    }
}

void EXTI3_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF3)) {
        int x = keypad_scan(3);
        SET_REG(EXTI->PR1, EXTI_PR1_PIF3, EXTI_PR1_PIF3);
    }
}

int keypad_scan(int target) {
    int key_map[4][4] = {
        {1, 4, 7, 15}, {2, 5, 8, 0}, {3, 6, 9, 14}, {10, 11, 12, 13}};
    uint32_t x_pin[4] = {KEYPAD_OUT_0, KEYPAD_OUT_1, KEYPAD_OUT_2, KEYPAD_OUT_3};
    uint32_t y_pin[4] = {KEYPAD_IN_0, KEYPAD_IN_1, KEYPAD_IN_2, KEYPAD_IN_3};
    int result = -1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                KEYPAD_PORT->BSRR |= x_pin[j];
            } else {
                KEYPAD_PORT->BRR |= x_pin[j];
            }
        }
        int cnt = 0;
        for (int k = 0; k < 100; k++) {
            uint32_t on = KEYPAD_PORT->IDR & y_pin[target];
            if (on) cnt++;
        }
        if (cnt >= 99) result = key_map[i][target];
    }
    for (int j = 0; j < 4; j++) {
        KEYPAD_PORT->BSRR |= x_pin[j];
    }
    return result;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

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
    MX_USART1_UART_Init();
    MX_SPI1_Init();
    NVIC_config();
    EXTI_config();
    KEYPAD_PORT->BSRR |= KEYPAD_OUT_0;
    KEYPAD_PORT->BSRR |= KEYPAD_OUT_1;
    KEYPAD_PORT->BSRR |= KEYPAD_OUT_2;
    KEYPAD_PORT->BSRR |= KEYPAD_OUT_3;
    printf("Init Done\r\n");

	EPD_test();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {

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

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 36;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

      /**Configure the Systick interrupt time
      */
      HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

      /**Configure the Systick
      */
      HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

      /* SysTick_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void EPD_test(void)
{
	DEV_Module_Init();
	EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
	EPD_2IN13_V2_Clear();
	DEV_Delay_ms(500);

	//Create a new image cache
	UBYTE *BlackImage;
	/* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
	UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
	    printf("Failed to apply for black memory...\r\n");
	    return -1;
	}

	Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
	Paint_SelectImage(BlackImage);
	Paint_SetMirroring(MIRROR_HORIZONTAL); //
	Paint_Clear(WHITE);
	EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
	EPD_2IN13_V2_DisplayPartBaseImage(BlackImage);

	EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);

	Paint_SelectImage(BlackImage);

	while(1) {
		char text[20];
		scanf("%s", text);
		Paint_ClearWindows(20, 15, 20 + Font20.Width * 15, 15 + Font20.Height, WHITE);
		Paint_DrawString_EN(20, 15, text, &Font16, WHITE, BLACK);
		EPD_2IN13_V2_DisplayPart(BlackImage);
		DEV_Delay_ms(1000)
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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
