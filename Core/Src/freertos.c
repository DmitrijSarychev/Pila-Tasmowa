/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "Pila.h"
#include "UART_LCD.h"
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define OS_DELAY 20
#define LCD_DELAY 60
#define BUTTON_PRESS_DELAY 10
#define BUTTON_HOLD_DELAY 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t Input_1[2];			//Input data buffer
uint8_t Input_2[2];
uint8_t Input_3[2];
uint8_t Input_4[2];
uint16_t Input_1_Sum = 0;
uint16_t Input_2_Sum = 0;
uint16_t Input_3_Sum = 0;
uint8_t SawTension = 0;
uint8_t SawMatClamp = 0;
uint8_t MatClamp = 0;
uint8_t SawDownDelay = 0;
uint8_t CheckForBrokenSaw = 0;
uint8_t Step = 0;
uint8_t IsRunning = 0;
uint8_t EmergencyStop = 0;
uint8_t BrokenBlade = 0;
///////////////////////////////////////////BUTTTON CONTROLL BEGIN
uint8_t LeftButtonPressed = 0;
uint8_t RightButtonPressed = 0;
uint8_t CenterButtonPressed = 0;
uint8_t LeftButtonPressDuration = BUTTON_PRESS_DELAY;
uint8_t RightButtonPressDuration = BUTTON_PRESS_DELAY;
uint8_t CenterButtonPressDuration = BUTTON_PRESS_DELAY;
///////////////////////////////////////////BUTTON CONTROLL END
uint16_t NominalPartCount =  1;
uint16_t CompletePartCount = 0;
char LCDBufferLine1 [25];
char LCDBufferLine2 [25];
/*************************/
uint8_t Output_1[3] = {OUTPUT_REGISTER, 0x00, 0x00};		//Output data buffer
uint8_t Output_2[3] = {OUTPUT_REGISTER, 0x00, 0x00};
uint16_t Output_1_Sum = 0;
uint16_t Output_2_Sum = 0;

///////////////////////////////////////////RGB CONTROLL BEGIN
uint8_t BacklightColor = BLUE;

///////////////////////////////////////////RGB CONTROLL END


/* USER CODE END Variables */
osThreadId InputHandle;
osThreadId ComputeHandle;
osThreadId OutputHandle;
osThreadId AutoHandle;
osThreadId ManualHandle;
osThreadId LCDUpdateHandle;
osTimerId OTimer1Handle;
osTimerId OTimer2Handle;
osTimerId OTimer3Handle;
osTimerId OTimer4Handle;
osTimerId OTimer5Handle;
osTimerId OTimer6Handle;
osTimerId OTimer7Handle;
osTimerId OTimer8Handle;
osTimerId OTimer9Handle;
osTimerId OTimer10Handle;
osSemaphoreId InOutSemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void AllOutputsOff(void);
void UpdateLCDDisplay (void);
void ButtonRoutine(void);
/* USER CODE END FunctionPrototypes */

