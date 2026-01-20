#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <stm32f10x_can.h>
#include "taskmanager/taskmanager.h"

typedef uint8_t byte;
#define CAN_MSG_BUFFER_SIZE			0x10
#define CAN_MSG_QUE_CNT_MASK		0x0f
#define CAN_BROADCAST_ADDRESS		0xFF
#define CAN_READ                    0b00        // 0x00
#define CAN_WRITE                   0b01        // 0x01
///
////////CAN MESSAGE ID//////////////////// 
#define CAN_MSG_DEVICE_POSITION             ((byte)0x00)
#define CAN_MSG_DEVICE_INFO                 ((byte)0x01)
#define CAN_MSG_FLASH_CONFIG                ((byte)0x02)
#define CAN_MSG_UNIQUE_ID                   ((byte)0x03)
#define CAN_MSG_PRE_DEFINED_ALIASES         ((byte)0x04)
#define CAN_MSG_USER_DEFINED_ALIASES        ((byte)0x05)
#define CAN_MSG_STATUS                      ((byte)0x06)
//#define CAN_MSG_HISTORY_RANGE_DEF         ((byte)0x07)    //NUKE DEPRECATE
//#define CAN_MSG_HISTORY_RANGES                ((byte)0x08)    //NUKE DEPRECATE
//#define CAN_MSG_HISTORY_COUNTERS          ((byte)0x09)    //NUKE DEPRECATE
#define CAN_MSG_PAGE_DEF                    ((byte)0x0a)
//#define CAN_MSG_TABLE_START_OFFSETS           ((byte)0x0b)    // not used in V1 -- tables are not loadable
//#define CAN_MSG_GLOBAL_STRUCT_INFO            ((byte)0x0c)    //NUKE DEPRECATE
#define CAN_MSG_DEVICE_INIT                 ((byte)0x0d)    // V1 - multi-page initialization based on deviceType

#define CAN_MSG_CONTROL_WORD                ((byte)0x10)
#define CAN_MSG_ADD_ALIAS                   ((byte)0x11)
#define CAN_MSG_REMOVE_ALIAS                ((byte)0x12)
#define CAN_MSG_AUTO_STATUS_CONTROL         ((byte)0x13)
//#define CAN_MSG_LED_CONTROL               ((byte)0x14)    //NUKE DEPRECATE
//#define CAN_MSG_EXTREMES                  ((byte)0x15)    // V0
#define CAN_MSG_EXTRUSION_TEMP_RANGES       ((byte)0x16)    // V1
//#define CAN_MSG_ERROR_REPORT_INTERVAL     ((byte)0x17)    //NUKE DEPRECATE
//#define CAN_MSG_SWITCH_DUTY                   ((byte)0x18)    // V0 only
//#define CAN_MSG_SWITCH_PWM                    ((byte)0x19)    // V0 only
//#define CAN_MSG_SWITCH_TEMP                   ((byte)0x1a)    // V0 only
#define CAN_MSG_KARL_FACTORS                ((byte)0x1b)
#define CAN_MSG_HSS_CONTROL                 ((byte)0x1c)    // V1
//#define CAN_MSG_HSS_FLAGS                 ((byte)0x1d)    // V1
#define CAN_MSG_MOTION_CONTROLS             ((byte)0x1e)

#define CAN_MSG_MOTOR_ENABLE                ((byte)0x20)    //NUKE DEPRICATED (USED IN V4, remove for V5) LEGACY
//#define CAN_MSG_MICROSTEPS                  ((byte)0x21)    // aliased to CAN_MSG_JET_INDEX - not used in V1
//#define CAN_MSG_JET_INDEX                   ((byte)0x21)    // aliased to CAN_MSG_MICROSTEPS - not used in V1
//#define CAN_MSG_CURRENT_BOOST             ((byte)0x22)    // not used in v1
#define CAN_MSG_FLOW_SCALE_FACTORS          ((byte)0x23)    // was CAN_MSG_fudge_factor
//#define CAN_MSG_EXTRUSION_STEPS_PER_NL        ((byte)0x24)    //NUKE DEPRICATED
//#define CAN_MSG_EXTRUSION_MAIN                ((byte)0x25)    // V0 only
#define CAN_MSG_EXTRUSION_UNPRIME           ((byte)0x26)
#define CAN_MSG_EXTRUSION_PRIME             ((byte)0x27)
//#define CAN_MSG_EXTRUSION_MANUAL          ((byte)0x28)    // V0 only
#define CAN_MSG_EXTRUSION_DWELL             ((byte)0x29)    //NUKE DEPRICATED
#define CAN_MSG_PRIME_AND_RUN_PARAMS        ((byte)0x2a)
//NUKE #define CAN_MSG_VELOCITY_SCALE_FACTOR          ((byte)0x2b)    //NUKE DEPRICATED
#define CAN_MSG_TOUCH_PROBE_OLD                ((byte)0x2c) // aliased with CAN_MSG_PICKNPLACE_CONTROLTOUCH PROBE -- BOO HISS LEGACY....
#define CAN_MSG_PICKNPLACE_CONTROL          ((byte)0x2c)
#define CAN_MSG_PICKNPLACE_DATA             ((byte)0x2d)
//#define CAN_MSG_CONTROL_VIA_PAGE            ((byte)0x2e)  // V1
#define CAN_MSG_TOUCH_PROBE                 ((byte)0x2f)


