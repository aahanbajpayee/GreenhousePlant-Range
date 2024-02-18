/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "htu21d.h"
#include "string.h"
#include "stdio.h"
#include "NanoEdgeAI.h"
#include "knowledge.h"
#include "sh1106.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_INPUT_USER 256
#define AXIS_NUMBER 2
#define CONFIRMATIONS_NB	(uint32_t)(3)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
float h21_buffer[DATA_INPUT_USER * AXIS_NUMBER] = {0};
float count = 0;
float output_class_buffer[CLASS_NUMBER]; // Buffer of class probabilities
const char *id2class[CLASS_NUMBER + 1] = { // Buffer for mapping class id to class name
	"unknown",
	"class18_19_75_76_data",
	"class18_19_76_77_data",
	"class18_19_77_78_data",
	"class18_19_78_79_data",
	"class18_19_79_80_data",
	"class18_19_80_81_data",
	"class18_19_81_82_data",
	"class18_19_82_83_data",
	"class18_19_83_84_data",
	"class18_19_84_85_data",
	"class19_20_75_76_data",
	"class19_20_76_77_data",
	"class19_20_77_78_data",
	"class19_20_78_79_data",
	"class19_20_79_80_data",
	"class19_20_80_81_data",
	"class19_20_81_82_data",
	"class19_20_82_83_data",
	"class19_20_83_84_data",
	"class19_20_84_85_data",
	"class20_21_75_76_data",
	"class20_21_76_77_data",
	"class20_21_77_78_data",
	"class20_21_78_79_data",
	"class20_21_79_80_data",
	"class20_21_80_81_data",
	"class20_21_81_82_data",
	"class20_21_82_83_data",
	"class20_21_83_84_data",
	"class20_21_84_85_data",
	"class21_22_75_76_data",
	"class21_22_76_77_data",
	"class21_22_77_78_data",
	"class21_22_78_79_data",
	"class21_22_79_80_data",
	"class21_22_80_81_data",
	"class21_22_81_82_data",
	"class21_22_82_83_data",
	"class21_22_83_84_data",
	"class21_22_84_85_data",
	"class22_23_75_76_data",
	"class22_23_76_77_data",
	"class22_23_77_78_data",
	"class22_23_78_79_data",
	"class22_23_79_80_data",
	"class22_23_80_81_data",
	"class22_23_81_82_data",
	"class22_23_82_83_data",
	"class22_23_83_84_data",
	"class22_23_84_85_data",
	"class23_24_75_76_data",
	"class23_24_76_77_data",
	"class23_24_77_78_data",
	"class23_24_78_79_data",
	"class23_24_79_80_data",
	"class23_24_80_81_data",
	"class23_24_81_82_data",
	"class23_24_82_83_data",
	"class23_24_83_84_data",
	"class23_24_84_85_data",
	"Coldroom",
	"Heatedroom",
	"outside",
	"roomtemperature",
	"acroom",
};
int Temp_,Humid_;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void fill_h21_buffer();
//void Log();
void Inference();
void printtemp();
void printhumd();
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
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  OLED_init();
  wakeUpHTU21D();

//  HTU21DF_Init();
  enum neai_state error_code = neai_classification_init(knowledge);
        	if (error_code != NEAI_OK) {
        		/* This happens if the knowledge does not correspond to the library or if the library works into a not supported board. */
        		printf("Knowledge Initialization ERROR!");
        	}
        	else{
        		printf("Knowledge Initialization DONE!");
        	}
    HAL_Delay(50);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Inference();
//	  Log();
//	  readTemperature();
//	  readHumidity();
//	  printf(" T= %.2f \t H= %.2f \r\n ",Temp,Humid);

	  HAL_Delay(15);
	  count = count + 1;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

void fill_h21_buffer() {
	for (int i = 0; i < DATA_INPUT_USER; i++) {
		readTemperature();
		readHumidity();
		printtemp();
		printhumd();
		h21_buffer[AXIS_NUMBER * i] = Temp;
		h21_buffer[AXIS_NUMBER * i + 1] = Humid;
//		h21_buffer[AXIS_NUMBER * i] = T;
//		h21_buffer[AXIS_NUMBER * i + 1] = H;
	}
}

void Log(){
	fill_h21_buffer();
	for (int i = 0; i < DATA_INPUT_USER; i++) {
		printf("%.2f", h21_buffer[AXIS_NUMBER * i]);
		printf(" ");
		printf("%.2f", h21_buffer[AXIS_NUMBER * i + 1]);
		printf(" ");
	}
	printf("\r\n");
	HAL_Delay(100);
}

void Inference(){
	uint16_t i, id_class_t0, id_class_tn;
//	char str[3];
	fill_h21_buffer();
	neai_classification(h21_buffer, output_class_buffer, &id_class_t0);
	for(i=0; i<CONFIRMATIONS_NB-1; i++){
		fill_h21_buffer();
		neai_classification(h21_buffer, output_class_buffer, &id_class_tn);
		if(id_class_t0 != id_class_tn){
			break;
		}
		if(id_class_t0 == id_class_tn){
			printf("Detected Class:");
			printf(id2class[id_class_t0]);
			printf("\r\n");

			if(id_class_t0 > 60){
				printf("Non Optimal");
				printf("\r\n");
				OLED_set_position(3, 5);
				printStr("Non Optimal");
//				sprintf(str, "%s",id2class[id_class_t0]);
//				printStr(str);
			}
			else{
				printf("Optimal");
				printf("\r\n");
				OLED_set_position(3, 5);
				printStr("Optimal");
				//				sprintf(str, "%s",id2class[id_class_t0]);
				//				printStr(str);
			}
		}
		else{
			printf("?");
			printf("\r\n");
		}
	}
}

void printtemp(){
	char str[3];
	OLED_set_position(1, 5);
	printStr("T: ");
	Temp_ = Temp;
	sprintf(str, "%d",Temp_);
	printStr(str);
	printStr(" degC");
	if(Temp_  >= 18 && Temp_ <= 24){
		OLED_set_position(4,5);
		printStr("Temp OK");
	}
	else{
		OLED_set_position(4,5);
		printStr("Temp Not OK");
	}
//	printStr("\r\n");
}
void printhumd(){
	char str[3];
	OLED_set_position(2, 5);
	printStr("H: ");
	Humid_ = Humid;
	sprintf(str, "%d",Humid_);
	printStr(str);
	printStr(" RH");
//	printStr("\r\n");
	if(Humid_  >= 75 && Humid_ <= 85){
			OLED_set_position(5,5);
			printStr("Hum OK");
		}
		else{
			OLED_set_position(5,5);
			printStr("Hum Not OK");
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
