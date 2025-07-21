#pragma 
#include "main.h"
#define PIN_LED_HEARTBEAT              (PIN_PORT_C | PIN_NUM_13 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT


#define PIN_LED_50              (PIN_PORT_A | PIN_NUM_15 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define PIN_LED_100             (PIN_PORT_B | PIN_NUM_03 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define PIN_LED_150             (PIN_PORT_B | PIN_NUM_04 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT
#define PIN_LED_175             (PIN_PORT_B | PIN_NUM_05 | PIN_INIT_HIGH    | OUT_PP_02MHZ) // DEFAULT

#define PIN_ADC_POSITION            (PIN_PORT_B | PIN_NUM_01 | PIN_INIT_HIZ     | ANALOG_IN)    // DEFAULT
#define PIN_ADC_RTD1_IN             (PIN_PORT_A | PIN_NUM_01 | PIN_INIT_HIZ     | ANALOG_IN)    // DEFAULT
#define PIN_ADC_RTD2_IN             (PIN_PORT_A | PIN_NUM_00 | PIN_INIT_HIZ     | ANALOG_IN)    // DEFAULT
