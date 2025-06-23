#pragma once


#define NUM_1000HZ                          4   // not restricted....larger the number, the less time per slice
#define NUM_100HZ                           10  // must remain 10 otherwise loop timing will not be correct
#define NUM_10HZ                            10  // must remain 10 otherwise loop timing will not be correct
#define NUM_1HZ                             10  // must remain 10 otherwise loop timing will not be correct

#define SYSTICKS_PER_SECOND                 (NUM_1000HZ * NUM_100HZ * NUM_10HZ * NUM_1HZ)

#define LOOP_1000HZ_MS_PER_CALL     1
#define LOOP_100HZ_MS_PER_CALL      10
#define LOOP_10HZ_MS_PER_CALL       100
#define LOOP_1HZ_MS_PER_CALL        1000

typedef void(*PFUNC)(void);


extern uint32_t HeartBeat;
void Spare(void);
void taskmanager_callback();
void taskmanager_blink_led();