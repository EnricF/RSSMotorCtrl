
// RSSMotorCtrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "RSSMotorCtrl.h"
#include "RSSMotorCtrlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//EF mod: use only to pass compilation while code editing
#ifndef CV_PI
#define CV_PI 3.1415
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRSSMotorCtrlDlg dialog



CRSSMotorCtrlDlg::CRSSMotorCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RSSMOTORCTRL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRSSMotorCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRSSMotorCtrlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RAMP_SEND, &CRSSMotorCtrlDlg::OnBnClickedButtonRampSend)
END_MESSAGE_MAP()



//Interface Thread
UINT ThreadInterface(LPVOID pParam)
{
	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;
	
	__int64 initial, final;
	CString szInfo;

	pObject->bThreadRIActive = true;

	while (pObject->bThreadRIActive)
	{
		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&final);

		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&initial);

		//Show Timings to interface
		/*pObject->ShowTiming();

		//Update Interface Control values
		pObject->ShowParameter(DEV_POS);
		pObject->ShowParameter(DEV_VEL);
		pObject->ShowParameter(DEV_ACC);
		pObject->ShowParameter(DEV_MPOS);
		pObject->ShowParameter(DEV_MVEL);
		pObject->ShowParameter(DEV_MACC);
		pObject->ShowParameter(DEV_TEMP);
		pObject->ShowParameter(DEV_CUR);
		pObject->ShowParameter(DEV_TPC);
		pObject->ShowParameter(DEV_AMP);
		pObject->ShowParameter(DEV_CTRL);
		pObject->ShowParameter(DEV_CAN);
		pObject->ShowParameter(DEV_PARAM1);
		pObject->ShowParameter(DEV_PARAM2);
		pObject->ShowParameter(DEV_PARAM3);*/
	}

	//Finish Thread
	pObject->bThreadRIFinished = true;
	
	return 0;
}

//Control thread
UINT ThreadWorker(LPVOID pParam)
{
	__int64 initial, final;

	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;

	pObject->bThreadWActive = true;
	//pObject->record = false;


	//Get Bus devices
	/*pObject->GetDevices();

	//Get and load default device parameters
	pObject->GetDefaultDeviceParameters();

	//Init current variables
	pObject->currentA[0] = 0;
	pObject->currentA[1] = 0;
	pObject->currentA[2] = 0;

	//Default values
	pObject->currentData[0] = 0.0;
	pObject->currentData[1] = 0.0;
	pObject->currentData[2] = 0.0;
	*/
	//Control bucle
	while (pObject->bThreadWActive)
	{
		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&initial);


		////////////////////////////////////////
		// COMMANDS

		//pObject->ExecuteCommands();

		////////////////////////////////////////
		// MOTION

		pObject->ExecuteMotion();

		////////////////////////////////////////
		// PARAMETERS

		//pObject->GetDeviceParameters();

		////////////////////////////////////////
		// UPDATE INTERFACE VALUES

		/*pObject->UpdateInterfaceParameters();


		////////////////////////////////////////
		// RECORD DEVICE DATA

		pObject->RecordDeviceData();
		pObject->RecordTemp();


		////////////////////////////////////////
		// POSITION CORRECTION DEVICE DATA

		pObject->ExecuteCalibPosition();


		////////////////////////////////////////
		// RECORD OFFSETS CALIBRATION

		pObject->RecordOffsetsCalibration();

		////////////////////////////////////////
		// RECORD ZEROS CALIBRATION

		pObject->RecordZerosCalibration();


		//Loop position
		//pObject->fLoopPos = (float)(pObject->motorParameters[BCAN_PARAM_AR_ACTIPOS] * 2.0*CV_PI / (pObject->gearFactor*2048.0));

		//Update Timings
		pObject->UpdateTimings();*/

		QueryPerformanceCounter((LARGE_INTEGER*)&final);
		double tpc = (final - initial)*1000.0 / pObject->iFrequency;

		//EEPROM
		/*if (pObject->bWriteEEPROM)
		{
			pObject->cCanESD.WriteEEPROM(pObject->iCanId);
			pObject->bWriteEEPROM = false;
		}*/

		//Cycle time information		
		QueryPerformanceCounter((LARGE_INTEGER*)&final);

		//Update Worker TPC
		pObject->dWorkerTPC = pObject->dWorkerTPC*0.9 + ((final - initial)*1000.0 / pObject->iFrequency)*0.1;

		//Wait Exact Time Fixed 25ms
		while ((final - initial)*1000.0 / pObject->iFrequency < 10) QueryPerformanceCounter((LARGE_INTEGER*)&final);
	}

	//Finish Thread
	pObject->bThreadWFinished = true;

	return 0;
}


