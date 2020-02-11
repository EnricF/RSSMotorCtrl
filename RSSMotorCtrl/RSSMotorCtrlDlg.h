
// RSSMotorCtrlDlg.h : header file
//

#pragma once

//EF mod: use only to pass compilation while code editing
#ifndef CV_PI
#define CV_PI 3.14159265359
#endif

//Other variables
#include "globalVar.h"


// CRSSMotorCtrlDlg dialog
class CRSSMotorCtrlDlg : public CDialogEx
{

	public:
		void						InitHomeModulePos(void);
		//////////////////////////////////////////////////////////////////////////////////////
		//									CONTROL THREADS									//
		//////////////////////////////////////////////////////////////////////////////////////

		//Definitions
		#define						THREAD_TPC 5
		#define						TIMER_EXIT 0

		//Variables
		__int64						iFrequency;
		int							iMTPC;

		//Process
		bool						bThreadWActive;
		bool						bThreadWFinished;
		CWinThread *				tWorker;
		double						dWorkerTPC;

		//Secondary
		bool						bThreadWSActive;
		bool						bThreadWSFinished;
		CWinThread *				tSecWorker;
		double						dSecWorkerTPC;

		//ECM
		bool						bThreadECMWActive;
		bool						bThreadECMWFinished;
		CWinThread *				tECMWorker;
		double						dECMWorkerTPC;

		//Interface
		bool						bThreadRIActive;
		bool						bThreadRIFinished;
		CWinThread *				tRefreshInterface;
		double						dRITPC;

		//Functions
		void						InitControlThreads();
		void						FinalizeControlThreads();

		//////////////////////////////////////////////////////////////////////////////////
		//									ETHERCAT SLAVE OBJECTS						//
		//////////////////////////////////////////////////////////////////////////////////

		CecmW						cecmW;		//EtherCAT object including a master and multiple slaves
		unsigned int				slaveActive = 0;//A numerical ID of the selected Slave to control/monitor (default is 0)
		
		//////////////////////////////////////////////////////////////////////////////////
		//									2D Interface								//
		//////////////////////////////////////////////////////////////////////////////////

		void						UpdateTimings();
		void						ShowTiming();
		void						ClearTiming(int id);
		void						ShowParameter(int id);

		//////////////////////////////////////////////////////////////////////////////////
		//						DEVICE DATA		(aka DEVICE PARAMETERS)						//
		//////////////////////////////////////////////////////////////////////////////////

		//Definitions
		#define						DEV_FIELDS		5	//Device Control Fields (TOTAL)
		#define						DEV_ACT			0	//Actual paramater value
		#define						DEV_TARGET		1	//Target parameter value
		#define						DEV_MIN			2	//Minimum reached value
		#define						DEV_MAX			3	//Maximum reached value
		#define						DEV_RANGE		4	//Range from specifications?

		#define						DEV_PARAMS		20	//Device TOTAL Parameters
		#define						DEV_IND			8	//Device Individual Parameters	- exclusive for each drive/motor
		#define						DEV_NONE		0	//None???		
		#define						DEV_POS			1	//Module Position [absolute encoder counts?]
		#define						DEV_VEL			2	//Module Velocity [rev/s?]
		#define						DEV_ACC			3	//Module Acceleration [rev/s^2?]
		#define						DEV_MPOS		4	//Motor Position [relative encoder counts]
		#define						DEV_MVEL		5	//Motor Velocity [rev/s]
		#define						DEV_MACC		6	//Motor Acceleration [rev/s^2]
		#define						DEV_AMP			7	//Module Amplitude???????
				//Extra
		#define						DEV_CUR			8	//Device Module Current?
		#define						DEV_MTEMP		9	//Motor Temperature	[ºC]
		#define						DEV_TEMP		10	//Drive Temperature [ºC]
		#define						DEV_STATUS		15	//Drive Status word
		#define						DEV_CURA		16	//Motor Current phase-A [A]
		#define						DEV_CURB		17	//Motor Current phase-B [A]
		#define						DEV_CURC		18	//Motor Current phase-C [A]
		#define						DEV_BUS_V		19	//Bus voltage [V]	
		#define						DEV_CURERR		19	//Current Error?????

		//EXECUTE MOTION (TYPES)
		#define						DEV_M_STOP		0
		#define						DEV_M_POS		1
		#define						DEV_M_VEL		2			
		#define						DEV_M_ROTOR		3				
		#define						DEV_M_PWM		4
		#define						DEV_M_STEP		5

		//Real parameters - Motor model 400 - IN GLOBAL_VAR.H file
		//#define						MOTOR_VEL_DR_REV_S	400	//Motor velocity dynamic range
		//#define						MOTOR_ACC_DR_REV_S	666	//Motor acceleration dynamic range

