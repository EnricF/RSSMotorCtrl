
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
// CThermalAnalysisDlg dialog used for Dialog 'Thermal Analysis Info'
//------------------------------------
class CThermalAnalysisDlg : public CDialogEx {
public:
	enum { IDD = IDD_THERMALANALYSIS_DIALOG	};

	CThermalAnalysisDlg();
	//void CThermalAnalysisDlg::setText();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
};

CThermalAnalysisDlg::CThermalAnalysisDlg() :CDialogEx(IDD){

	CStatic* myStatic = new CStatic;
	myStatic->Create(_T("my static"), WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(10, 10, 150, 50), this, 1201);// writing "this" doesn't make it work.
	//How to solve this?
}

void CThermalAnalysisDlg::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
}


//------------------------------------
// CAboutDlg dialog used for Dialog 'About'
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
	, cbThermalAnalysisBool(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRSSMotorCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_VEL_VEL, scVelocity);
	DDX_Control(pDX, IDC_SLIDER_VEL_ACC, scAcceleration);
	DDX_Control(pDX, IDC_COMBO_RAMP_UP_UNITS, cmbRampUpUnitsSel);
	DDX_Control(pDX, IDC_CHECK_ADV_THERMALANALYSIS, cbThermalAnalysis);
	DDX_Check(pDX, IDC_CHECK_ADV_THERMALANALYSIS, cbThermalAnalysisBool);
	DDX_Control(pDX, IDC_COMBO_ADV_SELECTSLAVE, cmbSlaveSel);
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
	ON_BN_CLICKED(IDC_BUTTON_DEV_BRAKE, &CRSSMotorCtrlDlg::OnBnClickedButtonDevBrake)
	ON_BN_CLICKED(IDC_CHECK_ADV_THERMALANALYSIS, &CRSSMotorCtrlDlg::OnBnClickedCheckThermalanalysis)
	ON_CBN_SELCHANGE(IDC_COMBO_RAMP_UP_UNITS, &CRSSMotorCtrlDlg::OnCbnSelchangeComboRampUpUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_ADV_SELECTSLAVE, &CRSSMotorCtrlDlg::OnCbnSelchangeComboAdvSelectslave)
END_MESSAGE_MAP()

//EF mod
char	*logFilename = "C:\\workspace\\Data\\logFile_%s.txt";

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

	WaitForSingleObject(pObject->tECMWorker, INFINITE);//Waits for Worker thread to finish	
	
	//Finish Thread
	pObject->bThreadRIFinished = true;
	
	return 0;
}


//EtherCAT thread
UINT ThreadECMWorker(LPVOID pParam) {
	__int64 initial, final;

	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)pParam;

	pObject->bThreadECMWActive = true;

	//Control bucle
	while (pObject->bThreadECMWActive || pObject->bThreadWActive)
	{
		//Init ECM and loop until end

		pObject->cecmW.Init(__argc, __argv, &pObject->bThreadWActive);//Receives main WORKER thread status (active TRUE or FALSE)
				
		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&final);

		//Initialize time counters
		QueryPerformanceCounter((LARGE_INTEGER*)&initial);

		//Reconnect (loop)
		if(pObject->bThreadECMWActive){
			PLOGI << " ThreadECM thread might be restarted";
			/*if(pObject->cecmW.GetStatus() != 0)//Destroy it only if stills alive
				pObject->cecmW.~CecmW();*/
		}
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
		// COMMANDS - Checks if "Thermal Analysis" check box is selected or not (more things to be done someday)
		pObject->ExecuteCommands();

		////////////////////////////////////////
		// MOTION - Main MOTION loop, calls either position or velocity motion modes
		pObject->ExecuteMotion();

		////////////////////////////////////////
		// PARAMETERS -- Reads CONTROL EditBoxes! (either from RAMP or VEL modes)
		pObject->GetDeviceParameters();

		////////////////////////////////////////
		// UPDATE INTERFACE VALUES - Writes PARAMETERS EditBoxes
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
		
		pObject->iLoopActPos_enc = (int)pObject->dDevData[DEV_MPOS][DEV_ACT];
		//pObject->iLoopPos = pObject->cecmW.GetActPos(slaveActive);

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

	WaitForSingleObject(pObject->tECMWorker, INFINITE);//Waits for ECM thread to finish	

	//Finish Thread
	pObject->bThreadWFinished = true;
	   
	Sleep(1000);

	return 0;
}

// CRSSMotorCtrlDlg message handlers
BOOL CRSSMotorCtrlDlg::OnInitDialog()
{

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1068);//Somewhere
	//_CrtSetBreakAlloc(694);//ThreadECMWorker
	//_CrtSetBreakAlloc(701);//App.DoModal()-->OnInit()
//....

	CString szInfo;

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

	//Start LOG objects
	InitLog();

	//Load list boxes
	LoadComboBox();

	//Load MAP with IngeniaMC/Summit Error Codes - Decoded from STATUS register is FAIL bit is active
	MapErrorCodes(&cecmW.errorCodesMap);

	//Init Device Data
	InitDevData();

	//Init Threads
	InitControlThreads();//EF mod : new thread to Init() EtherCAT communication
	
	//Init Home Position - To be Moved (user button)
	//InitHomeModulePos();

	//Init calibration correction 
	//InitCalibPosition();

	//Offsets Calibration
	//InitOffsetsCalibration();

	//Zeros Calibration
	//InitZerosCalibration();

	//Record data initialization
	InitRecordDeviceData();	//Device (setting variables set to default start values)
	InitRecordTemp();		//Temperature (setting variables set to default start values)

	//Strange but it seems to help App's performance (closing&re-opening)
	CRSSMotorCtrlDlg* pObject = (CRSSMotorCtrlDlg*)this;
	WaitForSingleObject(pObject->tECMWorker, INFINITE);//Waits for Worker thread to finish	
	
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
	//dWorkerTPC			= THREAD_TPC;
	//dECMWorkerTPC		= THREAD_TPC;
	//dSecWorkerTPC		= THREAD_TPC;


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

	//SetTimer(TIMER_EXIT, 5000, 0);
	Sleep(1000);
}

//Homing
void CRSSMotorCtrlDlg::InitHomeModulePos() {
	while (cecmW.GetStatus(slaveActive) == 0) {
		//May still here forever
	};
	homeModulePos_enc = (int)cecmW.GetModuleActPos(slaveActive);			//Save it
	PLOGI << "Module Home Position = " << homeModulePos_enc;
}