// CRSSMotorCtrlDlg message handlers

BOOL CRSSMotorCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	//Init Device Data
	//InitDevData();

	//Init Threads
	InitControlThreads();

	//Init calibration correction 
	//InitCalibPosition();

	//Offsets Calibration
	//InitOffsetsCalibration();

	//Zeros Calibration
	//InitZerosCalibration();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRSSMotorCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRSSMotorCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRSSMotorCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//////////////////////////////////////////////////////////////////////////////////////
//									CONTROL THREADS									//
//////////////////////////////////////////////////////////////////////////////////////

void CRSSMotorCtrlDlg::InitControlThreads()
{
	//Get System Frequency
	QueryPerformanceFrequency((LARGE_INTEGER*)&iFrequency);

	//Init Control Variables
	bThreadWFinished = false;
	bThreadRIFinished = false;
	dWorkerTPC = THREAD_TPC;
	dSecWorkerTPC = THREAD_TPC;

	//Create and run control thread
	tWorker = AfxBeginThread(ThreadWorker, this, THREAD_PRIORITY_TIME_CRITICAL);

	//Create and run interface thread
	tRefreshInterface = AfxBeginThread(ThreadInterface, this, THREAD_PRIORITY_NORMAL);
}

void CRSSMotorCtrlDlg::FinalizeControlThreads()
{
	bThreadWActive = false;
	bThreadRIActive = false;
	bThreadWSActive = false;

	SetTimer(TIMER_EXIT, 500, 0);
}



void CRSSMotorCtrlDlg::OnBnClickedButtonRampSend()
{
	CString szText;

	//Get target position, velocity and acceleration 
	GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);	fLoopTargetPos	= (float)(atof(szText)*CV_PI / 180.f);
	GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);		fLoopMaxVel		= (float)(atof(szText)*CV_PI / 180.f);
	GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);		fLoopMaxAcc		= (float)(atof(szText)*CV_PI / 180.f);

	bSendRamp = true;

	motionMode = DEV_M_POS;
}


void CRSSMotorCtrlDlg::ExecuteRampMotionControl()
{
	double moduleAIPos = 0;

	//Update values
	//UpdateLoopValues();

	//Get actual position in ticks
	//cCanESD.GetActIPos(iCanId, moduleAIPos);

	//Loop Mode
	if (bLoopStart)
	{
		//LoopSM(moduleAIPos);

		if (bSendLoopCommand)
		{
			/*cCanESD.SetTargetVel(iCanId, fLoopMaxVel);
			cCanESD.SetTargetAcc(iCanId, fLoopMaxAcc);
			cCanESD.MotionFRampMode(iCanId, fLoopTargetPos);*/
			bSendLoopCommand = false;
		}
	}
	//Normal Mode
	else if (bSendRamp)
	{
		/*if (bTrajectoryRunning)
		{
			cCanESD.SetTargetVel(iCanId, (float)dTrajectoryVel);
			cCanESD.SetTargetAcc(iCanId, (float)dTrajectoryAcc);
			cCanESD.MotionFRampMode(iCanId, (float)dTrajectoryPos);
		}
		else
		{
			cCanESD.SetTargetVel(iCanId, fLoopMaxVel);
			cCanESD.SetTargetAcc(iCanId, fLoopMaxAcc);
			cCanESD.MotionFRampMode(iCanId, fLoopTargetPos);
		}*/

		bSendRamp = false;
	}
}

////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS											OK
////////////////////////////////////////////////////////////////

void CRSSMotorCtrlDlg::GetDevices()
{/*
	unsigned int serial;
	CString szDevice;

	//Default
	iCanId = 0;
	iCan2Id = 0;
	iSerial = 0;
	serial = 0;

	//Detect can bus devices
	for (int i = 0; i < 31; i++)
	{
		if (i != 14 && i != 15 && i != 24 && i != 25)
		{
			if (cCanESD.GetDefSerial(i, serial))
			{
				iCanId = i;
				iSerial = serial;

				szDevice.Format("%02d - %d ", i, serial);

				cbDevices.AddString(szDevice);
				cbDevices.SetCurSel(0);
			}
		}
	}*/
}