void StartInput(void const * argument);
void StartCompute(void const * argument);
void StartOutput(void const * argument);
void StartAuto(void const * argument);
void StartManual(void const * argument);
void StartLCDUpdate(void const * argument);
void OTimer1Callback(void const * argument);
void OTimer2Callback(void const * argument);
void OTimer3Callback(void const * argument);
void OTimer4Callback(void const * argument);
void OTimer5Callback(void const * argument);
void OTimer6Callback(void const * argument);
void OTimer7Callback(void const * argument);
void OTimer8Callback(void const * argument);
void OTimer9Callback(void const * argument);
void OTimer10Callback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of InOutSem */
  osSemaphoreDef(InOutSem);
  InOutSemHandle = osSemaphoreCreate(osSemaphore(InOutSem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of OTimer1 */
  osTimerDef(OTimer1, OTimer1Callback);
  OTimer1Handle = osTimerCreate(osTimer(OTimer1), osTimerOnce, NULL);

  /* definition and creation of OTimer2 */
  osTimerDef(OTimer2, OTimer2Callback);
  OTimer2Handle = osTimerCreate(osTimer(OTimer2), osTimerOnce, NULL);

  /* definition and creation of OTimer3 */
  osTimerDef(OTimer3, OTimer3Callback);
  OTimer3Handle = osTimerCreate(osTimer(OTimer3), osTimerOnce, NULL);

  /* definition and creation of OTimer4 */
  osTimerDef(OTimer4, OTimer4Callback);
  OTimer4Handle = osTimerCreate(osTimer(OTimer4), osTimerOnce, NULL);

  /* definition and creation of OTimer5 */
  osTimerDef(OTimer5, OTimer5Callback);
  OTimer5Handle = osTimerCreate(osTimer(OTimer5), osTimerOnce, NULL);

  /* definition and creation of OTimer6 */
  osTimerDef(OTimer6, OTimer6Callback);
  OTimer6Handle = osTimerCreate(osTimer(OTimer6), osTimerOnce, NULL);

  /* definition and creation of OTimer7 */
  osTimerDef(OTimer7, OTimer7Callback);
  OTimer7Handle = osTimerCreate(osTimer(OTimer7), osTimerOnce, NULL);

  /* definition and creation of OTimer8 */
  osTimerDef(OTimer8, OTimer8Callback);
  OTimer8Handle = osTimerCreate(osTimer(OTimer8), osTimerOnce, NULL);

  /* definition and creation of OTimer9 */
  osTimerDef(OTimer9, OTimer9Callback);
  OTimer9Handle = osTimerCreate(osTimer(OTimer9), osTimerOnce, NULL);

  /* definition and creation of OTimer10 */
  osTimerDef(OTimer10, OTimer10Callback);
  OTimer10Handle = osTimerCreate(osTimer(OTimer10), osTimerOnce, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Input */
  osThreadDef(Input, StartInput, osPriorityRealtime, 0, 128);
  InputHandle = osThreadCreate(osThread(Input), NULL);

  /* definition and creation of Compute */
  osThreadDef(Compute, StartCompute, osPriorityNormal, 0, 128);
  ComputeHandle = osThreadCreate(osThread(Compute), NULL);

  /* definition and creation of Output */
  osThreadDef(Output, StartOutput, osPriorityNormal, 0, 128);
  OutputHandle = osThreadCreate(osThread(Output), NULL);

  /* definition and creation of Auto */
  osThreadDef(Auto, StartAuto, osPriorityNormal, 0, 128);
  AutoHandle = osThreadCreate(osThread(Auto), NULL);

  /* definition and creation of Manual */
  osThreadDef(Manual, StartManual, osPriorityNormal, 0, 128);
  ManualHandle = osThreadCreate(osThread(Manual), NULL);

  /* definition and creation of LCDUpdate */
  osThreadDef(LCDUpdate, StartLCDUpdate, osPriorityNormal, 0, 128);
  LCDUpdateHandle = osThreadCreate(osThread(LCDUpdate), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartInput */
/**
  * @brief  Function implementing the Input thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartInput */
void StartInput(void const * argument)
{
  /* USER CODE BEGIN StartInput */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreWait(InOutSemHandle, 1000);
	  HAL_I2C_Master_Receive(&hi2c1, INPUT_MODULE_ADDR_1, Input_1, 2, 10);
	  HAL_I2C_Master_Receive(&hi2c1, INPUT_MODULE_ADDR_2, Input_2, 2, 10);
	  HAL_I2C_Master_Receive(&hi2c1, INPUT_MODULE_ADDR_3, Input_3, 2, 10);
	  HAL_I2C_Master_Receive(&hi2c1, INPUT_MODULE_ADDR_4, Input_4, 2, 10);
	  Input_1_Sum = (Input_1[1]<<8) | Input_1[0];				//Combine 2 8bit registers to 1 16 bit for the ease of operation
	  Input_2_Sum = (Input_2[1]<<8) | Input_2[0];				//Combine 2 8bit registers to 1 16 bit for the ease of operation
	  Input_3_Sum = (Input_3[1]<<8) | Input_3[0];				//Combine 2 8bit registers to 1 16 bit for the ease of operation
	  osSemaphoreRelease(InOutSemHandle);

	osDelay(OS_DELAY);
  }
  /* USER CODE END StartInput */
}

/* USER CODE BEGIN Header_StartCompute */
/**
* @brief Function implementing the Compute thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCompute */
void StartCompute(void const * argument)
{
  /* USER CODE BEGIN StartCompute */
  /* Infinite loop */
  for(;;)
  {
	  if(IN_0_16_AUTO_MANUAL)
	  	    {
	  	    	osThreadSuspend(ManualHandle);
	  	    	osThreadResume(AutoHandle);
	  	    	BacklightColor = GREEN;
	  	    }
	  	    else
	  	    {
	  	    	osThreadSuspend(AutoHandle);
	  	    	osThreadResume(ManualHandle);
	  	    	BacklightColor = WHITE;
	  	    }
	  osDelay(OS_DELAY);
  }
  /* USER CODE END StartCompute */
}

/* USER CODE BEGIN Header_StartOutput */
/**
* @brief Function implementing the Output thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOutput */
void StartOutput(void const * argument)
{
  /* USER CODE BEGIN StartOutput */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreWait(InOutSemHandle, 1000);
	  Output_2[1] = Output_2_Sum>>8;
	  Output_2[2] = Output_2_Sum & 0x00ff;
	  Output_1[1] = Output_1_Sum>>8;
	  Output_1[2] = Output_1_Sum & 0x00ff;
	  HAL_I2C_Master_Transmit(&hi2c1, OUTPUT_MODULE_ADDR_1, Output_1, 3, 10);
	  HAL_I2C_Master_Transmit(&hi2c1, OUTPUT_MODULE_ADDR_2, Output_2, 3, 10);
	  //UpdateLCDDisplay();
	  osSemaphoreRelease(InOutSemHandle);
	  osDelay(OS_DELAY);
  }
  /* USER CODE END StartOutput */
}

/* USER CODE BEGIN Header_StartAuto */
/**
* @brief Function implementing the Auto thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAuto */
void StartAuto(void const * argument)
{
  /* USER CODE BEGIN StartAuto */
  /* Infinite loop */
  for(;;)
  {

	  	/*Start automatic cycle, hydraulic pump has to be on, stop button NOT pressed, saw position up (up limit switch)*/
	if (IN_0_15_HPUMP && IN_0_14_START && IN_0_13_STOP && IN_1_13_SAFETY && !(/*IN_1_14_SAWUP_LS*/IN_1_9_SAWDOWN_LS))
	{
		/*Set the first step. Steps in increments of 10 for easy code modification and updates*/
		EmergencyStop = 0;
		BrokenBlade = 0;
		IsRunning = 1;
		Step = 10;
	}

	else if (!IsRunning)
	{
		if (IN_1_9_SAWDOWN_LS)
		{
			EmergencyStop = 1;
		}
		else
		{
			EmergencyStop = 0;
		}
	}


		/*Anytime if STOP is pressed, either hood open (safety latch), Hydraulic Pump turned off or either fuses blown (10 and 11), stop AUTO*/
	if(!(IN_0_13_STOP) || !(IN_1_13_SAFETY) || !(IN_0_15_HPUMP) || IN_1_16_FUSE10 || IN_1_15_FUSE11)
	{
		/*Step 0 is the stop for all output and the AUTO cycle*/
		BacklightColor = RED;
		EmergencyStop = 1;
		Step = 0;
	}
		/*Chip collector spiral ON/OFF. Can be toggled at any time in AUTO or MANUAL*/
	if(IN_0_2_CHIP)
	{
		OUT_0_7_CHIP_ON;
	}
	else
	{
		OUT_0_7_CHIP_OFF;
	}
		/*Broken blade detection. Passive wheel stall detection - stall means broken blade. This particular check is for the positive signal*/
	if (IN_1_8_ROTATION && CheckForBrokenSaw)
	{
		osTimerStart(OTimer7Handle, 3000);
	}
		/*Broken blade detection. Passive wheel stall detection - stall means broken blade. This particular check is for the negative signal*/
	if(!(IN_1_8_ROTATION) && CheckForBrokenSaw)
	{
		osTimerStart(OTimer8Handle, 4000);
	}
		/*Hydraulic Pump can be turned ON/OFF any time in the cycle. Turning off the HPump will stop the AUTO cycle and return the Step count to 0*/
	if(IN_0_15_HPUMP)
	{
		OUT_0_12_HPUMP_ON;
	}
	else
	{
		OUT_0_12_HPUMP_OFF;
	}

	if(IsRunning)
	{
		BacklightColor = GREEN;
		sprintf(LCDBufferLine1, "AUTO            ");
		if(!(IN_1_4_SELECT_COUNTER))
			sprintf(LCDBufferLine2, "CIECIE %d/%d      ", CompletePartCount, NominalPartCount);
		else
			sprintf(LCDBufferLine2, "LICZ MECHANICZNY");
	}

	switch (Step)
	{
		case 0: /*Stops all output and AUTO cycle*/
			AllOutputsOff();
				/*Display information if open hatch limit switch or safety E-STOP*/
			if (EmergencyStop)
			{
				if (!(IN_1_13_SAFETY)) 	//Hatch open/E-Stop active
				{
					//BacklightColor = RED;
					sprintf(LCDBufferLine1, "E-STOP/KRANCOWKA");
					sprintf(LCDBufferLine2, "KLAPA           ");
				}

				else if (IN_1_16_FUSE10)		//Blown Resetable fuse 10F1
				{
					//BacklightColor = RED;
					sprintf(LCDBufferLine1, "BEZPIECZNIK 10F1");
					sprintf(LCDBufferLine2, "                ");
				}

				else if (IN_1_15_FUSE11)		//Blown Resetable fuse 11F1
				{
					//BacklightColor = RED;
					sprintf(LCDBufferLine1, "BEZPIECZNIK 11F1");
					sprintf(LCDBufferLine2, "                ");
				}

				else if (!(IN_0_15_HPUMP))		//Hydraulic pump turned off
				{
					//BacklightColor = RED;
					sprintf(LCDBufferLine1, "POMPA HYDR.     ");
					sprintf(LCDBufferLine2, "WYLACZONA!      ");
				}
				else if ((!IsRunning) && (IN_1_9_SAWDOWN_LS))
				{
					BacklightColor = RED;
					sprintf(LCDBufferLine1, "OPUSC PILE      ");
					sprintf(LCDBufferLine2, "(TRYB RECZNY)   ");
				}

			}

			else if(BrokenBlade)
			{
				BacklightColor = RED;
				sprintf(LCDBufferLine1, "PILA ZERWANA    ");
				sprintf(LCDBufferLine2, "                ");
			}

			else
			{
				BacklightColor = GREEN;
				sprintf(LCDBufferLine1, "WCISNIJ         ");
				sprintf(LCDBufferLine2, "START!          ");
			}

				/*Display if broken blade detection worked*/

		break;
		case 10: /*Raises the blade. Redundant action since the blade has to be raised for the cycle to start. Leaving it here. Try and stop me!*/
				/*prepare material for cutting, now the blade starts at the bottom*/
			if(IN_1_7_FEED)
				{
				if (IN_1_14_SAWUP_LS)
					{
						OUT_0_3_SAWUP_ON;
					}
				}
				else
				{
					osDelay(500);
					OUT_0_3_SAW_UP_OFF;
					Step = 11;
				}
		break;
		case 11: /*Rear vice unclamp for reposition. Vice opening has no limit switch, timer based.*/
			if (IN_1_12_MAT_REV_LS)
			{

				OUT_0_9_MAT_CL_UNCLAMP_ON;
				osDelay(500);
				OUT_0_9_MAT_CL_UNCLAMP_OFF;
			}
			else if (!SawMatClamp)
			{

				osDelay(500);
			}
				Step = 12;
		break;

		case 12:/*Move the rear vice in to loading position. Limit switch based.*/
			if(IN_1_12_MAT_REV_LS)
			{
				OUT_0_10_MAT_FWD_ON;
			}
			else
			{
				OUT_0_10_MAT_FWD_OFF;
				Step = 13;
			}
		break;

		case 13: /*Clamp rear vice. Timer based.*/


				OUT_0_8_MAT_CL_CLAMP_ON;
				osDelay(1500);
				OUT_0_8_MAT_CL_CLAMP_OFF;
				Step = 14;

		break;

		case 14: /*Open the main vice slightly, release the cut material. No limit switches, timer based.*/

				OUT_0_6_SAW_CL_UNCLAMP_ON;
				osDelay(700);
				OUT_0_6_SAW_CL_UNCLAMP_OFF;
				Step = 15;
		break;

		case 15: /*Move the material forward, prepare next cut. Limit switch based.*/
			if(IN_1_11_MAT_FWD_LS)
			{
				OUT_0_11_MAT_REV_ON;
			}
			else
			{
				OUT_0_11_MAT_REV_OFF;
				Step = 20;
			}
		break;

		case 20: /*Clamp the main vice (front). No limit and pressure switch (yet). Clamping lasts 2sec. Manual material positioning requirement. Remove after installing a pressure switch.*/
			if (!SawMatClamp)
			{
				SawMatClamp = 1;
				OUT_0_5_SAW_CL_CLAMP_ON;
				osTimerStart(OTimer2Handle, 2000);
			}
		break;

		case 21: /*Rear vice unclamp for reposition. Vice opening has no limit switch, timer based.*/
			if (!(SawMatClamp) && IN_1_12_MAT_REV_LS)
			{
				SawMatClamp = 1;
				OUT_0_9_MAT_CL_UNCLAMP_ON;
				osTimerStart(OTimer6Handle, 500);
			}
			else if (!SawMatClamp)
			{
				SawMatClamp = 1;
				osTimerStart(OTimer6Handle, 500);
			}
		break;

		case 30: /*Move the rear vice in to loading position. Limit switch based.*/
			if(IN_1_12_MAT_REV_LS)
			{
				OUT_0_10_MAT_FWD_ON;
			}
			else
			{
				OUT_0_10_MAT_FWD_OFF;
				Step = 40;
			}
		break;

		case 40: /*Clamp rear vice. Timer based.*/
			if(!SawMatClamp)
			{
				SawMatClamp = 1;
				OUT_0_8_MAT_CL_CLAMP_ON;
				osTimerStart(OTimer9Handle, 1000);
			}

		break;

		case 50: /*Start the saw and according broken blade detection timers*/
			OUT_0_1_SAW_ON;
			/*This parameter starts the "Broken blade detection" feature. It starts after the blade is in motion. Otherwise it will stop preemptively.*/
			CheckForBrokenSaw = 1;
			osTimerStart(OTimer7Handle, 1000);
			osTimerStart(OTimer8Handle, 2000);
			Step = 60;
		break;

		case 60: /*Lower the saw. Start the cutting process. Limit switch based.*/
			if (IN_1_9_SAWDOWN_LS)
			{
				OUT_0_4_SAWDOWN_ON;
				OUT_0_7_CHIP_ON;
				OUT_0_16_3PHASE_BACK_ON;
				if(IN_1_7_FEED)
				{
					OUT_0_13_FAST_FEED_OFF;
				}
				else
				{
					OUT_0_13_FAST_FEED_ON;
				}
			}
			else
			{
				OUT_0_13_FAST_FEED_OFF;
				/*Reach the lower limit switch and wait for 2 seconds.*/
				if(!(SawDownDelay))
				{
					SawDownDelay = 1;
					/*Increment the counter*/ //TODO Old Timer incrementation lives here
					if(!(IN_1_4_SELECT_COUNTER))
						CompletePartCount++;
					else
					{
						OUT_0_2_COUNTER_ON;
						osDelay(100);
						OUT_0_2_COUNTER_OFF;
					}
					osTimerStart(OTimer3Handle, 1000);
				}
			}
		break;

		case 70: /*Raise the blade again to the feed limit switch. Limit switch based. You know the drill... saw...*/
			if(IN_1_7_FEED)
				{
				if (IN_1_14_SAWUP_LS)
					{
						OUT_0_3_SAWUP_ON;
					}
				}
				else
				{
					osDelay(500);
					OUT_0_3_SAW_UP_OFF;
					Step = 80;
				}
		break;

		case 80: /*Open the main vice slightly, release the cut material. No limit switches, timer based.*/
			if (!(SawMatClamp) && IN_1_5_SAW_CL_UNCLAMP_LS)
			{
				SawMatClamp = 1;
				OUT_0_6_SAW_CL_UNCLAMP_ON;
				osTimerStart(OTimer4Handle, 700);
			}
		break;

		case 90: /*Move the material forward, prepare next cut. Limit switch based.*/
			if(IN_1_11_MAT_FWD_LS)
			{
				OUT_0_11_MAT_REV_ON;
			}
			else
			{
				OUT_0_11_MAT_REV_OFF;
				Step = 100;
			}
		break;

		case 100: /*Reclamp the main vice. No limit switches, timer based.*/
			if (!SawMatClamp)
			{
				SawMatClamp = 1;
				OUT_0_5_SAW_CL_CLAMP_ON;
				osTimerStart(OTimer5Handle, 2000);
			}
		break;

		case 110: /*Loosen the rear vice slightly. Timer based. Repeat the cutting cycle. Jump to Step 21*/
			if(!(MatClamp))
			{
				MatClamp = 1;
				//OUT_0_9_MAT_CL_UNCLAMP_ON;
				osTimerStart(OTimer2Handle, 300);
			}
		break;

	}
    osDelay(OS_DELAY);
  }
  /* USER CODE END StartAuto */
}

/* USER CODE BEGIN Header_StartManual */
/**
* @brief Function implementing the Manual thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartManual */
void StartManual(void const * argument)
{
  /* USER CODE BEGIN StartManual */
  /* Infinite loop */



  for(;;)
  {
	  if (IsRunning == 1)
	  {
		  AllOutputsOff();
		  IsRunning = 0;
	  }
	  /*Reset the broken saw check, so that it works correctly when the machine is switched back to AUTO.*/
	  CheckForBrokenSaw = 0;
	  EmergencyStop = 0;
	  BrokenBlade = 0;

	  /*Use this function to set the status and part counter*/
	  ButtonRoutine();
	  BacklightColor = WHITE;
	  sprintf(LCDBufferLine1, "Wprowadz ilosc  ");
	  sprintf(LCDBufferLine2, "           %d    ", NominalPartCount);

	if (IN_0_15_HPUMP)
	{
		OUT_0_12_HPUMP_ON;
	}
	else
	{
		OUT_0_12_HPUMP_OFF;
	}

	if (IN_0_14_START && IN_0_13_STOP)
		{
			OUT_0_1_SAW_ON;
		}

	if(!(IN_0_13_STOP))
	{
		OUT_0_1_SAW_OFF;
	}

	if(IN_0_12_SAWUP && IN_1_14_SAWUP_LS)
	{
		OUT_0_3_SAWUP_ON;
	}
	else
	{
		OUT_0_3_SAW_UP_OFF;
	}

	if(IN_0_11_SDOWN && IN_1_9_SAWDOWN_LS)
	{
		OUT_0_4_SAWDOWN_ON;
	}
	else
	{
		OUT_0_4_SAW_DOWN_OFF;
	}

	if(IN_0_10_MAT_CL_UNCLAMP)
	{
		OUT_0_9_MAT_CL_UNCLAMP_ON;
	}
	else
	{
		OUT_0_9_MAT_CL_UNCLAMP_OFF;
	}

	if(IN_0_9_MAT_CL_CLAMP)
	{
		OUT_0_8_MAT_CL_CLAMP_ON;
	}
	else
	{
		OUT_0_8_MAT_CL_CLAMP_OFF;
	}

	if(IN_0_8_MAT_REV && IN_1_11_MAT_FWD_LS)
	{
		OUT_0_11_MAT_REV_ON;
	}
	else
	{
		OUT_0_11_MAT_REV_OFF;
	}

	if(IN_0_7_MAT_FWD && IN_1_12_MAT_REV_LS)
	{
		OUT_0_10_MAT_FWD_ON;
	}
	else
	{
		OUT_0_10_MAT_FWD_OFF;
	}

	if(IN_0_6_SAW_CL_UNCLAMP && IN_1_5_SAW_CL_UNCLAMP_LS)
	{
		OUT_0_6_SAW_CL_UNCLAMP_ON;
	}
	else
	{
		OUT_0_6_SAW_CL_UNCLAMP_OFF;
	}

	if(IN_0_5_SAW_CL_CLAMP)
	{
		OUT_0_5_SAW_CL_CLAMP_ON;
	}
	else
	{
		OUT_0_5_SAW_CL_CLAMP_OFF;
	}

	if(IN_0_4_TENSION_UP)
	{
		if (!(SawTension))
		{
		SawTension = 1;
		OUT_0_14_TENSION_UP_ON;
		osTimerStart(OTimer1Handle, 5000);
		}
	}
	else
	{
		OUT_0_14_TENSION_UP_OFF;
		osTimerStop(OTimer1Handle);
	}

	if(IN_0_3_TENSION_DOWN)
	{
		SawTension = 0;
		OUT_0_15_TENSION_DOWN_ON;
	}
	else
	{
		OUT_0_15_TENSION_DOWN_OFF;
	}

	if(IN_0_2_CHIP)
	{
		OUT_0_7_CHIP_ON;
	}
	else
	{
		OUT_0_7_CHIP_OFF;
	}

    osDelay(OS_DELAY);
  }
  /* USER CODE END StartManual */
}

/* USER CODE BEGIN Header_StartLCDUpdate */
/**
* @brief Function implementing the LCDUpdate thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLCDUpdate */
void StartLCDUpdate(void const * argument)
{
  /* USER CODE BEGIN StartLCDUpdate */
  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreWait(InOutSemHandle, 1000);
	  UpdateLCDDisplay();
	  osSemaphoreRelease(InOutSemHandle);
	  osDelay(LCD_DELAY);
  }
  /* USER CODE END StartLCDUpdate */
}

/* OTimer1Callback function */
void OTimer1Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer1Callback */
	OUT_0_14_TENSION_UP_OFF;
  /* USER CODE END OTimer1Callback */
}