void CRSSMotorCtrlDlg::ShowParameter(int id)
{
	CString szInfo;
	//Time variables
	static ULONGLONG tnow	= GetTickCount64();
	static ULONGLONG tprev	= GetTickCount64();

	//Update time info
	tnow = GetTickCount64();

	//typical resolution in range 10:16 ms
	if ( (tnow - tprev) > 10) {
		tprev = tnow; //update

		//if (id < DEV_IND)//undo this mod ASAP
		if (id < 7)//undo this mod ASAP
		{
			//Show Actual, Min, Max and Target values in [counts], [rev/s] or [rev/s^2]
			//if (id == DEV_MPOS || id == DEV_MVEL || id == DEV_MACC) {//MOTOR POSITION
				//szInfo.Format("%.1f", dDevData[id][DEV_ACT] * 360.f / dDevData[id][DEV_RANGE]);
				szInfo.Format("%.0f", dDevData[id][DEV_ACT]);		SetDlgItemText(IDC_EPA1		+ id - 1, szInfo);
				szInfo.Format("%.0f", dDevData[id][DEV_MIN]);		SetDlgItemText(IDC_EPMIN1	+ id - 1, szInfo);
				szInfo.Format("%.0f", dDevData[id][DEV_MAX]);		SetDlgItemText(IDC_EPMAX1	+ id - 1, szInfo);
				szInfo.Format("%.0f", dDevData[id][DEV_TARGET]);	SetDlgItemText(IDC_EPTARGET1+ id - 1, szInfo);
			//}
		}
		else
		{
			//Drive Temperature (internal) [ºC]
			if ( id == DEV_TEMP ) {//No TARGET value --> TODO : modify ID_EDIT name, just like above
				szInfo.Format("%.1f", dDevData[DEV_TEMP][DEV_ACT]);	SetDlgItemText(IDC_EDIT_PARAM_T_ACT, szInfo);
				szInfo.Format("%.1f", dDevData[DEV_TEMP][DEV_MIN]);	SetDlgItemText(IDC_EDIT_PARAM_T_MIN, szInfo);
				szInfo.Format("%.1f", dDevData[DEV_TEMP][DEV_MAX]);	SetDlgItemText(IDC_EDIT_PARAM_T_MAX, szInfo);
			}

			//Motor Temperature [ºC]
			else if ( id == DEV_MTEMP ) {//No TARGET value --> TODO : modify ID_EDIT name, just like above
				szInfo.Format("%.1f", dDevData[DEV_MTEMP][DEV_ACT]);	SetDlgItemText(IDC_EDIT_PARAM_TM_ACT, szInfo);
				szInfo.Format("%.1f", dDevData[DEV_MTEMP][DEV_MIN]);	SetDlgItemText(IDC_EDIT_PARAM_TM_MIN, szInfo);
				szInfo.Format("%.1f", dDevData[DEV_MTEMP][DEV_MAX]);	SetDlgItemText(IDC_EDIT_PARAM_TM_MAX, szInfo);
			}

			//Current [A]
			else if ( id == DEV_CURA || id == DEV_CURB  || id == DEV_CURC ) {//Current - NO TARGET VALUE YET!
				szInfo.Format("%.3f", dDevData[id][DEV_ACT]);		SetDlgItemText(IDC_EPA16 + id - 16, szInfo);
				szInfo.Format("%.3f", dDevData[id][DEV_MIN]);		SetDlgItemText(IDC_EPMIN16 + id - 16, szInfo);
				szInfo.Format("%.3f", dDevData[id][DEV_MAX]);		SetDlgItemText(IDC_EPMAX16 + id - 16, szInfo);
			}

			//Bus voltage [V]
			else if (id == DEV_BUS_V) {
				szInfo.Format("%.2f", dDevData[id][DEV_ACT]);		SetDlgItemText(IDC_EDIT_PARAM_BUS_V_ACT, szInfo);
				szInfo.Format("%.2f", dDevData[id][DEV_MIN]);		SetDlgItemText(IDC_EDIT_PARAM_BUS_V_MIN, szInfo);
				szInfo.Format("%.2f", dDevData[id][DEV_MAX]);		SetDlgItemText(IDC_EDIT_PARAM_BUS_V_MAX, szInfo);
			}
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

	//MODULE Position Initialization
	dDevData[DEV_POS][DEV_ACT] = 0;		dDevData[DEV_POS][DEV_TARGET] = 0;		dDevData[DEV_POS][DEV_MIN] = -1;	dDevData[DEV_POS][DEV_MAX] = -1;	dDevData[DEV_POS][DEV_RANGE] = 360.f;

	//MODULE Velocity Initialization
	dDevData[DEV_VEL][DEV_ACT] = 0;		dDevData[DEV_VEL][DEV_TARGET] = 0;		dDevData[DEV_VEL][DEV_MIN] = -1;	dDevData[DEV_VEL][DEV_MAX] = -1;	dDevData[DEV_VEL][DEV_RANGE] = 360.f;

	//MODULE Acceleration Initialization
	dDevData[DEV_ACC][DEV_ACT] = 0;		dDevData[DEV_ACC][DEV_TARGET] = 0;		dDevData[DEV_ACC][DEV_MIN] = -1;	dDevData[DEV_ACC][DEV_MAX] = -1;	dDevData[DEV_ACC][DEV_RANGE] = 32768.f;

	//MOTOR Position Initialization
	dDevData[DEV_MPOS][DEV_ACT] = 0;	dDevData[DEV_MPOS][DEV_TARGET] = 0;		dDevData[DEV_MPOS][DEV_MIN] = -1;	dDevData[DEV_MPOS][DEV_MAX] = -1;	dDevData[DEV_MPOS][DEV_RANGE] = 2048;

	//MOTOR Velocity Initialization
	dDevData[DEV_MVEL][DEV_ACT] = 0;	dDevData[DEV_MVEL][DEV_TARGET] = 0;		dDevData[DEV_MVEL][DEV_MIN] = -1;	dDevData[DEV_MVEL][DEV_MAX] = -1;	dDevData[DEV_MVEL][DEV_RANGE] = 10000;

	//MOTOR Acceleration Initialization
	dDevData[DEV_MACC][DEV_ACT] = 0;	dDevData[DEV_MACC][DEV_TARGET] = 0;		dDevData[DEV_MACC][DEV_MIN] = -1;	dDevData[DEV_MACC][DEV_MAX] = -1;	dDevData[DEV_MACC][DEV_RANGE] = 20000;

	//MOTOR Temperature Initialization
	dDevData[DEV_MTEMP][DEV_ACT] = 0;	dDevData[DEV_MTEMP][DEV_TARGET] = 0;	dDevData[DEV_MTEMP][DEV_MIN] = -1;	dDevData[DEV_MTEMP][DEV_MAX] = -1;	dDevData[DEV_MTEMP][DEV_RANGE] = 256;

	//DRIVE(INTERNAL) Temperature Initialization
	dDevData[DEV_TEMP][DEV_ACT] = 0;	dDevData[DEV_TEMP][DEV_TARGET] = 0;	dDevData[DEV_TEMP][DEV_MIN] = -1;	dDevData[DEV_TEMP][DEV_MAX] = -1;	dDevData[DEV_TEMP][DEV_RANGE] = 256;

	//MOTOR Current Initialization
	dDevData[DEV_CUR][DEV_ACT] = 0;		dDevData[DEV_CUR][DEV_TARGET] = 0;		dDevData[DEV_CUR][DEV_MIN] = -1;	dDevData[DEV_CUR][DEV_MAX] = -1;	dDevData[DEV_CUR][DEV_RANGE] = 2400;

	//TPC Initialization
	//dDevData[DEV_TPC][DEV_ACT] = 0;		dDevData[DEV_TPC][DEV_TARGET] = 0;		dDevData[DEV_TPC][DEV_MIN] = -1;	dDevData[DEV_TPC][DEV_MAX] = -1;	dDevData[DEV_TPC][DEV_RANGE] = 256.f;//6000;

	//Amplitude Initialization
	dDevData[DEV_AMP][DEV_ACT] = 0;		dDevData[DEV_AMP][DEV_TARGET] = 0;		dDevData[DEV_AMP][DEV_MIN] = -1;	dDevData[DEV_AMP][DEV_MAX] = -1;	dDevData[DEV_AMP][DEV_RANGE] = 240.f;

	//Control Initialization
	//dDevData[DEV_CTRL][DEV_ACT] = 0;	dDevData[DEV_CTRL][DEV_TARGET] = 0;		dDevData[DEV_CTRL][DEV_MIN] = -1;	dDevData[DEV_CTRL][DEV_MAX] = -1;	dDevData[DEV_CTRL][DEV_RANGE] = 256.f;

	//CAN Initialization // Voltage
	//dDevData[DEV_CAN][DEV_ACT] = 0;		dDevData[DEV_CAN][DEV_TARGET] = 0;		dDevData[DEV_CAN][DEV_MIN] = -1;	dDevData[DEV_CAN][DEV_MAX] = -1;	dDevData[DEV_CAN][DEV_RANGE] = 256.f;

	//Param1
	//dDevData[DEV_PARAM1][DEV_ACT] = 0;	dDevData[DEV_PARAM1][DEV_TARGET] = 0;	dDevData[DEV_PARAM1][DEV_MIN] = -1;	dDevData[DEV_PARAM1][DEV_MAX] = -1;	dDevData[DEV_PARAM1][DEV_RANGE] = 4096.f;

	//Param2
	//dDevData[DEV_PARAM2][DEV_ACT] = 0;	dDevData[DEV_PARAM2][DEV_TARGET] = 0;	dDevData[DEV_PARAM2][DEV_MIN] = -1;	dDevData[DEV_PARAM2][DEV_MAX] = -1;	dDevData[DEV_PARAM2][DEV_RANGE] = 4096.f;

	//Param3
	//dDevData[DEV_PARAM3][DEV_ACT] = 0;	dDevData[DEV_PARAM3][DEV_TARGET] = 0;	dDevData[DEV_PARAM3][DEV_MIN] = -1;	dDevData[DEV_PARAM3][DEV_MAX] = -1;	dDevData[DEV_PARAM3][DEV_RANGE] = 4096.f;

	//Current A
	dDevData[DEV_CURA][DEV_ACT] = 0;	dDevData[DEV_CURA][DEV_TARGET] = 0;		dDevData[DEV_CURA][DEV_MIN] = -1;	dDevData[DEV_CURA][DEV_MAX] = -1;	dDevData[DEV_CURA][DEV_RANGE] = 1024.f;

	//Current B
	dDevData[DEV_CURB][DEV_ACT] = 0;	dDevData[DEV_CURB][DEV_TARGET] = 0;		dDevData[DEV_CURB][DEV_MIN] = -1;	dDevData[DEV_CURB][DEV_MAX] = -1;	dDevData[DEV_CURB][DEV_RANGE] = 1024.f;

	//Current C
	dDevData[DEV_CURC][DEV_ACT] = 0;	dDevData[DEV_CURC][DEV_TARGET] = 0;		dDevData[DEV_CURC][DEV_MIN] = -1;	dDevData[DEV_CURC][DEV_MAX] = -1;	dDevData[DEV_CURC][DEV_RANGE] = 1024.f;

	//Current Error???
	//or Bus Voltage
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
	
	//Status Variables
	bMotorHalted = false;
	
	//Setting motor specification parameters
	motorParameters[BETH_PARAM_DR_HOMEOFFSET]	= 0.;	// TODO : defined by USER in run-time?
	motorParameters[BETH_PARAM_DR_GEARRATIO]	= 1200.;
	motorParameters[BETH_PARAM_DR_MINPOS]		= 0.;
	motorParameters[BETH_PARAM_DR_MAXPOS]		= ENC_MODULE_DR * ENC_MOTOR_DR*gearFactor;//A full loop (outter)
	motorParameters[BETH_PARAM_DR_MINVEL]		= -MOTOR_VEL_DR_REV_S/2;
	motorParameters[BETH_PARAM_DR_MAXVEL]		= MOTOR_VEL_DR_REV_S/2;
	motorParameters[BETH_PARAM_DR_MINACC]		= -MOTOR_ACC_DR_REV_S/2;
	motorParameters[BETH_PARAM_DR_MAXACC]		= MOTOR_ACC_DR_REV_S/2;

	//Init Ramp GroupBox variables and Edit Boxes
	InitRampGB();

	//InitVelGB();

	//Sliders controls
	scVelocity.SetRange( (int)(-MOTOR_VEL_DR_REV_S * 360 / 2), (int) MOTOR_VEL_DR_REV_S * 360 / 2, TRUE);
	scVelocity.SetPos(0);
	velTargetVel = scVelocity.GetPos()/360.;//store SC value
	scAcceleration.SetRange(0, (int)MOTOR_ACC_DR_REV_S * 360 / 2, TRUE);
	scAcceleration.SetPos(MOTOR_ACC_DR_REV_S * 360 / (2*10) );//10%
	velTargetAcc = scAcceleration.GetPos()/360.;//store SC value
	SetDlgItemInt(IDC_EDIT_VEL_VEL_DEG_S, scVelocity.GetPos());
	SetDlgItemInt(IDC_EDIT_VEL_VEL_RPM, scVelocity.GetPos()*60);
	SetDlgItemInt(IDC_EDIT_VEL_ACC_DEG_S2, scAcceleration.GetPos());

	//Set STATUS Semaphore picture to yellow (Start)
	picStatusSemaphore.LoadBitmapA("res\\StatusSemaphoreYellow.bmp");
}

void CRSSMotorCtrlDlg::SetLoopValue(int id_editbox, float * var) {
	CString szText;

	GetDlgItemText(id_editbox, szText);

	if (cmbRampUpUnitsSel.GetCurSel() == DEGREES) {//DEGREES
		*var = (float)(atof(szText)*ENC_MOTOR_DR*gearFactor/360.);			//[encoder counts] - Motor(inner)
	}
	else if (cmbRampUpUnitsSel.GetCurSel() == COUNTS) {//COUNTS
		*var = (float)(atof(szText)*ENC_MOTOR_DR*gearFactor/ENC_MODULE_DR);	//[encoder counts] - Motor(inner)
	}
	else if (cmbRampUpUnitsSel.GetCurSel() == RADIANS) {//RADIANS 
		*var = (float)(atof(szText)*ENC_MOTOR_DR*gearFactor / (2 * CV_PI)); //[encoder counts] - Motor(inner)
	}
	else {//COUNTS by default (no more options yet)
		*var = (float)(atof(szText)*gearFactor);							//[encoder counts] - Motor(inner)
	}
}

void CRSSMotorCtrlDlg::OnBnClickedButtonRampSend()
{
	//1.Get GUI value in [encoder counts]
	SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial_EB_enc);
	//1.1. Set TargetPos in [encoder counts] - Motor(inner)
	iLoopTargetPos_enc = (int)fLoopInitial_EB_enc;
	
	PLOGI << "Click on RAMP SEND button with value " << iLoopTargetPos_enc << " [enc]";
	
	//2.Update Profile limits/settings
	UpdateProfilerLimits();

	//3. Set single position motion variables
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
		//1. Get GUI value in [encoder counts]
		SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial_EB_enc);
		//1.1. Set TargetPos in [encoder counts] - Motor(inner)
		iLoopTargetPos_enc = (int)fLoopInitial_EB_enc;

		PLOGI << "Click on START RAMP LOOP button with an initial value of " << iLoopTargetPos_enc << " [enc]";
					   
		//2. Set PROFILER! Never forget to do this before any other EtherCAT command to the slave
		UpdateProfilerLimits();

		//3. Get/save loop position
		UpdateLoopValues();

		//4. Set loop start variables
		motionMode			= DEV_M_POS;
		iDelayCounter		= 0;
		iLoopState			= LOOP_STATE_INITIAL;
		bSendLoopCommand	= true;
	}
	else { //STOP
		PLOGI << "Click on STOP RAMP LOOP button";
		//1. Restart loop start variables
		motionMode			= DEV_M_STOP;//Needed?
		iDelayCounter		= 0;
		iLoopState			= LOOP_STATE_INITIAL;
		bSendLoopCommand	= false;
	}
}


