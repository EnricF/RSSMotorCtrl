
// RSSMotorCtrlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "RSSMotorCtrl.h"
#include "RSSMotorCtrlDlg.h"
#include "afxdialogex.h"

//EF mod
#include "globalVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* Prevent MS VC8 compiler and later warnings about deprecated CRT function */
#if (_MSC_VER >= 1400)
# define _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_WARNINGS
#endif

//EF mod: use only to pass compilation while code editing
#ifndef CV_PI
#define CV_PI 3.14159265359
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
	ON_BN_CLICKED(IDOK, &CRSSMotorCtrlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RAMP_STARTLOOP, &CRSSMotorCtrlDlg::OnBnClickedButtonRampStartloop)
	ON_BN_CLICKED(IDC_BUTTON_VEL_SET_VEL_RPM, &CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelRpm)
	ON_BN_CLICKED(IDC_BUTTON_VEL_START, &CRSSMotorCtrlDlg::OnBnClickedButtonVelStart)
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
	}

	//Finish Thread
	pObject->bThreadRIFinished = true;
	
	return 0;
}

UINT ThreadECMWorker(LPVOID pParam) {
	__int64 initial, final;

	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;

	pObject->bThreadECMWActive = true;

	//Init ECM and loop until end
	pObject->cecmTest.Init(__argc, __argv, &pObject->bThreadWActive);//Receives main WORKER thread status (active TRUE or FALSE)
			
	//WaitForSingleObject(pObject->tWorker, INFINITE);//Waits for Worker thread to finish	

	//Control bucle
	while (pObject->bThreadECMWActive)
	{
		//TODO
		//Sleep(1);
	}

	//Finish Thread
	pObject->bThreadECMWFinished = true;

	return 0;//OK
}

