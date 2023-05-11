/*
 FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
 All rights reserved

 VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

 This file is part of the FreeRTOS distribution.

 FreeRTOS is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License (version 2) as published by the
 Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

 ***************************************************************************
 >>!   NOTE: The modification to the GPL is included to allow you to     !<<
 >>!   distribute a combined work that includes FreeRTOS without being   !<<
 >>!   obliged to provide the source code for proprietary components     !<<
 >>!   outside of the FreeRTOS kernel.                                   !<<
 ***************************************************************************

 FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  Full license text is available on the following
 link: http://www.freertos.org/a00114.html

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS provides completely free yet professionally developed,    *
 *    robust, strictly quality controlled, supported, and cross          *
 *    platform software that is more than just the market leader, it     *
 *    is the industry's de facto standard.                               *
 *                                                                       *
 *    Help yourself get started quickly while simultaneously helping     *
 *    to support the FreeRTOS project by purchasing a FreeRTOS           *
 *    tutorial book, reference manual, or both:                          *
 *    http://www.FreeRTOS.org/Documentation                              *
 *                                                                       *
 ***************************************************************************

 http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
 the FAQ page "My application does not run, what could be wwrong?".  Have you
 defined configASSERT()?

 http://www.FreeRTOS.org/support - In return for receiving this top quality
 embedded software for free we request you assist our global community by
 participating in the support forum.

 http://www.FreeRTOS.org/training - Investing in training allows your team to
 be as productive as possible as early as possible.  Now you can receive
 FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
 Ltd, and the world's leading authority on the world's leading RTOS.

 http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
 including FreeRTOS+Trace - an indispensable productivity tool, a DOS
 compatible FAT file system, and our tiny thread aware UDP/IP stack.

 http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
 Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

 http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
 Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
 licenses offer ticketed support, indemnification and commercial middleware.

 http://www.SafeRTOS.com - High Integrity Systems also provide a safety
 engineered and independently SIL3 certified version for use in safety and
 mission critical applications that require provable dependability.

 1 tab == 4 spaces!
 */

/*
 FreeRTOS is a market leading RTOS from Real Time Engineers Ltd. that supports
 31 architectures and receives 77500 downloads a year. It is professionally
 developed, strictly quality controlled, robust, supported, and free to use in
 commercial products without any requirement to expose your proprietary source
 code.

 This simple FreeRTOS demo does not make use of any IO ports, so will execute on
 any Cortex-M3 of Cortex-M4 hardware.  Look for TODO markers in the code for
 locations that may require tailoring to, for example, include a manufacturer
 specific header file.

 This is a starter project, so only a subset of the RTOS features are
 demonstrated.  Ample source comments are provided, along with web links to
 relevant pages on the http://www.FreeRTOS.org site.

 Here is a description of the project's functionality:

 The main() Function:
 main() creates the tasks and software timers described in this section, before
 starting the scheduler.

 The Queue Send Task:
 The queue send task is implemented by the prvQueueSendTask() function.
 The task uses the FreeRTOS vTaskDelayUntil() and xQueueSend() API functions to
 periodically send the number 100 on a queue.  The period is set to 200ms.  See
 the comments in the function for more details.
 http://www.freertos.org/vtaskdelayuntil.html
 http://www.freertos.org/a00117.html

 The Queue Receive Task:
 The queue receive task is implemented by the prvQueueReceiveTask() function.
 The task uses the FreeRTOS xQueueReceive() API function to receive values from
 a queue.  The values received are those sent by the queue send task.  The queue
 receive task increments the ulCountOfItemsReceivedOnQueue variable each time it
 receives the value 100.  Therefore, as values are sent to the queue every 200ms,
 the value of ulCountOfItemsReceivedOnQueue will increase by 5 every second.
 http://www.freertos.org/a00118.html

 An example software timer:
 A software timer is created with an auto reloading period of 1000ms.  The
 timer's callback function increments the ulCountOfTimerCallbackExecutions
 variable each time it is called.  Therefore the value of
 ulCountOfTimerCallbackExecutions will count seconds.
 http://www.freertos.org/RTOS-software-timer.html

 The FreeRTOS RTOS tick hook (or callback) function:
 The tick hook function executes in the context of the FreeRTOS tick interrupt.
 The function 'gives' a semaphore every 500th time it executes.  The semaphore
 is used to synchronise with the event semaphore task, which is described next.

 The event semaphore task:
 The event semaphore task uses the FreeRTOS xSemaphoreTake() API function to
 wait for the semaphore that is given by the RTOS tick hook function.  The task
 increments the ulCountOfReceivedSemaphores variable each time the semaphore is
 received.  As the semaphore is given every 500ms (assuming a tick frequency of
 1KHz), the value of ulCountOfReceivedSemaphores will increase by 2 each second.

 The idle hook (or callback) function:
 The idle hook function queries the amount of free FreeRTOS heap space available.
 See vApplicationIdleHook().

 The malloc failed and stack overflow hook (or callback) functions:
 These two hook functions are provided as examples, but do not contain any
 functionality.
 */

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include "stm32f4_discovery.h"
/* Kernel includes. */
#include "stm32f4xx.h"
#include "../FreeRTOS_Source/include/FreeRTOS.h"
#include "../FreeRTOS_Source/include/queue.h"
#include "../FreeRTOS_Source/include/semphr.h"
#include "../FreeRTOS_Source/include/task.h"
#include "../FreeRTOS_Source/include/timers.h"