void CRSSMotorCtrlDlg::GetDefaultDeviceParameters()
{/*
	for (int i = 0; i < 256; i++) motorParameters[i] = 0;

	//Load default Parameters
	cCanESD.GetDefHomeOffset(iCanId, motorParameters[BCAN_PARAM_DR_HOMEOFFSET]);
	cCanESD.GetDefGearRatio(iCanId, motorParameters[BCAN_PARAM_DR_GEARRATIO]);
	cCanESD.GetDefMinPos(iCanId, motorParameters[BCAN_PARAM_DR_MINPOS]);
	cCanESD.GetDefMaxPos(iCanId, motorParameters[BCAN_PARAM_DR_MAXPOS]);
	cCanESD.GetDefMaxDeltaPos(iCanId, motorParameters[BCAN_PARAM_DR_MAXDELTAPOS]);
	cCanESD.GetDefMaxDeltaVel(iCanId, motorParameters[BCAN_PARAM_DR_MAXDELTAVEL]);
	cCanESD.GetDefTorqueRatio(iCanId, motorParameters[BCAN_PARAM_DR_TORQUERATIO]);
	cCanESD.GetDefMinVel(iCanId, motorParameters[BCAN_PARAM_DR_MINVEL]);
	cCanESD.GetDefMaxVel(iCanId, motorParameters[BCAN_PARAM_DR_MAXVEL]);
	cCanESD.GetDefMinAcc(iCanId, motorParameters[BCAN_PARAM_DR_MINACC]);
	cCanESD.GetDefMaxAcc(iCanId, motorParameters[BCAN_PARAM_DR_MAXACC]);
	cCanESD.GetDefMinCur(iCanId, motorParameters[BCAN_PARAM_DR_MINCUR]);
	cCanESD.GetDefMaxCur(iCanId, motorParameters[BCAN_PARAM_DR_MAXCUR]);
	cCanESD.GetDefHomeVel(iCanId, motorParameters[BCAN_PARAM_DR_HOMEVEL]);
	cCanESD.GetDefHomeAcc(iCanId, motorParameters[BCAN_PARAM_DR_HOMEACC]);
	cCanESD.GetDefRSSSerial(iCanId, motorParameters[BCAN_PARAM_DR_RSSSERIAL]);
	cCanESD.GetDefConfig(iCanId, motorParameters[BCAN_PARAM_DR_CONFIG]);
	cCanESD.GetDefPulsesPerTurn(iCanId, motorParameters[BCAN_PARAM_DR_PULSESPERTURN]);
	cCanESD.GetDefRSSVersion(iCanId, motorParameters[BCAN_PARAM_DR_RSSVERSION]);
	cCanESD.GetDefBrakeTimeOut(iCanId, motorParameters[BCAN_PARAM_DR_BRAKETIMEOUT]);
	cCanESD.GetDefAddress(iCanId, motorParameters[BCAN_PARAM_DR_ADDRESS]);
	cCanESD.GetDefPrimBaud(iCanId, motorParameters[BCAN_PARAM_DR_PRIMEBAUD]);
	cCanESD.GetDefGearRatio(iCanId, gearFactor);
	cCanESD.GetActConfig(iCanId, moduleConfig);
	cCanESD.GetDefSecondaryAddress(iCanId, iCan2Id);

	//Get Default Phase offsets
	cCanESD.GetCurrentOffsets(iCanId, iPhaseOffsetX, iPhaseOffsetY, iPhaseOffsetZ);
	cCanESD.GetCurrentZeros(iCanId, iPhaseZeroX, iPhaseZeroY, iPhaseZeroZ);

	CString szGearRatio; szGearRatio.Format("%.2f", motorParameters[BCAN_PARAM_DR_GEARRATIO]);
	SetDlgItemText(IDC_ACT_GEARRATIO, szGearRatio);

	//Update Interface Values
	SetDlgItemInt(IDC_PWM_SINA, iPhaseOffsetX);
	SetDlgItemInt(IDC_PWM_SINB, iPhaseOffsetY);
	SetDlgItemInt(IDC_PWM_SINC, iPhaseOffsetZ);
	SetDlgItemInt(IDC_PWM_IA, iPhaseZeroX);
	SetDlgItemInt(IDC_PWM_IB, iPhaseZeroY);
	SetDlgItemInt(IDC_PWM_IC, iPhaseZeroZ);

	//Module Configuration
	if (moduleConfig & 0x00000001) bActiveBrake.SetCheck(true);
	else bActiveBrake.SetCheck(false);

	if (moduleConfig & 0x00000002) bActive24V1.SetCheck(true);
	else bActive24V1.SetCheck(false);

	if (moduleConfig & 0x00000004) bActive24V2.SetCheck(true);
	else bActive24V2.SetCheck(false);

	if (moduleConfig & 0x00000008) bActiveFANS.SetCheck(true);
	else bActiveFANS.SetCheck(false);

	lowPriorityParams = PRIORITY2;*/
}

