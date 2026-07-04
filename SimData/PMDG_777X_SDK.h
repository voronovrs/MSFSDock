//------------------------------------------------------------------------------
//
//  PMDG 777 external connection SDK
//  Copyright (c) 2024 Precision Manuals Development Group
// 
//------------------------------------------------------------------------------

#ifndef PMDG_777X_SDK_H
#define PMDG_777X_SDK_H

// SimConnect data area definitions
#define PMDG_777X_DATA_NAME				"PMDG_777X_Data"
#define PMDG_777X_DATA_ID				0x504D4447
#define PMDG_777X_DATA_DEFINITION		0x504D4448
#define PMDG_777X_CONTROL_NAME			"PMDG_777X_Control"
#define PMDG_777X_CONTROL_ID			0x504D4449
#define PMDG_777X_CONTROL_DEFINITION	0x504D444A
#define PMDG_777X_CDU_0_NAME			"PMDG_777X_CDU_0"
#define PMDG_777X_CDU_1_NAME			"PMDG_777X_CDU_1"
#define PMDG_777X_CDU_2_NAME			"PMDG_777X_CDU_2"
#define PMDG_777X_CDU_0_ID				0x4E477835
#define PMDG_777X_CDU_1_ID				0x4E477836
#define PMDG_777X_CDU_2_ID				0x4E477837
#define PMDG_777X_CDU_0_DEFINITION		0x4E477838
#define PMDG_777X_CDU_1_DEFINITION		0x4E477839
#define PMDG_777X_CDU_2_DEFINITION		0x4E47783A

// NOTE - add these lines to the 777_Options.ini file: 
//
//[SDK]
//EnableDataBroadcast=1
//
// to enable the aircraft data sending from the 777.
//
//
// Add any of these lines to the [SDK] section of the 777_Options.ini file: 
//
//EnableCDUBroadcast.0=1 
//EnableCDUBroadcast.1=1 
//EnableCDUBroadcast.2=1 
//
// to enable the contents of the corresponding CDU screen to be sent to external programs.


// 777X data structure
struct PMDG_777X_Data
{
	////////////////////////////////////////////
	// Controls and indicators
	////////////////////////////////////////////

	// Overhead Maintenance Panel
	//------------------------------------------

	// Backup Window Heat
	bool			ICE_WindowHeatBackUp_Sw_OFF[2];		  

	// Standby Power
	unsigned char	ELEC_StandbyPowerSw;				// 0: OFF  1: AUTO  2: BAT

	// Flight Controls		
	bool			FCTL_WingHydValve_Sw_SHUT_OFF[3];	// left/right/center	
	bool			FCTL_TailHydValve_Sw_SHUT_OFF[3];	// left/right/center	
	bool			FCTL_annunTailHydVALVE_CLOSED[3];	// left/right/center	
	bool			FCTL_annunWingHydVALVE_CLOSED[3];	// left/right/center	
	bool			FCTL_PrimFltComputersSw_AUTO;		// true: AUTO  false: DISC
	bool			FCTL_annunPrimFltComputersDISC;

	// APU MAINT
	bool			APU_Power_Sw_TEST;					

	// EEC MAINT
	bool			ENG_EECPower_Sw_TEST[2];			

	// ELECTRICAL
	bool			ELEC_TowingPower_Sw_BATT;			
	bool			ELEC_annunTowingPowerON_BATT;

	// CARGO TEMP SELECTORS
	unsigned char   AIR_CargoTemp_Selector[2];			// aft / bulk  0=OFF  1=LOW  2=HIGH   AFT/BULK
	unsigned char   AIR_CargoTemp_MainDeckFwd_Sel;		// 0: C ... 60: W
	unsigned char   AIR_CargoTemp_MainDeckAft_Sel;		// 0: C ... 60: W
	unsigned char   AIR_CargoTemp_LowerFwd_Sel;			// 0: C ... 60: W
	unsigned char   AIR_CargoTemp_LowerAft_Sel;			// 0: C ... 60: W  67: HEAT H  70: HEAT OFF  73: HEAT L


	// Overhead Panel
	//------------------------------------------

	// ADIRU
	bool			ADIRU_Sw_On;			
	bool			ADIRU_annunOFF;	
	bool			ADIRU_annunON_BAT;	
	
	// Flight Controls
	bool			FCTL_ThrustAsymComp_Sw_AUTO;
	bool			FCTL_annunThrustAsymCompOFF;

	// Electrical
	bool			ELEC_CabUtilSw;
	bool			ELEC_annunCabUtilOFF;
	bool			ELEC_IFEPassSeatsSw;
	bool			ELEC_annunIFEPassSeatsOFF;
	bool			ELEC_Battery_Sw_ON;					
	bool			ELEC_annunBattery_OFF;
	bool			ELEC_annunAPU_GEN_OFF;
	bool			ELEC_APUGen_Sw_ON;
	unsigned char	ELEC_APU_Selector;					// 0: OFF  1: ON  2: START	
	bool			ELEC_annunAPU_FAULT;
	bool			ELEC_BusTie_Sw_AUTO[2];
	bool			ELEC_annunBusTieISLN[2];
	bool			ELEC_ExtPwrSw[2];					// primary/secondary - MOMENTARY SWITCHES
	bool			ELEC_annunExtPowr_ON[2];	
	bool			ELEC_annunExtPowr_AVAIL[2];				
	bool			ELEC_Gen_Sw_ON[2];
	bool			ELEC_annunGenOFF[2];
	bool			ELEC_BackupGen_Sw_ON[2];
	bool			ELEC_annunBackupGenOFF[2];
	bool			ELEC_IDGDiscSw[2];					// MOMENTARY SWITCHES
	bool			ELEC_annunIDGDiscDRIVE[2];

	// Wiper Selectors
	unsigned char	WIPERS_Selector[2];					// left/right   0: OFF  1: INT  2: LOW  3:HIGH

	// Emergency Lights
	unsigned char	LTS_EmerLightsSelector;				// 0: OFF  1: ARMED  2: ON
	
	// Service Interphone
	bool			COMM_ServiceInterphoneSw;

	// Passenger Oxygen
	bool			OXY_PassOxygen_Sw_On;							
	bool			OXY_annunPassOxygenON;

	// Window Heat
	bool			ICE_WindowHeat_Sw_ON[4];			// L-Side/L-Fwd/R-Fwd/R-Side
	bool			ICE_annunWindowHeatINOP[4];			// L-Side/L-Fwd/R-Fwd/R-Side

	// Hydraulics
	bool			HYD_RamAirTurbineSw;
	bool			HYD_annunRamAirTurbinePRESS;
	bool			HYD_annunRamAirTurbineUNLKD;
	bool			HYD_PrimaryEngPump_Sw_ON[2];
	bool			HYD_PrimaryElecPump_Sw_ON[2];
	unsigned char	HYD_DemandElecPump_Selector[2];		// 0: OFF  1: AUTO  2: ON
	unsigned char	HYD_DemandAirPump_Selector[2];		// 0: OFF  1: AUTO  2: ON
	bool			HYD_annunPrimaryEngPumpFAULT[2];
	bool			HYD_annunPrimaryElecPumpFAULT[2];
	bool			HYD_annunDemandElecPumpFAULT[2];			
	bool			HYD_annunDemandAirPumpFAULT[2];	

	// Passenger Signs
	unsigned char	SIGNS_NoSmokingSelector;			// 0: OFF  1: AUTO   2: ON
	unsigned char	SIGNS_SeatBeltsSelector;			// 0: OFF  1: AUTO   2: ON

	// Flight Deck Lights
	unsigned char	LTS_DomeLightKnob;					// Position 0...100
	unsigned char	LTS_CircuitBreakerKnob;				// Position 0...100
	unsigned char	LTS_OvereadPanelKnob;				// Position 0...100
	unsigned char	LTS_GlareshieldPNLlKnob;			// Position 0...100
	unsigned char	LTS_GlareshieldFLOODKnob;			// Position 0...100
	bool			LTS_Storm_Sw_ON;
	bool			LTS_MasterBright_Sw_ON;
	unsigned char	LTS_MasterBrigntKnob;				// Position 0...100
	unsigned char	LTS_IndLightsTestSw;				// 0: TEST  1: BRT  2: DIM

	// Exterior Lights
	bool			LTS_LandingLights_Sw_ON[3];			// Left/Right/Nose 
	bool			LTS_Beacon_Sw_ON;
	bool			LTS_NAV_Sw_ON;
	bool			LTS_Logo_Sw_ON;
	bool			LTS_Wing_Sw_ON;
	bool			LTS_RunwayTurnoff_Sw_ON[2];
	bool			LTS_Taxi_Sw_ON;
	bool			LTS_Strobe_Sw_ON;

	// Engine, APU and Cargo Fire
	bool			FIRE_CargoFire_Sw_Arm[2];			// FWD/AFT
	bool			FIRE_annunCargoFire[2];				// FWD/AFT
	bool			FIRE_CargoFireDisch_Sw;				// MOMENTARY SWITCH
	bool			FIRE_annunCargoDISCH;
	bool			FIRE_FireOvhtTest_Sw;				// MOMENTARY SWITCH
	unsigned char	FIRE_APUHandle;						// 0: IN (NORMAL)  1: PULLED OUT  2: TURNED LEFT  3: TURNED RIGHT  (2 & 3 ane momnentary positions)
	bool			FIRE_APUHandleUnlock_Sw;			// MOMENTARY SWITCH resets when handle pulled
	bool			FIRE_annunAPU_BTL_DISCH;		
	bool			FIRE_EngineHandleIlluminated[2];
	bool			FIRE_APUHandleIlluminated;
	bool			FIRE_EngineHandleIsUnlocked[2];
	bool			FIRE_APUHandleIsUnlocked;
	bool			FIRE_annunMainDeckCargoFire;
	bool			FIRE_annunCargoDEPR;				// DEPR light in DEPR/DISCH guarded switch

	// Engine
	bool			ENG_EECMode_Sw_NORM[2];				// left / right
	unsigned char	ENG_Start_Selector[2];				// left / right  0: START  1: NORM
	bool			ENG_Autostart_Sw_ON;
	bool			ENG_annunALTN[2];
	bool			ENG_annunAutostartOFF;
	
	// Fuel
	bool			FUEL_CrossFeedFwd_Sw;
	bool			FUEL_CrossFeedAft_Sw;
	bool			FUEL_PumpFwd_Sw[2];					// left fwd / right fwd
	bool			FUEL_PumpAft_Sw[2];					// left aft / right aft
	bool			FUEL_PumpCtr_Sw[2];					// ctr left / ctr right
	bool			FUEL_JettisonNozle_Sw[2];			// left / right
	bool			FUEL_JettisonArm_Sw;
	bool			FUEL_FuelToRemain_Sw_Pulled;
	unsigned char	FUEL_FuelToRemain_Selector;			// 0: DECR  1: Neutral  2: INCR
	bool			FUEL_annunFwdXFEED_VALVE;
	bool			FUEL_annunAftXFEED_VALVE;
	bool			FUEL_annunLOWPRESS_Fwd[2];			// left fwd / right fwd
	bool			FUEL_annunLOWPRESS_Aft[2];			// left aft / right aft
	bool			FUEL_annunLOWPRESS_Ctr[2];			// ctr left / ctr right
	bool			FUEL_annunJettisonNozleVALVE[2];	// left / right
	bool			FUEL_annunArmFAULT;	
	
	// Anti-Ice
	unsigned char	ICE_WingAntiIceSw;					// 0: OFF  1: AUTO  2: ON
	unsigned char	ICE_EngAntiIceSw[2];				// 0: OFF  1: AUTO  2: ON


	// Air Conditioning
	bool			AIR_Pack_Sw_AUTO[2];				// left / right
	bool			AIR_TrimAir_Sw_On[2];				// left / right
	bool			AIR_RecircFan_Sw_On[2];				// upper / lower	
	unsigned char	AIR_TempSelector[2];				// flt deck / cabin  0: C ... 60: W ... 70: MAN (flt deck only)  	
	bool			AIR_AirCondReset_Sw_Pushed;			// MOMENTARY action		
	bool			AIR_EquipCooling_Sw_AUTO;		
	bool			AIR_Gasper_Sw_On;
	bool			AIR_annunPackOFF[2];		
	bool			AIR_annunTrimAirFAULT[2];		
	bool			AIR_annunEquipCoolingOVRD;	
	bool			AIR_AltnVentSw_ON;
	bool			AIR_annunAltnVentFAULT;
	bool			AIR_MainDeckFlowSw_NORM;			// M/D FLOW  true: NORM  false: HIGH

	// Bleed Air
	bool			AIR_EngBleedAir_Sw_AUTO[2];			// left engine / right engine
	bool			AIR_APUBleedAir_Sw_AUTO;
	bool			AIR_IsolationValve_Sw[2];			// left / right 
	bool			AIR_CtrIsolationValve_Sw;			// left / right 
	bool			AIR_annunEngBleedAirOFF[2];			   // left engine / right engine
	bool			AIR_annunAPUBleedAirOFF;		
	bool			AIR_annunIsolationValveCLOSED[2];	// left / right 
	bool			AIR_annunCtrIsolationValveCLOSED;	

	
	// Pressurization
	bool			AIR_OutflowValve_Sw_AUTO[2];		// fwd / aft
	unsigned char	AIR_OutflowValveManual_Selector[2];	// fwd / aft   0: OPEN  1: Neutral  2: CLOSE
	bool			AIR_LdgAlt_Sw_Pulled;
	unsigned char	AIR_LdgAlt_Selector;				// 0: DECR  1: Neutral  2: INCR
	bool			AIR_annunOutflowValve_MAN[2];		// fwd / aft
		


	// Forward panel
	//------------------------------------------

	// Center 
	unsigned char	GEAR_Lever;							// 0: UP  1: DOWN
	bool			GEAR_LockOvrd_Sw;					// MOMENTARY SWITCH (resets when gear lever moved)
	bool			GEAR_AltnGear_Sw_DOWN;					
	bool			GPWS_FlapInhibitSw_OVRD;
	bool			GPWS_GearInhibitSw_OVRD;
	bool			GPWS_TerrInhibitSw_OVRD;
	bool			GPWS_RunwayOvrdSw_OVRD;
	bool			GPWS_GSInhibit_Sw;
	bool			GPWS_annunGND_PROX_top;
	bool			GPWS_annunGND_PROX_bottom;
	unsigned char	BRAKES_AutobrakeSelector;			// 0: RTO  1: OFF  2: DISARM   3: "1" ... 5: MAX AUTO

	// Standby - ISFD  (all are MOMENTARY action switches)
	bool			ISFD_Baro_Sw_Pushed;
	bool			ISFD_RST_Sw_Pushed;
	bool			ISFD_Minus_Sw_Pushed;
	bool			ISFD_Plus_Sw_Pushed;
	bool			ISFD_APP_Sw_Pushed;
	bool			ISFD_HP_IN_Sw_Pushed;

	// Left 
	bool			ISP_Nav_L_Sw_CDU;
	bool			ISP_DsplCtrl_L_Sw_Altn;
	bool			ISP_AirDataAtt_L_Sw_Altn;
	unsigned char	DSP_InbdDspl_L_Selector;			//0: ND  1: NAV  2: MFD  3: EICAS
	bool			EFIS_HdgRef_Sw_Norm;
	bool			EFIS_annunHdgRefTRUE;
	int				BRAKES_BrakePressNeedle;			// Value 0...100 (corresponds to 0...4000 PSI)
	bool			BRAKES_annunBRAKE_SOURCE;

	// Right 
	bool			ISP_Nav_R_Sw_CDU;
	bool			ISP_DsplCtrl_R_Sw_Altn;
	bool			ISP_AirDataAtt_R_Sw_Altn;
	unsigned char	ISP_FMC_Selector;					//0: LEFT   1: AUTO  2: RIGHT
	unsigned char	DSP_InbdDspl_R_Selector;			//0: EICAS  1: MFD   2: ND  3: PFD

	// Left & Right Sidewalls
	unsigned char	AIR_ShoulderHeaterKnob[2];			// Left / Right  Position 0...100
	unsigned char	AIR_FootHeaterSelector[2];			// Left / Right  0: OFF  1: LOW  2: HIGH
	unsigned char	LTS_LeftFwdPanelPNLKnob;			// Position 0...100
	unsigned char	LTS_LeftFwdPanelFLOODKnob;			// Position 0...100
	unsigned char	LTS_LeftOutbdDsplBRIGHTNESSKnob;	// Position 0...100
	unsigned char	LTS_LeftInbdDsplBRIGHTNESSKnob;		// Position 0...100