#include "STMRTOSConfig.h"

//Timer declarations
void vGreenLightTimerCallback( xTimerHandle xTimer);
void vYellowLightTimerCallback( xTimerHandle xTimer);
void vRedLightTimerCallback( xTimerHandle xTimer);


#define mainQUEUE_LENGTH 100
/*-----------------------------------------------------------*/

//#define PORT                     GPIOC
//#define ADC_PIN                  GPIO_Pin_3
//
//
//#define SHIFT_REG_1_RESET_PIN    GPIO_Pin_8
//#define SHIFT_REG_1_DATA_PIN     GPIO_Pin_6
//#define SHIFT_REG_CLK_1_PIN      GPIO_Pin_7
//
//
//#define TRAFFIC_LIGHT_RED_PIN    GPIO_Pin_0
//#define TRAFFIC_LIGHT_YELLOW_PIN GPIO_Pin_1
//#define TRAFFIC_LIGHT_GREEN_PIN  GPIO_Pin_2
xQueueHandle xQueue_handle = 0;

int k = 0;

void inithardware() {

	/* Ensure all priority bits are assigned as preemption priority bits.
	 http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping(0);

	// 1. Init GPIO
	GPIO_InitTypeDef SHIFT_1_GPIO_InitStructure;
	GPIO_InitTypeDef TRAFFIC_GPIO_InitStructure;

	// Enable all GPIO clocks for GPIO, reduce potential of missing one in future updates.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	SHIFT_1_GPIO_InitStructure.GPIO_Pin = SHIFT_REG_1_RESET_PIN
			| SHIFT_REG_CLK_1_PIN | SHIFT_REG_1_DATA_PIN; // Shift register 1 output and clock set on same unique GPIO port.
	SHIFT_1_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // Set output mode
	SHIFT_1_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Set push-pull mode
	SHIFT_1_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Disable pull-ups / pull-downs
	SHIFT_1_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; // Set higher speed to allow quick shifting refresh for shift register (Max for shift register itself is 25Mhz)
	GPIO_Init(PORT, &SHIFT_1_GPIO_InitStructure);

	TRAFFIC_GPIO_InitStructure.GPIO_Pin = TRAFFIC_LIGHT_RED_PIN
			| TRAFFIC_LIGHT_YELLOW_PIN | TRAFFIC_LIGHT_GREEN_PIN; // Traffic light GPIO same unique GPIO port.
	TRAFFIC_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     // Set output mode
	TRAFFIC_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Set push-pull mode
	TRAFFIC_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Disable pull-ups / pull-downs
	GPIO_Init(PORT, &TRAFFIC_GPIO_InitStructure);

	// 2. Init ADC
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef ADC_GPIO_InitStructure;

	// Enable GPIO and ADC clocks for ADC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// Configure ADC1 Channel11 pin as analog input
	ADC_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // Using PC3, channel 11 of ADC
	ADC_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;          // Set analog mode
	ADC_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   // Set push-pull mode
	GPIO_Init(PORT, &ADC_GPIO_InitStructure);

	// ADC1 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // Set ADC for 12 bit resolution (highest)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Enable continuous scanning for ADC
	ADC_InitStructure.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1; // Perform a single conversion when start conversion is called
	ADC_Init(PORT, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);                                        // Enable ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
	srand(time(NULL));
}

void TrafficFlowTask(void);
void TrafficGeneratorTask(void);
void TrafficLightTask(void);
/*-----------------------------------------------------------*/


int main(void) {
	/* Ensure all priority bits are assigned as preemption priority bits.
	 http://www.freertos.org/RTOS-Cortex-M3-M4.html */

     NVIC_SetPriorityGrouping(0);

	/* Create the queue used by the queue send and queue receive tasks.
	 http://www.freertos.org/a00116.html */
	xQueue_handle = xQueueCreate(mainQUEUE_LENGTH, /* The number of items the queue can hold. */
	sizeof( uint16_t )); /* The size of each item the queue holds. */

	/* Add to the registry, for the benefit of kernel aware debugging. */
	vQueueAddToRegistry(xQueue_handle, "MainQueue");

	/* Configure the system ready to run the demo.  The clock configuration
	 can be done here if it was not done before main() was called. */

	inithardware();
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_RED_PIN);
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_YELLOW_PIN);
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_GREEN_PIN);

	xMutexFlow = xSemaphoreCreateMutex();
	if (xMutexFlow == NULL) {
		printf("ERROR: FLOW SEMAPHORE NOT CREATED. \n"); /* There was insufficient FreeRTOS heap available for the semaphore to be created successfully. */
	} else {
		xSemaphoreGive(xMutexFlow); // need to give semaphore after it is defined
	}

	xMutexLight = xSemaphoreCreateMutex();
	if (xMutexLight == NULL) {
		printf("ERROR: FLOW SEMAPHORE NOT CREATED. \n"); /* There was insufficient FreeRTOS heap available for the semaphore to be created successfully. */
	} else {
		xSemaphoreGive(xMutexLight); // need to give semaphore after it is defined
	}

	xRedLightTimer = xTimerCreate("red_light", 5000/portTICK_PERIOD_MS, pdFALSE, 0,vRedLightTimerCallback);
	xYellowLightTimer = xTimerCreate("yellow_light", 2000/portTICK_PERIOD_MS, pdFALSE, 0, vYellowLightTimerCallback);
	xGreenLightTimer = xTimerCreate("green_light", 10000/portTICK_PERIOD_MS , pdFALSE,	0, vGreenLightTimerCallback);

	xTaskCreate( TrafficFlowTask , "FlowTask", configMINIMAL_STACK_SIZE ,NULL ,2,   NULL);
	xTaskCreate( TrafficGeneratorTask , "GeneratorTask", configMINIMAL_STACK_SIZE ,NULL ,1,   NULL);
	xTaskCreate(TrafficLightTask, "Light Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	GPIO_SetBits(GPIOC, SHIFT_REG_1_RESET_PIN);
	GPIO_ResetBits(GPIOC, SHIFT_REG_1_RESET_PIN);
	GPIO_SetBits(GPIOC, SHIFT_REG_1_RESET_PIN);

	GPIO_SetBits(GPIOC,TRAFFIC_LIGHT_GREEN_PIN);
	global_light_color = 1;
	xTimerStart(xGreenLightTimer, 0);
	vTaskStartScheduler();
	return 0;

}