//Control thread
UINT ThreadWorker(LPVOID pParam)
{
	__int64 initial, final;

	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;

	pObject->bThreadWActive = true;

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

		pObject->GetDeviceParameters();

		////////////////////////////////////////
		// UPDATE INTERFACE VALUES

		//pObject->UpdateInterfaceParameters();


		////////////////////////////////////////
		// RECORD DEVICE DATA

		/*pObject->RecordDeviceData();
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

//	DWORD ret = WaitForSingleObject(pObject->tECMWorker, INFINITE);//Waits for Worker thread to finish	

	//Finish Thread
	pObject->bThreadWFinished = true;
	   
	Sleep(1000);

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
	InitDevData();

	//Init Threads
	InitControlThreads();//EF mod : new thread to Init() EtherCAT communication

	//Init calibration correction 
	//InitCalibPosition();

	//Offsets Calibration
	//InitOffsetsCalibration();

	//Zeros Calibration
	//InitZerosCalibration();
	
	//Wait for another thread? unknown yet
	//WaitForSingleObject(ThreadWorker, INFINITE);

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
	bThreadWFinished	= false;
	bThreadECMWFinished = false;
	bThreadRIFinished	= false;
	dWorkerTPC			= THREAD_TPC;
	dECMWorkerTPC		= THREAD_TPC;
	dSecWorkerTPC		= THREAD_TPC;


	//Create and run ECM thread
	tECMWorker	= AfxBeginThread(ThreadECMWorker, this, THREAD_PRIORITY_TIME_CRITICAL);
	
	//Create and run control thread
	tWorker		= AfxBeginThread(ThreadWorker, this, THREAD_PRIORITY_TIME_CRITICAL);

	//Create and run interface thread
	tRefreshInterface = AfxBeginThread(ThreadInterface, this, THREAD_PRIORITY_NORMAL);
}

void CRSSMotorCtrlDlg::FinalizeControlThreads()
{

	bThreadWActive		= false;
	bThreadRIActive		= false;
	bThreadECMWActive	= false;
	bThreadWSActive		= false;

	SetTimer(TIMER_EXIT, 500, 0);
}


//////////////////////////////////////////////////////////////////////////////////////
//									DEVICE DATA										//
//////////////////////////////////////////////////////////////////////////////////////

void CRSSMotorCtrlDlg::InitDevData()
{
	for (int p = 0; p < DEV_PARAMS; p++)
	{
		for (int f = 0; f < DEV_FIELDS; f++)
		{
			dDevData[p][f] = 0;
		}
	}

	//Position Initialization
	dDevData[DEV_POS][DEV_ACT] = 0;		dDevData[DEV_POS][DEV_TARGET] = 0;		dDevData[DEV_POS][DEV_MIN] = -1;	dDevData[DEV_POS][DEV_MAX] = -1;	dDevData[DEV_POS][DEV_RANGE] = 360.f;

	//Velocity Initialization
	dDevData[DEV_VEL][DEV_ACT] = 0;		dDevData[DEV_VEL][DEV_TARGET] = 0;		dDevData[DEV_VEL][DEV_MIN] = -1;	dDevData[DEV_VEL][DEV_MAX] = -1;	dDevData[DEV_VEL][DEV_RANGE] = 360.f;

	//Acceleration Initialization
	dDevData[DEV_ACC][DEV_ACT] = 0;		dDevData[DEV_ACC][DEV_TARGET] = 0;		dDevData[DEV_ACC][DEV_MIN] = -1;	dDevData[DEV_ACC][DEV_MAX] = -1;	dDevData[DEV_ACC][DEV_RANGE] = 32768.f;

	//Motor Position Initialization
	dDevData[DEV_MPOS][DEV_ACT] = 0;	dDevData[DEV_MPOS][DEV_TARGET] = 0;		dDevData[DEV_MPOS][DEV_MIN] = -1;	dDevData[DEV_MPOS][DEV_MAX] = -1;	dDevData[DEV_MPOS][DEV_RANGE] = 2048;

	//Motor Velocity Initialization
	dDevData[DEV_MVEL][DEV_ACT] = 0;	dDevData[DEV_MVEL][DEV_TARGET] = 0;		dDevData[DEV_MVEL][DEV_MIN] = -1;	dDevData[DEV_MVEL][DEV_MAX] = -1;	dDevData[DEV_MVEL][DEV_RANGE] = 10000;

	//Motor Acceleration Initialization
	dDevData[DEV_MACC][DEV_ACT] = 0;	dDevData[DEV_MACC][DEV_TARGET] = 0;		dDevData[DEV_MACC][DEV_MIN] = -1;	dDevData[DEV_MACC][DEV_MAX] = -1;	dDevData[DEV_MACC][DEV_RANGE] = 20000;

	//Motor Temperature Initialization
	dDevData[DEV_TEMP][DEV_ACT] = 0;	dDevData[DEV_TEMP][DEV_TARGET] = 0;		dDevData[DEV_TEMP][DEV_MIN] = -1;	dDevData[DEV_TEMP][DEV_MAX] = -1;	dDevData[DEV_TEMP][DEV_RANGE] = 256;

	//Motor Current Initialization
	dDevData[DEV_CUR][DEV_ACT] = 0;		dDevData[DEV_CUR][DEV_TARGET] = 0;		dDevData[DEV_CUR][DEV_MIN] = -1;	dDevData[DEV_CUR][DEV_MAX] = -1;	dDevData[DEV_CUR][DEV_RANGE] = 2400;

	//TPC Initialization
	dDevData[DEV_TPC][DEV_ACT] = 0;		dDevData[DEV_TPC][DEV_TARGET] = 0;		dDevData[DEV_TPC][DEV_MIN] = -1;	dDevData[DEV_TPC][DEV_MAX] = -1;	dDevData[DEV_TPC][DEV_RANGE] = 256.f;//6000;

	//Amplitude Initialization
	dDevData[DEV_AMP][DEV_ACT] = 0;		dDevData[DEV_AMP][DEV_TARGET] = 0;		dDevData[DEV_AMP][DEV_MIN] = -1;	dDevData[DEV_AMP][DEV_MAX] = -1;	dDevData[DEV_AMP][DEV_RANGE] = 240.f;

	//Control Initialization
	dDevData[DEV_CTRL][DEV_ACT] = 0;	dDevData[DEV_CTRL][DEV_TARGET] = 0;		dDevData[DEV_CTRL][DEV_MIN] = -1;	dDevData[DEV_CTRL][DEV_MAX] = -1;	dDevData[DEV_CTRL][DEV_RANGE] = 256.f;

	//CAN Initialization // Voltage
	dDevData[DEV_CAN][DEV_ACT] = 0;		dDevData[DEV_CAN][DEV_TARGET] = 0;		dDevData[DEV_CAN][DEV_MIN] = -1;	dDevData[DEV_CAN][DEV_MAX] = -1;	dDevData[DEV_CAN][DEV_RANGE] = 256.f;

	//Param1
	dDevData[DEV_PARAM1][DEV_ACT] = 0;	dDevData[DEV_PARAM1][DEV_TARGET] = 0;	dDevData[DEV_PARAM1][DEV_MIN] = -1;	dDevData[DEV_PARAM1][DEV_MAX] = -1;	dDevData[DEV_PARAM1][DEV_RANGE] = 4096.f;

	//Param2
	dDevData[DEV_PARAM2][DEV_ACT] = 0;	dDevData[DEV_PARAM2][DEV_TARGET] = 0;	dDevData[DEV_PARAM2][DEV_MIN] = -1;	dDevData[DEV_PARAM2][DEV_MAX] = -1;	dDevData[DEV_PARAM2][DEV_RANGE] = 4096.f;

	//Param3
	dDevData[DEV_PARAM3][DEV_ACT] = 0;	dDevData[DEV_PARAM3][DEV_TARGET] = 0;	dDevData[DEV_PARAM3][DEV_MIN] = -1;	dDevData[DEV_PARAM3][DEV_MAX] = -1;	dDevData[DEV_PARAM3][DEV_RANGE] = 4096.f;

	//Current A
	dDevData[DEV_CURA][DEV_ACT] = 0;	dDevData[DEV_CURA][DEV_TARGET] = 0;		dDevData[DEV_CURA][DEV_MIN] = -1;	dDevData[DEV_CURA][DEV_MAX] = -1;	dDevData[DEV_CURA][DEV_RANGE] = 1024.f;

	//Current B
	dDevData[DEV_CURB][DEV_ACT] = 0;	dDevData[DEV_CURB][DEV_TARGET] = 0;		dDevData[DEV_CURB][DEV_MIN] = -1;	dDevData[DEV_CURB][DEV_MAX] = -1;	dDevData[DEV_CURB][DEV_RANGE] = 1024.f;

	//Current C
	dDevData[DEV_CURC][DEV_ACT] = 0;	dDevData[DEV_CURC][DEV_TARGET] = 0;		dDevData[DEV_CURC][DEV_MIN] = -1;	dDevData[DEV_CURC][DEV_MAX] = -1;	dDevData[DEV_CURC][DEV_RANGE] = 1024.f;

	//Current Error
	dDevData[DEV_CURERR][DEV_ACT] = 0;	dDevData[DEV_CURERR][DEV_TARGET] = 0;	dDevData[DEV_CURERR][DEV_MIN] = -1;	dDevData[DEV_CURERR][DEV_MAX] = -1;	dDevData[DEV_CURERR][DEV_RANGE] = 50.f;

	//Init control variables
	bSetModuleZero	= false;
	bSetMotorZero	= false;
	bSetGearRatio	= false;
	bSetSerial		= false;
	bSetCAN1Id		= false;
	bSetCAN2Id		= false;
	bStopMotor		= false;
	bMoveMotorVel	= false;
	bHaltMotor		= false;
	bResetMotor		= false;
	bBrakeMotor		= false;
	bBrakeState		= false;
	bWriteEEPROM	= false;

	bMotorAddTemps = false;
	//bCalibPosition = false;

	InitLoopSM();

	//Status Variables
	bMotorHalted = false;
	
	//Init temperatures stabilization data
	for (int i = 0; i < STABLE_TEMPS; i++)
	{
		for (int j = 0; j < STABLE_TIME; j++)
			dTBuffer[i][j] = 0.0;

		dTInc[i] = 0.0;
	}

	iTIdx = 0;
	dMaxIncTemp = 0;
	dAmbIncTemp = 0;

	motorAddTemps[0] = 0.0;
	motorAddTemps[1] = 0.0;
	motorAddTemps[2] = 0.0;
	motorAddTemps[3] = 0.0;
	motorAddTemps[4] = 0.0;
	motorAddTemps[5] = 0.0;
	motorAddTemps[6] = 0.0;
}


void CRSSMotorCtrlDlg::OnBnClickedButtonRampSend()
{
	CString szText;

	//Get target position, velocity and acceleration 
	GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);	
	iLoopTargetPos	= (int)(atoi(szText));			//[encoder absolute position]
	//fLoopTargetPos	= (float)(atof(szText)*CV_PI / 180.f);	//old example in [rad]

	GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
	fLoopMaxVel = (float)(atof(szText));
	GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
	fLoopMaxAcc = (float)(atof(szText));
	
	//Protection guards - Max Vel
	/*if (fLoopMaxVel < 5.0)			fLoopMaxVel = 5.0;
	else if(fLoopMaxVel > 20.0)		fLoopMaxVel = 20.0;
	//Protection guards - Max Acc
	if (fLoopMaxAcc < 5.0)			fLoopMaxAcc = 5.0;
	else if (fLoopMaxAcc > 20.0)	fLoopMaxAcc = 20.0;*/

	//Always before MOTION starts!!
	cecmTest.SetProfiler(&fLoopMaxVel, &fLoopMaxAcc);//Writes selected values to execute a Ramp Motion

	//MOTION is ready
	bSendRamp	= true;		
	motionMode	= DEV_M_POS;
}