void CRSSMotorCtrlDlg::ExecuteCommands()
{
	//Set Module Zero Offset
	/*SetModuleZero();

	//Set Rotor Zero Offset
	SetRotorZero();

	//Set Gear Ratio
	SetGearRatio();

	//Set CAN1Id Command
	SetCAN1Id();

	//Set CAN1Id Command
	SetCAN2Id();

	//Set Serial Command
	SetSerial();

	//Amplitude Control
	UpdateAmplitudeControl();

	//Update Module Config
	UpdateModuleConfig();

	//Set current offsets
	if (bSetCurrentOffsets)
	{
		cCanESD.SetCurrentOffsets(iCanId, GetDlgItemInt(IDC_PWM_SINA), GetDlgItemInt(IDC_PWM_SINB), GetDlgItemInt(IDC_PWM_SINC));
		bSetCurrentOffsets = false;
	}

	//Set current offsets
	if (bSetCurrentZeros)
	{
		cCanESD.SetCurrentZeros(iCanId, GetDlgItemInt(IDC_PWM_IA), GetDlgItemInt(IDC_PWM_IB), GetDlgItemInt(IDC_PWM_IC));
		bSetCurrentZeros = false;
	}*/
}

void CRSSMotorCtrlDlg::ExecuteMotion()
{
	//Trajectory control
	//ExecuteTrajectoryStep();

	//Controller Commands
	if (!bMotorHalted && !bHaltMotor)
	{
		//Motion Mode Selection
		switch (motionMode)
		{
			//Ramp Motion Command
			case DEV_M_POS:
			{
				ExecuteRampMotionControl();
				break;
			}

		//Velocity Motion Command
		/*case DEV_M_VEL:
		{
			ExecuteVelMotionControl();
			break;
		}*/
		}
	}
	else if (bHaltMotor)
	{
		//cCanESD.Halt(iCanId);
		bMotorHalted = true;
		bHaltMotor = false;
	}

	if (bResetMotor)
	{
		//cCanESD.Reset(iCanId);
		bResetMotor = false;
	}

	if (bBrakeMotor)
	{
		//cCanESD.Brake(iCanId, bBrakeState);
		bBrakeMotor = false;
	}
}

