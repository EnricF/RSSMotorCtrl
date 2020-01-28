
// RSSMotorCtrlDlg.h : header file
//

#pragma once

#include "ecmTest.h" //Add class header

// CRSSMotorCtrlDlg dialog
class CRSSMotorCtrlDlg : public CDialogEx
{

	public:

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

		CecmTest					cecmTest;//EtherCAT slave


		//////////////////////////////////////////////////////////////////////////////////
		//									2D Interface								//
		//////////////////////////////////////////////////////////////////////////////////

		void						UpdateTimings();
		void						ShowTiming();
		void						ClearTiming(int id);
		void						ShowParameter(int id);

		//////////////////////////////////////////////////////////////////////////////////
		//						DEVICE DATA		(aka PARAMETERS)						//
		//////////////////////////////////////////////////////////////////////////////////

		//Definitions
		#define						DEV_FIELDS		5								//Device Control Fields
		#define						DEV_ACT			0								//Actual paramater value
		#define						DEV_TARGET		1								//Target parameter value
		#define						DEV_MIN			2								//Actual Min paramenter value
		#define						DEV_MAX			3								//Actual Max parameter value
		#define						DEV_RANGE		4								//Max parameter range value

		#define						DEV_PARAMS		20								//Device Parameters
		#define						DEV_IND			8								//Device Individual Parameters	- exclusive for each drive/motor
		#define						DEV_NONE		0								//Device Module None			
		#define						DEV_POS			1								//Device Module Position
		#define						DEV_VEL			2								//Device Module Velocity
		#define						DEV_ACC			3								//Device Module Acceleration
		#define						DEV_MPOS		4								//Device Motor Position
		#define						DEV_MVEL		5								//Device Motor Velocity
		#define						DEV_MACC		6								//Device Motor Acceleration
		#define						DEV_AMP			7								//Device Module Amplitude
		#define						DEV_CUR			8								//Device Module Current			- Common to all drives/motors
		#define						DEV_MTEMP		9								//Device Motor Temperature	
		#define						DEV_TPC			9								//Device Module TPC cycle
		//#define						DEV_TPC_SEC		10								//Device Module TPC cycle secondary thread
		#define						DEV_TEMP		10								//Device Module Temperature
		#define						DEV_CTRL		11								//Device Module Control Parameters
		#define						DEV_CAN			12								//Device Module CAN errors
		#define						DEV_DELTAVEL	12								//Device Module CAN errors
		#define						DEV_PARAM1		13								//Device Module CAN errors
		#define						DEV_PARAM2		14								//Device Module CAN errors
		#define						DEV_PARAM3		15								//Device Module CAN errors
		#define						DEV_STATUS		15								//Device Module Status word
		#define						DEV_CURA		16								//Device Module CAN errors
		#define						DEV_CURB		17								//Device Module CAN errors
		#define						DEV_CURC		18								//Device Module CAN errors
		#define						DEV_CURERR		19								//Current Error

		#define						DEV_M_STOP		0
		#define						DEV_M_POS		1
		#define						DEV_M_VEL		2			
		#define						DEV_M_ROTOR		3				
		#define						DEV_M_PWM		4
		#define						DEV_M_STEP		5

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


		double						gearFactor;
		unsigned int				moduleConfig;

		bool						bUpdateConfig;

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
		#define ENC_MOTOR_DR 65535.	//16-bits
		#define ENC_ROTOR_DR 8192.	//13-bits

		//Auxiliar temperatures
		#define MOTOR_AUX_TEMPS				7
		#define MOTOR_AUX_TEMP_FRE			0
		#define MOTOR_AUX_TEMP_MICRO_IN		1
		#define MOTOR_AUX_TEMP_MICRO_CO		2
		#define MOTOR_AUX_TEMP_DA			3
		#define MOTOR_AUX_TEMP_INTERIOR		4
		#define MOTOR_AUX_TEMP_TAPA			5
		#define MOTOR_AUX_TEMP_IND			6

		double						motorAddTemps[MOTOR_AUX_TEMPS];
		bool						bMotorAddTemps;