/* OTimer2Callback function */
void OTimer2Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer2Callback */
	MatClamp = 0;
	SawMatClamp = 0;
	OUT_0_5_SAW_CL_CLAMP_OFF;
	OUT_0_9_MAT_CL_UNCLAMP_OFF;
	Step = 21;
  /* USER CODE END OTimer2Callback */
}

/* OTimer3Callback function */
void OTimer3Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer3Callback */
	SawDownDelay = 0;
	OUT_0_4_SAW_DOWN_OFF;
	if(!(IN_1_4_SELECT_COUNTER))
	{
		if((CompletePartCount != NominalPartCount)/*!(IN_0_1_COUNTER)*/ && IN_1_10_MAT_CLAMP)		//TODO wyedytowany licznik fizyczny na licznik cyfrowy z LCD
		{
			Step = 70;
		}
		else
		{
			Step = 0;
			IsRunning = 0;
			CompletePartCount = 0;
		}
	}
	else
	{
		if(/*(CompletePartCount != NominalPartCount)*/!(IN_0_1_COUNTER) && IN_1_10_MAT_CLAMP)		//TODO wyedytowany licznik fizyczny na licznik cyfrowy z LCD
				{
					Step = 70;
				}
				else
				{
					Step = 0;
					IsRunning = 0;
					CompletePartCount = 0;
				}
	}
  /* USER CODE END OTimer3Callback */
}