/*pushes bits to the board.*/

void sendTrafficToBoard(int u) {

	GPIO_SetBits(GPIOC, SHIFT_REG_1_RESET_PIN);			// set reset register.

	for (int i = 0; i < 100; i++) {						// add small delay.
	};
	if (u == 0) {										// 0 pushes a low (no car).
		GPIO_ResetBits(PORT, SHIFT_REG_1_DATA_PIN);
	} else {											// Otherwise we have a 1, so car exists.
		GPIO_SetBits(PORT, SHIFT_REG_1_DATA_PIN);
	}
	for (int i = 0; i < 100; i++) {
	};
	GPIO_SetBits(PORT, SHIFT_REG_CLK_1_PIN);        // set clock register high
	for (int i = 0; i < 100; i++) {
	};
	GPIO_ResetBits(PORT, SHIFT_REG_CLK_1_PIN);      // set clock register low
	for (int i = 0; i < 100; i++) {
	};
	GPIO_SetBits(PORT, SHIFT_REG_CLK_1_PIN);
}

/*pushes array bit by bit*/
void pushTraffic(uint16_t traffic[]) {
	for (int i = 0; i < 19; i++) {
		sendTrafficToBoard(traffic[18 - i]);
	}
	printf("\n");
}

/*
 * Determines car exists or not based on flow value from flow task. Pushes the array on board.
 *
 * */