//bool CRSSMotorCtrlDlg::LoopSM(double modulePos)//param was never used
bool CRSSMotorCtrlDlg::LoopSM(void)
{
	switch (iLoopState)
	{
	case LOOP_STATE_INITIAL:
	{
		if (abs(iLoopTargetPos_enc - iLoopActPos_enc) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_UPPER;
				iLoopTargetPos_enc = (int)iLoopUpper_enc;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}

	case LOOP_STATE_GO_UPPER:
	{
		if (abs(iLoopTargetPos_enc - iLoopActPos_enc) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_INITIAL_FU;
				//Update Target Position
				iLoopTargetPos_enc = (int)iLoopInitial_enc;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}
	case LOOP_STATE_GO_INITIAL_FU:
	{

		if (abs(iLoopTargetPos_enc - iLoopActPos_enc) < LOOP_MIN_ERROR_ENC)
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
			//Update Target Position
			iLoopTargetPos_enc = (int)iLoopLower_enc;
			iDelayCounter = 0;
			bSendLoopCommand = true;
		}
		else iDelayCounter++;
		break;
	}
	case LOOP_STATE_GO_LOWER:
	{
		if (abs(iLoopTargetPos_enc - iLoopActPos_enc) < LOOP_MIN_ERROR_ENC)
		{
			iDelayCounter++;

			if (iDelayCounter >= iLoopDelay)
			{
				iLoopState = LOOP_STATE_GO_INITIAL_FL;
				//Update Target Position
				iLoopTargetPos_enc = (int)iLoopInitial_enc;
				iDelayCounter = 0;
				bSendLoopCommand = true;
			}
		}
		break;
	}
	case LOOP_STATE_GO_INITIAL_FL:
	{
		if (abs(iLoopTargetPos_enc - iLoopActPos_enc) < LOOP_MIN_ERROR_ENC)
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
			//Update Target Position
			iLoopTargetPos_enc = (int)iLoopUpper_enc;
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
			isFinished = cecmW.MotionFRampMode(iLoopTargetPos_enc, slaveActive);//single drive condition
			if(isFinished)	PLOGI << "iLoopTargetPos_enc = " << iLoopTargetPos_enc;//Just once

			bSendLoopCommand = !isFinished;//testing
		}
	}
	//Normal Mode - Single position
	else if (bSendRamp)
	{
		isFinished = cecmW.MotionFRampMode(iLoopTargetPos_enc, slaveActive);//single drive condition
		if (isFinished)	PLOGI << "iLoopTargetPos_enc = " << iLoopTargetPos_enc;//Just once
		bSendRamp = !isFinished;//testing
	}
}


void CRSSMotorCtrlDlg::ExecuteVelMotionControl()
{
	static bool isFinished = false;

	if (!isFinished) {
		//Start Profile Velocity mode!
		isFinished = cecmW.MotionFProfileVelMode((float)velTargetVel);//single drive condition
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

void CRSSMotorCtrlDlg::ExecuteCommands(){
	//Set Module Zero Offset
	//SetModuleZero();

	//Set Rotor Zero Offset
	//SetRotorZero();

	//Set Gear Ratio
	SetGearRatio();

	//Check - Thermal Analysis
	if(cbThermalAnalysisBool)
		CheckThermalAnalysis();
}


void CRSSMotorCtrlDlg::CheckThermalAnalysis(void) {

	CString szText;//To show messages in Thermal Analysis Dialog

	static int elapsedTimeTotal_seconds;	//Total time lapse
	static int prevTempUpdate_seconds;		//Previous Temperature update value update timetag
	static int maxDuration = STABLE_TIME_PERIOD_S * 5;

	static bool timePrevIsUpdated = false;//To know if a temperature sample in stable range is found
	
	static std::vector<float>	temperatures;
	static std::vector<float>::iterator it;//write
	static std::vector<float>::iterator itr;//read
	static int					samplesMax	= (int)STABLE_TIME_PERIOD_S;
	static int					samplesQ	= 0;//samples in vector
	/*static double sum		= 0.;//The sumatory of all temperature values
	static double mean		= 0.;//The mean temperature value
	static double sum_pot2	= 0.;//The square difference between a temperature sample and the mean temperature
	static double variance	= 0.;
	static double typ_dev	= 0.;*/
	static bool	  isReady	= false;
	
	//Temperature variables
	float min_val;
	float max_val;
	float temperature_diff;
	   
	//initial values
	if (!isReady && (motorParameters[BETH_PARAM_AR_ACT_FTEMP] != 0.0) ) {//Set INITIAL value
		//temperature_prev	= (float)motorParameters[BETH_PARAM_AR_ACT_FTEMP];
		temperatures.resize(STABLE_TIME_PERIOD_S);
		//temperatures.push_back( (float)motorParameters[BETH_PARAM_AR_ACT_FTEMP] );//[0]
		it = temperatures.end();
		isReady = true;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&ThermalAnalysisTimeNow);
	elapsedTimeTotal_seconds = (int)((ThermalAnalysisTimeNow - ThermalAnalysisTimeStart) / ThermalAnalysisFreq);
	if (prevTempUpdate_seconds == 0.0)//Set INITIAL value
		prevTempUpdate_seconds = elapsedTimeTotal_seconds;

	//Main algorithm
	if ( (elapsedTimeTotal_seconds < maxDuration) && isReady ) {//MAX. ANALYSIS PERIOD

		if ( (elapsedTimeTotal_seconds - prevTempUpdate_seconds) >= 1 ) {//checks every second approx.

			prevTempUpdate_seconds = elapsedTimeTotal_seconds;//Update prev Temperature value update timetag

			if (it == temperatures.end())//overflow?????
				it = temperatures.begin();
			else
				it++;

			//Estandar deviation calcs (obsolete)
/*
			//Update mean only if vector is already FULL of real samples
			if ( samplesQ == STABLE_TIME_PERIOD_S ) {
				sum -= (double)*it;//substract oldest sample value
				sum += motorParameters[BETH_PARAM_AR_ACT_FTEMP];//add newest sample value
				mean = sum / samplesMax;//update mean value
				
			}
			else {//Add temperature to sum - keep filling vector until it's full
				sum += (double)motorParameters[BETH_PARAM_AR_ACT_FTEMP];
				//Increase real samples quantity in temperature vector
				samplesQ++;
				if(samplesQ == STABLE_TIME_PERIOD_S)
					mean = sum / samplesMax;//First mean value
			}
			*/

			//Insert/Update temperatures vector
			*it = (float)motorParameters[BETH_PARAM_AR_ACT_FTEMP];//Add new sample

			//Increase real samples quantity in temperature vector
			if (samplesQ < STABLE_TIME_PERIOD_S)	samplesQ++;

			//Set values out of real temperature range to be updated easily with real temperature values
			min_val = 1000.;
			max_val = -100.;
			//Update MIN or MAX values in temperature vector
			for (itr = temperatures.begin(); itr < temperatures.end(); itr++) {
				if (*itr < min_val)	min_val = *itr;
				if (*itr > max_val)	max_val = *itr;
			}
			//Calculate (MAX - MIN) difference 
			temperature_diff = max_val - min_val;

			//Check if temperature is already stabilized or not
			//But only if elapsed time is more than one hour and the temperature vector is full
			if ( (elapsedTimeTotal_seconds >= STABLE_TIME_PERIOD_S) && (samplesQ == STABLE_TIME_PERIOD_S) ) {
				
				//Estandar deviation calcs (obsolete)
				/*
				//Iterate through each sample
				for (itr = temperatures.begin(); itr < temperatures.end(); itr++) {
					double base = *itr - mean;
					static double exp = 2.0;
					sum_pot2 += pow(base, exp);// element_diff^2
				}

				//Divide by total samples
				variance = sum_pot2 / (double)STABLE_TIME_PERIOD_S;//[ºC^2]
				//Calculate typical deviation
				typ_dev = sqrt(variance);//[ºC]

				if (typ_dev < TEMPERATURE_TYP_DEV) {*/
				if (temperature_diff <= TEMPERATURE_MAX_DIFF){
					//END: TEMPERATURE IS STABILIZED!!
					szText.Format("'Thermal Analysis' FINISHED SUCCESSFULLY after %d [s] with a max. temperature diff. of %.2f [ºC]",
						(unsigned int)elapsedTimeTotal_seconds, temperature_diff);
					PLOGI << szText;

					//SetDlgItemText(IDC_TA_STATIC, szText);//Someday
					SetDlgItemText(IDC_STATIC_TEMP_SHOWDATA, szText);//Temporal solution
					
					//Show Thermal Analysis Dialog
					CThermalAnalysisDlg().DoModal();
					// TODO : Modify Text in Thermal Analysis Dialog

					//STOP recording
					if (recCurData) {
						recCurData = false;
						curDataFile.Close();
					}
					//CLOSE SW
					FinalizeControlThreads();
				}
			}		
		}
	}
	else {//TIMEOUT - END THERMAL ANALYSIS
		//Close and exit!
		szText.Format("'Thermal Analysis' timeout: Temperature stabilization is not reached after %d [s].", maxDuration);
		SetDlgItemText(IDC_STATIC_TEMP_SHOWDATA, szText);//Temporal solution
		PLOGI << szText;

		CThermalAnalysisDlg().DoModal();
		// TODO : Modify Text in Thermal Analysis Dialog
	}
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


void CRSSMotorCtrlDlg::MapErrorCodes(std::map<uint32_t, CString> *errCodes) 
{
	for (int i = 0; i < 30; i++) {
		errCodes->insert( { err[i].hex, err[i].description } );
		//errCodes->insert( std::make_pair(err[i].hex, err[i].description) );	//another way
	}
}

void CRSSMotorCtrlDlg::ShowErrorCodeDescription(int * lastError, std::map<uint32_t, CString> *errCodes) 
{
	static CString szText;
	static bool isDone = false;
	static std::map<uint32_t, CString>::iterator it;
	
	if (!isDone) {//Do it only once
		//MAP find
		it = errCodes->find((uint32_t)*lastError);

		szText.Format("[0x%08X] %s", (uint32_t)it->first, it->second );
		SetDlgItemText(IDC_STATIC_TEMP_SHOWDATA, szText);//Temporally written here, pending mod.
		//LOG
		PLOGI << szText;

		*lastError = 0;//Set to 0, although it will be rewritten in the next EtherCAT Cycle
		isDone = true;//Do not repeat (not expected more than one error by execution, by now)
	}
}

void CRSSMotorCtrlDlg::GetDeviceParameters()
{
	static short	StatusAct	= 0;
	static int		LastError	= 0;
	static char		pBuffer8[8];
	//char pBuffer16[16];

	StatusAct = cecmW.GetStatus(slaveActive);//CiA register
	if (StatusAct != 0) {//Only if there is REAL data
		
		dDevData[DEV_STATUS][DEV_ACT] = (double)StatusAct;//CiA register
		sprintf(&pBuffer8[0], "0x");
		_itoa((int)dDevData[DEV_STATUS][DEV_ACT], &pBuffer8[2], 16);
		SetDlgItemText(IDC_EDIT_DEV_STATUS, pBuffer8);//temporally done here, to be moved out!

		UpdateStatusSemaphore(&StatusAct);

		//Last error update
		LastError = cecmW.GetLastError(slaveActive);//CiA register
		SetDlgItemInt(IDC_EDIT_DEV_LASTERROR, LastError);//temporally done here, to be moved out!
		if (LastError != 0) {
			//Show Error drescription
			ShowErrorCodeDescription(&LastError, &cecmW.errorCodesMap);
		}

		////////////Actual Values
			//Motor (inner)
		motorParameters[BETH_PARAM_AR_ACTPOS]		= (double)cecmW.GetActPos(slaveActive);
		motorParameters[BETH_PARAM_AR_ACTVEL]		= (double)cecmW.GetActVel(slaveActive);
		motorParameters[BETH_PARAM_AR_ACTACC]		= 0.0;//simplification
			//Module (outter)
		motorParameters[BETH_PARAM_AR_ACTMODULEPOS] = (double)cecmW.GetModuleActPos(slaveActive);
		motorParameters[BETH_PARAM_AR_ACTMODULEVEL] = (double)cecmW.GetActVel(slaveActive) * 1000 / (gearFactor * ENC_MOTOR_DR);// = (double)cecmW.GetModuleActVel();	//TBD
		motorParameters[BETH_PARAM_AR_ACTMODULEACC] = 0.0;// = (double)cecmW.GetModuleActACC();	//TBD
			//Currents
		cecmW.GetCurrentsABC(&motorParameters[BETH_PARAM_AW_CURA], &motorParameters[BETH_PARAM_AW_CURB], &motorParameters[BETH_PARAM_AW_CURC]);
			//Temperatures
		motorParameters[BETH_PARAM_AR_ACT_TEMP]		= (double)cecmW.GetTemperaturePrimary(slaveActive);
		motorParameters[BETH_PARAM_AR_ACT_FTEMP]	= (double)cecmW.GetTemperatureMotor(slaveActive);
			//Bus Voltage
		motorParameters[BETH_PARAM_AW_BUSV]			= (double)cecmW.GetBusVoltage(slaveActive);
		
		////////////Target Values
			//Motor (inner)
		motorParameters[BETH_PARAM_AW_TARGET_POS]		= (double)iLoopActPos_enc;	//simplification
		motorParameters[BETH_PARAM_AW_TARGET_VEL]		= velTargetVel;		//
		motorParameters[BETH_PARAM_AW_TARGET_ACC]		= fLoopMaxVel_enc_s;		// TODO : REVIEW
			//Module (outter)
		motorParameters[BETH_PARAM_AW_TARGETMODULEPOS]	= motorParameters[BETH_PARAM_AW_TARGET_POS] / (gearFactor * ENC_MOTOR_DR);
		motorParameters[BETH_PARAM_AW_TARGETMODULEVEL]	= motorParameters[BETH_PARAM_AW_TARGET_VEL] * 1000 / (gearFactor * ENC_MOTOR_DR);//simplification
		motorParameters[BETH_PARAM_AW_TARGETMODULEACC]	= fLoopMaxAcc_enc_s2;		// TODO : REVIEW
	}
}


void CRSSMotorCtrlDlg::UpdateInterfaceControls()
{
	static CString szInfo = "";

	// Updates Interface EditBoxes in "PARAMETERS" GroupBox by calling function ShowUpdate() once for each selected field
//Core
	//Module
	ShowParameter(DEV_POS);
	ShowParameter(DEV_VEL);
	ShowParameter(DEV_ACC);
	//Motor
	ShowParameter(DEV_MPOS);
	ShowParameter(DEV_MVEL);
	ShowParameter(DEV_MACC);
//Extras
	//Temperatures
	ShowParameter(DEV_TEMP);
	ShowParameter(DEV_MTEMP);
	//Motor/Drive Currents
	ShowParameter(DEV_CURA);
	ShowParameter(DEV_CURB);
	ShowParameter(DEV_CURC);
	//Bus voltage
	ShowParameter(DEV_BUS_V);


	//Load fixed/hard-coded variables
	//NIC+TextID
	if (szInfo == "" & (cecmW.PrimaryNIC[0] != 0) ) {
		szInfo.Format("%s", cecmW.PrimaryNIC);
		SetDlgItemText(IDC_EDIT_DEV_IDNIC, szInfo);
	}

	//TODO : Update STATUS + STATUS_SEMAPHORE + LASTERROR here!
}

void CRSSMotorCtrlDlg::UpdateInterfaceParameters()
{
	CString szState;

	static float MotorToModuleCFactor = (float)(ENC_MODULE_DR / (gearFactor*ENC_MOTOR_DR));//Conversion factor from inner to outter encoder counts
	
	if (0 != cecmW.GetStatus(slaveActive)){ //{Only if there is REAL data
////////////////
// MODULE - outter (wheel)

	//Actual values
		UpdateValue(DEV_POS, motorParameters[BETH_PARAM_AR_ACTMODULEPOS] );	//Module Actual Position			[absolute encoder counts]
		//TO BE DONE - GetRotorActVel() & GetRotorActAcc()
		UpdateValue(DEV_VEL, motorParameters[BETH_PARAM_AR_ACTMODULEVEL]);	//Module Actual Velocity			[mrev/s]
		UpdateValue(DEV_ACC, motorParameters[BETH_PARAM_AR_ACTMODULEACC]);	//Module Actual Acceleration		[rev/s^2]
	//Target values
		UpdateTargetValue(DEV_POS, 0.0 );											//Module target position - TODO : Save HOME and substract it
		UpdateTargetValue(DEV_VEL, velTargetVel * 1000. * MotorToModuleCFactor);	//Module TARGET Velocity		[mrev/s]
		UpdateTargetValue(DEV_ACC, 0.0/*cecmW.GetActAcc() * MotorToModuleCFactor*/);//Module TARGET Acceleration	[rev/s^2]

////////////////
// MOTOR - inner

	//Actual values
		UpdateValue(DEV_MPOS, motorParameters[BETH_PARAM_AR_ACTPOS]);
		UpdateValue(DEV_MVEL, motorParameters[BETH_PARAM_AR_ACTVEL]);
		UpdateValue(DEV_MACC, motorParameters[BETH_PARAM_AR_ACTACC]);
	//Target values
		UpdateTargetValue(DEV_MPOS, (double)iLoopTargetPos_enc);
		UpdateTargetValue(DEV_MVEL, velTargetVel);
		UpdateTargetValue(DEV_MACC, 0.0/*fLoopMaxAcc*/);

	//Actual Primary Temperature
		UpdateValue(DEV_TEMP,	motorParameters[BETH_PARAM_AR_ACT_TEMP]);	//Drive Temperature
		UpdateValue(DEV_MTEMP,	motorParameters[BETH_PARAM_AR_ACT_FTEMP]);	//Motor Temperature

	//Actual Currents (phase-A,B,C)
		UpdateValue(DEV_CURA,	motorParameters[BETH_PARAM_AW_CURA]);	//Current-A
		UpdateValue(DEV_CURB,	motorParameters[BETH_PARAM_AW_CURB]);	//Current-B
		UpdateValue(DEV_CURC,	motorParameters[BETH_PARAM_AW_CURC]);	//Current-C

	//Actual Bus voltage
		UpdateValue(DEV_BUS_V,	motorParameters[BETH_PARAM_AW_BUSV]);	//Bus Voltage
	}

}

//Actual value
void CRSSMotorCtrlDlg::UpdateValue(int id, double value)
{
	dDevData[id][DEV_ACT] = value;

	//float temp = 0.0;
	//temp = dDevData[id][DEV_ACT];

	//Update Max-Min
	if (dDevData[id][DEV_MIN] == -1 || dDevData[id][DEV_ACT] < dDevData[id][DEV_MIN]) 
		dDevData[id][DEV_MIN] = dDevData[id][DEV_ACT];
	if (dDevData[id][DEV_MAX] == -1 || dDevData[id][DEV_ACT] > dDevData[id][DEV_MAX]) {
		dDevData[id][DEV_MAX] = dDevData[id][DEV_ACT];
	}
	/*if (id == DEV_MPOS) {//MOTOR
		int a = 0;
		a++;
		a--;
	}

	if (id == DEV_POS) {//MODULE
		int b = 0;
		b++;
		b--;
	}*/
}

void CRSSMotorCtrlDlg::UpdateTargetValue(int id, double value)
{
	dDevData[id][DEV_TARGET] = value;

	/*if (id == DEV_MPOS) {//MOTOR
		int a = 0;
		a++;
		a--;
	}

	if (id == DEV_POS) {//MODULE
		int b = 0;
		b++;
		b--;
	}*/
}

void CRSSMotorCtrlDlg::UpdateStatusSemaphore(short * val) {

	static short prev_val = 0;
	static CStatic *pic = (CStatic *)GetDlgItem(IDC_STATIC_STATUS_SEMAPHORE);//Status Semaphore picture
	static CStatic *but = (CStatic *)GetDlgItem(IDC_BUTTON_DEV_FAULT_RESET);//Fault Reset button

	static HBITMAP hbitG = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreGreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	static HBITMAP hbitR = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreGreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (prev_val != *val) {

		if (*val & (1 << 3)) {//FAULT - RED
			//hbitR = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreRed.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			pic->SetBitmap(hbitR);			
			if ( !but->IsWindowVisible() )	but->ShowWindow(SW_SHOW);
			PLOGI << "STATUS semaphore updated to RED (FAULT)";
		}
		else if (*val > 0) {//Any NON-FAULT value - GREEN
			//hbitG = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCSTR)"res\\StatusSemaphoreGreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			pic->SetBitmap(hbitG);
			if ( but->IsWindowVisible() )	but->ShowWindow(SW_HIDE);
			PLOGI << "STATUS semaphore updated to GREEN (OK)";
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

	PLOGI << "AboudDialog is being shown";
}

//void CRSSMotorCtrlDlg::InitLoopSM()
void CRSSMotorCtrlDlg::InitRampGB()
{
	CString szText;

	bLoopStart		= false;
	bSendRamp		= false;
	bSendLoopCommand= false;
	   
	//Initial values
	gearFactor			= motorParameters[BETH_PARAM_DR_GEARRATIO];
	iLoopDelay			= 2;
	float fLoopInitial	= 10.;	//[degrees/s] - Module
	float fLoopUpper	= 20.;	//[degrees/s] - Module
	float fLoopLower	= 0.;	//[degrees/s] - Module
	float fLoopMaxVel	= (float)(motorParameters[BETH_PARAM_DR_MAXVEL]*360./(10.*gearFactor));//[degrees/s] - Module; but GUI default is [degrees/s]
	float fLoopMaxAcc	= (float)(motorParameters[BETH_PARAM_DR_MAXACC]*360./(10.*gearFactor));//[degrees/s^2] - Module; but GUI default is [degrees/s^2]

	//Initial State
	iLoopState		= LOOP_STATE_INITIAL;

	//Load Initial values (some may be converted)
	szText.Format("%.d",  iLoopDelay);		SetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);
	szText.Format("%.2f", fLoopInitial);	SetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
	szText.Format("%.2f", fLoopUpper);		SetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);
	szText.Format("%.2f", fLoopLower);		SetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);
	szText.Format("%.2f", fLoopMaxVel);		SetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);//To [º/s] and in-out
	szText.Format("%.2f", fLoopMaxAcc);		SetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);//To [º/s] and in-out
	szText.Format("%.0f", gearFactor);		SetDlgItemText(IDC_EDIT_DEV_GFACTOR, szText);

	cmbRampUpUnitsSel.SetCurSel(DEGREES);//selects default "0" which is [degrees] units
	cmbSlaveSel.SetCurSel(BR1_M1);//O is default (B1_M1)

	PLOGI << "InitLoopSM() function called";
}

void CRSSMotorCtrlDlg::UpdateLoopValues()
{
	CString szText;

	static float actLoopMaxVel;//[rev/s] - motor
	static float actLoopMaxAcc;//[rev/s^2] - motor

	//Load Initial values
	GetDlgItemText(IDC_EDIT_RAMP_DELAY, szText);		iLoopDelay		= atoi(szText);
	
	//Get Loop values from Edit Boxes and save it on loop position variables
	SetLoopValue(IDC_EDIT_RAMP_INITIALPOS, &fLoopInitial_EB_enc);
	SetLoopValue(IDC_EDIT_RAMP_UPPERPOS, &fLoopUpper_EB_enc);
	SetLoopValue(IDC_EDIT_RAMP_LOWERPOS, &fLoopLower_EB_enc);
	//Update INTEGERS
	iLoopInitial_enc	= (int)fLoopInitial_EB_enc;
	iLoopUpper_enc		= (int)fLoopUpper_EB_enc;
	iLoopLower_enc		= (int)fLoopLower_EB_enc;


	//Update profiler settings
	//UpdateProfilerLimits();//NOT BY NOW - Always before MOTION starts!!
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
		velTargetRPM = (int)velTargetVel * 60;//[rpm]

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;
	}
	velRunning = !velRunning;
	PLOGI << "velRunning = " << (int)velRunning;

	scVelocity.SetPos((int)velTargetVel);//update Slider control in [rev/s]

	UpdateProfilerLimits();

	UpdateVelMotionInterface();

	PLOGI << "Click on 'Velocity Set Vel[deg/s]' button";
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
	PLOGI << "velRunning = " << (int)velRunning;

	//scVelocity.SetPos(velTargetVelValue);//TODO : why?
	scVelocity.SetPos((int)velTargetVel);//[rev/s]

	//Update Profiler limits from RAMP group box
	UpdateProfilerLimits();

	UpdateVelMotionInterface();

	PLOGI << "Click on 'Velocity Set RPM mode' button";
}