	unsigned char	LTS_RightFwdPanelPNLKnob;			// Position 0...100
	unsigned char	LTS_RightFwdPanelFLOODKnob;			// Position 0...100	
	unsigned char	LTS_RightInbdDsplBRIGHTNESSKnob;	// Position 0...100
	unsigned char	LTS_RightOutbdDsplBRIGHTNESSKnob;	// Position 0...100


	// Chronometers (Left / Right)
	bool			CHR_Chr_Sw_Pushed[2];				// MOMENTARY SWITCH
	bool			CHR_TimeDate_Sw_Pushed[2];			// MOMENTARY SWITCH
	unsigned char	CHR_TimeDate_Selector[2];			// 0: UTC  1: MAN
	unsigned char	CHR_Set_Selector[2];				// 0: RUN  1: HLDY  2: MM  3: HD
	unsigned char	CHR_ET_Selector[2];					// 0: RESET (MOMENTARY spring-loaded to HLD)  1: HLD  2: RUN


	// Glareshield
	//------------------------------------------

	// EFIS switches (left / right)
	bool			EFIS_MinsSelBARO[2];
	bool			EFIS_BaroSelHPA[2];
	unsigned char	EFIS_VORADFSel1[2];					// 0: VOR  1: OFF  2: ADF
	unsigned char	EFIS_VORADFSel2[2];					// 0: VOR  1: OFF  2: ADF
	unsigned char	EFIS_ModeSel[2];					// 0: APP  1: VOR  2: MAP  3: PLAN
	unsigned char	EFIS_RangeSel[2];					// 0: 10 ... 6: 640
	unsigned char	EFIS_MinsKnob[2];					// 0..99
	unsigned char	EFIS_BaroKnob[2];					// 0..99

	// EFIS MOMENTARY action (left / right)
	bool			EFIS_MinsRST_Sw_Pushed[2];
	bool			EFIS_BaroSTD_Sw_Pushed[2];
	bool			EFIS_ModeCTR_Sw_Pushed[2];
	bool			EFIS_RangeTFC_Sw_Pushed[2];
	bool			EFIS_WXR_Sw_Pushed[2];
	bool			EFIS_STA_Sw_Pushed[2];
	bool			EFIS_WPT_Sw_Pushed[2];
	bool			EFIS_ARPT_Sw_Pushed[2];
	bool			EFIS_DATA_Sw_Pushed[2];
	bool			EFIS_POS_Sw_Pushed[2];
	bool			EFIS_TERR_Sw_Pushed[2];

	// MCP - Variables
	float			MCP_IASMach;						// Mach if < 10.0
	bool			MCP_IASBlank;
	unsigned short	MCP_Heading;
	unsigned short	MCP_Altitude;
	short			MCP_VertSpeed;
	float			MCP_FPA;
	bool			MCP_VertSpeedBlank;
	
	// MCP - Switches
	bool			MCP_FD_Sw_On[2];					// left / right
	bool			MCP_ATArm_Sw_On[2];					// left / right
	unsigned char	MCP_BankLimitSel;					// 0: AUTO  1: 5  2: 10 ... 5: 25
	bool			MCP_AltIncrSel;						// false: AUTO  true: 1000
	bool			MCP_DisengageBar;
	unsigned char	MCP_Speed_Dial;						// 0 ... 99
	unsigned char	MCP_Heading_Dial;					// 0 ... 99
	unsigned char	MCP_Altitude_Dial;					// 0 ... 99
	unsigned char	MCP_VS_Wheel;						// 0 ... 99

	unsigned char	MCP_HDGDial_Mode;					// 0: Dial shows HDG, 1: Dial shows TRK
	unsigned char	MCP_VSDial_Mode;					// 0: Dial shows VS, 1: Dial shows FPA

	// MCP - MOMENTARY action switches
	bool			MCP_AP_Sw_Pushed[2];				// left / right
	bool			MCP_CLB_CON_Sw_Pushed;
	bool			MCP_AT_Sw_Pushed;
	bool			MCP_LNAV_Sw_Pushed;
	bool			MCP_VNAV_Sw_Pushed;
	bool			MCP_FLCH_Sw_Pushed;
	bool			MCP_HDG_HOLD_Sw_Pushed;
	bool			MCP_VS_FPA_Sw_Pushed;
	bool			MCP_ALT_HOLD_Sw_Pushed;
	bool			MCP_LOC_Sw_Pushed;
	bool			MCP_APP_Sw_Pushed;
	bool			MCP_Speeed_Sw_Pushed;
	bool			MCP_Heading_Sw_Pushed;
	bool			MCP_Altitude_Sw_Pushed;
	bool			MCP_IAS_MACH_Toggle_Sw_Pushed;
	bool			MCP_HDG_TRK_Toggle_Sw_Pushed;
	bool			MCP_VS_FPA_Toggle_Sw_Pushed;

	// MCP - Annunciator lights
	bool			MCP_annunAP[2];						// left / right
	bool			MCP_annunAT;
	bool			MCP_annunLNAV;
	bool			MCP_annunVNAV;
	bool			MCP_annunFLCH;
	bool			MCP_annunHDG_HOLD;
	bool			MCP_annunVS_FPA;
	bool			MCP_annunALT_HOLD;
	bool			MCP_annunLOC;
	bool			MCP_annunAPP;

	// Display Select Panel	- These are all MOMENTARY SWITCHES
	bool			DSP_L_INBD_Sw;						
	bool			DSP_R_INBD_Sw;
	bool			DSP_LWR_CTR_Sw;
	bool			DSP_ENG_Sw;
	bool			DSP_STAT_Sw;
	bool			DSP_ELEC_Sw;
	bool			DSP_HYD_Sw;
	bool			DSP_FUEL_Sw;
	bool			DSP_AIR_Sw;
	bool			DSP_DOOR_Sw;
	bool			DSP_GEAR_Sw;
	bool			DSP_FCTL_Sw;
	bool			DSP_CAM_Sw;
	bool			DSP_CHKL_Sw;
	bool			DSP_COMM_Sw;
	bool			DSP_NAV_Sw;
	bool			DSP_CANC_RCL_Sw;
	bool			DSP_annunL_INBD;
	bool			DSP_annunR_INBD;
	bool			DSP_annunLWR_CTR;

	// Master Warning/Caution
	bool			WARN_Reset_Sw_Pushed[2];			// MOMENTARY action
	bool			WARN_annunMASTER_WARNING[2];
	bool			WARN_annunMASTER_CAUTION[2];


	// Forward Aisle Stand Panel
	//------------------------------------------

	bool			ISP_DsplCtrl_C_Sw_Altn;
	unsigned char	LTS_UpperDsplBRIGHTNESSKnob;		// Position 0...100
	unsigned char	LTS_LowerDsplBRIGHTNESSKnob;		// Position 0...100
	bool			EICAS_EventRcd_Sw_Pushed;			// MOMENTARY action		
	
	// CDU (Left/Right/Center)
	bool			CDU_annunEXEC[3];
	bool			CDU_annunDSPY[3];
	bool			CDU_annunFAIL[3];
	bool			CDU_annunMSG[3];
	bool			CDU_annunOFST[3];
	unsigned char	CDU_BrtKnob[3];						// 0: DecreasePosition 1: Neutral  2: Increase


	// Control Stand
	//------------------------------------------

	bool			FCTL_AltnFlaps_Sw_ARM;				
	unsigned char	FCTL_AltnFlaps_Control_Sw;			// 0: RET  1: OFF  2: EXT
	bool			FCTL_StabCutOutSw_C_NORMAL;
	bool			FCTL_StabCutOutSw_R_NORMAL;
	unsigned char	FCTL_AltnPitch_Lever;				// 0: NOSE DOWN  1: NEUTRAL  2: NOSE UP
	unsigned char	FCTL_Speedbrake_Lever;				// Position 0...100  0: DOWN,  25: ARMED, 26...100: DEPLOYED 
	unsigned char	FCTL_Flaps_Lever;					// 0: UP  1: 1  2: 5  3: 15  4: 20  5: 25  6: 30 	
	bool			ENG_FuelControl_Sw_RUN[2];
	bool			BRAKES_ParkingBrakeLeverOn;


	// Aft Aisle Stand Panel
	//------------------------------------------
	
	// Audio Control Panels								// Comm Systems: 0=VHFL 1=VHFC 2=VHFR 3=FLT 4=CAB 5=PA 6=HFL 7=HFR 8=SAT1 9=SAT2 10=SPKR 11=VOR/ADF 12=APP
	unsigned char   COMM_SelectedMic[3];				// array: 0=capt, 1=F/O, 2=observer  values: 0..9 (VHF..SAT2) as listed above; -1 if no MIC is selected
	unsigned short	COMM_ReceiverSwitches[3];			// array: 0=capt, 1=F/O, 2=observer.  Bit mask for selected receivers with bits indicating: 0=VHFL 1=VHFC 2=VHFR 3=FLT 4=CAB 5=PA 6=HFL 7=HFR 8=SAT1 9=SAT2 10=SPKR 11=VOR/ADF 12=APP
	unsigned char	COMM_OBSAudio_Selector;				// 0: CAPT  1: NORMAL  2: F/O

	// Radio Control Panels								// arrays: 0=capt, 1=F/O, 2=observer
	unsigned char   COMM_SelectedRadio[3];				// 0: VHFL  1: VHFC  2: VHFL  3: HFL  5: HFR (4 not used)
	bool			COMM_RadioTransfer_Sw_Pushed[3];	// MOMENTARY action
	bool			COMM_RadioPanelOff[3];
	bool			COMM_annunAM[3];					

	// TCAS Panel
	bool			XPDR_XpndrSelector_R;				// true: R     false: L
	bool			XPDR_AltSourceSel_ALTN;				// true: ALTN  false: NORM  
	unsigned char	XPDR_ModeSel;						// 0: STBY  1: ALT RPTG OFF  2: XPNDR  3: TA ONLY  4: TA/RA
	bool			XPDR_Ident_Sw_Pushed;				// MOMENTARY action

	// Engine Fire 
	unsigned char	FIRE_EngineHandle[2];				// ENG 1/ENG2   0: IN (NORMAL)  1: PULLED OUT  2: TURNED LEFT  3: TURNED RIGHT  (2 & 3 are momenentary positions)
	bool			FIRE_EngineHandleUnlock_Sw[2];		// ENG 1/ENG2   MOMENTARY SWITCH resets when handle pulled
	bool			FIRE_annunENG_BTL_DISCH[2];			// ENG 1/ENG2

	// Aileron & Rudder Trim
	unsigned char	FCTL_AileronTrim_Switches;			// 0: LEFT WING DOWN  1: NEUTRAL  2: RIGHT WING DOWN (both switches move together)
	unsigned char	FCTL_RudderTrim_Knob;				// 0: NOSE LEFT  1: NEUTRAL  2: NOSE RIGHT
	bool			FCTL_RudderTrimCancel_Sw_Pushed;	// MOMENTARY action

	// Evacuation Panel
	bool			EVAC_Command_Sw_ON;
	bool			EVAC_PressToTest_Sw_Pressed;
	bool			EVAC_HornSutOff_Sw_Pulled;
	bool			EVAC_LightIlluminated;


	// Aisle Stand PNL/FLOOD & Floor lights
	unsigned char	LTS_AisleStandPNLKnob;				// Position 0...100
	unsigned char	LTS_AisleStandFLOODKnob;			// Position 0...100
	unsigned char	LTS_FloorLightsSw; 					// 0: BRT  1: OFF  2: DIM


	// Door state
	// Possible values are, 0: open, 1: closed, 2: closed and armed, 3: closing, 4: opening.
	// The array contains these doors:
	//  0: Entry 1L,
	//  1: Entry 1R,
	//  2: Entry 2L,
	//  3: Entry 2R,
	//  4: Entry 3L,				(This is the door aft of the wing. It is marked 4L on -300)
	//  5: Entry 3R,		
	//  6: Entry 4L,				(marked 5L on -300)
	//  7: Entry 4R,
	//  8: Entry 5L,
	//  9: Entry 5R,
	// 10: Cargo Fwd,
	// 11: Cargo Aft,
	// 12: Cargo Main,				(Freighter)
	// 13: Cargo Bulk,
	// 14: Avionics Access,
	// 15: EE Access
	unsigned char	DOOR_state[16];
	bool			DOOR_CockpitDoorOpen;

	// Additional variables
	//------------------------------------------
	
	bool			ENG_StartValve[2];					// true: valve open
	float			AIR_DuctPress[2];					// PSI
	float			FUEL_QtyCenter;						// LBS
	float			FUEL_QtyLeft;						// LBS
	float			FUEL_QtyRight;						// LBS
	float			FUEL_QtyAux;						// LBS
	bool			IRS_aligned;						// at least one IRU is aligned

	bool			EFIS_BaroMinimumsSet[2];			// left/right control panel. Note: check EFIS_MinsSelBARO[2] to determine if the active minimums is BARO or RADIO
	int				EFIS_BaroMinimums[2];
	bool			EFIS_RadioMinimumsSet[2];
	int				EFIS_RadioMinimums[2];

	// Display formats selected on the display units
	// Values are:
	// 	0:	Off,
	// 	1:	Blank,
	// 	2:	PFD,
	// 	3:	ND,
	// 	4:	EICAS,
	// 	5:	ENG,
	// 	6:	STAT,
	// 	7:	CHKL,
	// 	8:	COMM,
	// 	9:	CAM,
	// 10:	ELEC,
	// 11:	HYD,
	// 12:	FUEL,
	// 13:	AIR,
	// 14:	DOOR,
	// 15:	GEAR,
	// 16:	FCTL
	unsigned char	EFIS_Display[6];					// Display units:  0: Capt outboard, 1: Capt inboard, 2: Upper, 3: Lower, 4: FO Inboard, 5: FO Outboard

	unsigned char   AircraftModel;						// 1: -200  2: -200ER  3: -300  4: -200LR  5: 777F  6: -300ER
	bool			WeightInKg;							// false: LBS  true: KG
	bool			GPWS_V1CallEnabled;					// GPWS V1 call-out option enabled
	bool			GroundConnAvailable;				// can connect/disconnect ground air/electrics

	unsigned char	FMC_TakeoffFlaps;					// degrees, 0 if not set
	unsigned char	FMC_V1;								// knots, 0 if not set
	unsigned char	FMC_VR;								// knots, 0 if not set
	unsigned char	FMC_V2;								// knots, 0 if not set
	unsigned short	FMC_ThrustRedAlt;					// 1: FLAPS 1,  5: FLAPS 5,  otherwise altitude in ft
	unsigned short	FMC_AccelerationAlt;				// ft
	unsigned short	FMC_EOAccelerationAlt;				// ft
	unsigned char	FMC_LandingFlaps;					// degrees, 0 if not set
	unsigned char	FMC_LandingVREF;					// knots, 0 if not set
	unsigned short  FMC_CruiseAlt;						// ft, 0 if not set
	short			FMC_LandingAltitude;				// ft; -32767 if not available
	unsigned short  FMC_TransitionAlt;					// ft
	unsigned short  FMC_TransitionLevel;				// ft
	bool			FMC_PerfInputComplete;
	float			FMC_DistanceToTOD;					// nm; 0.0 if passed, negative if n/a
	float			FMC_DistanceToDest;					// nm; negative if n/a
	char			FMC_flightNumber[9];
	bool			WheelChocksSet;
	bool			APURunning;

	// FMC thrust limit mode
	// Values are:
	//  0:  TO,
	//  1:  TO 1,
	//  2:  TO 2,
	//  3:  TO B,
	//  4:  CLB,
	//  5:  CLB 1,
	//  6:  CLB 2,
	//  7:  CRZ,
	//  8:  CON,
	//  9:  G/A,
	// 10:  D-TO,
	// 11:  D-TO 1,
	// 12:  D-TO 2,
	// 13:  A-TO,
	// 14:  A-TO 1,
	// 15:  A-TO 2,
	// 16:  A-TO B
	unsigned char	FMC_ThrustLimitMode;

