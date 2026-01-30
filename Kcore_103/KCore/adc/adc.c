#include "main.h"
#include "adc.h"
#include "ADC_Tables.h"
#include "stm32f10x_dma.h"



AdcTableStruct const HeadPositionTable2[] __attribute__((aligned(4))) =
{
	// from the Hot Head Resistor Value and 12-bit ADC Value spec
		// 27 entries, 4 bytes each, 108 total bytes
		// (uint16_t)adcVal, (uint16)devPos   (MUST be in order with increasing adcVals)
		{ 108, 91 },
	// max adc value for hotbed1 position
		{ 294, 26 },
	// max adc value for yoke 2 aux2
		{ 452, 16 },
	// max adc value for yoke 1 aux2
		{ 635, 21 },
	// max adc value for yoke 2 hot head 1
		{ 804, 11 },
	// max adc value for yoke 1 hot head 1
		{ 943, 22 },
	// max adc value for yoke 2 hot head 2
		{ 1082, 12 },
	// max adc value for yoke 1 hot head 2
		{ 1228, 23 },
	// max adc value for yoke 2 hot head 3
		{ 1389, 13 },
	// max adc value for yoke 1 hot head 3
		{ 1563, 24 },
	// max adc value for yoke 2 hot head 4
		{ 1751, 14 },
	// max adc value for yoke 1 hot head 4
		{ 1946, 25 },
	// max adc value for yoke 2 aux1
		{ 2140, 15 },
	// max adc value for yoke 1 aux1
		{ 2345, 92 },
	// max adc value for hotbed2 position
		{ 2544, 46 },
	// max adc value for yoke 4 aux2
		{ 2723, 36 },
	// max adc value for yoke 3 aux2
		{ 2901, 41 },
	// max adc value for yoke 4 hot head 1
		{ 3065, 31 },
	// max adc value for yoke 3 hot head 1
		{ 3200, 42 },
	// max  value for yoke 4 hot head 2
		{ 3317, 32 },
	// max adc value for yoke 3 hot head 2
		{ 3439, 43 },
	// max adc value for yoke 4 hot head 3
		{ 3557, 33 },
	// max adc value for yoke 3 hot head 3
		{ 3667, 44 },
	// max adc value for yoke 4 hot head 4
		{ 3780, 34 },
	// max adc value for yoke 3 hot head 4
		{ 3906, 45 },
	// max adc value for yoke 4 aux1
		{ 4034, 35 },
	// max adc value for yoke 3 aux1
		{ MAX_ADC12, HH_POSITION_UNPLUGGED },
	// max adc value an unplugged hothead
};


float ScaledADCData[ADC_CHANNEL_NUM] = { 0 }; //converted adc buffer values
uint16_t ADC_Work_Channel_Index = 0; //used to walk thru the channels and update the working variables.
adcStruct ADC_Channel[ADC_CHANNEL_NUM];
adcStruct *ADC_Work_Channel;

uint16_t ADC_DMA_Buffer[6] __attribute__((aligned(4))); //6 active channels
uint16_t RawADCDataBuffer[ADC_CHANNEL_NUM] = { 0 };
float laserTemperature = 0;
float UvataVoltage = 0;
float CurrentSetPoint = 0;
float adcConversionFactor = 0.0008f;
float UvataVoltageConversionFactor = 0.00437f;
uint16_t UvataDuty = 0;
uint16_t CanHeadAddress = 0;
uint32_t tmpreg = 0;
uint16_t sampleIndex = 0; //points to the next sample postion of the adc olympic smoothing buffer

uint32_t sum = 0; 
uint16_t raw = 0; 
uint16_t low = 0; 
uint16_t high = 0; 
uint16_t i = 0; //persistant variables
float adc_conversionFactor = 3.3 / 4096;



/*the following method will take the data that is already waiting in the  dma buffer   ADC_DMA_Buffer[]
 *and plug it into current channel raw reading and also the current channel 10x buffer for data smoothing later*/
