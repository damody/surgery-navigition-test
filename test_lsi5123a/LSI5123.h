#include <windows.h>
typedef char            i8;
typedef unsigned char   u8;
typedef short           i16;
typedef unsigned short  u16;
typedef long            i32;
typedef unsigned long   u32;
typedef float           f32;
typedef double          f64;
////////////////////////////////
//          Error code        //
////////////////////////////////
#define		JSUSB_NO_ERROR				0
#define		JSUSB_INIT_ERROR			2		
#define		JSUSB_UNLOCK_ERROR			3	// Driver Error 
#define		JSUSB_LOCK_COUNTER_ERROR	4	// Driver Error 
#define		JSUSB_SET_SECURITY_ERROR	5	// Driver Error 

#define	DEVICE_RW_ERROR					100	//Device Error	
#define	JSUSB_NO_CARD					101		
#define	JSUSB_DUPLICATE_ID				102

#define	JSLSI_ID_ERROR					300 //User Parameter Error
#define	JSLSI_COUNTER_MODE_ERROR		301
#define	JSLSI_QUADRATURE_TIMES_ERROR	302
#define	JSLSI_LATCH_CTRL_ERROR			303
#define	JSLSI_LATCH_MODE_ERROR			304
#define	JSLSI_POINT_ERROR				305
#define	JSLSI_AXIS_ERROR				306
#define	JSLSI_CLR_OUT_MODE_ERROR		307
#define	JSLSI_HOME_MODE_ERROR			308
#define	JSLSI_POLARITY_ERROR			309
#define	JSLSI_ON_OFF_ERROR				310
#define	JSLSI_TRIGGER_CTRL_ERROR		311
#define JSLSI_COMPARE_OUT_MODE_ERROR	312
#define JSLSI_CHANNEL_ERROR				313
#define	JSLSI_DRIVER_NOT_SUPPORT		400	//Function Error 	

#define	JSLSI_ID_INCONSISTENT_ERROR     500

////////////////////////////////
//	Constant define           //
////////////////////////////////
#define		AXIS_X				0x00	//Axis define
#define		AXIS_Y				0x01
#define		AXIS_Z				0x02
#define		AXIS_A				0x03

#define		A_PHASE_IN			0		//Input point
#define		B_PHASE_IN			1		//Input point
#define		ZERO_IN				2		//Input point
#define		HOME_IN				3		//Input point
#define		LATCH_IN			4		//Input point
#define		CLEAR_IN			5		//Input point
#define		GENERAL_IN			6		//Input point
#define		ZERO_TOGGLE_IN		7		//Input point

#define		GENERAL_OUT			0		//Output point
#define		CLEAR_OUT			1		//Output point

#define		R_HOMING_MODE		0		//Read Parameters(Homing Mode) 
#define		R_COUNTER_MODE		1		//Read Parameters(Counter Mode) 
#define		R_QUADRATURE_TIMES	2		//Read Parameters(Quadrature Timers) 
#define		R_LATCH_MODE		3		//Read Parameters(Latch Mode) 
#define		R_LATCH_CONTROL		4		//Read Parameters(Latch Control) 
#define		R_CLEAR_OUT_MODE	5		//Read Parameters(Clear Out Mode) 
#define		R_TRIGGER_CONTROL	6		//Read Parameters(Trigger Control) 
#define		R_COMPARE_OUT_MODE	7		//Read Parameters(Compoare Out Mode) 
#define		R_INT_MASK			8		//Read Parameters(INT mask) 
#define		R_INT_SOURCE		9		//Read Parameters(INT source) 
#define		R_DURATION_CONSTANT	10		//Read Parameters(Duration constant) 
#define		R_CMP_DURATION_CONSTANT	11	//Read Parameters(CMP Duration constant) 
#define		R_MASK_GENER_O_SRC	12		//Read Parameters(MASK GENER OUT SRC) 
#define     TIMES_X4			0
#define     NONINVERTING		0
/////////////////////////////////////////////////////

#ifndef __lsi5123_H
  #define __lsi5123_H