void CRSSMotorCtrlDlg::OnBnClickedButtonVelSetVelAcc()
{
	CString szText;

	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_BUTTON_VEL_SET_VEL_ACC, "To be done");
	
	//Set Profiler limits
	GetDlgItemText(IDC_EDIT_VEL_ACC_DEG_S2, szText);
	UpdateProfilerLimits();

	UpdateVelMotionInterface();

	PLOGI << "Click on 'SetVel & SetAcc' button: TBD!";
}

void CRSSMotorCtrlDlg::UpdateProfilerLimits()
{
	CString szTextVel, szTextAcc;
	//double vel_rev_s, acc_rev_s2;

	//Get Profiler limits from user
	if (motionMode = DEV_M_POS) {
		GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szTextVel);//options: [degrees/s], [counts/s], [rad/s]
		GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szTextAcc);

		fModuleLoopMaxVel_EB = (float)atof(szTextVel);//Module (outter)
		fModuleLoopMaxAcc_EB = (float)atof(szTextAcc);//Module (outter)

		//Convert units
		if (cmbRampUpUnitsSel.GetCurSel() == DEGREES) {//[degrees/s]
			fLoopMaxVel_enc_s = (float)(fModuleLoopMaxVel_EB*gearFactor / 360.);		//[rev/s] - Motor (inner)
			fLoopMaxAcc_enc_s2 = (float)(fModuleLoopMaxAcc_EB*gearFactor / 360.);		//[rev/s^2] - Motor (inner)
		}
		else if (cmbRampUpUnitsSel.GetCurSel() == COUNTS) {//[counts/s]
			fLoopMaxVel_enc_s = (float)(fModuleLoopMaxVel_EB*gearFactor / (ENC_MODULE_DR));	//[rev/s] - Motor (inner)
			fLoopMaxAcc_enc_s2 = (float)(fModuleLoopMaxAcc_EB*gearFactor / (ENC_MODULE_DR));	//[rev/s^2] - Motor (inner)
		}
		else if (cmbRampUpUnitsSel.GetCurSel() == RADIANS) {//[rad/s]
			fLoopMaxVel_enc_s = (float)(fModuleLoopMaxVel_EB*gearFactor / (2 * CV_PI));	//[rev/s] - Motor (inner)
			fLoopMaxAcc_enc_s2 = (float)(fModuleLoopMaxAcc_EB*gearFactor / (2 * CV_PI));	//[rev/s^2] - Motor (inner)
		}
		else {//DEGREES by default (no more options by now)
			fLoopMaxVel_enc_s = (float)(fModuleLoopMaxVel_EB*gearFactor / 360.);		//[rev/s] - Motor (inner)
			fLoopMaxAcc_enc_s2 = (float)(fModuleLoopMaxAcc_EB*gearFactor / 360.);		//[rev/s^2] - Motor (inner)
		}
	}
	//Get profiler limits from user
	else if (motionMode = DEV_M_VEL) {
		GetDlgItemText(IDC_EDIT_VEL_VEL_DEG_S, szTextVel);//options: [degrees/s], [counts/s], [rad/s]
		GetDlgItemText(IDC_EDIT_VEL_ACC_DEG_S2, szTextAcc);
		//TEMPORAL REUSE!
		fModuleLoopMaxVel_EB = (float)atof(szTextVel);//Module (outter)
		fModuleLoopMaxAcc_EB = (float)atof(szTextAcc);//Module (outter)
	}
	else
		PLOGE << "updateing Profiler when not needed?!";

	//Send Profiler limits to drive (motor)
	cecmW.SetProfiler(&fLoopMaxVel_enc_s, &fLoopMaxAcc_enc_s2, slaveActive);

	PLOGI << "Setting/Sending Profiler limits: " << fLoopMaxVel_enc_s << " [rev/s] and " << fLoopMaxAcc_enc_s2 << " [rev/s^2]";
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
		PLOGI << "Click on 'Velocity mode' button: STOP";

		cecmW.SetOperationalState(slaveActive);
		motionMode = DEV_M_STOP;
	}
	else//Velocity mode starts
	{
		//Get Velocity in RPM
		velTargetRPM		= GetDlgItemInt(IDC_EDIT_VEL_VEL_RPM);//[rpm]
		velTargetVel		= velTargetRPM / 60.0;//[rev/s]
		//velTargetVelValue	= velTargetRPM + 7500;//[rpm]+7500
		
		//Set Profiler limits
		UpdateProfilerLimits();

		SetDlgItemText(IDC_BUTTON_VEL_START, "Stop");
		motionMode = DEV_M_VEL;

		PLOGI << "Click on 'Velocity mode' button: START";
	}
	velRunning = !velRunning;
	PLOGI << "velRunning = " << (int)velRunning;
}