void SmoothDataUsingOlympicVotingAverage(void)
{
	float stuntfloat = 0;
	ADC_Work_Channel = &ADC_Channel[ADC_Work_Channel_Index];
	ADC_Work_Channel->adcRaw = ADC_DMA_Buffer[ADC_Work_Channel_Index]; //update last reading
	//now we need to plug into the 10 reading buffer for smoothing.
	ADC_Work_Channel->sampleHistory[sampleIndex] = ADC_Work_Channel->adcRaw; //plug in the latest reading.
	sampleIndex++;//point to the next sample location
	if (sampleIndex < ADC_NUM_SAMPLES) return;//done, need 10 samples before we can work our magic
			// history buffer is full, so enough good adc values to proceed

			// this code does olympic voting (toss high and low and then average the rest)
			// the ADC_NUM_SAMPLES must be equal to (2^n) + 2 and ADC_SHIFT_FOR_AVG
			// must equal "n", as the code will shift to get the average instead of divide. set temporary
			// variables to record highest and lowest values as each of the ADC_NUM_SAMPLES is inspected
			// at the same time, record the sum of all ADC_NUM_SAMPLES samples.  when done looking at all values,
			// subtract the high and low from the sum and then calculate the average of the remaining sum.
			int32_t sum, raw, low, high, i;
			low = 0x7fffffff; // MAXINT
			high = 0x80000000; // MININT
			sum = 0;
			for (i = 0; i < ADC_NUM_SAMPLES; i++)
			{
				raw = ADC_Work_Channel->sampleHistory[i];
				sum += raw; // keep running total
				if (raw < low) low = raw; // update the lowest reading
				if (raw > high)high = raw; // update the highest reading
			}
			sum -= (low + high); // sum is now the total of the middle N values

			//next we will shift by n to effect a divide by 2^n to get the average of the 2^n remaining samples
			ADC_Work_Channel->adcAvg = (sum >> ADC_SHIFT_FOR_AVG); // update the RAW average
			//ADC_Work_Channel->convAvg = ScaledADCData[ADC_Work_Channel_Index] = (float)(((float)ADC_Work_Channel->adcAvg * 3.3) / 4095);
			stuntfloat = ((float)ADC_Work_Channel->adcAvg) * adcConversionFactor;
			ADC_Work_Channel->convVolt = stuntfloat;
			ADC_Work_Channel->convAvg = convertRtdDataFromRawADCValue(AdcChannelTable[ADC_Work_Channel_Index].ConvertionTable, ADC_Work_Channel->adcAvg);
		// setup next conversion so data will be ready for the next call in ~10ms
	switch (ADC_Work_Channel_Index)
	{
		
	case 0: UvataVoltage		= ADC_Work_Channel->adcAvg * UvataVoltageConversionFactor; break;// PA0  RTD2
	case 1:	laserTemperature	= ADC_Work_Channel->convAvg; break;				// PA1  RTD1
	case 2: CurrentSetPoint		= ADC_Work_Channel->convVolt * 2; break;		// PA3  ISET
	case 3: break;	// PA4  RTD3
	case 4: CanHeadAddress		= convertRawAdcToCanHeadAddress(ADC_Work_Channel->adcAvg); break;// PB1  POSition
	case 5: break; // Internal Temp	
		break;

	}
	//finished processing this channel so index to the next one
	sampleIndex = 0;
	ADC_Work_Channel_Index++;
	if (ADC_Work_Channel_Index >= ADC_CHANNEL_NUM) ADC_Work_Channel_Index = 0; //keep in range
}


