/*
 * Pila.h
 *
 *  Created on: Apr 24, 2022
 *      Author: Dima
 */



#ifndef INC_PILA_H_
#define INC_PILA_H_

//ADDRESS CONFIG PARAMETERS AND DEFINES
#define INPUT_MODULE_ADDR_1 0x41
#define INPUT_MODULE_ADDR_2 0x43
#define INPUT_MODULE_ADDR_3 0x45
#define INPUT_MODULE_ADDR_4 0x47


#define CONFIG_REGISTER 0x07
#define OUTPUT_REGISTER 0x03

#define OUTPUT_MODULE_ADDR_1 0x48
#define OUTPUT_MODULE_ADDR_2 0x46

	/*Upper panel input buttons and switches.*/
#define IN_0_16_AUTO_MANUAL			~Input_1_Sum & 0b1000000000000000
#define IN_0_15_HPUMP				~Input_1_Sum & 0b0100000000000000
#define IN_0_14_START				~Input_1_Sum & 0b0010000000000000
#define IN_0_13_STOP				~Input_1_Sum & 0b0001000000000000
#define IN_0_12_SAWUP				~Input_1_Sum & 0b0000100000000000
#define IN_0_11_SDOWN				~Input_1_Sum & 0b0000010000000000
#define IN_0_10_MAT_CL_UNCLAMP		~Input_1_Sum & 0b0000001000000000
#define IN_0_9_MAT_CL_CLAMP			~Input_1_Sum & 0b0000000100000000
#define IN_0_8_MAT_REV				~Input_1_Sum & 0b0000000010000000
#define IN_0_7_MAT_FWD				~Input_1_Sum & 0b0000000001000000
#define IN_0_6_SAW_CL_UNCLAMP		~Input_1_Sum & 0b0000000000100000
#define IN_0_5_SAW_CL_CLAMP			~Input_1_Sum & 0b0000000000010000
#define IN_0_4_TENSION_UP			~Input_1_Sum & 0b0000000000001000
#define IN_0_3_TENSION_DOWN			~Input_1_Sum & 0b0000000000000100
#define IN_0_2_CHIP					~Input_1_Sum & 0b0000000000000010
#define IN_0_1_COUNTER				~Input_1_Sum & 0b0000000000000001

	/*Limit switches and fuses*/
#define IN_1_16_FUSE10				~Input_2_Sum & 0b1000000000000000
#define IN_1_15_FUSE11				~Input_2_Sum & 0b0100000000000000
#define IN_1_14_SAWUP_LS			~Input_2_Sum & 0b0010000000000000
#define IN_1_13_SAFETY				~Input_2_Sum & 0b0001000000000000
#define IN_1_12_MAT_REV_LS			~Input_2_Sum & 0b0000100000000000
#define IN_1_11_MAT_FWD_LS			~Input_2_Sum & 0b0000010000000000
#define IN_1_10_MAT_CLAMP			~Input_2_Sum & 0b0000001000000000
#define IN_1_9_SAWDOWN_LS			~Input_2_Sum & 0b0000000100000000
#define IN_1_8_ROTATION				~Input_2_Sum & 0b0000000010000000
#define IN_1_7_FEED					~Input_2_Sum & 0b0000000001000000
#define IN_1_6_						~Input_2_Sum & 0b0000000000100000
#define IN_1_5_SAW_CL_UNCLAMP_LS	~Input_2_Sum & 0b0000000000010000
#define IN_1_4_SELECT_COUNTER		~Input_2_Sum & 0b0000000000001000
#define IN_1_3_HMI_LEFT				~Input_2_Sum & 0b0000000000000100
#define IN_1_2_HMI_CENTER			~Input_2_Sum & 0b0000000000000010
#define IN_1_1_HMI_RIGHT			~Input_2_Sum & 0b0000000000000001
	/*TBA*/
#define IN_2_16_					~Input_3_Sum & 0b1000000000000000
#define IN_2_15_					~Input_3_Sum & 0b0100000000000000
#define IN_2_14_					~Input_3_Sum & 0b0010000000000000
#define IN_2_13_					~Input_3_Sum & 0b0001000000000000
#define IN_2_12_					~Input_3_Sum & 0b0000100000000000
#define IN_2_11_					~Input_3_Sum & 0b0000010000000000
#define IN_2_10_					~Input_3_Sum & 0b0000001000000000
#define IN_2_9_						~Input_3_Sum & 0b0000000100000000
#define IN_2_8_						~Input_3_Sum & 0b0000000010000000
#define IN_2_7_						~Input_3_Sum & 0b0000000001000000
#define IN_2_6_						~Input_3_Sum & 0b0000000000100000
#define IN_2_5_						~Input_3_Sum & 0b0000000000010000
#define IN_2_4_						~Input_3_Sum & 0b0000000000001000
#define IN_2_3_						~Input_3_Sum & 0b0000000000000100
#define IN_2_2_						~Input_3_Sum & 0b0000000000000010
#define IN_2_1_						~Input_3_Sum & 0b0000000000000001

	/*Main functionality, hydraulic valve and relay operations. ENABLE.*/
