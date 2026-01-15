#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>


#define CAN_PRESCALE_VALUE 12  // 48=125Kbps; 12=500Kbps; 6=1Mbit/sec  [APB1_CLK=36MHz]
#define GUI_THEME_DEFAULT						0
#define GUI_THEME_BLACK_AND_BLUE_ON_WHITE		1
#define GUI_THEME_WHITE_AND_YELLOW_ON_BLACK		2


#define CAN_NUM_FILTERS                         14
#define NUM_PRE_DEFINED_ALIASES                 4
#define NUM_USER_DEFINED_ALIASES                8   // note, if this increases past 8, change  canbus.c are needed to
// support more than 1 pg
// also filters can at most support 9
#define ALIAS_PHYSICAL_FILTER_INDEX             0
#define ALIAS_BROADCAST_ALL_FILTER_INDEX        1
#define ALIAS_BROADCAST_TYPE_FILTER_INDEX       2
#define ALIAS_BROADCAST_YOKE_FILTER_INDEX       3
#define ALIAS_USER_DEFINED_START_FILTER_INDEX   4
#define ALIAS_IMMEDIATE_FILTER_INDEX            13

#define ALIAS_ALL_DEVICES                   0
#define ALIAS_ALL_YOKE1_DEVICES             10
#define ALIAS_ALL_YOKE2_DEVICES             20
#define ALIAS_ALL_YOKE3_DEVICES             30
#define ALIAS_ALL_YOKE4_DEVICES             40
#define ALIAS_ALL_CAN_AXIS_MOTORS			80
#define ALIAS_ALL_CAN_AXIS_MOTORS_X			81
#define ALIAS_ALL_CAN_AXIS_MOTORS_Y			82
#define ALIAS_ALL_CAN_AXIS_MOTORS_Z			83
#define ALIAS_ALL_CAN_AXIS_MOTORS_A			84
#define ALIAS_ALL_CAN_AXIS_MOTORS_B			85
#define ALIAS_ALL_CAN_AXIS_MOTORS_C			86

#define ALIAS_ALL_HOTBEDS                   90
#define ALIAS_ALL_EXTRUDERS                 100
#define ALIAS_UNUSED                        0xff

#define ALIAS_TOOL_NUMBER_MIN               50
#define ALIAS_TOOL_NUMBER_MAX               57


#define FILTER_CONSTANT_VALUE           0x80000004
#define FILTER_MASK_CONSTANT_HEAD       0xE0040004
#define FILTER_MASK_CONSTANT_SYS        0xE0000004
#define FILTER_MASK_DEVICE              0x07F80000
#define FILTER_IMMEDIATE_BIT            0x00040000
#define FILTER_DEVICE_LSB_POSITION      19


#define CAN_TX_QUEUE_SIZE 128 //192
#define CAN_RX_QUEUE_SIZE 128 //256
#define WORKING_BUFFER_SIZE_IN_BYTES             1024
#define WORKING_BUFFER_SIZE_IN_WORDS             (WORKING_BUFFER_SIZE_IN_BYTES/4)

typedef union {
	uint8_t     u8[8];
	int8_t      i8[8];
	uint16_t    u16[4];
	int16_t     i16[4];
	uint32_t    u32[2];
	int32_t     i32[2];
	uint64_t    u64;
	int64_t     i64;
	float		flt[2];
} payloadUnion;

typedef struct {
	unsigned fixed_b0   : 1; // Start-of-Frame - must be '1'
	unsigned RTR        : 1; // RTR = Remote Transmission Request (RTR)
	unsigned IDE        : 1; // IDE = Identifier extension bit (IDE) - must be '1' (only use extended msg)
	unsigned page       : 8;
	unsigned msgId      : 7;
	unsigned immediate  : 1;
	unsigned device     : 8;
	unsigned msgType    : 2;
	unsigned fixed_b100 : 3; // End-of-Frame

	unsigned numBytes   : 4;
	unsigned fromCAN2   : 1;
	unsigned pad        : 27;

	payloadUnion payload;
} canSwStruct;
typedef struct {
	uint32_t IR; /*!< CAN receive FIFO mailbox identifier register */
	uint32_t DTR; /*!< CAN receive FIFO mailbox data length control and time stamp register */
	uint32_t DLR; /*!< CAN receive FIFO mailbox data low register */
	uint32_t DHR; /*!< CAN receive FIFO mailbox data high register */
} canHwStruct;

typedef struct {
	union {
		canHwStruct hw __attribute__((aligned(8))); // get on double word boundry for directy copying of data
		canSwStruct sw;
	};
} canStruct;
typedef struct {
	canStruct   Q[CAN_TX_QUEUE_SIZE];
	uint16_t    nextIn;
	uint16_t    nextOut;
	uint16_t    numMsg;
} txQueueStruct;

typedef struct {
	canStruct   Q[CAN_RX_QUEUE_SIZE];
	uint16_t    nextIn;
	uint16_t    nextOut;
	uint16_t    numMsg;
} rxQueueStruct;






void canInit(void);
void initClkAndResetAPB1(uint32_t periph);
void canInitFilter(CAN_TypeDef *CANx, uint8_t filterNum, uint8_t alias, bool immediate, bool enable);
void canAddUserDefinedAlias(uint8_t newAlias, uint8_t aliases[]);