void CRSSMotorCtrlDlg::UpdateVelMotionInterface()
{
	CString szData;

	SetDlgItemInt(IDC_EDIT_VEL_VEL_RPM, (unsigned int)(velTargetVel*60.));//[rpm]

	szData.Format("%.3f", velTargetVel*360.);
	SetDlgItemText(IDC_EDIT_VEL_VEL_DEG_S, szData);//[degrees/sec]

	szData.Format("%.3f", velTargetAcc*360.);
	SetDlgItemText(IDC_EDIT_VEL_ACC_DEG_S2, szData);//[degrees/sec^2]
}

void CRSSMotorCtrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar *)&scVelocity)
	{
		velTargetVel = (float)scVelocity.GetPos()/360.;//from [degrees/s] to [rev/s]
		UpdateVelMotionValues();
	}
	else if (pScrollBar == (CScrollBar *)&scAcceleration)
	{
		velTargetAcc = scAcceleration.GetPos()/360.;//from [degrees/s^2] to [rev/s]
		UpdateVelMotionValues();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRSSMotorCtrlDlg::UpdateVelMotionValues()
{
	velTargetRPM		= (int)(velTargetVel * 60.);
	//velTargetAccRPM		= (int)(velTargetAcc/360 * 60.);
	
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

		PLOGI << "Click on 'Device Record Data' button: STOP";
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
		szLine.Format("#Sample\tMActPos[enc]\tMTarPos[enc]\tMActVel[rev/s]\tMTarVel[rev/s]\tMActAcc[rev/s^2]\tMTarAcc[rev/s^2]\tActPos[enc]\tActVel[mrev/s]\tActAcc[mrev/^2]\tPrimaryT[ºC]\tMotorT[ºC]\tCurrentA[A]\tCurrentB[A]\tCurrentC[A]\tTimeStamp\r\n");
		curDataFile.Write(szLine.GetBuffer(), szLine.GetLength());

		//Update Interface
		SetDlgItemText(IDC_BUTTON_REC_DATA, "Stop");

		PLOGI << "Click on 'Device Record Data' button: START";
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
		szData.Format("TimeId\tAmp\tT_FaseA\tT_FaseB\tT_FaseC\tT_Motor\tCurrent\tTPC\r\n");


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
		/*dTBuffer[0][iTIdx] = dTemps[TEMP_T1];
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
		*/

		//Generate String
		CString szAddTemp;
		/*szAddTemp.Format("T1: %.1f  T2: %.1f  T3: %.1f  T4: %.1f  T5: %.1f \r\nT6: %.1f  T7: %.1f  T8: %.1f  T9: %.1f  T10: %.1f  TA: %.1f\r\nMaxInc: %.1f  AmbInc: %.1f  Seconds: %d",
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
		);*/

		

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


			szData.Format("%d\t%.1f\t%.2f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\r\n",
				iDataTempRecord / 40,
				0.0,//motorParameters[BCAN_PARAM_AW_ACTROTORAMPLITUDE],
				curT,
				dTemps[TEMP_T1],
				dTemps[TEMP_T2],
				dTemps[TEMP_T3],
				dTemps[TEMP_M],

				elapsedTime / 1000);

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


	CString szLine; szLine.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\r\n",

		recCurSample,								//Current sample	
		//Motor
		motorParameters[BETH_PARAM_AR_ACTPOS],						
		motorParameters[BETH_PARAM_AW_TARGET_POS],						
		motorParameters[BETH_PARAM_AR_ACTVEL],						
		motorParameters[BETH_PARAM_AW_TARGET_VEL],					
		motorParameters[BETH_PARAM_AR_ACTACC],		
		motorParameters[BETH_PARAM_AW_TARGET_ACC],		
		//Module
		motorParameters[BETH_PARAM_AR_ACTMODULEPOS],
		motorParameters[BETH_PARAM_AR_ACTMODULEVEL],
		motorParameters[BETH_PARAM_AR_ACTMODULEACC],
		//Temperature
		motorParameters[BETH_PARAM_AR_ACT_TEMP],			
		motorParameters[BETH_PARAM_AR_ACT_FTEMP],			
		//Currents
		motorParameters[BETH_PARAM_AW_CURA],					
		motorParameters[BETH_PARAM_AW_CURB],						
		motorParameters[BETH_PARAM_AW_CURC],
		//Bus voltage
		motorParameters[BETH_PARAM_AW_BUSV],

		timestamp);//Time stamp [ms]
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

void CRSSMotorCtrlDlg::convertUnits(int oldUnits, int newUnits, double *val){

	float degreesToCounts	= (float)(ENC_MODULE_DR / 360.);
	float degreesToRads		= (float)(CV_PI / 180.);

	float countsToDegrees	= (float)(360. / ENC_MODULE_DR);
	float countsToRads		= (float)((2 * CV_PI) / ENC_MODULE_DR);

	float radsToDegrees		= (float)(180. / CV_PI);
	float radsToCounts		= (float)(ENC_MODULE_DR / (2 * CV_PI));

	switch (oldUnits) {
		case 0://DEGREES
			if (newUnits == 1)	*val = (*val) * degreesToCounts;
			else				*val = (*val) * degreesToRads;
			break;

		case 1://COUNTS
			if (newUnits == 0)	*val = (*val) * countsToDegrees;
			else 				*val = (*val) * countsToRads;
			break;

		case 2://RADIANS
			if (newUnits == 0)	*val = (*val) * radsToDegrees;
			else 				*val = (*val) * radsToCounts;
			break;
	default:
		PLOGF << "Units conversion selected out of range! Sel = " << newUnits;
		break;
	}
}

void CRSSMotorCtrlDlg::OnCbnSelchangeComboRampUpUnits()
{
	CString m_strItemSelected;
	static CString szText;
	double val;//Value from/to EditBox
	static int oldUnits = 0;//Degrees(0) is default
	static int newUnits = 0;

	cmbRampUpUnitsSel.GetLBText(cmbRampUpUnitsSel.GetCurSel(), m_strItemSelected);
	newUnits = cmbRampUpUnitsSel.GetCurSel();

	if (newUnits != oldUnits) {//Update GUI
	//INITIALPOS
			//Get old value
			GetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
			val = atof(szText);
			//Convert
			convertUnits(oldUnits, newUnits, &val);
			//Set converted value
			szText.Format("%.2f", val);
			SetDlgItemText(IDC_EDIT_RAMP_INITIALPOS, szText);
	//UPPERPOS		
			//Get old value
			GetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);
			val = atof(szText);
			//Convert
			convertUnits(oldUnits, newUnits, &val);
			//Set converted value
			szText.Format("%.2f", val);
			SetDlgItemText(IDC_EDIT_RAMP_UPPERPOS, szText);
	//LOWERPOS
			//Get old value
			GetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);
			val = atof(szText);
			//Convert
			convertUnits(oldUnits, newUnits, &val);
			//Set converted value
			szText.Format("%.2f", val);
			SetDlgItemText(IDC_EDIT_RAMP_LOWERPOS, szText);
	//MAXVEL		
			//Get old value
			GetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
			val = atof(szText);
			//Convert
			convertUnits(oldUnits, newUnits, &val);
			//Set converted value
			szText.Format("%.2f", val);
			SetDlgItemText(IDC_EDIT_RAMP_MAXVEL, szText);
	//MAXACC	
			//Get old value
			GetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);
			val = atof(szText);
			//Convert
			convertUnits(oldUnits, newUnits, &val);
			//Set converted value
			szText.Format("%.2f", val);
			SetDlgItemText(IDC_EDIT_RAMP_MAXACC, szText);

			//update oldUnits value
			oldUnits = newUnits;
	}

	UpdateData(FALSE);//¿?
}
void CRSSMotorCtrlDlg::OnCbnSelchangeComboAdvSelectslave()
{
	CString m_strItemSelected;

	cmbSlaveSel.GetLBText(cmbSlaveSel.GetCurSel(), m_strItemSelected);

	slaveActive = (unsigned int) cmbSlaveSel.GetCurSel();//Set numerical ID of selected slave

	UpdateData(FALSE);//¿?
}