#define OUT_0_16_3PHASE_BACK_ON		Output_2_Sum |= 0b1000000000000000
#define OUT_0_15_TENSION_DOWN_ON	Output_2_Sum |= 0b0100000000000000
#define OUT_0_14_TENSION_UP_ON		Output_2_Sum |= 0b0010000000000000
#define OUT_0_13_FAST_FEED_ON		Output_2_Sum |= 0b0001000000000000
#define OUT_0_12_HPUMP_ON			Output_2_Sum |= 0b0000100000000000
#define OUT_0_11_MAT_REV_ON			Output_2_Sum |= 0b0000010000000000
#define OUT_0_10_MAT_FWD_ON			Output_2_Sum |= 0b0000001000000000
#define OUT_0_9_MAT_CL_UNCLAMP_ON	Output_2_Sum |= 0b0000000100000000
#define OUT_0_8_MAT_CL_CLAMP_ON		Output_2_Sum |= 0b0000000010000000
#define OUT_0_7_CHIP_ON				Output_2_Sum |= 0b0000000001000000
#define OUT_0_6_SAW_CL_UNCLAMP_ON	Output_2_Sum |= 0b0000000000100000
#define OUT_0_5_SAW_CL_CLAMP_ON		Output_2_Sum |= 0b0000000000010000
#define OUT_0_4_SAWDOWN_ON			Output_2_Sum |= 0b0000000000001000
#define OUT_0_3_SAWUP_ON			Output_2_Sum |= 0b0000000000000100
#define OUT_0_2_COUNTER_ON			Output_2_Sum |= 0b0000000000000010
#define OUT_0_1_SAW_ON				Output_2_Sum |= 0b0000000000000001

	/*Indicator and status LEDs. ENABLE*/
#define OUT_1_16__ON				Output_2_Sum |= 0b1000000000000000
#define OUT_1_15__ON				Output_2_Sum |= 0b0100000000000000
#define OUT_1_14__ON				Output_2_Sum |= 0b0010000000000000
#define OUT_1_13__ON				Output_2_Sum |= 0b0001000000000000
#define OUT_1_12__ON				Output_2_Sum |= 0b0000100000000000
#define OUT_1_11__ON				Output_2_Sum |= 0b0000010000000000
#define OUT_1_10__ON				Output_2_Sum |= 0b0000001000000000
#define OUT_1_9__ON					Output_2_Sum |= 0b0000000100000000
#define OUT_1_8__ON					Output_2_Sum |= 0b0000000010000000
#define OUT_1_7__ON					Output_2_Sum |= 0b0000000001000000
#define OUT_1_6__ON					Output_2_Sum |= 0b0000000000100000
#define OUT_1_5__ON					Output_2_Sum |= 0b0000000000010000
#define OUT_1_4__ON					Output_2_Sum |= 0b0000000000001000
#define OUT_1_3__ON					Output_2_Sum |= 0b0000000000000100
#define OUT_1_2__ON					Output_2_Sum |= 0b0000000000000010
#define OUT_1_1__ON					Output_2_Sum |= 0b0000000000000001

	/*Main functionality, hydraulic valve and relay operations. DISABLE.*/
#define OUT_0_16_3PHASE_BACK_OFF	Output_2_Sum &= ~0b1000000000000000
#define OUT_0_15_TENSION_DOWN_OFF	Output_2_Sum &= ~0b0100000000000000
#define OUT_0_14_TENSION_UP_OFF		Output_2_Sum &= ~0b0010000000000000
#define OUT_0_13_FAST_FEED_OFF		Output_2_Sum &= ~0b0001000000000000
#define OUT_0_12_HPUMP_OFF			Output_2_Sum &= ~0b0000100000000000
#define OUT_0_11_MAT_REV_OFF		Output_2_Sum &= ~0b0000010000000000
#define OUT_0_10_MAT_FWD_OFF		Output_2_Sum &= ~0b0000001000000000
#define OUT_0_9_MAT_CL_UNCLAMP_OFF	Output_2_Sum &= ~0b0000000100000000
#define OUT_0_8_MAT_CL_CLAMP_OFF	Output_2_Sum &= ~0b0000000010000000
#define OUT_0_7_CHIP_OFF			Output_2_Sum &= ~0b0000000001000000
#define OUT_0_6_SAW_CL_UNCLAMP_OFF	Output_2_Sum &= ~0b0000000000100000
#define OUT_0_5_SAW_CL_CLAMP_OFF	Output_2_Sum &= ~0b0000000000010000
#define OUT_0_4_SAW_DOWN_OFF		Output_2_Sum &= ~0b0000000000001000
#define OUT_0_3_SAW_UP_OFF			Output_2_Sum &= ~0b0000000000000100
#define OUT_0_2_COUNTER_OFF			Output_2_Sum &= ~0b0000000000000010
#define OUT_0_1_SAW_OFF				Output_2_Sum &= ~0b0000000000000001

	/*Indicator and status LEDs. DISABLE*/
#define OUT_1_16__OFF				Output_2_Sum &= ~0b1000000000000000
#define OUT_1_15__OFF				Output_2_Sum &= ~0b0100000000000000
#define OUT_1_14__OFF				Output_2_Sum &= ~0b0010000000000000
#define OUT_1_13__OFF				Output_2_Sum &= ~0b0001000000000000
#define OUT_1_12__OFF				Output_2_Sum &= ~0b0000100000000000
#define OUT_1_11__OFF				Output_2_Sum &= ~0b0000010000000000
#define OUT_1_10__OFF				Output_2_Sum &= ~0b0000001000000000
#define OUT_1_9__OFF				Output_2_Sum &= ~0b0000000100000000
#define OUT_1_8__OFF				Output_2_Sum &= ~0b0000000010000000
#define OUT_1_7__OFF				Output_2_Sum &= ~0b0000000001000000
#define OUT_1_6__OFF				Output_2_Sum &= ~0b0000000000100000
#define OUT_1_5__OFF				Output_2_Sum &= ~0b0000000000010000
#define OUT_1_4__OFF				Output_2_Sum &= ~0b0000000000001000
#define OUT_1_3__OFF				Output_2_Sum &= ~0b0000000000000100
#define OUT_1_2__OFF				Output_2_Sum &= ~0b0000000000000010
#define OUT_1_1__OFF				Output_2_Sum &= ~0b0000000000000001



#endif /* INC_PILA_H_ */
