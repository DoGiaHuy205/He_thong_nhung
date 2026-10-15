/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

osThreadId Task_ButtonHandle;
osThreadId Task_LED1Handle;
osThreadId Task_LED2Handle;
osThreadId Task_LED3Handle;
osMessageQId myQueueHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void StartTaskButton(void const * argument);
void StartTaskLED1(void const * argument);
void StartTaskLED2(void const * argument);
void StartTaskLED3(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t led_duty[3] = {0, 0, 0}; // Mang luu do sang cua 3 LED (0 den 1000)
uint8_t current_led = 0;          // ID den dang chon (0: LED1, 1: LED2, 2: LED3)
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //cap xung pwm
  /* USER CODE END 2 */


  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue */
  osMessageQDef(myQueue, 16, uint16_t);
  myQueueHandle = osMessageCreate(osMessageQ(myQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_Button */
  osThreadDef(Task_Button, StartTaskButton, osPriorityNormal, 0, 128);
  Task_ButtonHandle = osThreadCreate(osThread(Task_Button), NULL);

  /* definition and creation of Task_LED1 */
  osThreadDef(Task_LED1, StartTaskLED1, osPriorityNormal, 0, 128);
  Task_LED1Handle = osThreadCreate(osThread(Task_LED1), NULL);

  /* definition and creation of Task_LED2 */
  osThreadDef(Task_LED2, StartTaskLED2, osPriorityNormal, 0, 128);
  Task_LED2Handle = osThreadCreate(osThread(Task_LED2), NULL);

  /* definition and creation of Task_LED3 */
  osThreadDef(Task_LED3, StartTaskLED3, osPriorityNormal, 0, 128);
  Task_LED3Handle = osThreadCreate(osThread(Task_LED3), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999; //cong thuc chia tan de bam xung
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : BTN_SELECT_Pin BTN_UP_Pin BTN_DOWN_Pin */ //pin12: select, pin13: up, pin14 : down
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTaskButton */
/**
  * @brief  Function implementing the Task_Button thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskButton */
void StartTaskButton(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
 for(;;)
  {
    // 1. NUT SELECT - Chon LED (Chan PB12)
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) {
      osDelay(50); // Chong rung phim
      if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) {
        current_led++;
        if (current_led > 2) current_led = 0; // Xoay vong 0 -> 1 -> 2
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) osDelay(10); // Cho nha nut
      }
    }

    // 2. NUT UP - Tang do sang (Chan PB13)
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET) {
      osDelay(50);
      if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET) {
        if (led_duty[current_led] < 1000) {
          led_duty[current_led] += 100; // Tang 10%
        }
        
        // DONG GOI BIT: Dich ID len 12 bit cao, giu do sang o 12 bit thap
        uint16_t msg = (current_led << 12) | led_duty[current_led];
        osMessagePut(myQueueHandle, msg, 100); // Gui tin nhan vao Queue
        
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET) osDelay(10);
      }
    }

    // 3. NUT DOWN - Giam do sang (Chan PB14)
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET) {
      osDelay(50);
      if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET) {
        if (led_duty[current_led] >= 100) {
          led_duty[current_led] -= 100; // Giam 10%
        }
        
        // Dong goi bit tuong tu nhu tren
        uint16_t msg = (current_led << 12) | led_duty[current_led];
        osMessagePut(myQueueHandle, msg, 100); 
        
        while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET) osDelay(10);
      }
    }

    osDelay(20); // Nhuong CPU cho Task khac
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskLED1 */
/**
* @brief Function implementing the Task_LED1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED1 */
void StartTaskLED1(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED1 */
  /* Infinite loop */
 for(;;)
  {
    osEvent evt = osMessageGet(myQueueHandle, osWaitForever); // Cho lay tin nhan
    if (evt.status == osEventMessage) {
      uint16_t msg = evt.value.v;
      
      // GIAI MA BIT
      uint8_t target_led = msg >> 12;         // Doc 4 bit cao lay ID den
      uint16_t duty = msg & 0x0FFF;           // Mat na bit de lay 12 bit thap chua do sang
      
      if (target_led == 0) { 
        // Dung la ID cua LED 1 -> Cap nhat PWM o CHANNEL 1
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
      } else {
        // Cua LED khac -> Tra lai vao Queue
        osMessagePut(myQueueHandle, msg, 0);
        osDelay(2); // Di ngu 2ms de nhuong cho Task cua LED khac lay
      }
    }
  }
  /* USER CODE END StartTaskLED1 */
}

/* USER CODE BEGIN Header_StartTaskLED2 */
/**
* @brief Function implementing the Task_LED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED2 */
void StartTaskLED2(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED2 */
  /* Infinite loop */
  for(;;)
  {
    osEvent evt = osMessageGet(myQueueHandle, osWaitForever);
    if (evt.status == osEventMessage) {
      uint16_t msg = evt.value.v;
      uint8_t target_led = msg >> 12;
      uint16_t duty = msg & 0x0FFF;
      
      if (target_led == 1) { // LED 2 ID la 1
        // Cap nhat PWM o CHANNEL 2
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty);
      } else {
        osMessagePut(myQueueHandle, msg, 0);
        osDelay(2); 
      }
    }
  }
  /* USER CODE END StartTaskLED2 */
}

/* USER CODE BEGIN Header_StartTaskLED3 */
/**
* @brief Function implementing the Task_LED3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED3 */
void StartTaskLED3(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED3 */
  /* Infinite loop */
  for(;;)
  {
    osEvent evt = osMessageGet(myQueueHandle, osWaitForever);
    if (evt.status == osEventMessage) {
      uint16_t msg = evt.value.v;
      uint8_t target_led = msg >> 12;
      uint16_t duty = msg & 0x0FFF;
      
      if (target_led == 2) { // LED 3 ID la 2
        // Cap nhat PWM o CHANNEL 3
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty);
      } else {
        osMessagePut(myQueueHandle, msg, 0);
        osDelay(2);
      }
    }
  }
  /* USER CODE END StartTaskLED3 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