		//Variables
		double						dDevData[DEV_PARAMS][DEV_FIELDS];				//Device Data PARAMETERS
		bool						bSetModuleZero;									//Control module zero set
		bool						bSetMotorZero;									//Control motor zero set
		bool						bSetGearRatio;									//Control motor gear ratio set
		bool						bStopMotor;										//Stop Motor
		bool						bMoveMotorVel;									//Move motor
		bool						bHaltMotor;										//Halt Motor
		bool						bResetMotor;									//Reset Motor
		bool						bBrakeMotor;									//Reset Motor
		bool						bBrakeState;									//Reset Motor
		bool						bWriteEEPROM;
		bool						bSetCAN1Id;
		bool						bSetCAN2Id;
		bool						bSetSerial;

		bool						bMotorHalted;									//Motor Status Halted
		bool						bContinuous;

		int							motionMode;
		
		//Functions
		void						InitDevData();									//Initialize device data structs
		void						UpdateValue(int id, double v);					//Update Value
		void						UpdateTargetValue(int id, double v);			//Update Target Value
		
		/*
		* Updates Status Semaphore BMP picture
		* YELLOW at start
		* RED if STATUS_FAULT bit is read as 1
		* GREEN otherwise
		*/
		void						UpdateStatusSemaphore(short *val);

		double						gearFactor;
		unsigned int				moduleConfig;

		bool						bUpdateConfig;

		//////////////////////////////////////////////////////////////////////////////////
		//									RECORD GLOBAL								//
		//////////////////////////////////////////////////////////////////////////////////
		CString						genericFolder	= "C:\\workspace\\Data\\Motor\\%s";	//For all log files over time
		CString						uniqueFolder	= "%s\\%s";							//It has DATE information

		//////////////////////////////////////////////////////////////////////////////////
		//									RECORD DEVICE								//
		//////////////////////////////////////////////////////////////////////////////////
		void						RecordDeviceData();

		/*
		* Sets variables set to their default start values
		*/
		void						InitRecordDeviceData();

		bool						recCurData;		// True if Device recording is selected, false otherwise
		long						recCurSample;	// Iterator counter
		__int64						recCurITime;	// Timestamps from CPU
		CFile						curDataFile;	// The file where Device Data is stored
		
		//////////////////////////////////////////////////////////////////////////////////
		//									RECORD TEMPERATURE							//
		//////////////////////////////////////////////////////////////////////////////////

		#define						TEMPS				4	//¿?
		#define						TEMP_T1				0
		#define						TEMP_T2				1
		#define						TEMP_T3				2
		#define						TEMP_M				3

		bool						bRecordTemp;		// True if Temperature recording is selected, false otherwise
		CFile						fTempInfo;			// The file where Temperature readings are stored
		int							iDataTempRecord;	// iterator counter
		__int64						iTimeStamp;			// Timestamps from CPU

		double						dTemps[TEMPS];		//¿?

		/*
		* Sets variables set to their default start values
		*/
		void						InitRecordTemp();

		void						RecordTemp();

		//////////////////////////////////////////////////////////////////////////////////
		//							      GLOBAL FUNCTIONS								//
		//////////////////////////////////////////////////////////////////////////////////

		#define						PRIORITY1	0
		#define						PRIORITY2	1
		#define						PRIORITY3	2
		#define						PRIORITY4	3

		#define						MAX_PARAMS	255
		double						motorParameters[MAX_PARAMS];
		int							lowPriorityParams;		//1: Set 1 | 2: Set2 

		//ENCODERS dynamic ranges [counts/loop]: Motor and Rotor
		#define ENC_MOTOR_DR	65535.	//16-bits
		#define ENC_MODULE_DR	8192.	//13-bits

		//Auxiliar temperatures
		#define MOTOR_AUX_TEMPS				7
		#define MOTOR_AUX_TEMP_FRE			0
		#define MOTOR_AUX_TEMP_MICRO_IN		1
		#define MOTOR_AUX_TEMP_MICRO_CO		2
		#define MOTOR_AUX_TEMP_DA			3
		#define MOTOR_AUX_TEMP_INTERIOR		4
		#define MOTOR_AUX_TEMP_TAPA			5
		#define MOTOR_AUX_TEMP_IND			6

		//double						motorAddTemps[MOTOR_AUX_TEMPS];
		//bool						bMotorAddTemps;