/* OTimer4Callback function */
void OTimer4Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer4Callback */
	SawMatClamp = 0;
	OUT_0_6_SAW_CL_UNCLAMP_OFF;
	Step = 90;
  /* USER CODE END OTimer4Callback */
}

/* OTimer5Callback function */
void OTimer5Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer5Callback */
	SawMatClamp = 0;
	OUT_0_5_SAW_CL_CLAMP_ON;
	Step = 110;
  /* USER CODE END OTimer5Callback */
}

/* OTimer6Callback function */
void OTimer6Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer6Callback */
	SawMatClamp = 0;
	OUT_0_9_MAT_CL_UNCLAMP_OFF;
	Step = 30;
  /* USER CODE END OTimer6Callback */
}

/* OTimer7Callback function */
void OTimer7Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer7Callback */
	Step = 0;
	BrokenBlade = 1;
  /* USER CODE END OTimer7Callback */
}

/* OTimer8Callback function */
void OTimer8Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer8Callback */
	Step = 0;
	BrokenBlade = 2;
  /* USER CODE END OTimer8Callback */
}

/* OTimer9Callback function */
void OTimer9Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer9Callback */
	SawMatClamp = 0;
	OUT_0_8_MAT_CL_CLAMP_OFF;
	Step = 50;
  /* USER CODE END OTimer9Callback */
}