void CRSSMotorCtrlDlg::ExecuteRampMotionControl()
{
	double moduleAIPos = 0;

	bool isFinished = false;

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
		bool bTrajectoryRunning = false;
		if (bTrajectoryRunning)
		{
			/*cCanESD.SetTargetVel(iCanId, (float)dTrajectoryVel);
			cCanESD.SetTargetAcc(iCanId, (float)dTrajectoryAcc);
			cCanESD.MotionFRampMode(iCanId, (float)dTrajectoryPos);*/			
			isFinished = true;
		}
		else
		{
			//old
			/*cCanESD.SetTargetVel(iCanId, fLoopMaxVel);
			cCanESD.SetTargetAcc(iCanId, fLoopMaxAcc);
			cCanESD.MotionFRampMode(iCanId, fLoopTargetPos);*/
			isFinished = cecmTest.MotionFRampMode(iLoopTargetPos);//single drive condition
		}

		if (isFinished == true) {
			bSendRamp = false;
		}
			
	}
}


void CRSSMotorCtrlDlg::ExecuteVelMotionControl()
{
	bool isFinished = false;

	if (velRunning){
		//Start Profile Velocity mode!
		velRunning = !cecmTest.MotionFProfileVelMode(velTargetVel/60.);//single drive condition
			
		//Basic version Velocity mode, old. Useful to set a single velocity, not re-usable
		//velRunning = !cecmTest.MotionFVelMode(velTargetVelValue);//single drive condition
	}
	else{//STOP!
		//cecmTest.MotionStop();
		motionMode = DEV_M_STOP;
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
{
	for (int i = 0; i < 256; i++) motorParameters[i] = 0;
	
	//TODO set default device parameters
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
			case DEV_M_VEL:
			{
				ExecuteVelMotionControl();
				break;
			}
		}
	}
	else if (bHaltMotor)
	{
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
	char pBuffer8[8];
	char pBuffer16[16];
	//double						dDevData[DEV_PARAMS][DEV_FIELDS];				//Device Data

	dDevData[DEV_STATUS][DEV_ACT] = (float)cecmTest.GetStatus();//CiA register
	
	sprintf(&pBuffer8[0], "0x");
	_itoa((int)dDevData[DEV_STATUS][DEV_ACT], &pBuffer8[2], 16);
	SetDlgItemText(IDC_STATIC_RAMP_STATEVALUE, pBuffer8);
	
	
	int actPos = cecmTest.GetActPos();//CiA register
	char pBuffer2[16];
	_itoa(actPos, pBuffer2, 10);
	SetDlgItemText(IDC_EDIT_PARAM_POS_ACT, pBuffer2);


	int actVel = cecmTest.GetActVel();//CiA register
	char pBuffer3[16];
	_itoa(actVel, pBuffer3, 10);
	SetDlgItemText(IDC_EDIT_PARAM_VEL_ACT, pBuffer3);
	

/*	cecmTest.GetActPos();
	cecmTest.GetActVel();
	cecmTest.GetMode();
	cecmTest.GetPrimaryTemp();
	cecmTest.GetLastError();
	cecmTest.GetStatusAx1();//Axis1 register
	*/
	//old example (using a boleean array)
	//if (bPriSec[IFACE_PRI_SEC_MOTOR_POS]) cCanESD.GetActPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTPOS]);
}


void CRSSMotorCtrlDlg::UpdateInterfaceParameters()
{
	CString szState;

	////////////////
	// ROTOR

	//Actual values
	UpdateValue(DEV_MPOS, 2048);					//Range: 0..2048 (11 bits) ticks, up to 12 bits 
	UpdateValue(DEV_MVEL, 20.0);					//Range: 0..20	[rev/s]
	UpdateValue(DEV_MACC, 20.0);					//Range: 0..?	[rev/s^2]

	//Target values
	UpdateTargetValue(DEV_MPOS, (float)iLoopTargetPos);	//Rotor target position
	UpdateTargetValue(DEV_MVEL, fLoopMaxVel);			//Rotor target Velocity
	UpdateTargetValue(DEV_MACC, fLoopMaxAcc);			//Rotor target Acceleration

	////////////////
	// MOTOR (Who is rotor and who is motor?)

	//Actual values
	UpdateValue(DEV_POS, 2048);
	UpdateValue(DEV_VEL, 20.0);
	UpdateValue(DEV_ACC, 20.0);

	//Target values
	UpdateTargetValue(DEV_POS, (float)iLoopTargetPos);
	UpdateTargetValue(DEV_VEL, fLoopMaxVel);
	UpdateTargetValue(DEV_ACC, fLoopMaxAcc);
}

void CRSSMotorCtrlDlg::UpdateValue(int id, double value)
{
	dDevData[id][DEV_ACT] = value;

	//Update Max-Min
	if (dDevData[id][DEV_MIN] == -1 || dDevData[id][DEV_ACT] < dDevData[id][DEV_MIN]) dDevData[id][DEV_MIN] = dDevData[id][DEV_ACT];
	if (dDevData[id][DEV_MAX] == -1 || dDevData[id][DEV_ACT] > dDevData[id][DEV_MAX]) dDevData[id][DEV_MAX] = dDevData[id][DEV_ACT];
}

void CRSSMotorCtrlDlg::UpdateTargetValue(int id, double value)
{
	dDevData[id][DEV_TARGET] = value;
}


void CRSSMotorCtrlDlg::OnBnClickedOk()
{
	FinalizeControlThreads();
	
	CDialogEx::OnOK();
}


void CRSSMotorCtrlDlg::OnBnClickedButtonRampStartloop()
{
	bLoopStart = !bLoopStart;

	//swap
	if (bLoopStart) SetDlgItemText(IDC_BUTTON_RAMP_STARTLOOP, "Stop");
	else			SetDlgItemText(IDC_BUTTON_RAMP_STARTLOOP, "Start");

	//Get Loop Data
	if (bLoopStart)
	{
		motionMode = DEV_M_POS;

		iDelayCounter = 0;

		UpdateLoopValues();

		iLoopState = LOOP_STATE_INITIAL;

		//Going to initial position
		fLoopTargetPos = (float)(fLoopInitial*CV_PI / 180.f);

		bSendLoopCommand = true;

	}
}


void CRSSMotorCtrlDlg::InitLoopSM()
{
	CString szText;

	bLoopStart		= false;
	bSendRamp		= false;
	bSendLoopCommand= false;

	//Initial values
	iLoopDelay		= 0;
	fLoopUpper		= 1000.;
	fLoopInitial	= 0.;
	fLoopLower		= -1000.;
	fLoopMaxVel		= 20.0;
	fLoopMaxAcc		= 20.0;

	//Initial State
	iLoopState		= LOOP_STATE_INITIAL;

	//Load Initial values
	szText.Format("%.d",	iLoopDelay);	SetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);
	szText.Format("%.2f",	fLoopInitial);	SetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
	szText.Format("%.2f",	fLoopUpper);	SetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);
	szText.Format("%.2f",	fLoopLower);	SetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);
	szText.Format("%.2f",	fLoopMaxVel);	SetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
	szText.Format("%.2f",	fLoopMaxAcc);	SetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
}