		//Estabilització de la temperatura: Temperatures en 1 hora (3600s)
		#define						STABLE_TIME_PERIOD_S	3600//Temperature stabilization period [s] (default: 3600)
		#define						TEMPERATURE_MAX_DIFF	2.0	//Allowed temperature typical deviation [ªC] 
		int							homeModulePos_enc;			//Module position at start, saved as HOME variable in [enc counts] - Module (outter)

		void						GetDevices();
		void						GetDefaultDeviceParameters();
		void						ExecuteCommands();
		void						ExecuteMotion();
		/*
		* Writes readings from PD to motorParameters[][] matrix
		*/
		void						GetDeviceParameters();
		/*
		* Updates Interface Controls by calling function ShowUpdate() once for each selected field
		* @param pObject	A pointer of class CRSSMotorCtrlDlg
		*/
		void						UpdateInterfaceControls();
		/*
		* Writes readings from motorParameters[][] matrix to EDITBOXES in GUI
		*/
		void						UpdateInterfaceParameters();
		
		//////////////////////////////////////////////////////////////////////////////////
		//									LOOP CONTROL								//
		//////////////////////////////////////////////////////////////////////////////////

		#define						LOOP_STATE_INITIAL			0
		#define						LOOP_STATE_GO_UPPER			1
		#define						LOOP_STATE_GO_INITIAL_FU	2
		#define						LOOP_STATE_GO_INITIAL_FL	3
		#define						LOOP_STATE_GO_LOWER			4
		#define						LOOP_STATE_DELAY_FU			5
		#define						LOOP_STATE_DELAY_FL			6

		bool						bLoopStart;

	//Position
		//EDIT BOXES (GUI) - Unit conversion is done by SetLoopValue() function
		float						fLoopInitial_EB_enc;		//[encoder counts] - Motor(inner)
		float						fLoopUpper_EB_enc;			//[encoder counts] - Motor(inner)
		float						fLoopLower_EB_enc;			//[encoder counts] - Motor(inner)
		//INTEGER VALUES (EtherCAT) --> Drive&Motor!
		int							iLoopInitial_enc;			//[encoder counts] - Motor(inner)
		int							iLoopUpper_enc;				//[encoder counts] - Motor(inner)
		int							iLoopLower_enc;				//[encoder counts] - Motor(inner)
	//Velocity, Acc, Dec - Unit conversion is done by other means, from Module to Motor frame
		float						fModuleLoopMaxVel_EB;		//in GUI units		- Module(outter)
		float						fModuleLoopMaxAcc_EB;		//in GUI units		- Module(outter)
		float						fLoopMaxVel_enc_s;			//[rev/s]			- Motor(inner)
		float						fLoopMaxAcc_enc_s2;			//[rev/s]			- Motor(inner))
	//Delay			   		
		int							iLoopDelay;
	//State
		int							iLoopState;
	//LOOP CONTROL: INTEGER VALUES (EtherCAT) --> Drive&Motor!
		int							iLoopTargetPos_enc;			//EtherCAT: [encoder counts] - Motor(inner)
		int							iLoopActPos_enc;			//EtherCAT: [encoder counts] - Motor(inner
	//Margin
		#define						LOOP_MIN_ERROR_ENC			512 //[encoder counts] - Motor(inner)

		//¿¿¿¿¿?????
		int							iLoopTargetPosLast;//What is this?

		

		bool						bSendLoopCommand;

		bool						bSendRamp;

		int							iDelayCounter;

		/*
		* Initializes variables and EditBoxes related to "RAMP" GroupBox
		*/
		//void						InitLoopSM();
		void						InitRampGB();

		/*
		* A Finite state machine switch() function. It controls RAMP motion (initial, upper, lower, upper, lower, ...)
		*/
		bool						LoopSM();
		//bool						LoopSM(double modulePos);//old, its param was never used (why?)

		/*
		* 		To know if the user has changed GUI values on EditBoxes, work in progress.
		*		It will send new PROFILER parameters if changed (it doesn't set new POSITIONS yet!)
		*/
		void						UpdateLoopValues();

		void						ExecuteRampMotionControl();//RAMP mode
		//void						setLoopInitialValue();

		/*		Takes a value from an edit box (POSITION variables only!) and sets its associated variable in [encoder counts] units
		*		It is able to read on different units selection and do their conversion
		*		It multiplies internal value by gear factor, so GUI works with Module but control works with 
		*
		*		@param	id_editbox		EditBox Numerical ID
		*		@param	var				Pointer to the variable to be written (POSITION variables only!)
		*/
		void						SetLoopValue(int id_editbox, float * var);


		//////////////////////////////////////////////////////////////////////////////////
		//							     VELOCITY CONTROL								//
		//////////////////////////////////////////////////////////////////////////////////