/* OTimer10Callback function */
void OTimer10Callback(void const * argument)
{
  /* USER CODE BEGIN OTimer10Callback */
	OUT_0_3_SAW_UP_OFF;
	Step = 80;
  /* USER CODE END OTimer10Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void AllOutputsOff(void)
{
			OUT_0_16_3PHASE_BACK_OFF;	//Output_2_Sum &= ~			OUT_0_15_TENSION_DOWN_OFF;	Output_2_Sum &= ~0b0100000000000000
			//OUT_0_14_TENSION_UP_OFF;	//Output_2_Sum &= ~0b0010000000000000
			OUT_0_13_FAST_FEED_OFF;		//Output_2_Sum &= ~0b0001000000000000
			//OUT_0_12_HPUMP_OFF;		//Output_2_Sum &= ~0b0000100000000000
			OUT_0_11_MAT_REV_OFF;		//Output_2_Sum &= ~0b0000010000000000
			OUT_0_10_MAT_FWD_OFF;		//Output_2_Sum &= ~0b0000001000000000
			OUT_0_9_MAT_CL_UNCLAMP_OFF;	//Output_2_Sum &= ~0b0000000100000000
			OUT_0_8_MAT_CL_CLAMP_OFF;	//Output_2_Sum &= ~0b0000000010000000
			OUT_0_7_CHIP_OFF;			//Output_2_Sum &= ~0b0000000001000000
			OUT_0_6_SAW_CL_UNCLAMP_OFF;	//Output_2_Sum &= ~0b0000000000100000
			OUT_0_5_SAW_CL_CLAMP_OFF;	//Output_2_Sum &= ~0b0000000000010000
			OUT_0_4_SAW_DOWN_OFF;		//Output_2_Sum &= ~0b0000000000001000
			OUT_0_3_SAW_UP_OFF;			//Output_2_Sum &= ~0b0000000000000100
			//OUT_0_2_COUNTER_OFF;		//Output_2_Sum &= ~0b0000000000000010
			OUT_0_1_SAW_OFF;			//Output_2_Sum &= ~0b0000000000000001
			osTimerStop(OTimer1Handle);
			osTimerStop(OTimer2Handle);
			osTimerStop(OTimer3Handle);
			osTimerStop(OTimer4Handle);
			osTimerStop(OTimer5Handle);
			osTimerStop(OTimer6Handle);
			osTimerStop(OTimer7Handle);
			osTimerStop(OTimer8Handle);
			osTimerStop(OTimer9Handle);
			osTimerStop(OTimer10Handle);
			CheckForBrokenSaw = 0;

}

void ButtonRoutine(void)
{
///////////////////////////////////////LEFT BUTTON DECREMENT
	if (IN_1_3_HMI_LEFT && (NominalPartCount != 0))
	{
		if (!LeftButtonPressed)
		{
			LeftButtonPressed = 1;
			NominalPartCount--;
		}

		if (LeftButtonPressDuration == 0)
		{
			NominalPartCount--;
			LeftButtonPressDuration = BUTTON_HOLD_DELAY;
		}
		else
		{
			LeftButtonPressDuration--;
		}

	}
	else
	{
		LeftButtonPressDuration = BUTTON_PRESS_DELAY;
		LeftButtonPressed = 0;
	}

///////////////////////////////////////RIGHT BUTTON INCREMENT
	if (IN_1_1_HMI_RIGHT)
	{
		if (!RightButtonPressed && (NominalPartCount <= 999))
		{
			RightButtonPressed = 1;
			NominalPartCount++;
		}

		if (RightButtonPressDuration == 0)
		{
			NominalPartCount++;
			RightButtonPressDuration = BUTTON_HOLD_DELAY;
		}
		else
		{
			RightButtonPressDuration--;
		}

	}
	else
	{
		RightButtonPressDuration = BUTTON_PRESS_DELAY;
		RightButtonPressed = 0;
	}

///////////////////////////////////////CENTER BUTTON APPROVE
	if (IN_1_2_HMI_CENTER)
	{
		CenterButtonPressed = 1;
		LCD_Init();
	}
	else
	{
		CenterButtonPressed = 0;
	}
}

void UpdateLCDDisplay(void)
{
	LCD_Set_Cursor(1, 1);
	LCD_Write((uint8_t*)LCDBufferLine1, 16);
	LCD_Set_Cursor(1, 2);
	LCD_Write((uint8_t*)LCDBufferLine2, 16);
	if (BacklightColor == GREEN) LCD_SetRGB(0, 29, 0);
	else if (BacklightColor == RED) LCD_SetRGB(29, 0, 0);
	else if (BacklightColor == BLUE) LCD_SetRGB(0, 0, 29);
	else if (BacklightColor == WHITE) LCD_SetRGB(29, 29, 29);
}
/* USER CODE END Application */
