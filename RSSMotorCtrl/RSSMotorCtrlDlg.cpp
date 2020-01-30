
// RSSMotorCtrlDlg.cpp : implementation file of main dialog
// Created by: JB, EF
// Date: 2020
// Copyright: Rob Surgical S.L.

#include "pch.h"
#include "framework.h"
#include "RSSMotorCtrl.h"
#include "RSSMotorCtrlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* Prevent MS VC8 compiler and later warnings about deprecated CRT function */
#if (_MSC_VER >= 1400)
# define _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_WARNINGS
#endif

//------------------------------------
// CAboutDlg dialog used for App About
//------------------------------------
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
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX){
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()

//------------------------------------
// CRSSMotorCtrlDlg dialog
//------------------------------------


CRSSMotorCtrlDlg::CRSSMotorCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RSSMOTORCTRL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRSSMotorCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_VEL_VEL, scVelocity);
	DDX_Control(pDX, IDC_SLIDER_VEL_ACC, scAcceleration);
	DDX_Control(pDX, IDC_COMBO_RAMP_UP_UNITS, cmbRampUpUnitsSel);
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
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_VEL_SET_VEL, &CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVel)
	ON_BN_CLICKED(IDC_BUTTON_REC_DATA, &CRSSMotorCtrlDlg::OnBnClickedButtonRecData)
	ON_BN_CLICKED(IDC_BUTTON_DEV_FAULT_RESET, &CRSSMotorCtrlDlg::OnBnClickedButtonDevFaultReset)
	ON_BN_CLICKED(IDC_BUTTON_VEL_SET_VEL_ACC, &CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelAcc)
	ON_BN_CLICKED(IDCANCEL, &CRSSMotorCtrlDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REC_T, &CRSSMotorCtrlDlg::OnBnClickedButtonRecT)
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

		//Update Interface Control values
		pObject->UpdateInterfaceControls();
	}

	//Finish Thread
	pObject->bThreadRIFinished = true;
	
	return 0;
}


//EtherCAT thread
UINT ThreadECMWorker(LPVOID pParam) {
	__int64 initial, final;

	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;

	pObject->bThreadECMWActive = true;

	//Init ECM and loop until end
	pObject->cecmW.Init(__argc, __argv, &pObject->bThreadWActive);//Receives main WORKER thread status (active TRUE or FALSE)
			
	//WaitForSingleObject(pObject->tWorker, INFINITE);//Waits for Worker thread to finish	

	//Control bucle
	while (pObject->bThreadECMWActive)
	{
		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&final);

		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&initial);

		//More?
		Sleep(1000);
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
		pObject->ExecuteCommands();

		////////////////////////////////////////
		// MOTION
		pObject->ExecuteMotion();

		////////////////////////////////////////
		// PARAMETERS
		pObject->GetDeviceParameters();

		////////////////////////////////////////
		// UPDATE INTERFACE VALUES
		pObject->UpdateInterfaceParameters();
		
		////////////////////////////////////////
		// RECORD DEVICE DATA
		pObject->RecordDeviceData();
		pObject->RecordTemp();

		////////////////////////////////////////
		// POSITION CORRECTION DEVICE DATA
		/*pObject->ExecuteCalibPosition();


		////////////////////////////////////////
		// RECORD OFFSETS CALIBRATION
		pObject->RecordOffsetsCalibration();

		////////////////////////////////////////
		// RECORD ZEROS CALIBRATION
		pObject->RecordZerosCalibration();

		*/
		//Loop position
		//pObject->fLoopPos = (float)(pObject->motorParameters[BETH_PARAM_AR_ACTIPOS] * 2.0*CV_PI / (pObject->gearFactor*2048.0));
		
		//pObject->fLoopPos = (float)( pObject->motorParameters[BETH_PARAM_AR_ACTIPOS] * 2.0*CV_PI / (2048.0) );
		//pObject->iLoopPos = (int) pObject->dDevData[DEV_MPOS][DEV_ACT];
		float temp = pObject->dDevData[DEV_MPOS][DEV_ACT];
		pObject->iLoopPos = pObject->cecmW.GetActPos();

		//Update Timings
		//pObject->UpdateTimings();

		QueryPerformanceCounter((LARGE_INTEGER*)&final);
		double tpc = (final - initial)*1000.0 / pObject->iFrequency;

		//Cycle time information		
		QueryPerformanceCounter((LARGE_INTEGER*)&final);

		//Update Worker TPC
		pObject->dWorkerTPC = pObject->dWorkerTPC*0.9 + ((final - initial)*1000.0 / pObject->iFrequency)*0.1;

		//Wait Exact Time Fixed 25ms
		while ((final - initial)*1000.0 / pObject->iFrequency < 10) QueryPerformanceCounter((LARGE_INTEGER*)&final);
	}

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

	//Load list boxes
	LoadComboBox();

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

	//Record data initialization
	InitRecordDeviceData();	//Device (setting variables set to default start values)
	InitRecordTemp();		//Temperature (setting variables set to default start values)

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