void TrafficGeneratorTask() {
	uint16_t flow_rate = 0;
	uint16_t car_value = 0;

	uint16_t light_color ;
	uint16_t lighttraffic[19] = { 0 };
	uint16_t newtraffic[19] = { 0 };
	int counter =0;
	int first_time = 1;
	while (1) {
		if (xQueueReceive(xQueue_handle, &flow_rate, 1000)) {	// updates value of flow_rate from queue.
			vTaskDelay(500);
		}

		if ( xSemaphoreTake( xMutexLight, ( TickType_t ) 0 ) == pdPASS) //use semaphore to get the value of light.
			{
			    light_color = global_light_color ;	// update local variable.
				xSemaphoreGive(xMutexLight); // Updated global light value, release light mutex.
			}


		car_value = (rand() % 100) < 10*(flow_rate);  // even-> no car, odd-> car.

		if(flow_rate==1){
			if(counter<6){
				car_value=0;
			}
			else{
				car_value =1;
				for(int i =0; i<5; i++){
					if(lighttraffic[i]==1){
						car_value =0;
						break;
					}
				}

				counter =0;
			}
		}

		counter +=1;

		if(flow_rate==8){
			car_value =1;
		}

		if(first_time==1 && flow_rate==1){
			car_value=1;
			first_time = 0;
		}

		printf("\ncar value(0/1): %u flowrate: %u\n", car_value, (flow_rate));
		if (light_color == 1) {
			//light is green
			k = 0;
			// shift all values to right.

			for (int i = 18; i > 0; i--) {
				lighttraffic[i] = lighttraffic[i - 1];
			}
			lighttraffic[0] = car_value;

			pushTraffic(lighttraffic);

		}

		if (light_color == 0) {
			// light is red.
			uint16_t zero =0;

			for(int i=7; i>0; i--){

				if(lighttraffic[i]==0){
					zero = 1;			// there is a 0 in the array
					newtraffic[0]=car_value;
					}

				if(zero==1){
					newtraffic[i]= lighttraffic[i-1];		// push previous bit to new array.
				}

				else{
					newtraffic[i] =lighttraffic[i];			// else push current bit to array.
				}
			}

			for(int i=0;i<8;i++){
				lighttraffic[i] = newtraffic[i];			// push new array to final array. (first 8 places).
			}

			//handle traffic after 8 places (after stop line).
			for (int i = 18; i > 7; i--) {
				lighttraffic[i] = lighttraffic[i - 1];
			}

			// no cars after stop line on red light.
			lighttraffic[8] = 0;

			pushTraffic(lighttraffic);

		}

	}

}


