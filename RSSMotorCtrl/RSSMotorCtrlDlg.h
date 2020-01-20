
// RSSMotorCtrlDlg.h : header file
//

#pragma once


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

		//Interface
		bool						bThreadRIActive;
		bool						bThreadRIFinished;
		CWinThread *				tRefreshInterface;
		double						dRITPC;

		//Functions
		void						InitControlThreads();
		void						FinalizeControlThreads();

		//////////////////////////////////////////////////////////////////////////////////
		//									DEVICE DATA									//
		//////////////////////////////////////////////////////////////////////////////////

			//Definitions
		#define						DEV_FIELDS		5								//Device Control Fields
		#define						DEV_ACT			0								//Actual paramater value
		#define						DEV_TARGET		1								//Target parameter value
		#define						DEV_MIN			2								//Actual Min paramenter value
		#define						DEV_MAX			3								//Actual Max parameter value
		#define						DEV_RANGE		4								//Max parameter range value

		#define						DEV_PARAMS		20								//Device Parameters
		#define						DEV_IND			8								//Device Individual Parameters
		#define						DEV_NONE		0								//Device Module None			
		#define						DEV_POS			1								//Device Module Position
		#define						DEV_VEL			2								//Device Module Velocity
		#define						DEV_ACC			3								//Device Module Acceleration
		#define						DEV_MPOS		4								//Device Motor Position
		#define						DEV_MVEL		5								//Device Motor Velocity
		#define						DEV_MACC		6								//Device Motor Acceleration
		#define						DEV_AMP			7								//Device Module Temperature
		#define						DEV_CUR			8								//Device Module Current
		#define						DEV_TPC			9								//Device Module TPC cycle
		#define						DEV_TPC_SEC		10								//Device Module TPC cycle secondary thread
		#define						DEV_TEMP		10								//Device Module Amplitude
		#define						DEV_CTRL		11								//Device Module Control Parameters
		#define						DEV_CAN			12								//Device Module CAN errors
		#define						DEV_DELTAVEL	12								//Device Module CAN errors
		#define						DEV_PARAM1		13								//Device Module CAN errors
		#define						DEV_PARAM2		14								//Device Module CAN errors
		#define						DEV_PARAM3		15								//Device Module CAN errors
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
		double						dDevData[DEV_PARAMS][DEV_FIELDS];				//Device Data
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
		float						fLoopInitial;
		float						fLoopUpper;
		float						fLoopLower;
		int							iLoopDelay;
		int							iLoopState;
		
		int							iLoopTargetPos;
		
		#define						LOOP_MIN_ERROR				0.0017

		float						fLoopPos;
		float						fLoopTargetPos;
		float						fLoopMaxVel;
		float						fLoopMaxAcc;
		
		int							iLoopTargetPosLast;
		bool						bSendLoopCommand;

		bool						bSendRamp;

		int							iDelayCounter;

		void						InitLoopSM();
		bool						LoopSM(double modulePos);
		void						UpdateLoopValues();

		void						ExecuteRampMotionControl();
		

		void						GetDevices();
		void						GetDefaultDeviceParameters();
		void						ExecuteCommands();
		void						ExecuteMotion();
		void						GetDeviceParameters();
		void						GetSecondaryDeviceParameters();
		void						UpdateInterfaceParameters();
		void						RecordDeviceData();
		void						InitRecordDeviceData();



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
		afx_msg void OnStnClickedStaticRampUpperpos();
		afx_msg void OnBnClickedButtonRampSend();
};