#define CAN_MSG_FILL_BUFFER                 ((byte)0x30)
#define CAN_MSG_ACCESS_BUFFER               ((byte)0x31)
//NUKE #define CAN_MSG_COPY_ADDR_TO_BUFFER          ((byte)0x32)
#define CAN_MSG_COPY_PAGE_TO_BUFFER         ((byte)0x33)
#define CAN_MSG_COPY_BUFFER_TO_PAGE         ((byte)0x34)
#define CAN_MSG_PAGE_CHECKSUM               ((byte)0x35)
//#define CAN_MSG_ACCESS_OPTION_BYTES           ((byte)0x36) //NUKE DEPRECATE
#define CAN_MSG_START_PRIMARY_PROGRAM       ((byte)0x37)
//#define CAN_MSG_READ_DEVICE_MEMORY            ((byte)0x38)    //NUKE DEPRECATE        //reads the device memory at location argument[0] for up to argument[1] words

#define CAN_MSG_CLOSED_LOOP_MOTOR_CONTROL	((byte)0x3a)

#define CAN_MSG_EVENT_MESSAGE               ((byte)0x40)
#define CAN_MSG_LOOPBACK                    ((byte)0x41)
#define CAN_MSG_REPORT_ERROR                ((byte)0x42)
//#define CAN_MSG_AUTO_STATUS                   ((byte)0x43)    //NUKE DEPRECATE
#define CAN_MSG_PRIME_RUN                   ((byte)0x44)
#define CAN_MSG_UNPRIME                     ((byte)0x45)
#define CAN_MSG_PRIME                       ((byte)0x46)
#define CAN_MSG_RUN                         ((byte)0x47)
#define CAN_MSG_STOP                        ((byte)0x48)
// all 0x2a are aliased and must have the same number of data bytes
//#define CAN_MSG_V0_HEAD_CONTROL             ((byte)0x49)  // V0
//#define CAN_MSG_SET_LASER_POWER               ((byte)0x49)    // V0
//#define CAN_MSG_SPINDLE_CONTROL               ((byte)0x49)    // V0
// all 0x2a are aliased and must have the same number of data bytes
#define CAN_MSG_V1_HEAD_FUNCTION_CONTROL    ((byte)0x49)    // V1
#define CAN_MSG_HEAD_EXTRUDE_CONTROL        ((byte)0x49)    // V1
#define CAN_MSG_HEAD_LASER_CONTROL          ((byte)0x49)    // V1
#define CAN_MSG_HEAD_INKJET_CONTROL         ((byte)0x49)    // V1
#define CAN_MSG_HEAD_SPINDLE_CONTROL        ((byte)0x49)    // V1

#define CAN_MSG_SET_POWER_LEVEL_ONLY        ((byte)0x4a)    // for laser and inkjet raster modes
#define CAN_MSG_STEP_MOTOR                  ((byte)0x4b)
#define CAN_MSG_STRING                      ((byte)0x4c)
#define CAN_MSG_DISPLAY_CONTROL             ((byte)0x4d)

///

#define CAN_PRESCALE_VALUE 12  // 48=125Kbps; 12=500Kbps; 6=1Mbit/sec  [APB1_CLK=36MHz]
#define GUI_THEME_DEFAULT						0
#define GUI_THEME_BLACK_AND_BLUE_ON_WHITE		1
#define GUI_THEME_WHITE_AND_YELLOW_ON_BLACK		2
#define LED_ON_MAXCOUNT							5

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