void CRSSMotorCtrlDlg::UpdateLoopValues()
{
	CString szText;

	float actLoopMaxVel;
	float actLoopMaxAcc;

	//Load Initial values
	GetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);		iLoopDelay		= atoi(szText);
	GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);	fLoopInitial	= (float)atof(szText);
	GetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);		fLoopUpper		= (float)atof(szText);
	GetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);		fLoopLower		= (float)atof(szText);
	GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);		actLoopMaxVel	= (float)atof(szText);
	GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);		actLoopMaxAcc	= (float)atof(szText);

	//Get Interface Target Values and parse to radians
	float auxfLoopMaxVel = (float)(actLoopMaxVel*CV_PI / 180.f);
	float auxfLoopMaxAcc = (float)(actLoopMaxAcc*CV_PI / 180.f);

	if (fLoopMaxVel != auxfLoopMaxVel)
	{
		fLoopMaxVel = auxfLoopMaxVel;
		cecmTest.SetProfilerMaxVel(fLoopMaxVel);
	}

	if (fLoopMaxAcc != auxfLoopMaxAcc)
	{
		fLoopMaxAcc = auxfLoopMaxAcc;
		cecmTest.SetProfilerMaxAcc(fLoopMaxAcc);
		cecmTest.SetProfilerMaxDec(fLoopMaxAcc);//re-use
	}
	//Always before MOTION starts!!
}

void CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelRpm()
{
	if (velRunning){
		SetDlgItemText(IDC_BUTTON_VEL_START, "Start");

		//motionMode = DEV_M_STOP;
	}
	else{//Velocity mode starts

		//Get control value	
		velTargetRPM = GetDlgItemInt(IDC_EDIT_VEL_VEL_RPM);
		//velTargetVel = velTargetRPM / 60.0;	//no gear, no [rad] units
		velTargetVel = velTargetRPM;		//TODO : it has no sense, review variables naming and their use

		setVelTargetVelValue((float)velTargetVel, IDC_EDIT_VEL_VEL_RPM);

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}
	velRunning = !velRunning;

	//Update Slider control
	/*scPosition.SetRange(0, 20);
	scPosition.SetTic(2);
	scPosition.SetTic(3);
	scPosition.SetTic(6);
	//scPosition.SetPos(2);
	scPosition.SetPos(velTargetVel);*/

	UpdateVelMotionInterface();
}

void CRSSMotorCtrlDlg::UpdateVelMotionInterface()
{
	CString szData;

	SetDlgItemInt(IDC_EDIT_VEL_VEL_RPM, velTargetRPM);

	szData.Format("%.3f", velTargetVel*360.);
	SetDlgItemText(IDC_EDIT_VEL_VEL_DEG_S, szData);

	szData.Format("%.3f", velTargetAcc*360.);
	SetDlgItemText(IDC_EDIT_VEL_ACC_DEG_S2, szData);
}