	// Normal checklist completion status
	// Array elements are:
	// 	0:  PREFLIGHT,
	// 	1:  BEFORE_START,
	// 	2:  BEFORE_TAXI,
	// 	3:  BEFORE_TAKEOFF,
	// 	4:  AFTER_TAKEOFF,
	// 	5:  DESCENT,
	// 	6:  APPROACH,
	// 	7:  LANDING,
	// 	8:  SHUTDOWN,
	// 	9:  SECURE

	bool ECL_ChecklistComplete[10];

	unsigned char   reserved[84];						
};

// 777 CDU Screen Cell Structure
//
// The Symbol is the ASCII code of the character to be drawn plus the following special symbols:
// \xA1: left arrow
// \xA2: right arrow
//
// See the note on enabling the CDU screen broadcast at the beginning of this file.

struct PMDG_777X_CDU_Cell
{
	unsigned char	Symbol;			
	unsigned char	Color;					// any of PMDG_777X_CDU_COLOR_ defines
	unsigned char	Flags;					// a combination of PMDG_777X_CDU_FLAG_ bits
};

// 777 CDU Screen Data Structure

#define CDU_COLUMNS	24
#define CDU_ROWS	14

struct PMDG_777X_CDU_Screen
{
	PMDG_777X_CDU_Cell Cells[CDU_COLUMNS][CDU_ROWS];	
	bool Powered;							// true if the CDU is powered
};

// 777X CDU Screen Cell Colors
#define PMDG_777X_CDU_COLOR_WHITE		0		
#define PMDG_777X_CDU_COLOR_CYAN		1
#define PMDG_777X_CDU_COLOR_GREEN		2
#define PMDG_777X_CDU_COLOR_MAGENTA		3
#define PMDG_777X_CDU_COLOR_AMBER		4
#define PMDG_777X_CDU_COLOR_RED			5

// 777X CDU Screen Cell flags
#define PMDG_777X_CDU_FLAG_SMALL_FONT	0x01		// small font, including that used for line headers 
#define PMDG_777X_CDU_FLAG_REVERSE		0x02		// character background is highlighted in reverse video
#define PMDG_777X_CDU_FLAG_UNUSED		0x04		// dimmed character color indicating inop/unused entries

// 777X Control Structure
struct PMDG_777X_Control
{
	unsigned int	Event;
	unsigned int	Parameter;
};

// Mouse flags for mouse click simulation

#define MOUSE_FLAG_RIGHTSINGLE		0x80000000
#define MOUSE_FLAG_MIDDLESINGLE		0x40000000
#define MOUSE_FLAG_LEFTSINGLE		0x20000000
#define MOUSE_FLAG_RIGHTDOUBLE		0x10000000
#define MOUSE_FLAG_MIDDLEDOUBLE		0x08000000
#define MOUSE_FLAG_LEFTDOUBLE		0x04000000
#define MOUSE_FLAG_RIGHTDRAG		0x02000000
#define MOUSE_FLAG_MIDDLEDRAG		0x01000000
#define MOUSE_FLAG_LEFTDRAG			0x00800000
#define MOUSE_FLAG_MOVE				0x00400000
#define MOUSE_FLAG_DOWN_REPEAT		0x00200000
#define MOUSE_FLAG_RIGHTRELEASE		0x00080000
#define MOUSE_FLAG_MIDDLERELEASE	0x00040000
#define MOUSE_FLAG_LEFTRELEASE		0x00020000
#define MOUSE_FLAG_WHEEL_FLIP		0x00010000			// invert direction of mouse wheel
#define MOUSE_FLAG_WHEEL_SKIP		0x00008000			// look at next 2 rect for mouse wheel commands
#define MOUSE_FLAG_WHEEL_UP			0x00004000
#define MOUSE_FLAG_WHEEL_DOWN		0x00002000


// Audio control panel selected receiver flags.
// The COMM_ReceiverSwitches[3] variables may contain any combination of these flags.
#define ACP_SEL_RECV_VHFL			0x0001
#define ACP_SEL_RECV_VHFC			0x0002
#define ACP_SEL_RECV_VHFR			0x0004
#define ACP_SEL_RECV_FLT			0x0008
#define ACP_SEL_RECV_CAB			0x0010
#define ACP_SEL_RECV_PA 			0x0020
#define ACP_SEL_RECV_HFL			0x0040
#define ACP_SEL_RECV_HFR			0x0080
#define ACP_SEL_RECV_SAT1			0x0100
#define ACP_SEL_RECV_SAT2			0x0200
#define ACP_SEL_RECV_SPKR			0x0400
#define ACP_SEL_RECV_VOR_ADF		0x0800
#define ACP_SEL_RECV_APP			0x1000



//////////////////////////////////////////////////////////////////
//
//  777 EVENTS 
//
//////////////////////////////////////////////////////////////////

#ifndef THIRD_PARTY_EVENT_ID_MIN
#define THIRD_PARTY_EVENT_ID_MIN	0x00011000			// equals to 69632
#endif


// Overhead - Hydraulic
#define EVT_OH_HYD_DEMAND_ELEC1							(THIRD_PARTY_EVENT_ID_MIN + 35)		
#define EVT_OH_HYD_AIR1									(THIRD_PARTY_EVENT_ID_MIN + 36)		
#define EVT_OH_HYD_AIR2									(THIRD_PARTY_EVENT_ID_MIN + 37)		
#define EVT_OH_HYD_DEMAND_ELEC2							(THIRD_PARTY_EVENT_ID_MIN + 38)		
#define EVT_OH_HYD_ENG1									(THIRD_PARTY_EVENT_ID_MIN + 39)		
#define EVT_OH_HYD_ELEC1								(THIRD_PARTY_EVENT_ID_MIN + 40)		
#define EVT_OH_HYD_ELEC2								(THIRD_PARTY_EVENT_ID_MIN + 41)		
#define EVT_OH_HYD_ENG2									(THIRD_PARTY_EVENT_ID_MIN + 42)		
#define EVT_OH_HYD_RAM_AIR								(THIRD_PARTY_EVENT_ID_MIN + 43)		
#define EVT_OH_HYD_RAM_AIR_COVER						(THIRD_PARTY_EVENT_ID_MIN + 44)	

// Overhead - Electric  
#define	EVT_OH_ELEC_BATTERY_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1)	    
#define	EVT_OH_ELEC_APU_GEN_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 2)		
#define	EVT_OH_ELEC_APU_SEL_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 3)		
#define	EVT_OH_ELEC_BUS_TIE1_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 5)		
#define	EVT_OH_ELEC_BUS_TIE2_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 6)		
#define	EVT_OH_ELEC_GRD_PWR_SEC_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 7)		
#define	EVT_OH_ELEC_GRD_PWR_PRIM_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 8)		
#define	EVT_OH_ELEC_GEN1_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 9)		
#define	EVT_OH_ELEC_GEN2_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 10)		
#define	EVT_OH_ELEC_BACKUP_GEN1_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 11)		
#define	EVT_OH_ELEC_BACKUP_GEN2_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 12)	
#define	EVT_OH_ELEC_DISCONNECT1_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 13)
#define	EVT_OH_ELEC_DISCONNECT1_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 14)
#define	EVT_OH_ELEC_DISCONNECT2_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 15)
#define	EVT_OH_ELEC_DISCONNECT2_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 16)
#define	EVT_OH_ELEC_IFE									(THIRD_PARTY_EVENT_ID_MIN + 17)		
#define	EVT_OH_ELEC_CAB_UTIL							(THIRD_PARTY_EVENT_ID_MIN + 18)
#define	EVT_OH_ELEC_STBY_PWR_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 81)	    
#define	EVT_OH_ELEC_STBY_PWR_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 82)	
#define	EVT_OH_ELEC_TOWING_PWR_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 150)	    
#define	EVT_OH_ELEC_TOWING_PWR_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 151)	
#define	EVT_OH_ELEC_GND_TEST_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 152)	    
#define	EVT_OH_ELEC_GND_TEST_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 153)	

// Overhead - FUEL Panel
#define EVT_OH_FUEL_JETTISON_NOZZLE_L					(THIRD_PARTY_EVENT_ID_MIN + 97)		
#define EVT_OH_FUEL_JETTISON_NOZZLE_L_GUARD				(THIRD_PARTY_EVENT_ID_MIN + 98)		
#define EVT_OH_FUEL_JETTISON_NOZZLE_R					(THIRD_PARTY_EVENT_ID_MIN + 99)		
#define EVT_OH_FUEL_JETTISON_NOZZLE_R_GUARD				(THIRD_PARTY_EVENT_ID_MIN + 100)	
#define EVT_OH_FUEL_TO_REMAIN_ROTATE					(THIRD_PARTY_EVENT_ID_MIN + 101)	
#define EVT_OH_FUEL_TO_REMAIN_PULL						(THIRD_PARTY_EVENT_ID_MIN + 1011)		
#define EVT_OH_FUEL_JETTISON_ARM						(THIRD_PARTY_EVENT_ID_MIN + 102)		
#define EVT_OH_FUEL_PUMP_1_FORWARD						(THIRD_PARTY_EVENT_ID_MIN + 103)		
#define EVT_OH_FUEL_PUMP_2_FORWARD						(THIRD_PARTY_EVENT_ID_MIN + 104)		
#define EVT_OH_FUEL_PUMP_1_AFT							(THIRD_PARTY_EVENT_ID_MIN + 105)		
#define EVT_OH_FUEL_PUMP_2_AFT							(THIRD_PARTY_EVENT_ID_MIN + 106)		
#define EVT_OH_FUEL_CROSSFEED_FORWARD					(THIRD_PARTY_EVENT_ID_MIN + 107)		
#define EVT_OH_FUEL_CROSSFEED_AFT						(THIRD_PARTY_EVENT_ID_MIN + 108)		
#define EVT_OH_FUEL_PUMP_L_CENTER						(THIRD_PARTY_EVENT_ID_MIN + 109)		
#define EVT_OH_FUEL_PUMP_R_CENTER						(THIRD_PARTY_EVENT_ID_MIN + 110)
#define EVT_OH_FUEL_PUMP_AUX							(THIRD_PARTY_EVENT_ID_MIN + 1037)


// Overhead Air
#define EVT_OH_BLEED_ENG_1_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 129)		
#define EVT_OH_BLEED_ENG_2_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 130)		
#define EVT_OH_BLEED_APU_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 131)		
#define EVT_OH_BLEED_ISOLATION_VALVE_SWITCH_L			(THIRD_PARTY_EVENT_ID_MIN + 132)		
#define EVT_OH_BLEED_ISOLATION_VALVE_SWITCH_C			(THIRD_PARTY_EVENT_ID_MIN + 133)		
#define EVT_OH_BLEED_ISOLATION_VALVE_SWITCH_R			(THIRD_PARTY_EVENT_ID_MIN + 134)		
#define EVT_OH_AIRCOND_PACK_SWITCH_L					(THIRD_PARTY_EVENT_ID_MIN + 135)		
#define EVT_OH_AIRCOND_PACK_SWITCH_R					(THIRD_PARTY_EVENT_ID_MIN + 136)	
#define EVT_OH_AIRCOND_TRIM_AIR_SWITCH_L				(THIRD_PARTY_EVENT_ID_MIN + 137)
#define EVT_OH_AIRCOND_TRIM_AIR_SWITCH_R				(THIRD_PARTY_EVENT_ID_MIN + 138)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_FLT_DECK			(THIRD_PARTY_EVENT_ID_MIN + 139)	
#define EVT_OH_AIRCOND_TEMP_SELECTOR_CABIN				(THIRD_PARTY_EVENT_ID_MIN + 140)	
#define EVT_OH_AIRCOND_RESET_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 141)	
#define EVT_OH_AIRCOND_RECIRC_FAN_UPP_SWITCH			(THIRD_PARTY_EVENT_ID_MIN + 142)		
#define EVT_OH_AIRCOND_RECIRC_FAN_LWR_SWITCH			(THIRD_PARTY_EVENT_ID_MIN + 143)	
#define EVT_OH_AIRCOND_EQUIP_COOLING_SWITCH				(THIRD_PARTY_EVENT_ID_MIN + 144)	
#define EVT_OH_AIRCOND_GASPER_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 145)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_CARGO_AFT			(THIRD_PARTY_EVENT_ID_MIN + 148)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_CARGO_BULK			(THIRD_PARTY_EVENT_ID_MIN + 149)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_LWR_CARGO_FWD		(THIRD_PARTY_EVENT_ID_MIN + 1050)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_LWR_CARGO_AFT		(THIRD_PARTY_EVENT_ID_MIN + 1051)
#define EVT_OH_AIRCOND_RECIRC_FANS_SWITCH				(THIRD_PARTY_EVENT_ID_MIN + 1052)	
#define EVT_OH_AIRCOND_TEMP_SELECTOR_MAIN_CARGO_FWD		(THIRD_PARTY_EVENT_ID_MIN + 1054)
#define EVT_OH_AIRCOND_TEMP_SELECTOR_MAIN_CARGO_AFT		(THIRD_PARTY_EVENT_ID_MIN + 1055)
#define EVT_OH_AIRCOND_MAIN_DECK_FLOW_SWITCH			(THIRD_PARTY_EVENT_ID_MIN + 1053)
#define EVT_OH_AIRCOND_ALT_VENT_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 1057)	
#define EVT_OH_AIRCOND_ALT_VENT_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 1111)

// Overhead - Cabin Press
#define EVT_OH_PRESS_VALVE_SWITCH_MANUAL_1				(THIRD_PARTY_EVENT_ID_MIN + 124)
#define EVT_OH_PRESS_VALVE_SWITCH_MANUAL_2				(THIRD_PARTY_EVENT_ID_MIN + 125)
#define EVT_OH_PRESS_VALVE_SWITCHES_MANUAL				(THIRD_PARTY_EVENT_ID_MIN + 1251)
#define EVT_OH_PRESS_LAND_ALT_KNOB_ROTATE				(THIRD_PARTY_EVENT_ID_MIN + 126)  
#define EVT_OH_PRESS_LAND_ALT_KNOB_PULL					(THIRD_PARTY_EVENT_ID_MIN + 1261)  
#define EVT_OH_PRESS_VALVE_SWITCH_1						(THIRD_PARTY_EVENT_ID_MIN + 127)  
#define EVT_OH_PRESS_VALVE_SWITCH_2						(THIRD_PARTY_EVENT_ID_MIN + 128)  

// Overhead - ANTI-ICE
#define EVT_OH_ICE_WINDOW_HEAT_1						(THIRD_PARTY_EVENT_ID_MIN + 45)		
#define EVT_OH_ICE_WINDOW_HEAT_2						(THIRD_PARTY_EVENT_ID_MIN + 46)		
#define EVT_OH_ICE_WINDOW_HEAT_3						(THIRD_PARTY_EVENT_ID_MIN + 47)		
#define EVT_OH_ICE_WINDOW_HEAT_4						(THIRD_PARTY_EVENT_ID_MIN + 48)		
#define EVT_OH_ICE_BU_WINDOW_HEAT_L						(THIRD_PARTY_EVENT_ID_MIN + 77)		
#define EVT_OH_ICE_BU_WINDOW_HEAT_L_GUARD				(THIRD_PARTY_EVENT_ID_MIN + 78)		
#define EVT_OH_ICE_BU_WINDOW_HEAT_R						(THIRD_PARTY_EVENT_ID_MIN + 79)		
#define EVT_OH_ICE_BU_WINDOW_HEAT_R_GUARD				(THIRD_PARTY_EVENT_ID_MIN + 80)		
#define EVT_OH_ICE_WING_ANTIICE							(THIRD_PARTY_EVENT_ID_MIN + 111)		
#define EVT_OH_ICE_ENGINE_ANTIICE_1						(THIRD_PARTY_EVENT_ID_MIN + 112)		
#define EVT_OH_ICE_ENGINE_ANTIICE_2						(THIRD_PARTY_EVENT_ID_MIN + 113)		