void CRSSMotorCtrlDlg::ShowParameter(int id)
{
	CString szInfo;

	static int decimator = 0;

	static ULONGLONG tnow, tprev = GetTickCount64();

	tnow = GetTickCount64();

	//typical resolution in range 10:16 ms
	if ( (tnow - tprev) > 10) {
		tprev = tnow; //update

		//if (id < DEV_IND)//temporal mod, for testing purposes only
		if (id < 7)//undo this mod ASAP
		{

			//Show Actual, Min and Max values in degrees
			if (id == DEV_MPOS) {
				//szInfo.Format("%.1f", dDevData[id][DEV_ACT] * 360.f / dDevData[id][DEV_RANGE]);
				szInfo.Format("%.1f", dDevData[id][DEV_ACT]);
				SetDlgItemText(IDC_EPA1 + id - 1, szInfo);
			}
			////else szInfo.Format( "%.1f", dDevData[id][DEV_ACT]*360.f/dDevData[id][DEV_RANGE] ); SetDlgItemText( IDC_EPA1+id-1, szInfo );
			else {
				szInfo.Format("%.3f", dDevData[id][DEV_ACT]);
				SetDlgItemText(IDC_EPA1 + id - 1, szInfo);
			}
			if (id == DEV_MVEL) {
				szInfo.Format("%.1f", dDevData[id][DEV_ACT]);
				SetDlgItemText(IDC_EPA1 + id - 1, szInfo);
			}
			if (id == DEV_MACC) {
				szInfo.Format("%.1f", dDevData[id][DEV_ACT]);
				SetDlgItemText(IDC_EPA1 + id - 1, szInfo);
			}

			szInfo.Format("%.1f", dDevData[id][DEV_MIN] * 360.f / dDevData[id][DEV_RANGE]); SetDlgItemText(IDC_EPMIN1 + id - 1, szInfo);
			szInfo.Format("%.1f", dDevData[id][DEV_MAX] * 360.f / dDevData[id][DEV_RANGE]); SetDlgItemText(IDC_EPMAX1 + id - 1, szInfo);

			szInfo.Format("%.1f", dDevData[id][DEV_TARGET]); SetDlgItemText(IDC_EPTARGET1 + id - 1, szInfo);
		}
		else
		{
			//Temporally disabled until GUI is complete
			//Show Actual, Min and Max values in ticks
			//szInfo.Format("%.1f", dDevData[id][DEV_ACT]); SetDlgItemText(IDC_EPA1 + id - 1, szInfo);
			//szInfo.Format("%.1f", dDevData[id][DEV_MIN]); SetDlgItemText(IDC_EPMIN1 + id - 1, szInfo);
			//szInfo.Format("%.1f", dDevData[id][DEV_MAX]); SetDlgItemText(IDC_EPMAX1 + id - 1, szInfo);

			if (id == DEV_TEMP) {
				szInfo.Format("%.1f", dDevData[DEV_TEMP][DEV_ACT]);
				SetDlgItemText(IDC_EDIT_PARAM_T_ACT, szInfo);
			}

			if (id == DEV_MTEMP) {
				szInfo.Format("%.1f", dDevData[DEV_MTEMP][DEV_ACT]);
				SetDlgItemText(IDC_EDIT_PARAM_TM_ACT, szInfo);
			}
			//motorParameters[BETH_PARAM_AR_ACT_FTEMP] = (double)cecmW.GetTemperatureMotor();

		}

	}
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

	//Init RAMP GroupBox variables
	InitLoopSM();

	//Sliders controls
	scVelocity.SetRange(-MOTOR_VEL_DR_REV_S / 2, MOTOR_VEL_DR_REV_S / 2, TRUE);
	scVelocity.SetPos(0);
	scAcceleration.SetRange(-MOTOR_ACC_DR_REV_S / 2, MOTOR_ACC_DR_REV_S / 2, TRUE);
	scAcceleration.SetPos(0);

	//Set STATUS Semaphore picture to yellow (Start)
	picStatusSemaphore.LoadBitmapA("res\\StatusSemaphoreYellow.bmp");
}

void CRSSMotorCtrlDlg::SetLoopValue(int id_editbox, float * var) {
	CString szText;

	GetDlgItemText(id_editbox, szText);
	if (cmbRampUpUnitsSel.GetCurSel() == DEGREES) {
		*var = (float)(atof(szText)*ENC_MOTOR_DR*gearFactor/360.);	//[external encoder absolute position]
	}

	else if (cmbRampUpUnitsSel.GetCurSel() == COUNTS) {
		*var = (float)(atof(szText)*gearFactor);	//[external encoder absolute position]
	}
	else if (cmbRampUpUnitsSel.GetCurSel() == RADIANS) {//RADIANS 
		*var = (float)(atof(szText)*ENC_MOTOR_DR*gearFactor / (2 * CV_PI));	//[external encoder absolute position]
	}
	else {//COUNTS by defaults (no more options by now)
		*var = (float)(atof(szText)*gearFactor);	//[encoder absolute position]
	}
}

void CRSSMotorCtrlDlg::OnBnClickedButtonRampSend()
{
	CString szText;

	SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial);//also used as a single-step motion
	iLoopTargetPos = (int)fLoopInitial;//Set TargetPos [encoder counts - Motor(inner)]
	
	//GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
	//iLoopTargetPos = atoi(szText);//Set TargetPos in [encoder absolute counts]
	//iLoopTargetPos = (int)fLoopTargetPos;
	
	UpdateProfilerLimits();
	   
	//MOTION is ready
	bSendRamp	= true;		
	motionMode	= DEV_M_POS;
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
		motionMode		= DEV_M_POS;
		iDelayCounter	= 0;
		iLoopState		= LOOP_STATE_INITIAL;

		SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial);

		//Update iLoopTargetPos [encoder absolute counts]
		iLoopTargetPos = (int)fLoopInitial;

		//Set PROFILER! Never forget to do this before any other EtherCAT command to the slave
		UpdateProfilerLimits();

		UpdateLoopValues();

		bSendLoopCommand = true;
	}
}