typedef enum {
	CAN_EVENT_DEVICE_ANNOUNCE              = 0,
	CAN_EVENT_BOOTLOADER_ANNOUNCE          = 1,
	CAN_EVENT_DEVICE_HEARTBEAT_0           = 2,
	CAN_EVENT_MANUAL_Z_MOVE                = 3,
	CAN_EVENT_PRIME_COMPLETE               = 4,
	CAN_EVENT_LIMIT_SWITCH_ON              = 5,
	CAN_EVENT_LIMIT_SWITCH_OFF             = 6,
	CAN_EVENT_PROBE_REPORT                 = 7,
	CAN_EVENT_PNP_DOWN_REPORT              = 8,
	CAN_EVENT_PNP_UP_REPORT                = 9,
	CAN_EVENT_PNP_MOTION_COMPLETE          = 10,
	CAN_EVENT_PNP_MOTION_BLOCKED           = 11,
	CAN_EVENT_PROBE_ARMED                  = 12,
	CAN_EVENT_PROBE_CONTACT_BEFORE_MOVE    = 13,
	CAN_EVENT_CO2_WATCHDOG_EXPIRED         = 14,
	CAN_EVENT_CO2_PRIME_FINISHED_START_RUN = 15,
	CAN_EVENT_MOTION_LIMIT1_STATE_CHANGE   = 16,
	CAN_EVENT_MOTION_LIMIT2_STATE_CHANGE   = 17,
	CAN_EVENT_ENCODER_CALIBRATION_PASS     = 18,
	CAN_EVENT_ENCODER_CALIBRATION_FAIL     = 19,
	CAN_EVENT_ENCODER_CONFIG_SAVE_PASS     = 20,
	CAN_EVENT_ENCODER_CONFIG_SAVE_FAIL     = 21,
	CAN_EVENT_ENCODER_BAD_CALIBRATION      = 22,
	CAN_EVENT_ENCODER_CALIBRATING          = 23,
	CAN_EVENT_MOTION_HOMING_IN_PROCESS     = 24,
	CAN_EVENT_MOTION_HOMING_COMPLETE       = 25,
	CAN_EVENT_MOTION_JOGGING_COMPLETE      = 26,
	CAN_EVENT_MOTION_LATHE_COMPLETE        = 27,
	CAN_EVENT_DEVICE_HEARTBEAT_1           = 28,
	CAN_EVENT_DEVICE_HEARTBEAT_2           = 29,
	CAN_EVENT_DEVICE_HEARTBEAT_3           = 30,
	CAN_EVENT_DEVICE_HEARTBEAT_4           = 31,
	CAN_EVENT_DEVICE_HEARTBEAT_5           = 32,
	CAN_EVENT_DEVICE_HEARTBEAT_6           = 33,
	CAN_EVENT_DEVICE_HEARTBEAT_7           = 34,
	CAN_EVENT_DEVICE_HEARTBEAT_8           = 35,
	CAN_EVENT_DEVICE_HEARTBEAT_9           = 36,
	CAN_EVENT_DEVICE_WIPE_SAVED_SETTINGS   = 37,
} canEventMsgType_t;

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

	payloadUnion PayLoad;
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

typedef struct
{
	uint32_t ID; //actual frame sent over canbus before 8 bytes of data
	uint8_t Immediate; //set to 1
	uint8_t TargetAddress; //where is this going
	uint8_t SourceAddress; //who sent it
	uint8_t MsgType; //read or write
	uint8_t MsgId;
	uint8_t Page;
	payloadUnion PayLoad;
	uint8_t DataSize;
}CANMsg;

extern uint16_t					HydraCanAddress;//can address of the motion controller 102207 pcb

extern uint16_t					CanRxLedCountDown;
extern uint16_t					CanTxLedCountDown;

extern uint8_t                  preDefinedAliases[NUM_PRE_DEFINED_ALIASES]; // current list of forced aliases
extern uint8_t                  userDefinedAliases[NUM_USER_DEFINED_ALIASES]; // current list of user added aliases

extern uint8_t 					CanTransmitMailbox;
extern uint32_t 				CanTrasmitMsgWaitCounter;
extern uint8_t					CanRxTargetId;
extern uint8_t					CanRxSourceId;
extern uint8_t					CanRxPage;
extern uint8_t					CanRxMsgType;
extern uint8_t					CanRxImmediateFlag;


extern uint16_t				CanRxInIndex;
extern uint16_t				CanRxOutIndex;

extern CANMsg 				CanRxMsgBuffer[CAN_MSG_BUFFER_SIZE];
extern uint16_t				CanTxQueHead;
extern uint16_t				CanTxQueTail;
extern uint16_t				CanTxWaitCount;
extern CANMsg 				CanTxMsgBuffer[CAN_MSG_BUFFER_SIZE];

extern uint8_t 				RemoteSoapString[0x400];

extern CANMsg				CanLastRxBuffer;
extern CANMsg				CanLastTxBuffer;
extern payloadUnion			TxWorkData;
extern uint16_t				CanRxLedCountDown;
extern uint16_t				CanTxLedCountDown;

void canInit(void);
void initClkAndResetAPB1(uint32_t periph);
void canInitFilter(CAN_TypeDef *CANx, uint8_t filterNum, uint8_t alias, bool immediate, bool enable);
void canAddUserDefinedAlias(uint8_t newAlias, uint8_t aliases[]);
void ProcessCanTxMessage(void);
void ProcessCanRxMessage(void);
uint8_t SendCanMessage(uint32_t id, uint8_t* data, uint8_t size);
void CanAddTxBuffer(uint16_t target, uint16_t Source, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate, payloadUnion* data, uint8_t size);

uint32_t GenerateFrameID(uint16_t target, uint16_t SourceAddress, uint8_t msgType, uint8_t msgId, uint8_t page, uint8_t immediate);
uint8_t canPackIntoTxQueue8x8(uint8_t msgType, uint8_t device, uint8_t msgId, uint8_t page, bool immediate, uint8_t payload[]);
extern uint8_t canAddToTxQueue(canSwStruct *);