// Overhead Lights Panel
#define EVT_OH_LIGHTS_LANDING_L							(THIRD_PARTY_EVENT_ID_MIN + 22)	
#define EVT_OH_LIGHTS_LANDING_NOSE						(THIRD_PARTY_EVENT_ID_MIN + 23)	
#define EVT_OH_LIGHTS_LANDING_R							(THIRD_PARTY_EVENT_ID_MIN + 24)	
#define EVT_OH_LIGHTS_LANDING_LNR						(THIRD_PARTY_EVENT_ID_MIN + 2341)
#define EVT_OH_LIGHTS_STORM								(THIRD_PARTY_EVENT_ID_MIN + 27)	
#define EVT_OH_LIGHTS_BEACON							(THIRD_PARTY_EVENT_ID_MIN + 114)	
#define EVT_OH_LIGHTS_NAV								(THIRD_PARTY_EVENT_ID_MIN + 115)
#define EVT_OH_LIGHTS_LOGO								(THIRD_PARTY_EVENT_ID_MIN + 116)	
#define EVT_OH_LIGHTS_WING								(THIRD_PARTY_EVENT_ID_MIN + 117)	
#define	EVT_OH_LIGHTS_IND_LTS_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 118)
#define EVT_OH_LIGHTS_L_TURNOFF							(THIRD_PARTY_EVENT_ID_MIN + 119)	
#define EVT_OH_LIGHTS_R_TURNOFF							(THIRD_PARTY_EVENT_ID_MIN + 120)
#define EVT_OH_LIGHTS_LR_TURNOFF						(THIRD_PARTY_EVENT_ID_MIN + 1201)	
#define EVT_OH_LIGHTS_TAXI								(THIRD_PARTY_EVENT_ID_MIN + 121)	
#define EVT_OH_LIGHTS_STROBE							(THIRD_PARTY_EVENT_ID_MIN + 122)	
#define EVT_OH_NO_SMOKING_LIGHT_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 29)	
#define EVT_OH_FASTEN_BELTS_LIGHT_SWITCH				(THIRD_PARTY_EVENT_ID_MIN + 30)	
#define EVT_OH_PANEL_LIGHT_CONTROL						(THIRD_PARTY_EVENT_ID_MIN + 25)		
#define EVT_OH_CB_LIGHT_CONTROL							(THIRD_PARTY_EVENT_ID_MIN + 2501)  	
#define EVT_OH_GS_PANEL_LIGHT_CONTROL					(THIRD_PARTY_EVENT_ID_MIN + 21)	
#define EVT_OH_GS_FLOOD_LIGHT_CONTROL					(THIRD_PARTY_EVENT_ID_MIN + 2101)	
#define	EVT_OH_DOME_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 26)
#define EVT_OH_MASTER_BRIGHT_ROTATE						(THIRD_PARTY_EVENT_ID_MIN + 28)	
#define EVT_OH_MASTER_BRIGHT_PUSH						(THIRD_PARTY_EVENT_ID_MIN + 2801)	

// Overhead - APU & Cargo Fire Panel
#define EVT_OH_FIRE_CARGO_ARM_FWD						(THIRD_PARTY_EVENT_ID_MIN + 85)
#define EVT_OH_FIRE_CARGO_ARM_AFT						(THIRD_PARTY_EVENT_ID_MIN + 86)
#define EVT_OH_FIRE_CARGO_DISCH							(THIRD_PARTY_EVENT_ID_MIN + 87)
#define EVT_OH_FIRE_CARGO_DISCH_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 88)
#define EVT_OH_FIRE_OVHT_TEST							(THIRD_PARTY_EVENT_ID_MIN + 89)
#define EVT_OH_FIRE_HANDLE_APU_TOP						(THIRD_PARTY_EVENT_ID_MIN + 84)
#define EVT_OH_FIRE_HANDLE_APU_BOTTOM					(THIRD_PARTY_EVENT_ID_MIN + 8401)
#define EVT_OH_FIRE_UNLOCK_SWITCH_APU					(THIRD_PARTY_EVENT_ID_MIN + 8402)
#define EVT_OH_FIRE_CARGO_ARM_MAIN_DECK					(THIRD_PARTY_EVENT_ID_MIN + 1074)
#define EVT_OH_FIRE_CARGO_DISCH_DEPR					(THIRD_PARTY_EVENT_ID_MIN + 1075)	

// Overhead - Engine control
#define	EVT_OH_EEC_L_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 90)
#define	EVT_OH_EEC_L_GUARD								(THIRD_PARTY_EVENT_ID_MIN + 91)
#define	EVT_OH_EEC_R_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 92)
#define	EVT_OH_EEC_R_GUARD								(THIRD_PARTY_EVENT_ID_MIN + 93)
#define EVT_OH_ENGINE_L_START							(THIRD_PARTY_EVENT_ID_MIN + 94)	
#define EVT_OH_ENGINE_R_START							(THIRD_PARTY_EVENT_ID_MIN + 95)	
#define EVT_OH_ENGINE_AUTOSTART							(THIRD_PARTY_EVENT_ID_MIN + 96)	

// Overhead - Miscellaneous
#define EVT_OH_CAMERA_LTS_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 19)	
#define EVT_OH_WIPER_LEFT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 20)	
#define EVT_OH_EMER_EXIT_LIGHT_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 49)	
#define	EVT_OH_EMER_EXIT_LIGHT_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 50)	
#define	EVT_OH_SERVICE_INTERPHONE_SWITCH				(THIRD_PARTY_EVENT_ID_MIN + 51)
#define	EVT_OH_OXY_PASS_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 52)
#define	EVT_OH_OXY_PASS_GUARD							(THIRD_PARTY_EVENT_ID_MIN + 53)
#define	EVT_OH_OXY_SUPRNMRY_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1076) 
#define	EVT_OH_OXY_SUPRNMRY_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 1077) 
#define	EVT_OH_THRUST_ASYM_COMP							(THIRD_PARTY_EVENT_ID_MIN + 54)
#define	EVT_OH_PRIM_FLT_COMPUTERS						(THIRD_PARTY_EVENT_ID_MIN + 55)
#define	EVT_OH_PRIM_FLT_COMPUTERS_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 56)
#define	EVT_OH_ADIRU_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 59)
#define	EVT_OH_HYD_VLV_PWR_WING_L						(THIRD_PARTY_EVENT_ID_MIN + 60)
#define	EVT_OH_HYD_VLV_PWR_WING_L_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 61)
#define	EVT_OH_HYD_VLV_PWR_WING_C						(THIRD_PARTY_EVENT_ID_MIN + 63)
#define	EVT_OH_HYD_VLV_PWR_WING_C_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 64)
#define	EVT_OH_HYD_VLV_PWR_WING_R						(THIRD_PARTY_EVENT_ID_MIN + 66)
#define	EVT_OH_HYD_VLV_PWR_WING_R_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 67)
#define	EVT_OH_HYD_VLV_PWR_TAIL_L						(THIRD_PARTY_EVENT_ID_MIN + 69)
#define	EVT_OH_HYD_VLV_PWR_TAIL_L_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 70)
#define	EVT_OH_HYD_VLV_PWR_TAIL_C						(THIRD_PARTY_EVENT_ID_MIN + 71)
#define	EVT_OH_HYD_VLV_PWR_TAIL_C_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 72)
#define	EVT_OH_HYD_VLV_PWR_TAIL_R						(THIRD_PARTY_EVENT_ID_MIN + 74)
#define	EVT_OH_HYD_VLV_PWR_TAIL_R_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 75)
#define EVT_OH_WIPER_RIGHT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 123)
#define EVT_OH_CVR_TEST									(THIRD_PARTY_EVENT_ID_MIN + 156)		
#define EVT_OH_CVR_ERASE								(THIRD_PARTY_EVENT_ID_MIN + 157)		
#define EVT_OH_APU_TEST_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 159)	
#define EVT_OH_APU_TEST_SWITCH_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 160)	
#define EVT_OH_EEC_TEST_L_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 161)	
#define EVT_OH_EEC_TEST_L_SWITCH_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 162)	
#define EVT_OH_EEC_TEST_R_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 163)	
#define EVT_OH_EEC_TEST_R_SWITCH_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 164)	
#define EVT_GPWS_RWY_OVRD_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1109)	
#define EVT_GPWS_RWY_OVRD_GUARD							(THIRD_PARTY_EVENT_ID_MIN + 1110)

// Forward Panel - Instrument Source Select
#define EVT_FWD_NAV_SOURCE_L							(THIRD_PARTY_EVENT_ID_MIN + 168)	
#define EVT_FWD_DSPL_CTRL_SOURCE_L						(THIRD_PARTY_EVENT_ID_MIN + 169)	
#define EVT_FWD_AIR_DATA_ATT_SOURCE_L					(THIRD_PARTY_EVENT_ID_MIN + 170)	
#define EVT_FWD_NAV_SOURCE_R							(THIRD_PARTY_EVENT_ID_MIN + 276)	
#define EVT_FWD_DSPL_CTRL_SOURCE_R						(THIRD_PARTY_EVENT_ID_MIN + 277)	
#define EVT_FWD_AIR_DATA_ATT_SOURCE_R					(THIRD_PARTY_EVENT_ID_MIN + 278)
#define EVT_FWD_FMC_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 291)

// Forward Panel - Display Selectors
#define	EVT_DSP_INDB_DSPL_L								(THIRD_PARTY_EVENT_ID_MIN + 315)	
#define	EVT_DSP_INDB_DSPL_R								(THIRD_PARTY_EVENT_ID_MIN + 290)	

// Forward Panel - Heading Reference
#define	EVT_EFIS_HDG_REF_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 313)	
#define	EVT_EFIS_HDG_REF_GUARD							(THIRD_PARTY_EVENT_ID_MIN + 314)	

// Forward Panel - Gear
#define	EVT_GEAR_LEVER									(THIRD_PARTY_EVENT_ID_MIN + 295)
#define	EVT_GEAR_LEVER_UNLOCK							(THIRD_PARTY_EVENT_ID_MIN + 296)
#define EVT_GEAR_ALTN_GEAR_DOWN							(THIRD_PARTY_EVENT_ID_MIN + 293)
#define EVT_GEAR_ALTN_GEAR_DOWN_GUARD					(THIRD_PARTY_EVENT_ID_MIN + 294)

// Forward Panel - GPWS
#define EVT_GPWS_TERR_OVRD_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 297)	
#define EVT_GPWS_TERR_OVRD_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 298)	
#define EVT_GPWS_GEAR_OVRD_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 299)	
#define EVT_GPWS_GEAR_OVRD_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 300)	
#define EVT_GPWS_FLAP_OVRD_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 301)	
#define EVT_GPWS_FLAP_OVRD_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 302)	
#define EVT_GPWS_GS_INHIBIT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 303)

// Forward Panel - Autobrakes
#define	EVT_ABS_AUTOBRAKE_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 292)	

// Forward Panel - ISFD
#define	EVT_ISFD_APP									(THIRD_PARTY_EVENT_ID_MIN + 810)	 	
#define	EVT_ISFD_HP_IN									(THIRD_PARTY_EVENT_ID_MIN + 811)		
#define	EVT_ISFD_PLUS									(THIRD_PARTY_EVENT_ID_MIN + 812)		
#define	EVT_ISFD_MINUS									(THIRD_PARTY_EVENT_ID_MIN + 813)		
#define	EVT_ISFD_ATT_RST								(THIRD_PARTY_EVENT_ID_MIN + 814)		
#define	EVT_ISFD_BARO									(THIRD_PARTY_EVENT_ID_MIN + 815)		
#define	EVT_ISFD_BARO_PUSH								(THIRD_PARTY_EVENT_ID_MIN + 816)	

// Forward Panel - non-ISFD standby instruments
#define	EVT_STANDBY_ASI_KNOB							(THIRD_PARTY_EVENT_ID_MIN + 308)	
#define	EVT_STANDBY_ASI_KNOB_PUSH						(THIRD_PARTY_EVENT_ID_MIN + 3080)	
#define	EVT_STANDBY_ALTIMETER_KNOB						(THIRD_PARTY_EVENT_ID_MIN + 311)	
#define	EVT_STANDBY_ALTIMETER_KNOB_PUSH					(THIRD_PARTY_EVENT_ID_MIN + 3110)	

// Forward Panel - Chronometers
#define EVT_CHRONO_L_CHR								(THIRD_PARTY_EVENT_ID_MIN + 171)	
#define EVT_CHRONO_L_TIME_DATE_SELECT					(THIRD_PARTY_EVENT_ID_MIN + 172)	
#define EVT_CHRONO_L_TIME_DATE_PUSH						(THIRD_PARTY_EVENT_ID_MIN + 1721) 
#define EVT_CHRONO_L_ET									(THIRD_PARTY_EVENT_ID_MIN + 173)	
#define EVT_CHRONO_L_SET								(THIRD_PARTY_EVENT_ID_MIN + 174)	
#define EVT_CHRONO_R_CHR								(THIRD_PARTY_EVENT_ID_MIN + 279)	
#define EVT_CHRONO_R_TIME_DATE_SELECT					(THIRD_PARTY_EVENT_ID_MIN + 280)	
#define EVT_CHRONO_R_TIME_DATE_PUSH						(THIRD_PARTY_EVENT_ID_MIN + 2802) 
#define EVT_CHRONO_R_ET									(THIRD_PARTY_EVENT_ID_MIN + 281)	
#define EVT_CHRONO_R_SET								(THIRD_PARTY_EVENT_ID_MIN + 282)	

// Forward Panel - Left Sidewall
#define EVT_FWD_LEFT_SHOULDER_HEATER					(THIRD_PARTY_EVENT_ID_MIN + 318)	
#define EVT_FWD_LEFT_FOOT_HEATER						(THIRD_PARTY_EVENT_ID_MIN + 319)	
#define EVT_FWD_LEFT_OUTBD_BRIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 320)
#define EVT_FWD_LEFT_INBD_BRIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 321)
#define EVT_FWD_LEFT_INBD_TERR_BRIGHT_CONTROL			(THIRD_PARTY_EVENT_ID_MIN + 3210)
#define EVT_FWD_LEFT_PANEL_LIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 322)	
#define EVT_FWD_LEFT_FLOOD_LIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 3220)	

// Forward Panel - Right Sidewall
#define EVT_FWD_RIGHT_FOOT_HEATER						(THIRD_PARTY_EVENT_ID_MIN + 288)	
#define EVT_FWD_RIGHT_SHOULDER_HEATER					(THIRD_PARTY_EVENT_ID_MIN + 289)	
#define EVT_FWD_RIGHT_PANEL_LIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 285)	
#define EVT_FWD_RIGHT_FLOOD_LIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 2850)	
#define EVT_FWD_RIGHT_INBD_BRIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 286)
#define EVT_FWD_RIGHT_INBD_TERR_BRIGHT_CONTROL			(THIRD_PARTY_EVENT_ID_MIN + 2860)
#define EVT_FWD_RIGHT_OUTBD_BRIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 287)

// Glareshield - EFIS Captain control panel
#define	EVT_EFIS_CPT_MINIMUMS_RADIO_BARO				(THIRD_PARTY_EVENT_ID_MIN + 181)	
#define EVT_EFIS_CPT_FIRST								EVT_EFIS_CPT_MINIMUMS_RADIO_BARO
#define	EVT_EFIS_CPT_MINIMUMS							(THIRD_PARTY_EVENT_ID_MIN + 182)
#define	EVT_EFIS_CPT_MINIMUMS_RST						(THIRD_PARTY_EVENT_ID_MIN + 183)	
#define	EVT_EFIS_CPT_VOR_ADF_SELECTOR_L					(THIRD_PARTY_EVENT_ID_MIN + 184)	
#define	EVT_EFIS_CPT_MODE								(THIRD_PARTY_EVENT_ID_MIN + 185)	
#define	EVT_EFIS_CPT_MODE_CTR							(THIRD_PARTY_EVENT_ID_MIN + 186)	
#define	EVT_EFIS_CPT_RANGE								(THIRD_PARTY_EVENT_ID_MIN + 187)	
#define	EVT_EFIS_CPT_RANGE_TFC							(THIRD_PARTY_EVENT_ID_MIN + 188)	
#define	EVT_EFIS_CPT_VOR_ADF_SELECTOR_R					(THIRD_PARTY_EVENT_ID_MIN + 189)	
#define	EVT_EFIS_CPT_BARO_IN_HPA						(THIRD_PARTY_EVENT_ID_MIN + 190)	
#define	EVT_EFIS_CPT_BARO								(THIRD_PARTY_EVENT_ID_MIN + 191)	
#define	EVT_EFIS_CPT_BARO_STD							(THIRD_PARTY_EVENT_ID_MIN + 192)			
#define	EVT_EFIS_CPT_FPV								(THIRD_PARTY_EVENT_ID_MIN + 193)	
#define	EVT_EFIS_CPT_MTRS								(THIRD_PARTY_EVENT_ID_MIN + 194)	
#define	EVT_EFIS_CPT_WXR								(THIRD_PARTY_EVENT_ID_MIN + 195)	
#define	EVT_EFIS_CPT_STA								(THIRD_PARTY_EVENT_ID_MIN + 196)	
#define	EVT_EFIS_CPT_WPT								(THIRD_PARTY_EVENT_ID_MIN + 197)	
#define	EVT_EFIS_CPT_ARPT								(THIRD_PARTY_EVENT_ID_MIN + 198)	
#define	EVT_EFIS_CPT_DATA								(THIRD_PARTY_EVENT_ID_MIN + 199)	
#define	EVT_EFIS_CPT_POS								(THIRD_PARTY_EVENT_ID_MIN + 200)	
#define	EVT_EFIS_CPT_TERR								(THIRD_PARTY_EVENT_ID_MIN + 201)	
#define EVT_EFIS_CPT_LAST								EVT_EFIS_CPT_TERR