//bool CRSSMotorCtrlDlg::LoopSM(double modulePos)//param was never used
bool CRSSMotorCtrlDlg::LoopSM(void)
{
	switch (iLoopState)
	{
	case LOOP_STATE_INITIAL:
	{
		if (abs(iLoopTargetPos - iLoopPos) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_UPPER;
				//fLoopTargetPos = (float)(fLoopUpper*CV_PI / 180.f);//old
				iLoopTargetPos = (int)fLoopUpper;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}

	case LOOP_STATE_GO_UPPER:
	{
		if (abs(iLoopTargetPos - iLoopPos) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_INITIAL_FU;
				//fLoopTargetPos = (float)(fLoopInitial*CV_PI / 180.f);
				iLoopTargetPos = (int)fLoopInitial;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}
	case LOOP_STATE_GO_INITIAL_FU:
	{

		if (abs(iLoopTargetPos - iLoopPos) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;
			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_DELAY_FU;
				iDelayCounter = 0;
			}
		}

		break;
	}
	case LOOP_STATE_DELAY_FU:
	{
		if (iDelayCounter >= iLoopDelay)
		{
			iLoopState = LOOP_STATE_GO_LOWER;
			//fLoopTargetPos = (float)(fLoopLower*CV_PI / 180.f);
			iLoopTargetPos = (int)fLoopLower;
			iDelayCounter = 0;
			bSendLoopCommand = true;
		}
		else iDelayCounter++;
		break;
	}
	case LOOP_STATE_GO_LOWER:
	{
		if (abs(iLoopTargetPos - iLoopPos) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_INITIAL_FL;
				//fLoopTargetPos = (float)(fLoopInitial*CV_PI / 180.f);
				iLoopTargetPos = (int)fLoopInitial;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}
	case LOOP_STATE_GO_INITIAL_FL:
	{
		if (abs(iLoopTargetPos - iLoopPos) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;
			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_DELAY_FL;
				iDelayCounter = 0;
			}
		}
		break;
	}
	case LOOP_STATE_DELAY_FL:
	{
		if (iDelayCounter >= iLoopDelay)
		{
			iLoopState = LOOP_STATE_GO_UPPER;
			//fLoopTargetPos = (float)(fLoopUpper*CV_PI / 180.f);
			iLoopTargetPos = (int)fLoopUpper;
			iDelayCounter = 0;
			bSendLoopCommand = true;
		}
		else iDelayCounter++;
		break;
	}
	}
	CString szData; szData.Format("%d- %d", iLoopState, iDelayCounter);
	SetDlgItemText(IDC_STATIC_RAMP_STATEVALUE, szData);

	return true;
}


void CRSSMotorCtrlDlg::ExecuteRampMotionControl()
{
	double moduleAIPos = 0;

	bool isFinished = false;

	//Update values - To know if the user has changed GUI values on EditBoxes
	UpdateLoopValues();

	//Loop Mode
	if (bLoopStart)
	{
		LoopSM();//IT DOESN'T NEED TICKS ANYMORE

		if (bSendLoopCommand)
		{
			isFinished = cecmW.MotionFRampMode(iLoopTargetPos);//single drive condition

			//bSendLoopCommand = false;
			bSendLoopCommand = !isFinished;//testing
		}
	}
	//Normal Mode
	else if (bSendRamp)
	{
		bool bTrajectoryRunning = false;
		if (bTrajectoryRunning){		
			isFinished = true;
		}
		else{
			//isFinished = cecmW.MotionFRampMode(-65536*100000/2);//single drive condition
			isFinished = cecmW.MotionFRampMode(iLoopTargetPos);//single drive condition
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
		velRunning = !cecmW.MotionFProfileVelMode((float)velTargetVel);//single drive condition
			
		//Basic version Velocity mode, old. Useful to set a single velocity, not re-usable
		//velRunning = !cecmW.MotionFVelMode(velTargetVelValue);//single drive condition
	}
	else{//STOP!
		//cecmW.MotionStop();
		motionMode = DEV_M_STOP;
	}
}

////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS											OK
////////////////////////////////////////////////////////////////

void CRSSMotorCtrlDlg::GetDevices()
{
	//TODO
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
	SetRotorZero();*/

	//Set Gear Ratio
	SetGearRatio();

	//Set CAN1Id Command
	/*SetCAN1Id();

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
	static short StatusAct	= 0;
	static int LastError	= 0;
	char pBuffer8[8];
	//char pBuffer16[16];

	//TODO : remove dDevData writings!!! They are done through UpdateValue() and UpdateTargetValue() calls
	StatusAct = cecmW.GetStatus();//CiA register
	dDevData[DEV_STATUS][DEV_ACT] = (double)StatusAct;//CiA register
	sprintf(&pBuffer8[0], "0x");
	_itoa((int)dDevData[DEV_STATUS][DEV_ACT], &pBuffer8[2], 16);
	SetDlgItemText(IDC_EDIT_DEV_STATUS, pBuffer8);//temporally done here, to be moved out!

	UpdateStatusSemaphore(&StatusAct);
		
	//Last error update
	LastError = cecmW.GetLastError();//CiA register
	SetDlgItemInt(IDC_EDIT_DEV_LASTERROR, LastError);//temporally done here, to be moved out!
	
	//old example (using a boleean array)
	//if (bPriSec[IFACE_PRI_SEC_MOTOR_POS]) cCanESD.GetActPos(iCanId, motorParameters[BCAN_PARAM_AR_ACTPOS]);

	motorParameters[BETH_PARAM_AR_ACTPOS]		= (double)cecmW.GetActPos();
	//motorParameters[BETH_PARAM_AW_TARGET_POS]	= cecmW.GetTargetPos();
	//motorParameters[BETH_PARAM_AW_TARGET_POS]	= (double)iLoopTargetPos;//simplification

	motorParameters[BETH_PARAM_AR_ACTVEL]		= (double)cecmW.GetActVel();
	//motorParameters[BETH_PARAM_AW_TARGET_VEL]	= cecmW.GetTargetVel();
	motorParameters[BETH_PARAM_AW_TARGET_VEL]	= velTargetVel;//simplification

	//motorParameters[BETH_PARAM_AR_ACTACC]		= cecmW.GetActAcc();
	motorParameters[BETH_PARAM_AR_ACTACC]		= fLoopMaxAcc;//simplification
	//motorParameters[BETH_PARAM_AW_TARGET_ACC]	= cecmW.GetTargetAcc();
	motorParameters[BETH_PARAM_AW_TARGET_ACC]	= fLoopMaxAcc;//simplification

	//Get+Set Actual Primary Temperature
	//dDevData[DEV_TEMP][DEV_ACT] = (double)cecmW.GetTemperaturePrimary();
	motorParameters[BETH_PARAM_AR_ACT_TEMP]		= (double)cecmW.GetTemperaturePrimary();
	motorParameters[BETH_PARAM_AR_ACT_FTEMP]	= (double)cecmW.GetTemperatureMotor();

	motorParameters[BETH_PARAM_AR_ACTROTORPOS]	= (double)cecmW.GetModuleActPos();

}


void CRSSMotorCtrlDlg::UpdateInterfaceControls()
{
	// Updates Interface Controls by calling function ShowUpdate() once for each selected field
	ShowParameter(DEV_POS);
	ShowParameter(DEV_VEL);
	ShowParameter(DEV_ACC);
	ShowParameter(DEV_MPOS);
	ShowParameter(DEV_MVEL);
	ShowParameter(DEV_MACC);
	ShowParameter(DEV_TEMP);
	ShowParameter(DEV_MTEMP);

	//TODO : Update STATUS + STATUS_SEMAPHORE + LASTERROR here!
}

void CRSSMotorCtrlDlg::UpdateInterfaceParameters()
{
	CString szState;
	////////////////
	// MODULE - outter (wheel)

	//Actual values
	UpdateValue(DEV_POS, (double)cecmW.GetModuleActPos());//Range: 0..2048 (11 bits) ticks, up to 12 bits 
	//TO BE DONE - GetRotorActVel() & GetRotorActAcc()
	UpdateValue(DEV_VEL, 0.);	//Range: 0..1?		[rev/s]
	UpdateValue(DEV_ACC, 0.);	//Range: 0..0.1?	[rev/s^2]
	//Target values
	//UpdateTargetValue(DEV_MPOS, (double)iLoopTargetPos * gearFactor);//Module target position
	UpdateTargetValue(DEV_POS, 0.0);//Module target position - TO BE DONE!
	UpdateTargetValue(DEV_VEL, fLoopMaxVel/gearFactor);			//Module target Velocity
	UpdateTargetValue(DEV_ACC, fLoopMaxAcc/gearFactor);			//Module target Acceleration

	////////////////
	// MOTOR - inner

	//Actual values
	UpdateValue(DEV_MPOS, (double)cecmW.GetActPos());
	UpdateValue(DEV_MVEL, (double)cecmW.GetActVel());
	//TO BE DONE - GetActAcc()
	UpdateValue(DEV_MACC, 0);
	//Target values
	UpdateTargetValue(DEV_MPOS, (double)iLoopTargetPos);
	UpdateTargetValue(DEV_MVEL, fLoopMaxVel);
	UpdateTargetValue(DEV_MACC, fLoopMaxAcc);

	//--------------------------
	//Actual Primary Temperature
	UpdateValue(DEV_TEMP, (double)cecmW.GetTemperaturePrimary());	//Drive Temperature
	UpdateValue(DEV_MTEMP, (double)cecmW.GetTemperatureMotor());	//Motor Temperature
}

//Actual value
void CRSSMotorCtrlDlg::UpdateValue(int id, double value)
{
	dDevData[id][DEV_ACT] = value;


	float temp = 0.0;
	temp = dDevData[id][DEV_ACT];

	//Update Max-Min
	if (dDevData[id][DEV_MIN] == -1 || dDevData[id][DEV_ACT] < dDevData[id][DEV_MIN]) 
		dDevData[id][DEV_MIN] = dDevData[id][DEV_ACT];
	if (dDevData[id][DEV_MAX] == -1 || dDevData[id][DEV_ACT] > dDevData[id][DEV_MAX]) 
		dDevData[id][DEV_MAX] = dDevData[id][DEV_ACT];

	if (id == DEV_MPOS) {//MOTOR
		int a = 0;
		a++;
		a--;
	}

	if (id == DEV_POS) {//MODULE
		int b = 0;
		b++;
		b--;
	}
}

void CRSSMotorCtrlDlg::UpdateTargetValue(int id, double value)
{
	dDevData[id][DEV_TARGET] = value;

	if (id == DEV_MPOS) {//MOTOR
		int a = 0;
		a++;
		a--;
	}

	if (id == DEV_POS) {//MODULE
		int b = 0;
		b++;
		b--;
	}

}

void CRSSMotorCtrlDlg::UpdateStatusSemaphore(short * val) {

	static short prev_val = 0;
	static CStatic *pic = (CStatic *)GetDlgItem(IDC_STATIC_STATUS_SEMAPHORE);//Status Semaphore picture
	static CStatic *but = (CStatic *)GetDlgItem(IDC_BUTTON_DEV_FAULT_RESET);//Fault Reset button

	static HBITMAP hbit = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreGreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (prev_val != *val) {

		if (*val & (1 << 3)) {//FAULT - RED
			hbit = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreRed.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			pic->SetBitmap(hbit);		
			
			if ( !but->IsWindowVisible() )	but->ShowWindow(SW_SHOW);
		}
		else if (*val > 0) {//Any NON-FAULT value - GREEN
			hbit = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreGreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			pic->SetBitmap(hbit);
			if ( but->IsWindowVisible() )	but->ShowWindow(SW_HIDE);
		}
	}
	//otherwise keeps YELLOW

	prev_val = *val;//update it for next iteration
}

void CRSSMotorCtrlDlg::OnBnClickedOk()
{
	//Old stuff
	//FinalizeControlThreads();
	//CDialogEx::OnOK();

	//New
	CAboutDlg().DoModal();
}

void CRSSMotorCtrlDlg::InitLoopSM()
{
	CString szText;

	bLoopStart		= false;
	bSendRamp		= false;
	bSendLoopCommand= false;

	//Initial values
	gearFactor		= 1200.;
	iLoopDelay		= 20;
	fLoopUpper		= 180.;//[degrees/s] - module
	fLoopInitial	= 150.;//[degrees/s] - module
	fLoopLower		= 120.;//[degrees/s] - module
	fLoopMaxVel		= 20.0;//[rev/s] - motor
	fLoopMaxAcc		= 33.0;//[rev/s^2] - motor

	//Setting motor parameters
	motorParameters[BETH_PARAM_DR_MINVEL] = 10.0;
	motorParameters[BETH_PARAM_DR_MAXVEL] = 200.0;
	motorParameters[BETH_PARAM_DR_MINACC] = 10.0;
	motorParameters[BETH_PARAM_DR_MAXACC] = 333.0;


	//Initial State
	iLoopState		= LOOP_STATE_INITIAL;

	//Load Initial values
	szText.Format("%.d",	iLoopDelay);	SetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);
	szText.Format("%.2f",	fLoopInitial);	SetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
	szText.Format("%.2f",	fLoopUpper);	SetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);
	szText.Format("%.2f",	fLoopLower);	SetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);
	szText.Format("%.2f",	fLoopMaxVel/gearFactor);	SetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
	szText.Format("%.2f",	fLoopMaxAcc/gearFactor);	SetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
	szText.Format("%.0f",	gearFactor);	SetDlgItemText(IDC_EDIT_DEV_GFACTOR, szText);

	cmbRampUpUnitsSel.SetCurSel(DEGREES);//selects default "0" which is [degrees] units
}

void CRSSMotorCtrlDlg::UpdateLoopValues()
{
	CString szText;

	float actLoopMaxVel;//[rev/s] - motor
	float actLoopMaxAcc;//[rev/s^2] - motor

	//Load Initial values
	GetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);		iLoopDelay		= atoi(szText);

	SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial);
	//GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);	fLoopInitial	= (float)atof(szText);
	SetLoopValue(IDC_EDIT_RAMP_UPPERPOS, &fLoopUpper);
	//GetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);		fLoopUpper		= (float)atof(szText);
	SetLoopValue(IDC_EDIT_RAMP_LOWERPOS, &fLoopLower);
	//GetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);		fLoopLower		= (float)atof(szText);

	//Get Interface Target Values
	GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);		actLoopMaxVel	= (float)atof(szText)*gearFactor;
	GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);		actLoopMaxAcc	= (float)atof(szText)*gearFactor;
	
	//float auxfLoopMaxVel = (float)(actLoopMaxVel);//[rev/s] - motor
	//float auxfLoopMaxAcc = (float)(actLoopMaxAcc);//[rev/s^2] - motor

	if ( fabs(fLoopMaxVel - actLoopMaxVel) > 1.)
	{
		fLoopMaxVel = actLoopMaxVel;
		cecmW.SetProfilerMaxVel(fLoopMaxVel);
	}

	if ( fabs(fLoopMaxAcc - actLoopMaxAcc) > 1.)
	{
		fLoopMaxAcc = actLoopMaxAcc;
		cecmW.SetProfilerMaxAcc(fLoopMaxAcc);
		cecmW.SetProfilerMaxDec(fLoopMaxAcc);//re-use
	}
	//Always before MOTION starts!!
}


void CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVel()
{
	CString szText;

	if (velRunning) {
		SetDlgItemText(IDC_BUTTON_VEL_START, "Start");
	}
	else {//Velocity mode starts

		//Get control value	
		GetDlgItemText(IDC_EDIT_VEL_VEL_DEG_S, szText);
		velTargetVel = (float)(atof(szText));//[deg/s]
		velTargetVel = velTargetVel / 360.;//[rev/s]
		velTargetRPM = (int)velTargetVel * 60;//no gear, no [rad] units --> in [rpm] units
		velTargetVelValue = velTargetRPM + 7500;	//TODO : why?

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}
	velRunning = !velRunning;

	scVelocity.SetPos((int)velTargetVel);//update Slider control in [rev/s]

	UpdateProfilerLimits();

	UpdateVelMotionInterface();
}

void CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelRpm()
{
	CString szText;

	if (velRunning){
		SetDlgItemText(IDC_BUTTON_VEL_START, "Start");
	}
	else{//Velocity mode starts

		//Get control value	
		velTargetRPM		= GetDlgItemInt(IDC_EDIT_VEL_VEL_RPM);//[RPM]
		velTargetVel		= (double)velTargetRPM / 60.;//no gear, no [rad] units --> in [rev/s] units
		velTargetVelValue	= velTargetRPM + 7500;	//TODO : why?

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}
	velRunning = !velRunning;

	//scVelocity.SetPos(velTargetVelValue);//TODO : why?
	scVelocity.SetPos((int)velTargetVel);//[rev/s]

	//Update Profiler limits from RAMP group box
	UpdateProfilerLimits();

	UpdateVelMotionInterface();
}


void CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelAcc()
{
	CString szText;

	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_BUTTON_VEL_SET_VEL_ACC, "To be done");

	//Set Profiler limits
	GetDlgItemText(IDC_EDIT_VEL_ACC_DEG_S2, szText);
	// TODO : Do something with Acceleration set by User

	UpdateProfilerLimits();

	UpdateVelMotionInterface();
}

void CRSSMotorCtrlDlg::UpdateProfilerLimits() {

	CString szText;

	//Get Profiler limits from user
	GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
	fLoopMaxVel = (float)atof(szText)*gearFactor;
	GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
	fLoopMaxAcc = (float)atof(szText)*gearFactor;
	//Send Profiler limits to drive (motor)
	//cecmW.SetProfiler(&fLoopMaxVel, &fLoopMaxAcc);
	float tVel = 20.0;
	float tAcc = 20.0;
	cecmW.SetProfiler(&tVel, &tAcc);//Test
}

void CRSSMotorCtrlDlg::UpdateVelMotionInterface()
{
	CString szData;

	SetDlgItemInt(IDC_EDIT_VEL_VEL_RPM, (unsigned int)(velTargetVel*60.));

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
		//GetDlgItemText(IDC_EDIT_VEL_VEL_RPM, szText);
		//velTargetRPM		= atof(szText);//[rpm]
		velTargetRPM		= GetDlgItemInt(IDC_EDIT_VEL_VEL_RPM);//[rpm]
		velTargetVel		= velTargetRPM / 60.0;//[rev/s]
		velTargetVelValue	= velTargetRPM + 7500;//[rpm]+7500
		
		//Set Profiler limits
		UpdateProfilerLimits();
		/*GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
		fLoopMaxVel = (float)atof(szText);
		GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
		fLoopMaxAcc = (float)atof(szText);
		cecmW.SetProfiler(&fLoopMaxVel, &fLoopMaxAcc);*/

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}

	velRunning = !velRunning;
}

void CRSSMotorCtrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar *)&scVelocity)
	{
		velTargetVel = (float)scVelocity.GetPos();//instead of velTargetVelValue
		UpdateVelMotionValues();
	}
	else if (pScrollBar == (CScrollBar *)&scAcceleration)
	{
		velTargetAccValue = scAcceleration.GetPos();
		UpdateVelMotionValues();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRSSMotorCtrlDlg::UpdateVelMotionValues()
{
	velTargetRPM		= (int)(velTargetVel * 60.);
	velTargetVelValue	= velTargetRPM + 7500;
	velTargetAcc		= (velTargetAccValue - 2500)*CV_PI / 1800.0;

	//old
	//velTargetRPM = velTargetVelValue - 7500;
	//velTargetVel = velTargetRPM * 2 * CV_PI / (gearFactor*60.0);
	//velTargetAcc = (velTargetAccValue - 2500)*CV_PI / 1800.0;
	
	UpdateVelMotionInterface();
}


void CRSSMotorCtrlDlg::OnBnClickedButtonRecData()
{
	if (recCurData)
	{
		//Close File
		curDataFile.Close();
		recCurData = false;

		//Update Interface
		SetDlgItemText(IDC_BUTTON_REC_DATA, "Rec. Data");
	}
	else
	{
		//Generate directory name
		CString szDirT, szDir;
		szDirT.Format(genericFolder, CTime::GetCurrentTime().Format("%Y%m%d"));
		CreateDirectory(szDirT, NULL);

		//Create Directory
		szDir.Format(uniqueFolder, szDirT, "logs");
		CreateDirectory(szDir, NULL);

		//Calib filename
		CString szFile;
		szFile.Format("%s\\data_info_%s.txt", szDir, CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));
		curDataFile.Open(szFile, CFile::modeCreate | CFile::modeWrite, 0);

		//Init control variables
		recCurData = true;
		recCurSample = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&recCurITime);

		//Write Header Infor
		//CString szLine; szLine.Format("Sample\tAbsPos\tPos\tTarget\tError\tVel\tTVel\tParamA\tParamB\tParamC\tRawA\tRawB\tRawC\tRawM\tCurA\tCurB\tCurC\tCurT\tCurM\tPosM\tTPosM\tVelM\tTVelM\tAccM\tTAccM\tTimeStamp\r\n");
		CString szLine; 
		szLine.Format("Sample\tActPos[enc]\tTarPos[enc]\tActVel[mrev/s]\tTarVel[mrev/s]\tActAcc[mrev/s^2]\tTarAcc[mrev/s^2]\tPrimaryT[ºC]\tMotorT[ºC]\tTimeStamp\r\n");
		curDataFile.Write(szLine.GetBuffer(), szLine.GetLength());

		//Update Interface
		SetDlgItemText(IDC_BUTTON_REC_DATA, "Stop");
	}
}