// Callback for the xGreenLightTimer
void vGreenLightTimerCallback( xTimerHandle xTimer) // green light time is up, change the light to yellow (red)
{
//	printf("GreenLightTimerCallback: Green light off, yellow light on. \n");
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_GREEN_PIN);       // turn off green light
	GPIO_SetBits(GPIOC, TRAFFIC_LIGHT_YELLOW_PIN);        // turn on yellow light

	if ( xSemaphoreTake( xMutexLight, ( TickType_t ) 0 ) == pdPASS) // can't wait for semaphore in callback, so ticks to wait is 0
	{
		global_light_color = 0;	// Update global light value to red as green is done (1 = green, 0 = red); yellow light is treated as red.
		xSemaphoreGive(xMutexLight); // Updated global light value, release light mutex
	   // printf("GreenLightTimerCallback: Updated light color to red. \n");
	} else {
		printf("GreenLightTimerCallback: xMutexLight unavailable \n");
	}

	xTimerStart(xYellowLightTimer, 0); // Green light finished, light color changed, light state changed, start yellow light timer.
}

// Callback for the xYellowLighTimer
void vYellowLightTimerCallback( xTimerHandle xTimer) // yellow light time is up, change the light to red. Note: Yellow light is like red.
{
	//printf("YellowLightTimerCallback: Yellow light off, red light on. \n");
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_YELLOW_PIN);     // turn off yellow light
	GPIO_SetBits(GPIOC, TRAFFIC_LIGHT_RED_PIN);            // turn on red light

	xTimerStart(xRedLightTimer, 0); // Yellow light finished, light colour changed, start red light timer
}

// Callback for the xRedLightTimer
void vRedLightTimerCallback( xTimerHandle xTimer) {
	//printf("RedLightTimerCallback: Red light off, green light on. \n");
	GPIO_ResetBits(GPIOC, TRAFFIC_LIGHT_RED_PIN);          // turn off red light
	GPIO_SetBits(GPIOC, TRAFFIC_LIGHT_GREEN_PIN);          // turn on green light

	if ( xSemaphoreTake( xMutexLight, ( TickType_t ) 0 ) == pdPASS) // can't wait for semaphore in callback, so ticks to wait is 0
	{
		global_light_color = 1;	// Update global light value to green as red is done. (1 = green, 0 = red)
		xSemaphoreGive(xMutexLight); // Updated global light value, release light mutex
		//printf("RedLightTimerCallback: Updated light color to green. \n");
	} else {
		printf("RedLightTimerCallback: xMutexLight unavailable \n");
	}

	xTimerStart(xGreenLightTimer, 0); // Red light finished, light color changed, light state changed, start green light timer
}

/*Controls the behavior of traffic lights using software timers and callbacks.*/