// Glareshield - EFIS F/O control panels
#define	EVT_EFIS_FO_MINIMUMS_RADIO_BARO					(THIRD_PARTY_EVENT_ID_MIN + 248)	
#define EVT_EFIS_FO_FIRST								EVT_EFIS_FO_MINIMUMS_RADIO_BARO
#define	EVT_EFIS_FO_MINIMUMS							(THIRD_PARTY_EVENT_ID_MIN + 249)
#define	EVT_EFIS_FO_MINIMUMS_RST						(THIRD_PARTY_EVENT_ID_MIN + 250)	
#define	EVT_EFIS_FO_VOR_ADF_SELECTOR_L					(THIRD_PARTY_EVENT_ID_MIN + 251)	
#define	EVT_EFIS_FO_MODE								(THIRD_PARTY_EVENT_ID_MIN + 252)	
#define	EVT_EFIS_FO_MODE_CTR							(THIRD_PARTY_EVENT_ID_MIN + 253)	
#define	EVT_EFIS_FO_RANGE								(THIRD_PARTY_EVENT_ID_MIN + 254)	
#define	EVT_EFIS_FO_RANGE_TFC							(THIRD_PARTY_EVENT_ID_MIN + 255)	
#define	EVT_EFIS_FO_VOR_ADF_SELECTOR_R					(THIRD_PARTY_EVENT_ID_MIN + 256)	
#define	EVT_EFIS_FO_BARO_IN_HPA							(THIRD_PARTY_EVENT_ID_MIN + 257)	
#define	EVT_EFIS_FO_BARO								(THIRD_PARTY_EVENT_ID_MIN + 258)	
#define	EVT_EFIS_FO_BARO_STD							(THIRD_PARTY_EVENT_ID_MIN + 259)			
#define	EVT_EFIS_FO_FPV									(THIRD_PARTY_EVENT_ID_MIN + 260)	
#define	EVT_EFIS_FO_MTRS								(THIRD_PARTY_EVENT_ID_MIN + 261)	
#define	EVT_EFIS_FO_WXR									(THIRD_PARTY_EVENT_ID_MIN + 262)	
#define	EVT_EFIS_FO_STA									(THIRD_PARTY_EVENT_ID_MIN + 263)	
#define	EVT_EFIS_FO_WPT									(THIRD_PARTY_EVENT_ID_MIN + 264)	
#define	EVT_EFIS_FO_ARPT								(THIRD_PARTY_EVENT_ID_MIN + 265)	
#define	EVT_EFIS_FO_DATA								(THIRD_PARTY_EVENT_ID_MIN + 2661) 
#define	EVT_EFIS_FO_POS									(THIRD_PARTY_EVENT_ID_MIN + 267)	
#define	EVT_EFIS_FO_TERR								(THIRD_PARTY_EVENT_ID_MIN + 268)	
#define EVT_EFIS_FO_LAST								EVT_EFIS_FO_TERR

// Glareshield - MCP
#define	EVT_MCP_FD_SWITCH_L								(THIRD_PARTY_EVENT_ID_MIN + 202)	
#define	EVT_MCP_AP_L_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 203)
#define	EVT_MCP_AT_ARM_SWITCH_L							(THIRD_PARTY_EVENT_ID_MIN + 204)	
#define	EVT_MCP_AT_ARM_SWITCH_R							(THIRD_PARTY_EVENT_ID_MIN + 205)	
#define	EVT_MCP_CLB_CON_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 206)	
#define	EVT_MCP_AT_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 207)	
#define	EVT_MCP_IAS_MACH_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 208)	
#define	EVT_MCP_SPEED_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 210)
#define	EVT_MCP_SPEED_PUSH_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 2100)
#define	EVT_MCP_LNAV_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 211)
#define	EVT_MCP_VNAV_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 212)	
#define	EVT_MCP_LVL_CHG_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 213)	
#define	EVT_MCP_DISENGAGE_BAR							(THIRD_PARTY_EVENT_ID_MIN + 214)
#define	EVT_MCP_HDG_TRK_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 216)
#define	EVT_MCP_HEADING_PUSH_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 218)	
#define	EVT_MCP_HEADING_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 2180)	
#define	EVT_MCP_BANK_ANGLE_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 2181)	
#define	EVT_MCP_HDG_HOLD_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 219)	
#define	EVT_MCP_VS_FPA_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 220)	
#define	EVT_MCP_VS_SELECTOR								(THIRD_PARTY_EVENT_ID_MIN + 222)
#define	EVT_MCP_VS_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 223)	
#define	EVT_MCP_ALTITUDE_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 2250)	
#define	EVT_MCP_ALTITUDE_PUSH_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 2251)	
#define	EVT_MCP_ALT_INCR_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 225)	
#define	EVT_MCP_ALT_HOLD_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 226)	
#define	EVT_MCP_LOC_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 227)	
#define	EVT_MCP_APP_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 228)	
#define	EVT_MCP_AP_R_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 229)
#define	EVT_MCP_FD_SWITCH_R								(THIRD_PARTY_EVENT_ID_MIN + 230)
#define	EVT_MCP_TOGA_SCREW_L							(THIRD_PARTY_EVENT_ID_MIN + 5001)
#define	EVT_MCP_TOGA_SCREW_R							(THIRD_PARTY_EVENT_ID_MIN + 5002)

// Glareshield - Display Select Panel
#define EVT_DSP_L_INBD_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 231)	
#define EVT_DSP_R_INBD_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 232)	
#define EVT_DSP_LWR_CTR_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 233)	
#define EVT_DSP_ENG_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 234)	
#define EVT_DSP_STAT_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 235)	
#define EVT_DSP_ELEC_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 236)	
#define EVT_DSP_HYD_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 237)	
#define EVT_DSP_FUEL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 238)	
#define EVT_DSP_AIR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 239)	
#define EVT_DSP_DOOR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 240)	
#define EVT_DSP_GEAR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 241)	
#define EVT_DSP_FCTL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 242)	
#define EVT_DSP_CAM_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 243)	
#define EVT_DSP_CHKL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 244)	
#define EVT_DSP_COMM_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 245)	
#define EVT_DSP_NAV_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 246)	
#define EVT_DSP_CANC_RCL_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 247)	

// Glareshield - Master Warning/caution
#define EVT_MASTER_WARNING_RESET_LEFT					(THIRD_PARTY_EVENT_ID_MIN + 177)
#define EVT_MASTER_WARNING_RESET_RIGHT					(THIRD_PARTY_EVENT_ID_MIN + 272)

// Glareshield - Data Link Switches
#define EVT_DATA_LINK_ACPT_LEFT							(THIRD_PARTY_EVENT_ID_MIN + 178)
#define EVT_DATA_LINK_CANC_LEFT							(THIRD_PARTY_EVENT_ID_MIN + 179)
#define EVT_DATA_LINK_RJCT_LEFT							(THIRD_PARTY_EVENT_ID_MIN + 180)
#define EVT_DATA_LINK_ACPT_RIGHT						(THIRD_PARTY_EVENT_ID_MIN + 269)
#define EVT_DATA_LINK_CANC_RIGHT						(THIRD_PARTY_EVENT_ID_MIN + 270)
#define EVT_DATA_LINK_RJCT_RIGHT						(THIRD_PARTY_EVENT_ID_MIN + 271)


// Glareshield - Map/Chart/Worktable Lights, MIC & Clock Switches
#define EVT_CLOCK_L										(THIRD_PARTY_EVENT_ID_MIN + 165)
#define EVT_MAP_LIGHT_L									(THIRD_PARTY_EVENT_ID_MIN + 166)	
#define EVT_MAP_LIGHT_L_PULL							(THIRD_PARTY_EVENT_ID_MIN + 1661)
#define EVT_GLARESHIELD_MIC_L							(THIRD_PARTY_EVENT_ID_MIN + 167)	
#define EVT_GLARESHIELD_MIC_R							(THIRD_PARTY_EVENT_ID_MIN + 273)
#define EVT_MAP_LIGHT_R									(THIRD_PARTY_EVENT_ID_MIN + 274)
#define EVT_MAP_LIGHT_R_PULL							(THIRD_PARTY_EVENT_ID_MIN + 2741)
#define EVT_CLOCK_R										(THIRD_PARTY_EVENT_ID_MIN + 275)
#define EVT_CHART_LIGHT_L								(THIRD_PARTY_EVENT_ID_MIN + 1107)	
#define EVT_CHART_LIGHT_R								(THIRD_PARTY_EVENT_ID_MIN + 1108)	
#define EVT_WORKTABLE_LIGHT_L							(THIRD_PARTY_EVENT_ID_MIN + 1105)	
#define EVT_WORKTABLE_LIGHT_R							(THIRD_PARTY_EVENT_ID_MIN + 1106)	

// Pedestal - Fwd Aisle Stand - CDUs
#define EVT_CDU_L_L1									(THIRD_PARTY_EVENT_ID_MIN +  328)	
#define EVT_CDU_L_L2									(THIRD_PARTY_EVENT_ID_MIN +  329)	
#define EVT_CDU_L_L3									(THIRD_PARTY_EVENT_ID_MIN +  330)	
#define EVT_CDU_L_L4									(THIRD_PARTY_EVENT_ID_MIN +  331)	
#define EVT_CDU_L_L5									(THIRD_PARTY_EVENT_ID_MIN +  332)	
#define EVT_CDU_L_L6									(THIRD_PARTY_EVENT_ID_MIN +  333)	
#define EVT_CDU_L_R1									(THIRD_PARTY_EVENT_ID_MIN +  334)	
#define EVT_CDU_L_R2									(THIRD_PARTY_EVENT_ID_MIN +  335)	
#define EVT_CDU_L_R3									(THIRD_PARTY_EVENT_ID_MIN +  336)	
#define EVT_CDU_L_R4									(THIRD_PARTY_EVENT_ID_MIN +  337)	
#define EVT_CDU_L_R5									(THIRD_PARTY_EVENT_ID_MIN +  338)	
#define EVT_CDU_L_R6									(THIRD_PARTY_EVENT_ID_MIN +  339)
#define EVT_CDU_L_INIT_REF								(THIRD_PARTY_EVENT_ID_MIN +  340)	
#define EVT_CDU_L_RTE									(THIRD_PARTY_EVENT_ID_MIN +  341)	
#define EVT_CDU_L_DEP_ARR								(THIRD_PARTY_EVENT_ID_MIN +  342)	
#define EVT_CDU_L_ALTN									(THIRD_PARTY_EVENT_ID_MIN +  343)	
#define EVT_CDU_L_VNAV									(THIRD_PARTY_EVENT_ID_MIN +  344)	
#define EVT_CDU_L_FIX									(THIRD_PARTY_EVENT_ID_MIN +  345)	
#define EVT_CDU_L_LEGS									(THIRD_PARTY_EVENT_ID_MIN +  346)	
#define EVT_CDU_L_HOLD									(THIRD_PARTY_EVENT_ID_MIN +  347)	
#define EVT_CDU_L_FMCCOMM								(THIRD_PARTY_EVENT_ID_MIN +  3471)	
#define EVT_CDU_L_PROG									(THIRD_PARTY_EVENT_ID_MIN +  348)	
#define EVT_CDU_L_EXEC									(THIRD_PARTY_EVENT_ID_MIN +  349)	
#define EVT_CDU_L_MENU									(THIRD_PARTY_EVENT_ID_MIN +  350)	
#define EVT_CDU_L_NAV_RAD								(THIRD_PARTY_EVENT_ID_MIN +  351)	
#define EVT_CDU_L_PREV_PAGE								(THIRD_PARTY_EVENT_ID_MIN +  352)	
#define EVT_CDU_L_NEXT_PAGE								(THIRD_PARTY_EVENT_ID_MIN +  353)	
#define EVT_CDU_L_1										(THIRD_PARTY_EVENT_ID_MIN +  354)	
#define EVT_CDU_L_2										(THIRD_PARTY_EVENT_ID_MIN +  355)	
#define EVT_CDU_L_3										(THIRD_PARTY_EVENT_ID_MIN +  356)	
#define EVT_CDU_L_4										(THIRD_PARTY_EVENT_ID_MIN +  357)	
#define EVT_CDU_L_5										(THIRD_PARTY_EVENT_ID_MIN +  358)	
#define EVT_CDU_L_6										(THIRD_PARTY_EVENT_ID_MIN +  359)	
#define EVT_CDU_L_7										(THIRD_PARTY_EVENT_ID_MIN +  360)	
#define EVT_CDU_L_8										(THIRD_PARTY_EVENT_ID_MIN +  361)	
#define EVT_CDU_L_9										(THIRD_PARTY_EVENT_ID_MIN +  362)	
#define EVT_CDU_L_DOT									(THIRD_PARTY_EVENT_ID_MIN +  363)	
#define EVT_CDU_L_0										(THIRD_PARTY_EVENT_ID_MIN +  364)	
#define EVT_CDU_L_PLUS_MINUS							(THIRD_PARTY_EVENT_ID_MIN +  365)	
#define EVT_CDU_L_A										(THIRD_PARTY_EVENT_ID_MIN +  366)	
#define EVT_CDU_L_B										(THIRD_PARTY_EVENT_ID_MIN +  367)	
#define EVT_CDU_L_C										(THIRD_PARTY_EVENT_ID_MIN +  368)	
#define EVT_CDU_L_D										(THIRD_PARTY_EVENT_ID_MIN +  369)	
#define EVT_CDU_L_E										(THIRD_PARTY_EVENT_ID_MIN +  370)	
#define EVT_CDU_L_F										(THIRD_PARTY_EVENT_ID_MIN +  371)	
#define EVT_CDU_L_G										(THIRD_PARTY_EVENT_ID_MIN +  372)	
#define EVT_CDU_L_H										(THIRD_PARTY_EVENT_ID_MIN +  373)	
#define EVT_CDU_L_I										(THIRD_PARTY_EVENT_ID_MIN +  374)	
#define EVT_CDU_L_J										(THIRD_PARTY_EVENT_ID_MIN +  375)	
#define EVT_CDU_L_K										(THIRD_PARTY_EVENT_ID_MIN +  376)	
#define EVT_CDU_L_L										(THIRD_PARTY_EVENT_ID_MIN +  377)	
#define EVT_CDU_L_M										(THIRD_PARTY_EVENT_ID_MIN +  378)	
#define EVT_CDU_L_N										(THIRD_PARTY_EVENT_ID_MIN +  379)	
#define EVT_CDU_L_O										(THIRD_PARTY_EVENT_ID_MIN +  380)	
#define EVT_CDU_L_P										(THIRD_PARTY_EVENT_ID_MIN +  381)	
#define EVT_CDU_L_Q										(THIRD_PARTY_EVENT_ID_MIN +  382)	
#define EVT_CDU_L_R										(THIRD_PARTY_EVENT_ID_MIN +  383)	
#define EVT_CDU_L_S										(THIRD_PARTY_EVENT_ID_MIN +  384)	
#define EVT_CDU_L_T										(THIRD_PARTY_EVENT_ID_MIN +  385)	
#define EVT_CDU_L_U										(THIRD_PARTY_EVENT_ID_MIN +  386)	
#define EVT_CDU_L_V										(THIRD_PARTY_EVENT_ID_MIN +  387)	
#define EVT_CDU_L_W										(THIRD_PARTY_EVENT_ID_MIN +  388)	
#define EVT_CDU_L_X										(THIRD_PARTY_EVENT_ID_MIN +  389)	
#define EVT_CDU_L_Y										(THIRD_PARTY_EVENT_ID_MIN +  390)	
#define EVT_CDU_L_Z										(THIRD_PARTY_EVENT_ID_MIN +  391)	
#define EVT_CDU_L_SPACE									(THIRD_PARTY_EVENT_ID_MIN +  392)	
#define EVT_CDU_L_DEL									(THIRD_PARTY_EVENT_ID_MIN +  393)	
#define EVT_CDU_L_SLASH									(THIRD_PARTY_EVENT_ID_MIN +  394)	
#define EVT_CDU_L_CLR									(THIRD_PARTY_EVENT_ID_MIN +  395)	
#define EVT_CDU_L_BRITENESS								(THIRD_PARTY_EVENT_ID_MIN +  400)	