		bool						velRunning;
		int							velTargetRPM;// Velocity Target in [rpm]	- Motor (inner)
		double						velTargetVel;// Velocity Target in [rev/s]	- Motor (inner)
		double						velTargetAcc;// Velocity Target in [rev/s^2]- Motor (inner)
		int							velTargetAccRPM;	// 'Profile Velocity' mode Acceleration in [rpm]

		int							velTargetVelValue;	// 'Profile Velocity' mode Velocity in [rpm] with an offset of 7500
		int							velTargetAccValue;	// 'Profile Velocity' mode Acceleration in [rpm]


		void						InitVelMotion();
		void						UpdateVelMotionValues();
		void						ExecuteVelMotionControl();//VEL mode
		void						UpdateVelMotionInterface();

		/*
		*	Gets Profiler limits from User (RAMP group box) and configures drive profiler
		*/
		void						UpdateProfilerLimits();



		//////////////////////////////////////////////////////////////////////////////////
		//									COMMANDS									//
		//////////////////////////////////////////////////////////////////////////////////

		//void						SetModuleZero();
		//void						SetRotorZero();
		void						SetGearRatio();
		//void						SetSerial();

		void MapErrorCodes(std::map<uint32_t, CString> *errCodes);
		void ShowErrorCodeDescription(int *LastError, std::map<uint32_t, CString> *errCodes);

	// Construction
	public:
		CRSSMotorCtrlDlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_RSSMOTORCTRL_DIALOG };
	#endif

		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedButtonRampSend();
		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedButtonRampStartloop();
		afx_msg void OnBnClickedButtonVelSetVelRpm();
		//afx_msg void OnBnClickedButtonVelSetVelAcc2();
		afx_msg void OnBnClickedButtonVelStart();

		/* 
		* Initializes main LOG object and allows to CecmW class to use it
		*/
		void InitLog();

		/*Does units conversion in Ramp GroupBox EditBoxes 
		*(internal variables are updated by other functions that read those editboxes)
		* @param oldUnits	An integer value of old units selection
		* @param newUnits	An integer value of new units selection
		* @param val		A double pointer to a field (editbox) value
		*/
		void CRSSMotorCtrlDlg::convertUnits(int oldUnits, int newUnits, double *val);

		//Sliders
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);		
		CSliderCtrl scVelocity;	// Velocity slider inside Velocity Motion group							
		CSliderCtrl scAcceleration;// Acceleration control slider in Velocity Motion group
		afx_msg void OnBnClickedButtonVelSetVel();
		//afx_msg void OnBnClickedStaticRamp3();
		/*
		* Device Data record file header openning/closing and header rows writing
		*/
		afx_msg void OnBnClickedButtonRecData();

		//Combo Boxes
		afx_msg void LoadComboBox();
		// Units selection in "Ramp motion - Upper Position" field
		CComboBox cmbRampUpUnitsSel;
		afx_msg void OnCbnSelchangeComboRampUpUnits();
		enum cmbUnits {	DEGREES, COUNTS, RADIANS };
		// Control for 'Select Slave' ComboBox (Advanced)
		CComboBox cmbSlaveSel;
		afx_msg void OnCbnSelchangeComboAdvSelectslave();


		// Picture "Semaphore" to shos STATUS register. YELLOW at start, RED if STATUS_FAULT_BIT == 1, GREEN otherwise
		CBitmap picStatusSemaphore;
		afx_msg void OnBnClickedButtonDevFaultReset();
		afx_msg void OnBnClickedButtonVelSetVelAcc();
		afx_msg void OnBnClickedCancel();

		/*
		* Temperature record file header openning/closing and header rows writing
		*/
		afx_msg void OnBnClickedButtonRecT();
		afx_msg void OnBnClickedButtonDevBrake();

		/*
		* Temperature record file header openning/closing and header rows writing
		*/

		// Control for 'Thermal Analysis' CheckBox selection
		CButton cbThermalAnalysis;
		// To know if CheckBox is selected or not
		BOOL cbThermalAnalysisBool;
		afx_msg void OnBnClickedCheckThermalanalysis();
		//int ThermalAnalysisMaxDuration_sec;		//default: 5 hours máx [seconds]
		int64_t ThermalAnalysisTimeStart;	//TTAG start Thermal analysis[seconds]
		int64_t ThermalAnalysisTimeNow;		//TTAG now Thermal analysis[seconds]
		int64_t ThermalAnalysisTimePrev;	//TTAG of previous Temperature recording [seconds]
		int64_t ThermalAnalysisFreq;		//Frequency (clock) value for Thermal analysis [Hz?]
		void CheckThermalAnalysis(void);
};
