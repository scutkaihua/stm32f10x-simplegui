#include "stm32f10x_lib.h"
#include "types.h"

//=======================================================================//
//= 系统时钟 配置               										    =//
//=======================================================================//
U8 ADC_RCC_DIV = 2;
void RCC_Config(void)
{
	//初始化系统时钟
	RCC_DeInit();
	//启用外部高速晶振
	RCC_HSEConfig(RCC_HSE_ON);

	if (RCC_WaitForHSEStartUp() == SUCCESS) {
		/* HCLK = 72M Max */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		/* PCLK2 72M Max */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		/* PCLK1 36M Max */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/* ADCCLK = PCLK2/6 */
		switch(ADC_RCC_DIV)
		{
			case 2:
				RCC_ADCCLKConfig(RCC_PCLK2_Div2);
				break;
			case 4:
				RCC_ADCCLKConfig(RCC_PCLK2_Div4);
				break;			
			case 8:
				RCC_ADCCLKConfig(RCC_PCLK2_Div8);
				break;
			default://6
				RCC_ADCCLKConfig(RCC_PCLK2_Div6);
				break;

		}
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		/* Flash wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		/* PLLCLK */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08);
	}	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

//=======================================================================//
//= FSMC 配置               										    =//
//=======================================================================//
void GUI_Configuration(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef xEPITiming;
	u32 nBank;

	//-- FSMC Clock enable ----------------------------------------------------
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	//-- FSMC 复用IO初始化 ----------------------------------------------------
	{
	 	GPIO_InitTypeDef GPIO_InitStructure;
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);
		/*PD 0,1,4,5,14,15*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		/*PE 7,8,9,10*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		/*PF 0*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  ;
		GPIO_Init(GPIOF, &GPIO_InitStructure);
				
		/*!< NOE and NWE configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		/*!< NE4 configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOG, &GPIO_InitStructure);
		
	}


  	//-- FSMC Configuration ----------------------------------------------------
	xEPITiming.FSMC_AddressSetupTime = 2; 
	xEPITiming.FSMC_AddressHoldTime = 0;
	xEPITiming.FSMC_DataSetupTime = 5;		
	xEPITiming.FSMC_BusTurnAroundDuration = 0;
	xEPITiming.FSMC_CLKDivision = 0;
	xEPITiming.FSMC_DataLatency = 0;
 	xEPITiming.FSMC_AccessMode = FSMC_AccessMode_A;

	nBank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_Bank = nBank;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
//	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &xEPITiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &xEPITiming;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(nBank, ENABLE);
}

