/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stm32f1xx_it.h" // 为了访问hdma_tim2_ch1
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_COUNT 8        // WS2812B灯的数量
#define WS2812B_ARR 89      // 定时器自动重载值
#define WS2812B_0_CODE 29   // 0码的占空比(约0.4μs)
#define WS2812B_1_CODE 58   // 1码的占空比(约0.8μs)
#define RESET_TIME_MS 50    // 复位时间(毫秒)
#define DATA_BUFFER_SIZE (LED_COUNT * 24 + 20)  // 数据缓冲区大小
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t ws2812b_buffer[DATA_BUFFER_SIZE]; // WS2812B数据缓冲区
extern DMA_HandleTypeDef hdma_tim2_ch1; // 声明外部DMA句柄
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void WS2812B_Init(void);
void WS2812B_SetColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void WS2812B_Show(void);
void WS2812B_Clear(void);
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
  MX_DMA_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  WS2812B_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 为所有6个LED设置不同的颜色
//    WS2812B_SetColor(0, 255, 0, 0);    // LED 0: 红色
//    WS2812B_SetColor(1, 0, 255, 0);    // LED 1: 绿色
//    WS2812B_SetColor(2, 0, 0, 255);    // LED 2: 蓝色
//    WS2812B_SetColor(3, 255, 255, 0);  // LED 3: 黄色
//    WS2812B_SetColor(4, 255, 0, 255);  // LED 4: 品红
//    WS2812B_SetColor(5, 0, 255, 255);  // LED 5: 青色

	  WS2812B_SetColor(0, 255, 120, 0);
	  WS2812B_SetColor(1, 255, 120, 0);
	  WS2812B_SetColor(2, 255, 120, 0);
	  WS2812B_SetColor(3, 255, 120, 0);
	  WS2812B_SetColor(4, 255, 120, 0);
	  WS2812B_SetColor(5, 255, 120, 0);
	  WS2812B_SetColor(6, 255, 120, 0);
	  WS2812B_SetColor(7, 255, 120, 0);


    // 发送数据到所有LED
    WS2812B_Show();
    
    // 延时一段时间
//    HAL_Delay(1500);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
}

/* USER CODE BEGIN 4 */
/**
  * @brief  初始化WS2812B
  * @retval None
  */
void WS2812B_Init(void)
{
  // 清除缓冲区
  memset(ws2812b_buffer, 0, sizeof(ws2812b_buffer));
  
  // 启动PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/**
  * @brief  设置WS2812B的颜色
  * @param  index: 灯的索引
  * @param  red: 红色值(0-255)
  * @param  green: 绿色值(0-255)
  * @param  blue: 蓝色值(0-255)
  * @retval None
  */
void WS2812B_SetColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
  if (index >= LED_COUNT)
    return;
  
  uint32_t start_index = index * 24;
  uint8_t color;
  
  // WS2812B的数据格式是GRB
  // 设置绿色
  color = green;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (color & 0x80)
      ws2812b_buffer[start_index + i] = WS2812B_1_CODE;
    else
      ws2812b_buffer[start_index + i] = WS2812B_0_CODE;
    color <<= 1;
  }
  
  // 设置红色
  color = red;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (color & 0x80)
      ws2812b_buffer[start_index + 8 + i] = WS2812B_1_CODE;
    else
      ws2812b_buffer[start_index + 8 + i] = WS2812B_0_CODE;
    color <<= 1;
  }
  
  // 设置蓝色
  color = blue;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (color & 0x80)
      ws2812b_buffer[start_index + 16 + i] = WS2812B_1_CODE;
    else
      ws2812b_buffer[start_index + 16 + i] = WS2812B_0_CODE;
    color <<= 1;
  }
}

/**
  * @brief  发送数据到WS2812B
  * @retval None
  */
void WS2812B_Show(void)
{
  // 确保之前的传输已完成
  while (HAL_DMA_GetState(&hdma_tim2_ch1) != HAL_DMA_STATE_READY);
  
  // 先停止PWM，提供一个明确的重置信号（低电平）
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
  
  // 确保有足够长的重置时间 (>50μs)
  // 这里使用简单的延时，实际项目中可以使用更精确的延时方法
  for (uint32_t i = 0; i < 800; i++) {}
  
  // 启动DMA传输
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, ws2812b_buffer, DATA_BUFFER_SIZE);
  
  // 等待DMA传输完成
  while (HAL_DMA_GetState(&hdma_tim2_ch1) != HAL_DMA_STATE_READY);
  
  // 传输完成后再次停止PWM，保持低电平
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

/**
  * @brief  清除WS2812B所有灯
  * @retval None
  */
void WS2812B_Clear(void)
{
  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    WS2812B_SetColor(i, 0, 0, 0);
  }
  WS2812B_Show();
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
#ifdef USE_FULL_ASSERT
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