#define EVT_CDU_R_L1									(THIRD_PARTY_EVENT_ID_MIN + 401 )
#define CDU_EVT_OFFSET_R								(EVT_CDU_R_L1-EVT_CDU_L_L1)
#define EVT_CDU_R_L2									(CDU_EVT_OFFSET_R + EVT_CDU_L_L2)
#define EVT_CDU_R_L3									(CDU_EVT_OFFSET_R + EVT_CDU_L_L3)
#define EVT_CDU_R_L4									(CDU_EVT_OFFSET_R + EVT_CDU_L_L4)
#define EVT_CDU_R_L5									(CDU_EVT_OFFSET_R + EVT_CDU_L_L5)
#define EVT_CDU_R_L6									(CDU_EVT_OFFSET_R + EVT_CDU_L_L6)
#define EVT_CDU_R_R1									(CDU_EVT_OFFSET_R + EVT_CDU_L_R1)
#define EVT_CDU_R_R2									(CDU_EVT_OFFSET_R + EVT_CDU_L_R2)
#define EVT_CDU_R_R3									(CDU_EVT_OFFSET_R + EVT_CDU_L_R3)
#define EVT_CDU_R_R4									(CDU_EVT_OFFSET_R + EVT_CDU_L_R4)
#define EVT_CDU_R_R5									(CDU_EVT_OFFSET_R + EVT_CDU_L_R5)
#define EVT_CDU_R_R6									(CDU_EVT_OFFSET_R + EVT_CDU_L_R6)
#define EVT_CDU_R_INIT_REF								(CDU_EVT_OFFSET_R + EVT_CDU_L_INIT_REF)
#define EVT_CDU_R_RTE									(CDU_EVT_OFFSET_R + EVT_CDU_L_RTE)
#define EVT_CDU_R_DEP_ARR								(CDU_EVT_OFFSET_R + EVT_CDU_L_DEP_ARR)
#define EVT_CDU_R_ALTN									(CDU_EVT_OFFSET_R + EVT_CDU_L_ALTN)
#define EVT_CDU_R_VNAV									(CDU_EVT_OFFSET_R + EVT_CDU_L_VNAV)
#define EVT_CDU_R_FIX									(CDU_EVT_OFFSET_R + EVT_CDU_L_FIX)
#define EVT_CDU_R_LEGS									(CDU_EVT_OFFSET_R + EVT_CDU_L_LEGS)
#define EVT_CDU_R_HOLD									(CDU_EVT_OFFSET_R + EVT_CDU_L_HOLD)
#define EVT_CDU_R_FMCCOMM								(THIRD_PARTY_EVENT_ID_MIN + 4201)
#define EVT_CDU_R_PROG									(CDU_EVT_OFFSET_R + EVT_CDU_L_PROG)
#define EVT_CDU_R_EXEC									(CDU_EVT_OFFSET_R + EVT_CDU_L_EXEC)
#define EVT_CDU_R_MENU									(CDU_EVT_OFFSET_R + EVT_CDU_L_MENU)
#define EVT_CDU_R_NAV_RAD								(CDU_EVT_OFFSET_R + EVT_CDU_L_NAV_RAD)
#define EVT_CDU_R_PREV_PAGE								(CDU_EVT_OFFSET_R + EVT_CDU_L_PREV_PAGE)
#define EVT_CDU_R_NEXT_PAGE								(CDU_EVT_OFFSET_R + EVT_CDU_L_NEXT_PAGE)
#define EVT_CDU_R_1										(CDU_EVT_OFFSET_R + EVT_CDU_L_1)
#define EVT_CDU_R_2										(CDU_EVT_OFFSET_R + EVT_CDU_L_2)
#define EVT_CDU_R_3										(CDU_EVT_OFFSET_R + EVT_CDU_L_3)
#define EVT_CDU_R_4										(CDU_EVT_OFFSET_R + EVT_CDU_L_4)
#define EVT_CDU_R_5										(CDU_EVT_OFFSET_R + EVT_CDU_L_5)
#define EVT_CDU_R_6										(CDU_EVT_OFFSET_R + EVT_CDU_L_6)
#define EVT_CDU_R_7										(CDU_EVT_OFFSET_R + EVT_CDU_L_7)
#define EVT_CDU_R_8										(CDU_EVT_OFFSET_R + EVT_CDU_L_8)
#define EVT_CDU_R_9										(CDU_EVT_OFFSET_R + EVT_CDU_L_9)
#define EVT_CDU_R_DOT									(CDU_EVT_OFFSET_R + EVT_CDU_L_DOT)
#define EVT_CDU_R_0										(CDU_EVT_OFFSET_R + EVT_CDU_L_0)
#define EVT_CDU_R_PLUS_MINUS							(CDU_EVT_OFFSET_R + EVT_CDU_L_PLUS_MINUS)
#define EVT_CDU_R_A										(CDU_EVT_OFFSET_R + EVT_CDU_L_A)
#define EVT_CDU_R_B										(CDU_EVT_OFFSET_R + EVT_CDU_L_B)
#define EVT_CDU_R_C										(CDU_EVT_OFFSET_R + EVT_CDU_L_C)
#define EVT_CDU_R_D										(CDU_EVT_OFFSET_R + EVT_CDU_L_D)
#define EVT_CDU_R_E										(CDU_EVT_OFFSET_R + EVT_CDU_L_E)
#define EVT_CDU_R_F										(CDU_EVT_OFFSET_R + EVT_CDU_L_F)
#define EVT_CDU_R_G										(CDU_EVT_OFFSET_R + EVT_CDU_L_G)
#define EVT_CDU_R_H										(CDU_EVT_OFFSET_R + EVT_CDU_L_H)
#define EVT_CDU_R_I										(CDU_EVT_OFFSET_R + EVT_CDU_L_I)
#define EVT_CDU_R_J										(CDU_EVT_OFFSET_R + EVT_CDU_L_J)
#define EVT_CDU_R_K										(CDU_EVT_OFFSET_R + EVT_CDU_L_K)
#define EVT_CDU_R_L										(CDU_EVT_OFFSET_R + EVT_CDU_L_L)
#define EVT_CDU_R_M										(CDU_EVT_OFFSET_R + EVT_CDU_L_M)
#define EVT_CDU_R_N										(CDU_EVT_OFFSET_R + EVT_CDU_L_N)
#define EVT_CDU_R_O										(CDU_EVT_OFFSET_R + EVT_CDU_L_O)
#define EVT_CDU_R_P										(CDU_EVT_OFFSET_R + EVT_CDU_L_P)
#define EVT_CDU_R_Q										(CDU_EVT_OFFSET_R + EVT_CDU_L_Q)
#define EVT_CDU_R_R										(CDU_EVT_OFFSET_R + EVT_CDU_L_R)
#define EVT_CDU_R_S										(CDU_EVT_OFFSET_R + EVT_CDU_L_S)
#define EVT_CDU_R_T										(CDU_EVT_OFFSET_R + EVT_CDU_L_T)
#define EVT_CDU_R_U										(CDU_EVT_OFFSET_R + EVT_CDU_L_U)
#define EVT_CDU_R_V										(CDU_EVT_OFFSET_R + EVT_CDU_L_V)
#define EVT_CDU_R_W										(CDU_EVT_OFFSET_R + EVT_CDU_L_W)
#define EVT_CDU_R_X										(CDU_EVT_OFFSET_R + EVT_CDU_L_X)
#define EVT_CDU_R_Y										(CDU_EVT_OFFSET_R + EVT_CDU_L_Y)
#define EVT_CDU_R_Z										(CDU_EVT_OFFSET_R + EVT_CDU_L_Z)
#define EVT_CDU_R_SPACE									(CDU_EVT_OFFSET_R + EVT_CDU_L_SPACE)
#define EVT_CDU_R_DEL									(CDU_EVT_OFFSET_R + EVT_CDU_L_DEL)
#define EVT_CDU_R_SLASH									(CDU_EVT_OFFSET_R + EVT_CDU_L_SLASH)
#define EVT_CDU_R_CLR									(CDU_EVT_OFFSET_R + EVT_CDU_L_CLR)
#define EVT_CDU_R_BRITENESS								(CDU_EVT_OFFSET_R + EVT_CDU_L_BRITENESS)

#define EVT_CDU_C_L1									(THIRD_PARTY_EVENT_ID_MIN + 653 )
#define CDU_EVT_OFFSET_C								(EVT_CDU_C_L1-EVT_CDU_L_L1)
#define EVT_CDU_C_L2									(CDU_EVT_OFFSET_C + EVT_CDU_L_L2)
#define EVT_CDU_C_L3									(CDU_EVT_OFFSET_C + EVT_CDU_L_L3)
#define EVT_CDU_C_L4									(CDU_EVT_OFFSET_C + EVT_CDU_L_L4)
#define EVT_CDU_C_L5									(CDU_EVT_OFFSET_C + EVT_CDU_L_L5)
#define EVT_CDU_C_L6									(CDU_EVT_OFFSET_C + EVT_CDU_L_L6)
#define EVT_CDU_C_R1									(CDU_EVT_OFFSET_C + EVT_CDU_L_R1)
#define EVT_CDU_C_R2									(CDU_EVT_OFFSET_C + EVT_CDU_L_R2)
#define EVT_CDU_C_R3									(CDU_EVT_OFFSET_C + EVT_CDU_L_R3)
#define EVT_CDU_C_R4									(CDU_EVT_OFFSET_C + EVT_CDU_L_R4)
#define EVT_CDU_C_R5									(CDU_EVT_OFFSET_C + EVT_CDU_L_R5)
#define EVT_CDU_C_R6									(CDU_EVT_OFFSET_C + EVT_CDU_L_R6)
#define EVT_CDU_C_INIT_REF								(CDU_EVT_OFFSET_C + EVT_CDU_L_INIT_REF)
#define EVT_CDU_C_RTE									(CDU_EVT_OFFSET_C + EVT_CDU_L_RTE)
#define EVT_CDU_C_DEP_ARR								(CDU_EVT_OFFSET_C + EVT_CDU_L_DEP_ARR)
#define EVT_CDU_C_ALTN									(CDU_EVT_OFFSET_C + EVT_CDU_L_ALTN)
#define EVT_CDU_C_VNAV									(CDU_EVT_OFFSET_C + EVT_CDU_L_VNAV)
#define EVT_CDU_C_FIX									(CDU_EVT_OFFSET_C + EVT_CDU_L_FIX)
#define EVT_CDU_C_LEGS									(CDU_EVT_OFFSET_C + EVT_CDU_L_LEGS)
#define EVT_CDU_C_HOLD									(CDU_EVT_OFFSET_C + EVT_CDU_L_HOLD)
#define EVT_CDU_C_FMCCOMM								(THIRD_PARTY_EVENT_ID_MIN + 6721)
#define EVT_CDU_C_PROG									(CDU_EVT_OFFSET_C + EVT_CDU_L_PROG)
#define EVT_CDU_C_EXEC									(CDU_EVT_OFFSET_C + EVT_CDU_L_EXEC)
#define EVT_CDU_C_MENU									(CDU_EVT_OFFSET_C + EVT_CDU_L_MENU)
#define EVT_CDU_C_NAV_RAD								(CDU_EVT_OFFSET_C + EVT_CDU_L_NAV_RAD)
#define EVT_CDU_C_PREV_PAGE								(CDU_EVT_OFFSET_C + EVT_CDU_L_PREV_PAGE)
#define EVT_CDU_C_NEXT_PAGE								(CDU_EVT_OFFSET_C + EVT_CDU_L_NEXT_PAGE)
#define EVT_CDU_C_1										(CDU_EVT_OFFSET_C + EVT_CDU_L_1)
#define EVT_CDU_C_2										(CDU_EVT_OFFSET_C + EVT_CDU_L_2)
#define EVT_CDU_C_3										(CDU_EVT_OFFSET_C + EVT_CDU_L_3)
#define EVT_CDU_C_4										(CDU_EVT_OFFSET_C + EVT_CDU_L_4)
#define EVT_CDU_C_5										(CDU_EVT_OFFSET_C + EVT_CDU_L_5)
#define EVT_CDU_C_6										(CDU_EVT_OFFSET_C + EVT_CDU_L_6)
#define EVT_CDU_C_7										(CDU_EVT_OFFSET_C + EVT_CDU_L_7)
#define EVT_CDU_C_8										(CDU_EVT_OFFSET_C + EVT_CDU_L_8)
#define EVT_CDU_C_9										(CDU_EVT_OFFSET_C + EVT_CDU_L_9)
#define EVT_CDU_C_DOT									(CDU_EVT_OFFSET_C + EVT_CDU_L_DOT)
#define EVT_CDU_C_0										(CDU_EVT_OFFSET_C + EVT_CDU_L_0)
#define EVT_CDU_C_PLUS_MINUS							(CDU_EVT_OFFSET_C + EVT_CDU_L_PLUS_MINUS)
#define EVT_CDU_C_A										(CDU_EVT_OFFSET_C + EVT_CDU_L_A)
#define EVT_CDU_C_B										(CDU_EVT_OFFSET_C + EVT_CDU_L_B)
#define EVT_CDU_C_C										(CDU_EVT_OFFSET_C + EVT_CDU_L_C)
#define EVT_CDU_C_D										(CDU_EVT_OFFSET_C + EVT_CDU_L_D)
#define EVT_CDU_C_E										(CDU_EVT_OFFSET_C + EVT_CDU_L_E)
#define EVT_CDU_C_F										(CDU_EVT_OFFSET_C + EVT_CDU_L_F)
#define EVT_CDU_C_G										(CDU_EVT_OFFSET_C + EVT_CDU_L_G)
#define EVT_CDU_C_H										(CDU_EVT_OFFSET_C + EVT_CDU_L_H)
#define EVT_CDU_C_I										(CDU_EVT_OFFSET_C + EVT_CDU_L_I)
#define EVT_CDU_C_J										(CDU_EVT_OFFSET_C + EVT_CDU_L_J)
#define EVT_CDU_C_K										(CDU_EVT_OFFSET_C + EVT_CDU_L_K)
#define EVT_CDU_C_L										(CDU_EVT_OFFSET_C + EVT_CDU_L_L)
#define EVT_CDU_C_M										(CDU_EVT_OFFSET_C + EVT_CDU_L_M)
#define EVT_CDU_C_N										(CDU_EVT_OFFSET_C + EVT_CDU_L_N)
#define EVT_CDU_C_O										(CDU_EVT_OFFSET_C + EVT_CDU_L_O)
#define EVT_CDU_C_P										(CDU_EVT_OFFSET_C + EVT_CDU_L_P)
#define EVT_CDU_C_Q										(CDU_EVT_OFFSET_C + EVT_CDU_L_Q)
#define EVT_CDU_C_R										(CDU_EVT_OFFSET_C + EVT_CDU_L_R)
#define EVT_CDU_C_S										(CDU_EVT_OFFSET_C + EVT_CDU_L_S)
#define EVT_CDU_C_T										(CDU_EVT_OFFSET_C + EVT_CDU_L_T)
#define EVT_CDU_C_U										(CDU_EVT_OFFSET_C + EVT_CDU_L_U)
#define EVT_CDU_C_V										(CDU_EVT_OFFSET_C + EVT_CDU_L_V)
#define EVT_CDU_C_W										(CDU_EVT_OFFSET_C + EVT_CDU_L_W)
#define EVT_CDU_C_X										(CDU_EVT_OFFSET_C + EVT_CDU_L_X)
#define EVT_CDU_C_Y										(CDU_EVT_OFFSET_C + EVT_CDU_L_Y)
#define EVT_CDU_C_Z										(CDU_EVT_OFFSET_C + EVT_CDU_L_Z)
#define EVT_CDU_C_SPACE									(CDU_EVT_OFFSET_C + EVT_CDU_L_SPACE)
#define EVT_CDU_C_DEL									(CDU_EVT_OFFSET_C + EVT_CDU_L_DEL)
#define EVT_CDU_C_SLASH									(CDU_EVT_OFFSET_C + EVT_CDU_L_SLASH)
#define EVT_CDU_C_CLR									(CDU_EVT_OFFSET_C + EVT_CDU_L_CLR)
#define EVT_CDU_C_BRITENESS								(CDU_EVT_OFFSET_C + EVT_CDU_L_BRITENESS)