void CRSSMotorCtrlDlg::LoadComboBox() {

	CString str = _T("");

	//Load 'Ramp Units'
	str.Format(_T("[degrees]"));
	cmbRampUpUnitsSel.AddString(str);
	str.Format(_T("[counts]"));
	cmbRampUpUnitsSel.AddString(str);
	str.Format(_T("[radians]"));
	cmbRampUpUnitsSel.AddString(str);

	//Load 'Select Slave'
	str.Format(_T("0 (BR1_M1)"));
	cmbSlaveSel.AddString(str);
	str.Format(_T("1 (BR2_M2)"));
	cmbSlaveSel.AddString(str);
}

void CRSSMotorCtrlDlg::InitLog(void) {
	CString szFilename;
	
	szFilename.Format(logFilename, CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));
	//LOG_LEVEL is defined by compilation mode (Debur or Release). This is LOG zero(0)! aka MAIN_LOG
	plog::init<0>(LOG_LEVEL, szFilename);
}

void CRSSMotorCtrlDlg::OnBnClickedButtonDevFaultReset(void){
	cecmW.StatusFaultReset(slaveActive);
	PLOGI << "Click on 'Device Fault Reset' button";
}

void CRSSMotorCtrlDlg::OnBnClickedButtonDevBrake(){
	// TODO: Add your control notification handler code here
	static uint16_t Data	= 0x02;//Force brake lock
	//static uint16_t Data	= 0x01;//Force brake release
	
	//static uint16_t Data	= CTRLW_OPERATION;
	//static uint16_t Data	= CTRLW_SWITCHON;
	//static uint16_t Data	= CTRLW_SWITCHOFF;
	void* pData				= &Data;//Pointer to the value

	static ECM_SLAVE_ADDR regAddress;

	static uint16_t pucCnt		= 0;
	static uint16_t numBytes	= sizeof(uint16_t);


	//{ 0x03E9, 0x2010, sizeof(uint16_t), FPWR, "Writes 0x010 axis-1 (Control Word - RW)"},//Switch on
	//regAddress.p.ado = 0x0129;//Axis-1 "Brake override"
	//regAddress.p.adp = 0;//Axis-1 "Brake override"
	//regAddress.l = 0x01290000;//Axis-1 "Brake override"
	regAddress.p.ado = 0x2010;//Axis-1 "Control Word"
	regAddress.p.adp = 0x03e9;//RSS-400
	//regAddress.p.adp = 0x03ea;//RSS-100
	//int res = cecmW.SendAsyncRequest(pData, FPWR, regAddress, numBytes, &pucCnt);//	ECM_SUCCESS = 0;
	int res = cecmW.SendAsyncRequest(pData, BWR, regAddress, numBytes, &pucCnt);//	ECM_SUCCESS = 0;
	PLOGI << "Click on 'Device BRAKE' button: " << res;


	//This is a functional "BRAKE", but previous code must be validated someday too
	cecmW.SetOperationalState(slaveActive);
	//res = cecmW.SetLatchState();
	//res = cecmW.SetOperationalState();
	//res = cecmW.SetSwitchOnState();
	//res = cecmW.SetSwitchOffState();
}

void CRSSMotorCtrlDlg::OnBnClickedCancel()
{
	PLOGI << "Click on 'Cancel/Close' button";

	//New
	FinalizeControlThreads();

	//Old
	CDialogEx::OnCancel();
}


void CRSSMotorCtrlDlg::OnBnClickedCheckThermalanalysis()
{
	// TODO: Add your control notification handler code here
	PLOGI << "Click on 'Thermal Analysis' check box";

	cbThermalAnalysisBool = !cbThermalAnalysisBool;//Update value (default/start is disabled!)

	if (cbThermalAnalysisBool) {
		PLOGI << "'Thermal Analysis' enabled";
		//ThermalAnalysisMaxDuration_sec		= 5 * 3600;	//5 hours máx

		QueryPerformanceCounter((LARGE_INTEGER*)&ThermalAnalysisTimeStart);
		QueryPerformanceFrequency((LARGE_INTEGER*)&ThermalAnalysisFreq);
	}
	else {
		PLOGI << "'Thermal Analysis' disabled";
	}
}