void CRSSMotorCtrlDlg::GetDeviceParameters()
{
	///////////////////
	//MOTOR	
	/*
	//Actual values
	if (bPriSec[IFACE_PRI_SEC_MOTOR_POS]) cCanESD.GetActPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTPOS]);
	if (bPriSec[IFACE_PRI_SEC_MOTOR_VEL]) cCanESD.GetActVel(iCanId, motorParameters[BCAN_PARAM_AR_ACTVEL]);
	if (bPriSec[IFACE_PRI_SEC_MOTOR_ACC]) cCanESD.GetActAcc(iCanId, motorParameters[BCAN_PARAM_AR_ACTACC]);

	//Target Values
	if (bPriSec[IFACE_PRI_SEC_MOTOR_POS]) cCanESD.GetTargetPos(iCanId, motorParameters[BCAN_PARAM_AW_TARGET_POS]);
	if (bPriSec[IFACE_PRI_SEC_MOTOR_VEL]) cCanESD.GetTargetVel(iCanId, motorParameters[BCAN_PARAM_AW_TARGET_VEL]);
	if (bPriSec[IFACE_PRI_SEC_MOTOR_ACC]) cCanESD.GetTargetAcc(iCanId, motorParameters[BCAN_PARAM_AW_TARGET_ACC]);

	///////////////////
	//ROTOR

	//Actual values
	if (bPriSec[IFACE_PRI_SEC_ROTOR_POS]) cCanESD.GetActRotorPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORPOS]);
	if (bPriSec[IFACE_PRI_SEC_ROTOR_VEL]) cCanESD.GetActRotorVel(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORVEL]);
	if (bPriSec[IFACE_PRI_SEC_ROTOR_ACC]) cCanESD.GetActRotorAcc(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORACC]);

	//Target values	
	if (bPriSec[IFACE_PRI_SEC_ROTOR_POS]) cCanESD.GetActRotorTargetPos(iCanId, motorParameters[BCAN_PARAM_AW_ACTROTORTARGETPOS]);
	if (bPriSec[IFACE_PRI_SEC_ROTOR_VEL]) cCanESD.GetActRotorTargetVel(iCanId, motorParameters[BCAN_PARAM_AW_ACTROTORTARGETVEL]);
	if (bPriSec[IFACE_PRI_SEC_ROTOR_ACC]) cCanESD.GetActRotorTargetAcc(iCanId, motorParameters[BCAN_PARAM_AW_ACTROTORTARGETACC]);

	//Amplitude
	if (bPriSec[IFACE_PRI_SEC_AMPLITUDE]) cCanESD.GetActRotorAmplitude(iCanId, motorParameters[BCAN_PARAM_AW_ACTROTORAMPLITUDE], motorParameters[BCAN_PARAM_AW_ACTROTORMAXAMPLITUDE]);

	//Actual Position in encoder ticks
	if (bPriSec[IFACE_PRI_SEC_TICKS]) cCanESD.GetActIPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTIPOS]);

	//Delta Pos i Delta Vel
	if (bPriSec[IFACE_PRI_SEC_DELTAPOS]) cCanESD.GetActRotorDeltaPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORDELTAPOS]);
	if (bPriSec[IFACE_PRI_SEC_DELTAVEL]) cCanESD.GetActRotorDeltaVel(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORDELTAVEL]);

	//Params
	if (bPriSec[IFACE_PRI_SEC_PARAM1]) cCanESD.GetActRotorParam1(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORPARAM1]);
	if (bPriSec[IFACE_PRI_SEC_PARAM2]) cCanESD.GetActRotorParam2(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORPARAM2]);
	if (bPriSec[IFACE_PRI_SEC_PARAM3]) cCanESD.GetActRotorParam3(iCanId, motorParameters[BCAN_PARAM_AR_ACTROTORPARAM3]);

	//Get temperatures
	if (bPriSec[IFACE_PRI_SEC_TEMP]) cCanESD.GetActFTemp(iCanId, dTemps[TEMP_T1], TEMP_T1);
	if (bPriSec[IFACE_PRI_SEC_TEMP]) cCanESD.GetActFTemp(iCanId, dTemps[TEMP_T2], TEMP_T2);
	if (bPriSec[IFACE_PRI_SEC_TEMP]) cCanESD.GetActFTemp(iCanId, dTemps[TEMP_T3], TEMP_T3);
	if (bPriSec[IFACE_PRI_SEC_TEMP]) cCanESD.GetActFTemp(iCanId, dTemps[TEMP_M], TEMP_M);

	//Power supply
	if (bPriSec[IFACE_PRI_SEC_PWRSUPPLY])
	{
		cCanESD.GetActFMotorSupply(iCanId, motorParameters[BCAN_PARAM_AR_ACT_FMOTOR_SUPPLY]);
		cCanESD.GetActFLogicSupply(iCanId, motorParameters[BCAN_PARAM_AR_ACT_FLOGIC_SUPPLY]);

		//Update Voltages
		dVoltages[VOLT_24] = motorParameters[BCAN_PARAM_AR_ACT_FMOTOR_SUPPLY];
		dVoltages[VOLT_12] = motorParameters[BCAN_PARAM_AR_ACT_FLOGIC_SUPPLY];
	}

	//Get Currents
	if (bPriSec[IFACE_PRI_SEC_CURRENTS])
	{
		cCanESD.GetMotorCurrent(iCanId, dCurrent[CURRENT_A], dCurrent[CURRENT_B], dCurrent[CURRENT_C]);
		cCanESD.GetActRawCur(iCanId, dCurrent[CURRENT_M]);
	}*/
}