void TrafficLightTask() {

	uint16_t speed_value = 0;
	uint16_t new_speed_value = 0;
    uint16_t green_value =0;
    uint16_t red_value =0;

	while (1) {
		// update speed by using flow rate mutex.
		if ( xSemaphoreTake( xMutexFlow, ( TickType_t ) 10 ) == pdPASS) {
			new_speed_value = global_flowrate;
			xSemaphoreGive(xMutexFlow);
		}

		//update speed value if changed.
		if(new_speed_value!=speed_value){
			printf("speed value changed from: %u to: %u\n", speed_value, new_speed_value);

			// Ticks/portTICK_PERIOD_MS -> time in ms.

    	if (xTimerIsTimerActive(xGreenLightTimer)) {
    		green_value = (1000 + 1000*new_speed_value);
    		red_value = 4000 + (green_value-2000)*0.125;
			xTimerStop(xGreenLightTimer, 0);  // stop the green timer to change the period
			xTimerChangePeriod(xGreenLightTimer,  green_value / portTICK_PERIOD_MS, 0 );  // starts the green timer
			xTimerChangePeriod(xRedLightTimer, red_value / portTICK_PERIOD_MS, 0 );     // starts the red timer    (don't want it started)
			xTimerStop(xRedLightTimer, 0);
			printf("\nGreen Period: %u Red Period: %u\n", green_value  / portTICK_PERIOD_MS, red_value / portTICK_PERIOD_MS );
	}

		else if(xTimerIsTimerActive( xYellowLightTimer )){
			green_value = (1000 + 1000*new_speed_value);
			red_value = 4000 + (green_value-2000)*0.125;
			xTimerChangePeriod(xGreenLightTimer, green_value  / portTICK_PERIOD_MS, 0 );  // starts the green timer (don't want it started)
			xTimerStop(xGreenLightTimer, 0);                                                                      // stop the green timer
			xTimerChangePeriod(xRedLightTimer, red_value / portTICK_PERIOD_MS, 0 );     // starts the red timer    (don't want it started)
			xTimerStop(xRedLightTimer, 0);
			printf("\nGreen Period: %u Red Period: %u\n", green_value  / portTICK_PERIOD_MS, red_value / portTICK_PERIOD_MS );
		}

	   else if(xTimerIsTimerActive( xRedLightTimer ))	{
			green_value = (1000 + 1000*new_speed_value);
			red_value = 4000 + (green_value-2000)*0.125;
			xTimerStop(xRedLightTimer, 0);																		  // stop the red timer to change the period
			xTimerChangePeriod(xGreenLightTimer, green_value  / portTICK_PERIOD_MS, 0 );  // starts the green timer (don't want it started)
			xTimerStop(xGreenLightTimer, 0);																	  // stop the green timer since red is on
			xTimerChangePeriod(xRedLightTimer, red_value / portTICK_PERIOD_MS, 0 );     // starts the red timer again
			printf("\nGreen Period: %u Red Period: %u\n", green_value  / portTICK_PERIOD_MS, red_value / portTICK_PERIOD_MS );
		}
		}
		speed_value=new_speed_value;

	}
}

/*
 *
 * Reads value from potentiometer and update in queue and flowrate semaphore.
 *
 */
void TrafficFlowTask() {

	uint16_t adc_value = 0;       // Start with adc_value as 0, until it is read

	while (1) {
		ADC_SoftwareStartConv(ADC1);		    // Start conversion of ADC value
		while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	// Wait for ADC to finish conversion
		adc_value = ADC_GetConversionValue(ADC1);       // Grab ADC value
		adc_value = adc_value/512;  // we have 8 divisions.

		adc_value += 1; // ensure value is between 1 and 8.

		if (xQueueSend(xQueue_handle, &adc_value, 1000)) {
			vTaskDelay(1000);
		}
		// calculate how flowrate is changing with adc_value.
		if ( xSemaphoreTake( xMutexFlow, ( TickType_t ) 10 ) == pdPASS) {
			global_flowrate = adc_value;
			printf("global_flowrate: %u", global_flowrate);
			xSemaphoreGive(xMutexFlow);
		}

	}

	return;
}

void vApplicationMallocFailedHook(void) {
	/* The malloc failed hook is enabled by setting
	 configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	 Called if a call to pvPortMalloc() fails because there is insufficient
	 free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	 internally by FreeRTOS API functions that create tasks, queues, software
	 timers, and semaphores.  The size of the FreeRTOS heap is set by the
	 configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected.  pxCurrentTCB can be
	 inspected in the debugger if the task name passed into this function is
	 corrupt. */
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void) {
	volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	 FreeRTOSConfig.h.

	 This function is called on each cycle of the idle task.  In this case it
	 does nothing useful, other than report the amount of FreeRTOS heap that
	 remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if (xFreeStackSpace > 100) {
		/* By now, the kernel has allocated everything it is going to, so
		 if there is a lot of heap remaining unallocated then
		 the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		 reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/