// ------------------------------ ENCODER DETAILS ---------------------------
// RM08 super small non - contact rotary encoder
// Up to 4096 counts per revolution
// Configured to 2048 counter per revolution
// Specifications: D:\Dropbox\1.1 EQUIP\4. COMPRES\Especificacions compra x prov\RLS_Renishaw\RM08SD0012B02L2G00

void CRSSMotorCtrlDlg::OnBnClickedButtonVelStart()
{
	CString szText;

	if (velRunning)
	{
		SetDlgItemText(IDC_BUTTON_VEL_START, "Start");

		//motionMode = DEV_M_STOP;
	}
	else//Velocity mode starts
	{
		//Get Velocity in RPM
		GetDlgItemText(IDC_EDIT_VEL_VEL_RPM, szText);
		velTargetVel = atof(szText);

		setVelTargetVelValue((float)velTargetVel, IDC_EDIT_VEL_VEL_RPM);
		
		//Set Profiler limits
		float fLoopMaxVel = 20.0;
		float fLoopMaxAcc = 20.0;

		cecmTest.SetProfiler(&fLoopMaxVel, &fLoopMaxAcc);

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}

	velRunning = !velRunning;
}

void CRSSMotorCtrlDlg::setVelTargetVelValue(float vel, int mode) {

	int incrementsPerLoop = 2048; //increments(counts) per loop
	
	switch (mode) {
		case IDC_EDIT_VEL_VEL_RPM:
			velTargetVelValue = (int)vel / 60;// [rev/s]
			if (velTargetVelValue > 1000)
				velTargetVelValue = 1;// 100 inc/second is the minimum velocity by default
			else if (velTargetVelValue < -1000)
				velTargetVelValue = -1;// 100 inc/second is the minimum velocity by default
			break;
		default:
			break;
	}
}

void CRSSMotorCtrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar *)&scPosition)
	{
		velTargetVelValue = scPosition.GetPos();
		UpdateVelMotionValues();
	}
	else if (pScrollBar == (CScrollBar *)&scAmplitude)
	{
		velTargetAccValue = scAmplitude.GetPos();
		UpdateVelMotionValues();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRSSMotorCtrlDlg::UpdateVelMotionValues()
{
	velTargetRPM = velTargetVelValue - 7500;
	velTargetVel = velTargetRPM * 2 * CV_PI / (gearFactor*60.0);
	velTargetAcc = (velTargetAccValue - 2500)*CV_PI / 1800.0;


	UpdateVelMotionInterface();
}