void CRSSMotorCtrlDlg::SetGearRatio()
{
	if (bSetGearRatio)
	{
		CString szGearRatio;

		GetDlgItemText(IDC_EDIT_DEV_GFACTOR, szGearRatio);
		gearFactor = atof(szGearRatio);

		bSetGearRatio = false;
	}
}

////////////////////////////////////////////////////////////////
// SECTION: RECORD INFORMATION	(Temperature and Device info)						
////////////////////////////////////////////////////////////////

//Temperature
void CRSSMotorCtrlDlg::InitRecordTemp()
{
	//Init control variables
	bRecordTemp			= false;
	iDataTempRecord		= 0;

	//Update Interface Text
	SetDlgItemText(IDC_BUTTON_REC_T, "Rec. ºC");//Default Start text
}

void CRSSMotorCtrlDlg::OnBnClickedButtonRecT()
{
	//Update Control Variable
	bRecordTemp = !bRecordTemp;

	if (bRecordTemp)
	{
		//Generate directory name
		CString szDirT, szDir;
		szDirT.Format(genericFolder, CTime::GetCurrentTime().Format("%Y%m%d"));
		CreateDirectory(szDirT, NULL);

		//Create Directory
		szDir.Format(uniqueFolder, szDirT, "temps");
		CreateDirectory(szDir, NULL);

		//Calib filename
		CString szFile;
		CString szFilename;
		szFilename.Format("temp_info_%s.txt", CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));
		szFile.Format(uniqueFolder, szDir, szFilename);
		fTempInfo.Open(szFile, CFile::modeCreate | CFile::modeWrite, 0);

		///////////////////////////////////////////////////////
		// HEADER

		CString szData;

		//Info
		szData.Format("\r\nRSS-400 TEMPERATURES LOG FILE\r\n*****************************\r\n");
		fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

		//Path
		szData.Format("PATH: %s\r\n", szDir);
		fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

		//File
		szData.Format("FILENAME: %s\r\n", szFilename);
		fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

		//Date
		szData.Format("DATE: %s\r\n\r\n", CTime::GetCurrentTime().Format("%Y/%m/%d-%H:%M:%S"));
		fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

		//COLUMNS
		if (bMotorAddTemps)
		{
			szData.Format("TimeId\tAmp\tCurrent\tT_FaseA\tT_FaseB\tT_FaseC\tT_Motor\tT_Fre\tT_Int\tT_Amb\tT_6011\tT_DAC\tT_Tapa\tT_5015\tTPC\r\n");
		}
		else
		{
			szData.Format("TimeId\tAmp\tT_FaseA\tT_FaseB\tT_FaseC\tT_Motor\tCurrent\tTPC\r\n");
		}

		fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

		//Init control variables
		QueryPerformanceCounter((LARGE_INTEGER*)&iTimeStamp);
		iDataTempRecord = 0;

		//Update Interface Text
		SetDlgItemText(IDC_BUTTON_REC_T, "Stop");
	}
	else
	{
		//Close log file
		fTempInfo.Close();

		//Update Interface Text
		SetDlgItemText(IDC_BUTTON_REC_T, "Rec. ºC");
	}
}