// Pedestal - Fwd Aisle Stand
#define EVT_PED_DSPL_CTRL_SOURCE_C						(THIRD_PARTY_EVENT_ID_MIN + 478)	
#define EVT_PED_EICAS_EVENT_RCD							(THIRD_PARTY_EVENT_ID_MIN + 479)
#define EVT_PED_UPPER_BRIGHT_CONTROL					(THIRD_PARTY_EVENT_ID_MIN + 480)
#define EVT_PED_LOWER_BRIGHT_CONTROL					(THIRD_PARTY_EVENT_ID_MIN + 481)
#define EVT_PED_LOWER_TERR_BRIGHT_CONTROL				(THIRD_PARTY_EVENT_ID_MIN + 4811)
#define EVT_PED_L_CCD_SIDE								(THIRD_PARTY_EVENT_ID_MIN + 482)
#define EVT_PED_L_CCD_INBD								(THIRD_PARTY_EVENT_ID_MIN + 483)
#define EVT_PED_L_CCD_LWR								(THIRD_PARTY_EVENT_ID_MIN + 484)
#define EVT_PED_R_CCD_LWR								(THIRD_PARTY_EVENT_ID_MIN + 489)
#define EVT_PED_R_CCD_INBD								(THIRD_PARTY_EVENT_ID_MIN + 490)
#define EVT_PED_R_CCD_SIDE								(THIRD_PARTY_EVENT_ID_MIN + 491)

// Pedestal - Control Stand - Fire protection panel
// Engine 1
#define EVT_FIRE_HANDLE_ENGINE_1_TOP					(THIRD_PARTY_EVENT_ID_MIN + 651)
#define EVT_FIRE_HANDLE_ENGINE_1_BOTTOM					(THIRD_PARTY_EVENT_ID_MIN + 6511)
#define EVT_FIRE_UNLOCK_SWITCH_ENGINE_1					(THIRD_PARTY_EVENT_ID_MIN + 6512)
// Engine 2
#define EVT_FIRE_HANDLE_ENGINE_2_TOP					(THIRD_PARTY_EVENT_ID_MIN + 652)
#define EVT_FIRE_HANDLE_ENGINE_2_BOTTOM					(THIRD_PARTY_EVENT_ID_MIN + 6521)
#define EVT_FIRE_UNLOCK_SWITCH_ENGINE_2					(THIRD_PARTY_EVENT_ID_MIN + 6522)

// Pedestal - Control Stand - Flaps
#define EVT_ALTN_FLAPS_ARM								(THIRD_PARTY_EVENT_ID_MIN + 510)
#define EVT_ALTN_FLAPS_ARM_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 511)
#define EVT_ALTN_FLAPS_POS								(THIRD_PARTY_EVENT_ID_MIN + 512)

// Pedestal - Control Stand - Fuel Control
#define EVT_CONTROL_STAND_ENG1_START_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 520)	
#define EVT_CONTROL_STAND_ENG2_START_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 521)	

// Pedestal - Aft Aisle Stand - COMM Panels
#define EVT_COM1_HF_SENSOR_KNOB							(THIRD_PARTY_EVENT_ID_MIN + 525)
#define EVT_COM1_START_RANGE							EVT_COM1_HF_SENSOR_KNOB
#define EVT_COM1_TRANSFER_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 526)
#define EVT_COM1_OUTER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 527)
#define EVT_COM1_INNER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 528)
#define EVT_COM1_VHFL_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 529)
#define EVT_COM1_VHFC_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 530)
#define EVT_COM1_VHFR_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 531)
#define EVT_COM1_PNL_OFF_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 532)
#define EVT_COM1_HFL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 533)
#define EVT_COM1_AM_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 534)
#define EVT_COM1_HFR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 535)
#define EVT_COM1_END_RANGE								EVT_COM1_HFR_SWITCH

#define EVT_COM2_HFR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 792)
#define EVT_COM2_START_RANGE							EVT_COM2_HFR_SWITCH
#define EVT_COM2_AM_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 793)
#define EVT_COM2_HFL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 794)
#define EVT_COM2_PNL_OFF_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 795)
#define EVT_COM2_VHFR_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 796)
#define EVT_COM2_VHFC_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 797)
#define EVT_COM2_VHFL_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 798)
#define EVT_COM2_INNER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 799)
#define EVT_COM2_OUTER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 800)
#define EVT_COM2_TRANSFER_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 801)
#define EVT_COM2_HF_SENSOR_KNOB							(THIRD_PARTY_EVENT_ID_MIN + 802)
#define EVT_COM2_END_RANGE								EVT_COM2_HF_SENSOR_KNOB

#define EVT_COM3_HF_SENSOR_KNOB							(THIRD_PARTY_EVENT_ID_MIN + 596)
#define EVT_COM3_START_RANGE							EVT_COM3_HF_SENSOR_KNOB
#define EVT_COM3_TRANSFER_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 597)
#define EVT_COM3_OUTER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 599)
#define EVT_COM3_INNER_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 598)
#define EVT_COM3_VHFL_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 600)
#define EVT_COM3_VHFC_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 601)
#define EVT_COM3_VHFR_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 602)
#define EVT_COM3_PNL_OFF_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 603)
#define EVT_COM3_HFL_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 604)
#define EVT_COM3_AM_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 605)
#define EVT_COM3_HFR_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 606)
#define EVT_COM3_END_RANGE								EVT_COM3_HFR_SWITCH

// Pedestal - Aft Aisle Stand - ACP Captain
#define EVT_ACP_CAPT_MIC_VHFL							(THIRD_PARTY_EVENT_ID_MIN + 536)
#define EVT_ACP_CAPT_MIC_VHFC							(THIRD_PARTY_EVENT_ID_MIN + 537)
#define EVT_ACP_CAPT_MIC_VHFR							(THIRD_PARTY_EVENT_ID_MIN + 538) 
#define EVT_ACP_CAPT_MIC_FLT							(THIRD_PARTY_EVENT_ID_MIN + 539)
#define EVT_ACP_CAPT_MIC_CAB							(THIRD_PARTY_EVENT_ID_MIN + 540)
#define EVT_ACP_CAPT_MIC_PA								(THIRD_PARTY_EVENT_ID_MIN + 541)
#define EVT_ACP_CAPT_MIC_HFL							(THIRD_PARTY_EVENT_ID_MIN + 555) 
#define EVT_ACP_CAPT_MIC_HFR							(THIRD_PARTY_EVENT_ID_MIN + 556) 
#define EVT_ACP_CAPT_MIC_SAT1							(THIRD_PARTY_EVENT_ID_MIN + 557) 
#define EVT_ACP_CAPT_MIC_SAT2							(THIRD_PARTY_EVENT_ID_MIN + 558) 
#define EVT_ACP_CAPT_REC_VHFL							(THIRD_PARTY_EVENT_ID_MIN + 548)
#define EVT_ACP_CAPT_REC_VHFC							(THIRD_PARTY_EVENT_ID_MIN + 549)
#define EVT_ACP_CAPT_REC_VHFR							(THIRD_PARTY_EVENT_ID_MIN + 550) 
#define EVT_ACP_CAPT_REC_FLT							(THIRD_PARTY_EVENT_ID_MIN + 551)
#define EVT_ACP_CAPT_REC_CAB							(THIRD_PARTY_EVENT_ID_MIN + 552)
#define EVT_ACP_CAPT_REC_PA								(THIRD_PARTY_EVENT_ID_MIN + 553)
#define EVT_ACP_CAPT_REC_HFL							(THIRD_PARTY_EVENT_ID_MIN + 565) 
#define EVT_ACP_CAPT_REC_HFR							(THIRD_PARTY_EVENT_ID_MIN + 566) 
#define EVT_ACP_CAPT_REC_SAT1							(THIRD_PARTY_EVENT_ID_MIN + 567) 
#define EVT_ACP_CAPT_REC_SAT2							(THIRD_PARTY_EVENT_ID_MIN + 568) 
#define EVT_ACP_CAPT_REC_SPKR							(THIRD_PARTY_EVENT_ID_MIN + 564) 
#define EVT_ACP_CAPT_REC_VORADF							(THIRD_PARTY_EVENT_ID_MIN + 571) 
#define EVT_ACP_CAPT_REC_APP							(THIRD_PARTY_EVENT_ID_MIN + 573) 
#define EVT_ACP_CAPT_MIC_INT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 554)
#define EVT_ACP_CAPT_FILTER_SELECTOR					(THIRD_PARTY_EVENT_ID_MIN + 572)
#define EVT_ACP_CAPT_VOR_ADF_SELECTOR					(THIRD_PARTY_EVENT_ID_MIN + 569)
#define EVT_ACP_CAPT_APP_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 575)
#define EVT_ACP_CAPT_FIRST								EVT_ACP_CAPT_MIC_VHFL
#define EVT_ACP_CAPT_LAST								EVT_ACP_CAPT_APP_SELECTOR

// Pedestal - Aft Aisle Stand - ACP F/O
#define EVT_ACP_FO_MIC_VHFL								(THIRD_PARTY_EVENT_ID_MIN + 791)
#define EVT_ACP_FO_MIC_VHFC								(THIRD_PARTY_EVENT_ID_MIN + 790)
#define EVT_ACP_FO_MIC_VHFR								(THIRD_PARTY_EVENT_ID_MIN + 789) 
#define EVT_ACP_FO_MIC_FLT								(THIRD_PARTY_EVENT_ID_MIN + 788)
#define EVT_ACP_FO_MIC_CAB								(THIRD_PARTY_EVENT_ID_MIN + 787)
#define EVT_ACP_FO_MIC_PA								(THIRD_PARTY_EVENT_ID_MIN + 786)
#define EVT_ACP_FO_MIC_HFL								(THIRD_PARTY_EVENT_ID_MIN + 772) 
#define EVT_ACP_FO_MIC_HFR								(THIRD_PARTY_EVENT_ID_MIN + 771) 
#define EVT_ACP_FO_MIC_SAT1								(THIRD_PARTY_EVENT_ID_MIN + 770) 
#define EVT_ACP_FO_MIC_SAT2								(THIRD_PARTY_EVENT_ID_MIN + 769) 
#define EVT_ACP_FO_REC_VHFL								(THIRD_PARTY_EVENT_ID_MIN + 779)
#define EVT_ACP_FO_REC_VHFC								(THIRD_PARTY_EVENT_ID_MIN + 778)
#define EVT_ACP_FO_REC_VHFR								(THIRD_PARTY_EVENT_ID_MIN + 777) 
#define EVT_ACP_FO_REC_FLT								(THIRD_PARTY_EVENT_ID_MIN + 776)
#define EVT_ACP_FO_REC_CAB								(THIRD_PARTY_EVENT_ID_MIN + 775)
#define EVT_ACP_FO_REC_PA								(THIRD_PARTY_EVENT_ID_MIN + 774)
#define EVT_ACP_FO_REC_HFL								(THIRD_PARTY_EVENT_ID_MIN + 762) 
#define EVT_ACP_FO_REC_HFR								(THIRD_PARTY_EVENT_ID_MIN + 761) 
#define EVT_ACP_FO_REC_SAT1								(THIRD_PARTY_EVENT_ID_MIN + 760) 
#define EVT_ACP_FO_REC_SAT2								(THIRD_PARTY_EVENT_ID_MIN + 759) 
#define EVT_ACP_FO_REC_SPKR								(THIRD_PARTY_EVENT_ID_MIN + 763) 
#define EVT_ACP_FO_REC_VORADF							(THIRD_PARTY_EVENT_ID_MIN + 756) 
#define EVT_ACP_FO_REC_APP								(THIRD_PARTY_EVENT_ID_MIN + 754) 
#define EVT_ACP_FO_MIC_INT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 773)
#define EVT_ACP_FO_FILTER_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 755)
#define EVT_ACP_FO_VOR_ADF_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 758)
#define EVT_ACP_FO_APP_SELECTOR							(THIRD_PARTY_EVENT_ID_MIN + 752)
#define EVT_ACP_FO_FIRST								EVT_ACP_FO_APP_SELECTOR
#define EVT_ACP_FO_LAST									EVT_ACP_FO_MIC_VHFL

// Pedestal - Aft Aisle Stand - ACP Observer
#define EVT_ACP_OBS_MIC_VHFL							(THIRD_PARTY_EVENT_ID_MIN + 607)
#define EVT_ACP_OBS_MIC_VHFC							(THIRD_PARTY_EVENT_ID_MIN + 608)
#define EVT_ACP_OBS_MIC_VHFR							(THIRD_PARTY_EVENT_ID_MIN + 609) 
#define EVT_ACP_OBS_MIC_FLT								(THIRD_PARTY_EVENT_ID_MIN + 610)
#define EVT_ACP_OBS_MIC_CAB								(THIRD_PARTY_EVENT_ID_MIN + 611)
#define EVT_ACP_OBS_MIC_PA								(THIRD_PARTY_EVENT_ID_MIN + 612)
#define EVT_ACP_OBS_MIC_HFL								(THIRD_PARTY_EVENT_ID_MIN + 626) 
#define EVT_ACP_OBS_MIC_HFR								(THIRD_PARTY_EVENT_ID_MIN + 627) 
#define EVT_ACP_OBS_MIC_SAT1							(THIRD_PARTY_EVENT_ID_MIN + 628) 
#define EVT_ACP_OBS_MIC_SAT2							(THIRD_PARTY_EVENT_ID_MIN + 629) 
#define EVT_ACP_OBS_REC_VHFL							(THIRD_PARTY_EVENT_ID_MIN + 619)
#define EVT_ACP_OBS_REC_VHFC							(THIRD_PARTY_EVENT_ID_MIN + 620)
#define EVT_ACP_OBS_REC_VHFR							(THIRD_PARTY_EVENT_ID_MIN + 621) 
#define EVT_ACP_OBS_REC_FLT								(THIRD_PARTY_EVENT_ID_MIN + 622)
#define EVT_ACP_OBS_REC_CAB								(THIRD_PARTY_EVENT_ID_MIN + 623)
#define EVT_ACP_OBS_REC_PA								(THIRD_PARTY_EVENT_ID_MIN + 624)
#define EVT_ACP_OBS_REC_HFL								(THIRD_PARTY_EVENT_ID_MIN + 636) 
#define EVT_ACP_OBS_REC_HFR								(THIRD_PARTY_EVENT_ID_MIN + 637) 
#define EVT_ACP_OBS_REC_SAT1							(THIRD_PARTY_EVENT_ID_MIN + 638) 
#define EVT_ACP_OBS_REC_SAT2							(THIRD_PARTY_EVENT_ID_MIN + 639) 
#define EVT_ACP_OBS_REC_SPKR							(THIRD_PARTY_EVENT_ID_MIN + 635) 
#define EVT_ACP_OBS_REC_VORADF							(THIRD_PARTY_EVENT_ID_MIN + 642) 
#define EVT_ACP_OBS_REC_APP								(THIRD_PARTY_EVENT_ID_MIN + 644) 
#define EVT_ACP_OBS_MIC_INT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 625)
#define EVT_ACP_OBS_FILTER_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 643)
#define EVT_ACP_OBS_VOR_ADF_SELECTOR					(THIRD_PARTY_EVENT_ID_MIN + 640)
#define EVT_ACP_OBS_APP_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 646)
#define EVT_ACP_OBS_FIRST								EVT_ACP_OBS_MIC_VHFL
#define EVT_ACP_OBS_LAST								EVT_ACP_OBS_APP_SELECTOR

