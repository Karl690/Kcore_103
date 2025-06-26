#pragma once
#include "main.h"

typedef uint32_t pinType;
typedef union {
	uint32_t u32;
	struct {
		unsigned pinNum     : 4; // shift 0
		unsigned portNum    : 4; // shift 4
		unsigned mode       : 4; // shift 8
		unsigned initVal    : 3; // shift 12
		unsigned initEn     : 1; // shift 15
		unsigned pinMask    : 16; // shift 16
	} b;
} pinUnion_t;

#define PIN_UNDEFINED   (0xffffffff)

#define PIN_NUM_SHIFT          0        // pin[3:0]
#define PIN_PORT_NUM_SHIFT     4        // pin[7:4]
#define PIN_MODE_SHIFT         8        // pin[11:8]
#define PIN_INIT_VAL_SHIFT     12       // pin[14:12]
#define PIN_INIT_EN_SHIFT      15       // pin[15]
#define PIN_MASK_SHIFT         16       // pin[31:16]


#define PIN_PORT_A			((uint32_t)(0x0) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_B			((uint32_t)(0x1) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_C			((uint32_t)(0x2) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_D			((uint32_t)(0x3) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_E			((uint32_t)(0x4) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_F			((uint32_t)(0x5) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_G			((uint32_t)(0x6) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_H			((uint32_t)(0x7) << PIN_PORT_NUM_SHIFT)
#define PIN_PORT_I			((uint32_t)(0x8) << PIN_PORT_NUM_SHIFT)

////////////////////////////////////////////////////////////////////////////////
//
// these define a simple pin number mapping (note the 103 included a bit mask)
//
#define PIN_NUM_00             ((0x0 << PIN_NUM_SHIFT) | (0x0001 << PIN_MASK_SHIFT))
#define PIN_NUM_01             ((0x1 << PIN_NUM_SHIFT) | (0x0002 << PIN_MASK_SHIFT))
#define PIN_NUM_02             ((0x2 << PIN_NUM_SHIFT) | (0x0004 << PIN_MASK_SHIFT))
#define PIN_NUM_03             ((0x3 << PIN_NUM_SHIFT) | (0x0008 << PIN_MASK_SHIFT))
#define PIN_NUM_04             ((0x4 << PIN_NUM_SHIFT) | (0x0010 << PIN_MASK_SHIFT))
#define PIN_NUM_05             ((0x5 << PIN_NUM_SHIFT) | (0x0020 << PIN_MASK_SHIFT))
#define PIN_NUM_06             ((0x6 << PIN_NUM_SHIFT) | (0x0040 << PIN_MASK_SHIFT))
#define PIN_NUM_07             ((0x7 << PIN_NUM_SHIFT) | (0x0080 << PIN_MASK_SHIFT))
#define PIN_NUM_08             ((0x8 << PIN_NUM_SHIFT) | (0x0100 << PIN_MASK_SHIFT))
#define PIN_NUM_09             ((0x9 << PIN_NUM_SHIFT) | (0x0200 << PIN_MASK_SHIFT))
#define PIN_NUM_10             ((0xa << PIN_NUM_SHIFT) | (0x0400 << PIN_MASK_SHIFT))
#define PIN_NUM_11             ((0xb << PIN_NUM_SHIFT) | (0x0800 << PIN_MASK_SHIFT))
#define PIN_NUM_12             ((0xc << PIN_NUM_SHIFT) | (0x1000 << PIN_MASK_SHIFT))
#define PIN_NUM_13             ((0xd << PIN_NUM_SHIFT) | (0x2000 << PIN_MASK_SHIFT))
#define PIN_NUM_14             ((0xe << PIN_NUM_SHIFT) | (0x4000 << PIN_MASK_SHIFT))
#define PIN_NUM_15             ((0xf << PIN_NUM_SHIFT) | (0x8000 << PIN_MASK_SHIFT))

//      STM32F10x uses CNFX and MODEX registers per I/O pin to encode the pin function
//      cnfx[1:0],modex[1:0]
//          if input (mode[1:0] == 00)
//              cnf 00 = analog
//              cnf 01 = floating input
//              cnf 10 = input with pull-up/pull-down
//              cnf 11 = reserved
//          else if output (mode[1:0] != 00)
//              cnf 00 = general purpose output push-pull
//              cnf 01 = general purpose output open drain
//              cnf 10 = alternate function output push-pull
//              cnf 11 = alternate function output open drain
//      
//              mode 00 = input
//              mode 01 = output max 10MHz
//              mode 10 = output max 2MHz
//              mode 11 = output max 50MHz