		//Estabilització de la temperatura: Temperatures en 1 hora (3600s)
		#define						STABLE_TEMPS 11
		#define						STABLE_TIME	360
		double						dTBuffer[STABLE_TEMPS][STABLE_TIME];
		double						dTInc[STABLE_TEMPS];
		int							iTIdx;
		double						dMaxIncTemp;
		double						dAmbIncTemp;

		void						GetDevices();
		void						GetDefaultDeviceParameters();
		void						ExecuteCommands();
		void						ExecuteMotion();
		/*
		* Writes readings from PD to motorParameters[][] matrix
		*/
		void						GetDeviceParameters();
		/*
		* Writes readings from motorParameters[][] matrix to EDITBOXES in GUI
		*/
		void						UpdateInterfaceParameters();
		void						RecordDeviceData();
		void						InitRecordDeviceData();

		bool						recCurData;										//Indicates current data recording
		long						recCurSample;
		__int64						recCurITime;
		CFile						curDataFile;

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
		//EDIT BOXES
		float						fLoopInitial;	//[absolute encoder counts]
		float						fLoopUpper;		//[absolute encoder counts]
		float						fLoopLower;		//[absolute encoder counts]
		int							iLoopDelay;
		int							iLoopState;
		

		
		#define						LOOP_MIN_ERROR				0.0017 //[rad]
		#define						LOOP_MIN_ERROR_ENC			655 //[encoder counts]

		float						fLoopPos;		//[absolute radians]
		int							iLoopPos;		//[encoder absolute position]
		float						fLoopTargetPos;	//[absolute radians]
		int							iLoopTargetPos;	//[encoder absolute position]
		float						fLoopMaxVel;
		float						fLoopMaxAcc;
		
		int							iLoopTargetPosLast;
		bool						bSendLoopCommand;

		bool						bSendRamp;

		int							iDelayCounter;

		/*
		* Initializes variables and EditBoxes related to "RAMP" GroupBox
		*/
		void						InitLoopSM();

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

		/*
		* 		Takes a value from and edit box and sets its associated variable
		*		It is able to read on different units selection
		*		@param	id_editbox		EditBox Numerical ID
		*		@param	var				Pointer to the variable to be written
		*/
		void						setLoopValue(int id_editbox, float * var);


		//////////////////////////////////////////////////////////////////////////////////
		//							     VELOCITY CONTROL								//
		//////////////////////////////////////////////////////////////////////////////////

		bool						velRunning;
		int							velTargetRPM;// Velocity Target in [rpm]
		double						velTargetVel;// Velocity Target in [rev/s]
		double						velTargetAcc;

		int							velTargetVelValue; // Velocity Target in [rpm] with and offset of 7500
		int							velTargetAccValue;

		void						InitVelMotion();
		void						UpdateVelMotionValues();
		void						ExecuteVelMotionControl();//VEL mode
		void						UpdateVelMotionInterface();



		//////////////////////////////////////////////////////////////////////////////////
		//									COMMANDS									//
		//////////////////////////////////////////////////////////////////////////////////

		//void						SetModuleZero();
		//void						SetRotorZero();
		void						SetGearRatio();
		//void						SetSerial();


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
		afx_msg void OnBnClickedButtonVelSetVelAcc2();
		afx_msg void OnBnClickedButtonVelStart();
		afx_msg void OnBnClickedButton1();

		//Sliders
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);		
		CSliderCtrl scVelocity;	// Velocity slider inside Velocity Motion group							
		CSliderCtrl scAcceleration;// Acceleration control slider in Velocity Motion group
		afx_msg void OnBnClickedButtonVelSetVel();
		afx_msg void OnBnClickedStaticRamp3();
		afx_msg void OnBnClickedButtonRecData();

		// Units selection in "Ramp motion - Upper Position" field
		CComboBox cmbRampUpUnitsSel;
		afx_msg void OnCbnSelchangeComboRampUpUnits();
		afx_msg void LoadComboBox();
		enum cmbUnits {	COUNTS, RADIANS	};
};