uint16_t convertRawAdcToCanHeadAddress(uint16_t rawValue)
{
	uint16_t headaddressToReturn = 0xff;
	for (int count = 0; count < 32; count++)
	{
		if (HeadPositionTable2[count].adcRaw > rawValue)
		{
			headaddressToReturn = (uint16_t) HeadPositionTable2[count].value;
			break;
		}
	}
	return headaddressToReturn;
}
//dma adc methods
/**
 * @brief  Initializes ADC1 with DMA in circular mode for 7 channels
 * @note   Uses ADC1, DMA1 Channel 1, scan mode, continuous conversion
 */
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)
void Init_ADC_DMA_Circular_Mode(void)
{
	// 1. Enable clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72 MHz / 6 = 12 MHz (max 14 MHz)

	// 2. GPIO configuration – analog inputs (MODE = 00, CNF = 00)
	GPIO_InitTypeDef GPIO_InitStructure;
    
	// PA0, PA1, PA3, PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PB1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 3. ADC configuration
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // very important!
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6; // ← number of conversions
	ADC_Init(ADC1, &ADC_InitStructure);

	// 4. ADC calibration
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1)) ;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)) ;

	// 5. Channel sequence – very important order!
	// Regular channel sequence (rank 1..7)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); // PA0  RTD2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); // PA1  RTD1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5); // PA3  ISET
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5); // PA4  RTD3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 5, ADC_SampleTime_55Cycles5); // PB1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 6, ADC_SampleTime_239Cycles5); // Internal Temp

	// Most common choice for internal temperature sensor
	//ADC_TempSensorCmd(ENABLE);

	// 6. DMA configuration – Circular, peripheral-to-memory, 16-bit
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ADC_DMA_Buffer;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize         = 6; // 6 words
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular; // ← circular!
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    
	//DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Get the DMAy_Channelx CCR value */
	tmpreg = DMA1_Channel1->CCR;
	/* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
	tmpreg &= CCR_CLEAR_Mask;
	/* Configure DMAy Channelx: data transfer, data size, priority level and mode */
	/* Set DIR bit according to DMA_DIR value */
	/* Set CIRC bit according to DMA_Mode value */
	/* Set PINC bit according to DMA_PeripheralInc value */
	/* Set MINC bit according to DMA_MemoryInc value */
	/* Set PSIZE bits according to DMA_PeripheralDataSize value */
	/* Set MSIZE bits according to DMA_MemoryDataSize value */
	/* Set PL bits according to DMA_Priority value */
	/* Set the MEM2MEM bit according to DMA_M2M value */
	tmpreg |= DMA_InitStructure.DMA_DIR | DMA_InitStructure.DMA_Mode |
	          DMA_InitStructure.DMA_PeripheralInc | DMA_InitStructure.DMA_MemoryInc |
	          DMA_InitStructure.DMA_PeripheralDataSize | DMA_InitStructure.DMA_MemoryDataSize |
	          DMA_InitStructure.DMA_Priority | DMA_InitStructure.DMA_M2M;

	/* Write to DMAy Channelx CCR */
	DMA1_Channel1->CCR = tmpreg;

	/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
	  /* Write to DMAy Channelx CNDTR */
	DMA1_Channel1->CNDTR = DMA_InitStructure.DMA_BufferSize;

	/*--------------------------- DMAy Channelx CPAR Configuration ----------------*/
	  /* Write to DMAy Channelx CPAR */
	DMA1_Channel1->CPAR = DMA_InitStructure.DMA_PeripheralBaseAddr;

	/*--------------------------- DMAy Channelx CMAR Configuration ----------------*/
	  /* Write to DMAy Channelx CMAR */
	DMA1_Channel1->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr;

	// Enable DMA transfer complete / half-transfer interrupts (optional)
	// DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);
	// NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}


/**
 * @brief  Starts continuous ADC conversions with DMA
 */
void Start_ADC_IN_CONTNOUS_DMA_MODE(void)
{
	// Enable DMA channel
	DMA_Cmd(DMA1_Channel1, ENABLE);

	// Enable ADC DMA requests
	ADC_DMACmd(ADC1, ENABLE);

	// Start conversion (continuous mode is already enabled)
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
float Get_Temperature_C(void)
{
	uint16_t raw = ADC_DMA_Buffer[5]; // your temp sensor slot

	float Vout = raw * 3.3f / 4096.0f;
	float temp = ((Vout - 1.41f) / 0.0043f) + 25.0f;

	return temp;
}
//must add stm32f10xDMA.h  and .c   from latest library download in downloads.