#define ANALOG_IN       (0b0000 << PIN_MODE_SHIFT)
#define IN_HI_Z         (0b0100 << PIN_MODE_SHIFT)
#define IN_PULL_UP      (0b1000 << PIN_MODE_SHIFT)
#define IN_PULL_DN      (0b1000 << PIN_MODE_SHIFT)

#define OUT_PP_02MHZ    (0b0010 << PIN_MODE_SHIFT)
#define OUT_OD_02MHZ    (0b0110 << PIN_MODE_SHIFT)
#define AF_OPP_02MHZ    (0b1010 << PIN_MODE_SHIFT)
#define AF_OOD_02MHZ    (0b1110 << PIN_MODE_SHIFT)

#define OUT_PP_10MHZ    (0b0001 << PIN_MODE_SHIFT)
#define OUT_OD_10MHZ    (0b0101 << PIN_MODE_SHIFT)
#define AF_OPP_10MHZ    (0b1001 << PIN_MODE_SHIFT)
#define AF_OOD_10MHZ    (0b1101 << PIN_MODE_SHIFT)

#define OUT_PP_50MHZ    (0b0011 << PIN_MODE_SHIFT)
#define OUT_OD_50MHZ    (0b0111 << PIN_MODE_SHIFT)
#define AF_OPP_50MHZ    (0b1011 << PIN_MODE_SHIFT)
#define AF_OOD_50MHZ    (0b1111 << PIN_MODE_SHIFT)

#define PIN_INIT_NONE   (0 << PIN_INIT_EN_SHIFT)
#define PIN_INIT_LOW    ((1 << PIN_INIT_EN_SHIFT) | (0 << PIN_INIT_VAL_SHIFT))   // lsb 0
#define PIN_INIT_HIGH   ((1 << PIN_INIT_EN_SHIFT) | (1 << PIN_INIT_VAL_SHIFT))   // lsb 1
#define PIN_INIT_PLDN   ((1 << PIN_INIT_EN_SHIFT) | (2 << PIN_INIT_VAL_SHIFT))   // lsb 0
#define PIN_INIT_PLUP   ((1 << PIN_INIT_EN_SHIFT) | (3 << PIN_INIT_VAL_SHIFT))   // lsb 1
#define PIN_INIT_HIZ    ((1 << PIN_INIT_EN_SHIFT) | (4 << PIN_INIT_VAL_SHIFT))   // lsb 0


#define PIN_CLEAR(a,b) (if (a) a->BRR  = b;)
//pinExtractPortPtr(pin)->BSRR = pinExtractPinMask(pin);
#define PIN_SET(a,b)   (if (a) a->BSRR = b;)

////////////////////////////////////////////////////////////////////////////////

#define pinExtractPinMask(pin)      ((uint32_t)(((pinUnion_t)pin).b.pinMask))


#define pinExtractPinNum(pin)       ((uint32_t)(((pinUnion_t)pin).b.pinNum))
#define pinExtractPortNum(pin)      ((uint32_t)(((pinUnion_t)pin).b.portNum))
#define pinExtractMode(pin)         ((uint32_t)(((pinUnion_t)pin).b.mode))
#define pinExtractSpeed(pin)        ((uint32_t)(((pinUnion_t)pin).b.speed))
#define pinExtractInitVal(pin)      ((uint32_t)(((pinUnion_t)pin).b.initVal))
#define pinExtractInitEn(pin)       ((uint32_t)(((pinUnion_t)pin).b.initEn))
// #define pinExtractPupd(pin)         ((GPIOPuPd_TypeDef)(((pinUnion_t)pin).b.pupd))
#define pinExtractAF(pin)           ((uint32_t)(((pinUnion_t)pin).b.af))
// #define pinExtractOType(pin)        ((GPIOOType_TypeDef)(((pinUnion_t)pin).b.otype))
#define pinExtractLock(pin)         ((uint32_t)(((pinUnion_t)pin).b.lock))
#define pinExtractPortChar(pin)     ((char)(((pinUnion_t)pin).b.portNum) + 'A')


void pinInit(pinType pin);
void pinClear(pinType pin);
void pinSet(pinType pin);
void pinWrite(pinType pin, uint32_t value);
uint32_t pinRead(pinType pin);
void pinToggleOutput(pinType pin);
uint32_t pinReadOutput(pinType pin);