void CRSSMotorCtrlDlg::RecordTemp()
{
	if (bRecordTemp)
	{
		__int64 iActTimeStamp, iFrequency;
		QueryPerformanceCounter((LARGE_INTEGER*)&iActTimeStamp);
		QueryPerformanceFrequency((LARGE_INTEGER*)&iFrequency);
		int elapsedTime = (int)((iActTimeStamp - iTimeStamp)*1000.0 / iFrequency);

		/////////////////////////////////////////////////////
		// TEMPERATURE STABILIZATION

		//Update temperature data
		dTBuffer[0][iTIdx] = dTemps[TEMP_T1];
		dTBuffer[1][iTIdx] = dTemps[TEMP_T2];
		dTBuffer[2][iTIdx] = dTemps[TEMP_T3];
		dTBuffer[3][iTIdx] = dTemps[TEMP_M];
		dTBuffer[4][iTIdx] = motorAddTemps[0];
		dTBuffer[5][iTIdx] = motorAddTemps[1];
		dTBuffer[6][iTIdx] = motorAddTemps[2];
		dTBuffer[7][iTIdx] = motorAddTemps[3];
		dTBuffer[8][iTIdx] = motorAddTemps[4];
		dTBuffer[9][iTIdx] = motorAddTemps[5];
		dTBuffer[10][iTIdx] = motorAddTemps[6];

		//Update Indexes
		iTIdx = (int)(elapsedTime / 10000);
		iTIdx = iTIdx % STABLE_TIME;
		int idxA = iTIdx % STABLE_TIME;
		int idxB = (iTIdx + 1) % STABLE_TIME;
		int firstHour = elapsedTime / 1000;

		//Reset max inc temperature
		dMaxIncTemp = 0;

		//Calcule temperatura increment
		for (int i = 0; i < STABLE_TEMPS; i++)
		{
			//First hour					
			if (firstHour < 3600)
			{
				dTInc[i] = (dTBuffer[i][idxA] - dTBuffer[i][0])*STABLE_TIME / iTIdx;
			}
			else
			{
				dTInc[i] = dTBuffer[i][idxA] - dTBuffer[i][idxB];
			}

			//Calcule max inc temperature
			if (dTInc[i] > dMaxIncTemp) dMaxIncTemp = dTInc[i];
		}

		//Max Internal Ambient Temperature
		dAmbIncTemp = dTInc[5];		//Internat temperature

		//Generate String
		CString szAddTemp;
		szAddTemp.Format("T1: %.1f  T2: %.1f  T3: %.1f  T4: %.1f  T5: %.1f \r\nT6: %.1f  T7: %.1f  T8: %.1f  T9: %.1f  T10: %.1f  TA: %.1f\r\nMaxInc: %.1f  AmbInc: %.1f  Seconds: %d",
			dTemps[TEMP_M],				//Motor
			motorAddTemps[0],			//Fre
			motorAddTemps[3],			//DSPIC6011
			motorAddTemps[6],			//DSPIC5015
			motorAddTemps[4],			//DAC
			dTemps[TEMP_T1],			//QA
			dTemps[TEMP_T2],			//QB
			dTemps[TEMP_T3],			//QC
			motorAddTemps[5],			//Tapa
			motorAddTemps[1],			//Ambient Interior
			motorAddTemps[2],			//TA
			dMaxIncTemp,				//Maximium temperature increment from last 1 hour
			dAmbIncTemp,				//Maximium internal ambient temperature increment from last 1 hour
			elapsedTime / 1000
		);

		

		//Record with frequency
		if (iDataTempRecord % 40 == 0)
		{
			//Calcule actual current			
			/*double curA = -0.0313*dCurrent[CURRENT_A] + 15.791;
			double curB = -0.0313*dCurrent[CURRENT_B] + 15.791;
			double curC = -0.0313*dCurrent[CURRENT_C] + 15.791;*/
			double curA = 0.0;
			double curB = 0.0;
			double curC = 0.0;
			double curT = (curA + curB + curC) / 2.;

			CString szData;

			if (bMotorAddTemps)
			{
				//szData.Format("TimeId\tAmp\tCurrent\tT_FaseA\tT_FaseB\tT_FaseC\tT_Motor\tT_Fre\tT_Int\tT_Amb\tT_6011\tT_DAC\tT_Tapa\tT_5015\tTPC\r\n");


				szData.Format("%d\t%.1f\t%.2f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\r\n",
					iDataTempRecord / 40,
					0.0,//motorParameters[BCAN_PARAM_AW_ACTROTORAMPLITUDE],
					curT,

					dTemps[TEMP_T1],
					dTemps[TEMP_T2],
					dTemps[TEMP_T3],
					dTemps[TEMP_M],
					motorAddTemps[0],
					motorAddTemps[1],
					motorAddTemps[2],
					motorAddTemps[3],
					motorAddTemps[4],
					motorAddTemps[5],
					motorAddTemps[6],

					elapsedTime / 1000);

				SetDlgItemText(IDC_STATIC_TEMP_SHOWDATA, szAddTemp);
			}
			else
			{
				szData.Format("%d\t%.1f\t%.2f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\r\n",
					iDataTempRecord / 40,
					0.0,//motorParameters[BCAN_PARAM_AW_ACTROTORAMPLITUDE],
					curT,
					dTemps[TEMP_T1],
					dTemps[TEMP_T2],
					dTemps[TEMP_T3],
					dTemps[TEMP_M],

					elapsedTime / 1000);
			}

			fTempInfo.Write(szData.GetBuffer(), szData.GetLength());

			//Show info interface
			CString szDlgInfo;
			szDlgInfo.Format("%d - %.1f°C %.1f°C %.1f°C %.1f°C %.2fA", iDataTempRecord / 40, dTemps[TEMP_T1], dTemps[TEMP_T2], dTemps[TEMP_T3], dTemps[TEMP_M], curT);
			SetDlgItemText(IDC_STATIC_TEMP_SHOWDATA, szDlgInfo);
		}

		//Show/Update record information
		if (iDataTempRecord % 10 == 0) {
			static CString szDlgInfo;
			szDlgInfo.Format("%d - %llu[kB]\r\n", iDataTempRecord, fTempInfo.GetLength() / 1000);
			SetDlgItemText(IDC_EDIT_REC_TEMPINFO, szDlgInfo);
		}

		iDataTempRecord++;
	}
}