// Pedestal - Aft Aisle Stand - WX RADAR panel
#define EVT_WXR_L_TFR									(THIRD_PARTY_EVENT_ID_MIN + 576)
#define EVT_WXR_L_WX									(THIRD_PARTY_EVENT_ID_MIN + 577)
#define EVT_WXR_L_WX_T									(THIRD_PARTY_EVENT_ID_MIN + 578)
#define EVT_WXR_L_MAP									(THIRD_PARTY_EVENT_ID_MIN + 579)
#define EVT_WXR_L_GC									(THIRD_PARTY_EVENT_ID_MIN + 580)
#define EVT_WXR_AUTO									(THIRD_PARTY_EVENT_ID_MIN + 583)
#define EVT_WXR_L_R										(THIRD_PARTY_EVENT_ID_MIN + 584)
#define EVT_WXR_TEST									(THIRD_PARTY_EVENT_ID_MIN + 585)
#define EVT_WXR_R_TFR									(THIRD_PARTY_EVENT_ID_MIN + 588)
#define EVT_WXR_R_WX									(THIRD_PARTY_EVENT_ID_MIN + 589)
#define EVT_WXR_R_WX_T									(THIRD_PARTY_EVENT_ID_MIN + 590)
#define EVT_WXR_R_MAP									(THIRD_PARTY_EVENT_ID_MIN + 591)
#define EVT_WXR_R_GC									(THIRD_PARTY_EVENT_ID_MIN + 592)
#define EVT_WXR_L_TILT_CONTROL							(THIRD_PARTY_EVENT_ID_MIN + 581)
#define EVT_WXR_L_GAIN_CONTROL							(THIRD_PARTY_EVENT_ID_MIN + 582)
#define EVT_WXR_R_TILT_CONTROL							(THIRD_PARTY_EVENT_ID_MIN + 586)
#define EVT_WXR_R_GAIN_CONTROL							(THIRD_PARTY_EVENT_ID_MIN + 587)

// Pedestal - Aft Aisle Stand - TCAS panel
#define EVT_TCAS_ALTSOURCE								(THIRD_PARTY_EVENT_ID_MIN + 743)
/* For the new XPDR with ABV/BLW and ABS/REL selectors -----------------------------------*/
#define EVT_TCAS_L_ABV_BLW								(THIRD_PARTY_EVENT_ID_MIN + 7431) 
#define EVT_TCAS_L_ABS_REL								(THIRD_PARTY_EVENT_ID_MIN + 7432)
#define EVT_TCAS_R_ABV_BLW								(THIRD_PARTY_EVENT_ID_MIN + 7433)
#define EVT_TCAS_R_ABS_REL								(THIRD_PARTY_EVENT_ID_MIN + 7434)
/* -------------------------------------------------------------------------------------- */
#define EVT_TCAS_KNOB_L_OUTER							(THIRD_PARTY_EVENT_ID_MIN + 744)
#define EVT_TCAS_KNOB_L_INNER							(THIRD_PARTY_EVENT_ID_MIN + 745)
#define EVT_TCAS_IDENT									(THIRD_PARTY_EVENT_ID_MIN + 746)
#define EVT_TCAS_KNOB_R_OUTER							(THIRD_PARTY_EVENT_ID_MIN + 747)
#define EVT_TCAS_KNOB_R_INNER							(THIRD_PARTY_EVENT_ID_MIN + 748)
#define EVT_TCAS_MODE									(THIRD_PARTY_EVENT_ID_MIN + 749)
#define EVT_TCAS_TEST									(THIRD_PARTY_EVENT_ID_MIN + 7491)
#define EVT_TCAS_XPNDR									(THIRD_PARTY_EVENT_ID_MIN + 751)

// Pedestal - Aft Aisle Stand - CALL Panel 				(Freighter only)
#define EVT_PED_CALL_GND								(THIRD_PARTY_EVENT_ID_MIN + 1078)	
#define EVT_PED_CALL_PANEL_FIRST_SWITCH					EVT_PED_CALL_GND
#define EVT_PED_CALL_CREW_REST							(THIRD_PARTY_EVENT_ID_MIN + 1079)	
#define EVT_PED_CALL_SUPRNMRY							(THIRD_PARTY_EVENT_ID_MIN + 1080)	
#define EVT_PED_CALL_CARGO								(THIRD_PARTY_EVENT_ID_MIN + 1081)	
#define EVT_PED_CALL_CARGO_AUDIO						(THIRD_PARTY_EVENT_ID_MIN + 1082)	
#define EVT_PED_CALL_MAIN_DK_ALERT						(THIRD_PARTY_EVENT_ID_MIN + 1083)	
#define EVT_PED_CALL_PANEL_LAST_SWITCH					EVT_PED_CALL_MAIN_DK_ALERT

// Pedestal - Aft Aisle Stand - Various
#define EVT_PED_OBS_AUDIO_SELECTOR						(THIRD_PARTY_EVENT_ID_MIN + 648)	
#define EVT_FCTL_AILERON_TRIM							(THIRD_PARTY_EVENT_ID_MIN + 727)	
#define EVT_FCTL_RUDDER_TRIM							(THIRD_PARTY_EVENT_ID_MIN + 728)
#define EVT_FCTL_RUDDER_TRIM_CANCEL						(THIRD_PARTY_EVENT_ID_MIN + 729)
#define EVT_PED_FLOOR_LIGHTS							(THIRD_PARTY_EVENT_ID_MIN + 735)
#define EVT_PED_PANEL_LIGHT_CONTROL						(THIRD_PARTY_EVENT_ID_MIN + 736)
#define EVT_PED_FLOOD_LIGHT_CONTROL						(THIRD_PARTY_EVENT_ID_MIN + 737)
#define EVT_PED_EVAC_SWITCH								(THIRD_PARTY_EVENT_ID_MIN + 739)
#define EVT_PED_EVAC_SWITCH_GUARD						(THIRD_PARTY_EVENT_ID_MIN + 740)
#define EVT_PED_EVAC_HORN_SHUTOFF						(THIRD_PARTY_EVENT_ID_MIN + 741)
#define EVT_PED_EVAC_TEST_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 742)

// Pedestal - Control Stand
//
#define EVT_CONTROL_STAND_SPEED_BRAKE_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 498)	
#define EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN		(THIRD_PARTY_EVENT_ID_MIN + 4981)
#define EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_ARM			(THIRD_PARTY_EVENT_ID_MIN + 4982)
#define EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP			(THIRD_PARTY_EVENT_ID_MIN + 4983)
#define EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_50			(THIRD_PARTY_EVENT_ID_MIN + 4984)

#define EVT_CONTROL_STAND_FLAPS_LEVER					(THIRD_PARTY_EVENT_ID_MIN + 507)			
#define EVT_CONTROL_STAND_FLAPS_LEVER_0					(THIRD_PARTY_EVENT_ID_MIN + 5071)
#define EVT_CONTROL_STAND_FLAPS_LEVER_1					(THIRD_PARTY_EVENT_ID_MIN + 5072)
#define EVT_CONTROL_STAND_FLAPS_LEVER_5					(THIRD_PARTY_EVENT_ID_MIN + 5073)
#define EVT_CONTROL_STAND_FLAPS_LEVER_15				(THIRD_PARTY_EVENT_ID_MIN + 5074)
#define EVT_CONTROL_STAND_FLAPS_LEVER_20				(THIRD_PARTY_EVENT_ID_MIN + 5075)
#define EVT_CONTROL_STAND_FLAPS_LEVER_25				(THIRD_PARTY_EVENT_ID_MIN + 5076)
#define EVT_CONTROL_STAND_FLAPS_LEVER_30				(THIRD_PARTY_EVENT_ID_MIN + 5077)

#define EVT_CONTROL_STAND_ALT_PITCH_TRIM_LEVER			(THIRD_PARTY_EVENT_ID_MIN + 496)
#define EVT_CONTROL_STAND_REV_THRUST1_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 499)
#define EVT_CONTROL_STAND_REV_THRUST2_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 503)
#define EVT_CONTROL_STAND_FWD_THRUST1_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 501)
#define EVT_CONTROL_STAND_FWD_THRUST2_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 505)
#define EVT_CONTROL_STAND_AT1_DISENGAGE_SWITCH			(THIRD_PARTY_EVENT_ID_MIN + 502)
#define EVT_CONTROL_STAND_AT2_DISENGAGE_SWITCH			(THIRD_PARTY_EVENT_ID_MIN + 506)
#define EVT_CONTROL_STAND_TOGA1_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 500)
#define EVT_CONTROL_STAND_TOGA2_SWITCH					(THIRD_PARTY_EVENT_ID_MIN + 504)
#define EVT_CONTROL_STAND_PARK_BRAKE_LEVER				(THIRD_PARTY_EVENT_ID_MIN + 515)
#define EVT_CONTROL_STAND_STABCUTOUT_SWITCH_C_GUARD		(THIRD_PARTY_EVENT_ID_MIN + 516)	
#define EVT_CONTROL_STAND_STABCUTOUT_SWITCH_C			(THIRD_PARTY_EVENT_ID_MIN + 517)	
#define EVT_CONTROL_STAND_STABCUTOUT_SWITCH_R_GUARD		(THIRD_PARTY_EVENT_ID_MIN + 518)	
#define EVT_CONTROL_STAND_STABCUTOUT_SWITCH_R			(THIRD_PARTY_EVENT_ID_MIN + 519)

// Oxygen Panels
//
#define	EVT_OXY_TEST_RESET_SWITCH_L						(THIRD_PARTY_EVENT_ID_MIN + 1063)
#define	EVT_OXY_TEST_RESET_SWITCH_R						(THIRD_PARTY_EVENT_ID_MIN + 1066)
#define	EVT_OXY_TEST_RESET_SWITCH_OBS_R					(THIRD_PARTY_EVENT_ID_MIN + 1069)
#define	EVT_OXY_TEST_RESET_SWITCH_OBS_L					(THIRD_PARTY_EVENT_ID_MIN + 1072)
#define	EVT_OXY_NORM_EMER_L								(THIRD_PARTY_EVENT_ID_MIN + 1064)
#define	EVT_OXY_NORM_EMER_R								(THIRD_PARTY_EVENT_ID_MIN + 1067)
#define	EVT_OXY_NORM_EMER_OBS_R							(THIRD_PARTY_EVENT_ID_MIN + 1070)
#define	EVT_OXY_NORM_EMER_OBS_L							(THIRD_PARTY_EVENT_ID_MIN + 1073)

// miscellaneous
//
#define EVT_YOKE_AP_DISC_SWITCH							(THIRD_PARTY_EVENT_ID_MIN + 1084)
#define EVT_CLICKSPOT_ISFD								(THIRD_PARTY_EVENT_ID_MIN + 1101)
#define EVT_CLICKSPOT_STBY_ADI							(THIRD_PARTY_EVENT_ID_MIN + 1102)
#define EVT_CLICKSPOT_STBY_ASI							(THIRD_PARTY_EVENT_ID_MIN + 1103)
#define EVT_CLICKSPOT_STBY_ALTIMETER					(THIRD_PARTY_EVENT_ID_MIN + 1104)
#define EVT_CLICKSPOT_GMCS_ZOOM							(THIRD_PARTY_EVENT_ID_MIN + 1112)

// CA and FO Armrests
#define EVT_CA_ARMREST_LEFT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1006)
#define EVT_CA_ARMREST_RIGHT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1007)
#define EVT_FO_ARMREST_LEFT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1008)
#define EVT_FO_ARMREST_RIGHT_SWITCH						(THIRD_PARTY_EVENT_ID_MIN + 1009)


//
// Custom shortcut special events
//
#define EVT_LDG_LIGHTS_TOGGLE							(THIRD_PARTY_EVENT_ID_MIN + 14000)
#define EVT_TURNOFF_LIGHTS_TOGGLE						(THIRD_PARTY_EVENT_ID_MIN + 14001)
#define EVT_COCKPIT_LIGHTS_TOGGLE						(THIRD_PARTY_EVENT_ID_MIN + 14002)
#define EVT_PANEL_LIGHTS_TOGGLE							(THIRD_PARTY_EVENT_ID_MIN + 14003)
#define EVT_FLOOD_LIGHTS_TOGGLE							(THIRD_PARTY_EVENT_ID_MIN + 14004)

#define EVT_DOOR_1L										(THIRD_PARTY_EVENT_ID_MIN + 14011)
#define EVT_DOOR_1R										(THIRD_PARTY_EVENT_ID_MIN + 14012)
#define EVT_DOOR_2L										(THIRD_PARTY_EVENT_ID_MIN + 14013)
#define EVT_DOOR_2R										(THIRD_PARTY_EVENT_ID_MIN + 14014)
#define EVT_DOOR_3L										(THIRD_PARTY_EVENT_ID_MIN + 14015)
#define EVT_DOOR_3R										(THIRD_PARTY_EVENT_ID_MIN + 14016)
#define EVT_DOOR_4L										(THIRD_PARTY_EVENT_ID_MIN + 14017)
#define EVT_DOOR_4R										(THIRD_PARTY_EVENT_ID_MIN + 14018)
#define EVT_DOOR_5L										(THIRD_PARTY_EVENT_ID_MIN + 14019)
#define EVT_DOOR_5R										(THIRD_PARTY_EVENT_ID_MIN + 14020)
#define EVT_DOOR_CARGO_FWD								(THIRD_PARTY_EVENT_ID_MIN + 14021)
#define EVT_DOOR_CARGO_AFT								(THIRD_PARTY_EVENT_ID_MIN + 14022)
#define EVT_DOOR_CARGO_BULK								(THIRD_PARTY_EVENT_ID_MIN + 14023)
#define EVT_DOOR_CARGO_MAIN								(THIRD_PARTY_EVENT_ID_MIN + 14024)
#define EVT_DOOR_FWD_ACCESS								(THIRD_PARTY_EVENT_ID_MIN + 14025)
#define EVT_DOOR_EE_ACCESS								(THIRD_PARTY_EVENT_ID_MIN + 14026)
#define EVT_AT_ARM_SWITCHES								(THIRD_PARTY_EVENT_ID_MIN + 14027)

//
// Window handle, crank and clipboard events
//
#define EVT_DOOR_WINDOW_CA								(THIRD_PARTY_EVENT_ID_MIN + 1012)
#define EVT_DOOR_WINDOW_FO								(THIRD_PARTY_EVENT_ID_MIN + 1013)
#define EVT_WINDOW_CA_HANDLE							(THIRD_PARTY_EVENT_ID_MIN + 1014)
#define EVT_WINDOW_CA_CLIPBOARD							(THIRD_PARTY_EVENT_ID_MIN + 1015)
#define EVT_WINDOW_FO_HANDLE							(THIRD_PARTY_EVENT_ID_MIN + 1016)
#define EVT_WINDOW_FO_CLIPBOARD							(THIRD_PARTY_EVENT_ID_MIN + 1017)

// MCP Direct Control 
#define EVT_MCP_IAS_SET									(THIRD_PARTY_EVENT_ID_MIN + 14502)	// Sets MCP IAS, if IAS mode is active
#define EVT_MCP_MACH_SET								(THIRD_PARTY_EVENT_ID_MIN + 14503)	// Sets MCP MACH (if active) to parameter*0.001 (e.g. send 780 to set M0.780)
#define EVT_MCP_HDGTRK_SET								(THIRD_PARTY_EVENT_ID_MIN + 14504)	// Sets new heading  or track, commands the shortest turn
#define EVT_MCP_ALT_SET									(THIRD_PARTY_EVENT_ID_MIN + 14505)	
#define EVT_MCP_VS_SET									(THIRD_PARTY_EVENT_ID_MIN + 14506)	// Sets MCP VS (if VS window open) to parameter-10000 (e.g. send 8200 for -1800fpm)
#define EVT_MCP_FPA_SET									(THIRD_PARTY_EVENT_ID_MIN + 14507)	// Sets MCP FPA (if VS window open) to (parameter*0.1-10) (e.g. send 82 for -1.8 degrees)

// Panel system events
#define EVT_CTRL_ACCELERATION_DISABLE					(THIRD_PARTY_EVENT_ID_MIN + 14600)
#define EVT_CTRL_ACCELERATION_ENABLE					(THIRD_PARTY_EVENT_ID_MIN + 14600)
#define EVT_2D_PANEL_OFFSET								20000  // added to events trigger by 2D panel pop-up windows

#endif