#ifdef __cplusplus
	extern "C"{
#endif

#define JSLSIStatus int __declspec(dllexport) APIENTRY //Standard calling convention 
//*******  Initialization Section  *****************************************
JSLSIStatus lsi5123_initial(void);										
JSLSIStatus lsi5123_close(void);										
JSLSIStatus lsi5123_info(u8 CardID,u8 *CardExist);																//2
		
//*******  Homing Section  *************************************************
JSLSIStatus lsi5123_set_hard_homing(u8 CardID,u8 Axis,u8 mode);					
JSLSIStatus lsi5123_read_hard_homing_flag(u8 CardID,u8 Axis,u8 *flag);	
JSLSIStatus lsi5123_soft_homing_command(u8 CardID,u8 Axis);				
//-------- for LSI5123A only -----------------------------------------------		
JSLSIStatus lsi5123_rebuilt_ABS_coordinate(u8 CardID,u8 Axis,u8 scale_type,u8 *flag);				//for LSI5123A only 			
JSLSIStatus lsi5123_setup_coordinate(u8 CardID,u8 Axis,i32 position,u8 scale_type,u8 *flag);	//for LSI5123A only
JSLSIStatus lsi5123_check_homed_calibration_flag(u8 CardID,u8 Axis,u8 *flag);						//for LSI5123A only
//--------------------------------------------------------------------------

//*******  Counter Section  ************************************************
JSLSIStatus lsi5123_set_counter_mode(u8 CardID,u8 Axis,u8 mode);			
JSLSIStatus lsi5123_set_quadrature_times(u8 CardID,u8 Axis,u8 times);		
JSLSIStatus lsi5123_read_counter(u8 CardID,u8 Axis,i32 *value);				
JSLSIStatus lsi5123_load_counter(u8 CardID,u8 Axis,i32 value);			
JSLSIStatus lsi5123_preset_counter(u8 CardID,u8 Axis,i32 preset_value);	
JSLSIStatus lsi5123_latch_control(u8 CardID,u8 Channel,u8 On_Off);		
JSLSIStatus lsi5123_latch_mode(u8 CardID,u8 mode);						
//JSLSIStatus lsi5123_latch_mode(u8 CardID,u8 Channel,u8 mode);			
JSLSIStatus lsi5123_read_latch_flag(u8 CardID,u8 Channel,u8 *Flag);		
JSLSIStatus lsi5123_read_latched_value(u8 CardID,u8 Axis,i32 *value);	

//*******  Input/Output Section  *******************************************
JSLSIStatus lsi5123_set_input_polarity(u8 CardID,u8 Axis,u8 point,u8 polarity);		
JSLSIStatus lsi5123_read_input_polarity(u8 CardID,u8 Axis,u8 point,u8 *polarity);			
JSLSIStatus lsi5123_set_led_polarity(u8 CardID,u8 polarity);
JSLSIStatus lsi5123_read_input_status(u8 CardID,u8 Axis,u8 point,u8 *state);
//-------- for LSI5123A only -----------------------------------------------
JSLSIStatus lsi5123_read_output_point(u8 CardID,u8 Axis,u8 point,u8 *on_off);	// for LSI5123A only
JSLSIStatus lsi5123_write_output_point(u8 CardID,u8 Axis,u8 point,u8 on_off);	// for LSI5123A only
//----------------------------------------------------------------------------

//*******  Miscellaneous Section  ******************************************
JSLSIStatus lsi5123_read_parameters(u8 CardID,u8 Axis,u8 Parameter_NO,u8 *Value);

/*
//*******  Interrupt Section  ***********************************************
JSLSIStatus lsi5123_enable_IRQ(u8 CardID,HANDLE *phEvent);
JSLSIStatus lsi5123_disable_IRQ(u8 CardID);
JSLSIStatus lsi5123_set_IRQ_mask(u8 CardID,u8 Axis,u16 Data);
JSLSIStatus lsi5123_IRQ_status(u8 CardID,u32 *Event_Status);
JSLSIStatus lsi5123_link_IRQ_process(u8 CardID,void ( __stdcall *callbackAddr)(u8 CardID));

//*******  Security Section  ***********************************************
JSLSIStatus lsi5123_read_security_status(u8 CardID,u8 *lock_status,u8 *seurity_enable);	
JSLSIStatus lsi5123_unlock_security(u8 CardID,u16 password[5]);	
JSLSIStatus lsi5123_set_password(u8 CardID,u16 password[5]);	
JSLSIStatus lsi5123_change_password(u8 CardID,u16 Oldpassword[5],u16 Password[5]);	
JSLSIStatus lsi5123_clear_password(u8 CardID,u16 Password[5]);
*/

#ifdef __cplusplus
}
#endif

#endif  // belongs to #ifndef __lsi5123_H#ifndef __cplusepluse