//Device Data
void CRSSMotorCtrlDlg::InitRecordDeviceData()
{
	//Init control variables
	recCurData		= false;
	recCurSample	= 0;

	//Update Interface Text
	SetDlgItemText(IDC_BUTTON_REC_DATA, "Rec. Data");
}

void CRSSMotorCtrlDlg::RecordDeviceData()
{
	//Current Device Data
	if (curDataFile == NULL || !recCurData) return;

	__int64 recCurATime, freq;

	QueryPerformanceCounter((LARGE_INTEGER*)&recCurATime);
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	int timestamp = (int)((recCurATime - recCurITime)*1000.0 / freq);


	CString szLine; szLine.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\r\n",

		recCurSample,													//Current sample	
		motorParameters[BETH_PARAM_AR_ACTPOS],							//Actual Module Position
		motorParameters[BETH_PARAM_AW_TARGET_POS],						//Actual Module Target Position

		motorParameters[BETH_PARAM_AR_ACTVEL],							//Actual Module Velocity
		motorParameters[BETH_PARAM_AW_TARGET_VEL],						//Actutal Module Target Velocity

		motorParameters[BETH_PARAM_AR_ACTACC],							//Actual Module Acceleration
		motorParameters[BETH_PARAM_AW_TARGET_ACC],						//Actual Module Target Acceleration
		motorParameters[BETH_PARAM_AR_ACT_TEMP],						//Actual Primary Temperature
		motorParameters[BETH_PARAM_AR_ACT_FTEMP],						//Actual Motor Temperature?

		timestamp);														//Time stamp
	//Write Data register
	curDataFile.Write(szLine.GetBuffer(), szLine.GetLength());

	//Show/Update record information
	if (recCurSample % 10 == 0) {
		CString szDlgInfo;
		szDlgInfo.Format("%d - %llu[kB]", recCurSample, curDataFile.GetLength()/1000);
		SetDlgItemText(IDC_EDIT_REC_DEVINFO, szDlgInfo);
	}

	recCurSample++;
}


void CRSSMotorCtrlDlg::OnCbnSelchangeComboRampUpUnits()
{
	CString m_strItemSelected;

	cmbRampUpUnitsSel.GetLBText(cmbRampUpUnitsSel.GetCurSel(), m_strItemSelected);
	UpdateData(FALSE);

}
void CRSSMotorCtrlDlg::LoadComboBox() {
	CString str = _T("");

	str.Format(_T("[degrees]"));
	cmbRampUpUnitsSel.AddString(str);

	str.Format(_T("[counts]"));
	cmbRampUpUnitsSel.AddString(str);

	str.Format(_T("[radians]"));
	cmbRampUpUnitsSel.AddString(str);
}

void CRSSMotorCtrlDlg::OnBnClickedButtonDevFaultReset(void){
	cecmW.StatusFaultReset();
}


void CRSSMotorCtrlDlg::OnBnClickedCancel()
{
	//New
	FinalizeControlThreads();

	//Old
	CDialogEx::OnCancel();
}
