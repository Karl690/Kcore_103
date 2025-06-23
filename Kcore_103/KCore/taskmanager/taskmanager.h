#pragma once

#define NUM_1000HZ              8
#define NUM_100HZ               8      // must remain 10 otherwise loop timing will not be correct
#define NUM_10HZ                8      // must remain 10 otherwise loop timing will not be correct
#define NUM_1HZ                 8     // must remain 10 otherwise loop timing will not be correct

#define SYSTICKS_PER_SECOND                 (NUM_1000HZ * NUM_100HZ * NUM_10HZ * NUM_1HZ)


typedef void(*PFUNC)(void);


extern uint32_t HeartBeat;
void Spare(void);
void func_SystickCallback();
void BlinkHeartBeat();