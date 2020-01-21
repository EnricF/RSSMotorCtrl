/************************************************************************/
/*                                                                      */
/*         Test/Demonstration of the EtherCAT master stack API          */
/*                                                                      */
/*      Copyright 2008 - 2015 esd electronic system design gmbh         */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*      Filename:      ecmTest.c                                        */
/*      Language:      ANSI C                                           */
/*      Targetsystem:  QNX, VxWorks, RTX / RTX64, Win XP/Vista/7/8+     */
/*                     OS-9, Linux, WinCE                               */
/*----------------------------------------------------------------------*/
/* Revision history:                                                    */
/*----------------------------------------------------------------------*/
/* 170,07oct15,ot  * Prevent repeated error messages in cycle handler   */
/* 169,22jul15,ot  * Indicate start and end of anapplication driven I/O */
/*                 *  cycle to the stack with ecmGetCycleRuntime()      */
/*                 * Option "--profiling" now has an optional argument  */
/*                 *  to configure the type/output of profiling data.   */
/*                 * Dump DC configuration in PrintSlaveDescription()   */
/*                 *  and PrimtMasterDescription()                      */
/*                 * Added call to ecmGetCycleRuntime() in DoEcatIo()   */
/*                 * Adapted variable name in ecmTestSetupProcesData()  */
/*                 *  to V2 of ECX-DIO8.                                */
/*                 * DC continuous drift compensation enabled with      */
/*                 *  option '--dc-drift-comp'                          */
/*                 * DC epoch configuration with option '--dc-epoch'    */
/*                 * Added support for ecmDcToUnixTime()                */
/* 168,16jun15,ot  * Decode ENI parser status of ECM_EVENT_CFG_SYNTAX   */                
/* 167,17mar15,ot  * Extended '-t' option to support INIT<->OP loops    */
/*                 * Indicate ECM_LOCAL_STATE_CTIME_EXCEEDED event      */
/* 166,17feb15,ot  * Added OS jitter check in CycleStartHandler()       */
/*                 *  with new option "--jitter"                        */
/*                 * Added support for thread stack size configuration  */
/* 165,07jan15,ot  * High res tick frequency global in g_ulCyclesPerSec */
/*                 * Enable system state preemption for OS-9 worker     */
/*                 *  threads if supported by kernel version.           */
/* 164,12dec14,ot  * Added new option '-s' to suppress log messages     */
/*                 * Argument of '-d' has to be passed to option        */
/*                 *  '--dc-clock-shift'                                */
/*                 * Call ecmProcessAcyclicCommunication() in cycle end */
/*                 *  handler                                           */
/*                 * Use ECM_SETUP_REMOTE_WATCHDOG to configure remote  */
/*                 *  connection watchdog                               */
/*                 * Added example code to adjust OS-9 tick             */
/* 163,23jun14,ot  * Added detection of 64-bit version of the stack.    */
/*                 * Added OS specific ecmTestCleanup()                 */
/*                 * Added support for ecmGetSlaveDiag()                */
/*                 * Added support for separate OS-9 SPF instance       */
/*                 * Use configured cycle time in ENI file              */
/* 162,06may14,kp  * New flags _DC_REF_NO_OFFS and _DCOFFS_TO_LOCALTIME */
/*                 *  implemented. Minor changes for LINUX priorities   */
/* 160,10mar14,ot  * Support to return ENI filename and Workbench       */
/*                 *  project GUID with ecmGetMasterState()             */
/*                 * Support to start remote server in monitoring mode  */
/*                 * Added support for ECM_EVENT_REMOTE                 */
/*                 * Support simplified remote channel description with */
/*                 *  just interface IP address                         */
/* 150,24jan14,ot  * Support for SoE using "--scan-dictionary" option   */
/*                 * Initial support for OS-9                           */
/*                 * Example to search for variables with RegEx pattern */
/*                 * Make state change events more clearer with new     */
/*                 *  helper code ecmTestEventToString()                */
/*                 * Call ecmTestSetupProcesData() before the master    */
/*                 *  instance is attached.                             */
/*                 * Added ecmTestInit() for Win32 which changes the    */
/*                 *  process into the HIGH_PRIORITY_CLASS              */
/*                 * Support for build string in ECM_VERSION            */
/* 14B,26aug13,ot  * Fixed result of ecmTestInit() are converted into   */
/*                 *  text with ecmResultToString() which might crash.  */
/* 14A,26aug13,ot  * Indicate ECM_LOCAL_STATE_TRIAL_EXPIRED event       */
/* 149,22may13,ot  * Adapted to change of name and data type in struct  */
/*                 *  ECM_OD_LIST                                       */
/* 148,13may13,ot  * Get data references in ecmTestSetupProcesData()    */
/*                 *  with ecmGetDataReference() instead of deprecated  */
/*                 *  ecmInputRef() and ecmOutputRef().                 */
/* 147,10dec12,kp  * Added ecmResultToString(). All printfs should      */
/*                 * now show the #define string instead of number      */
/* 146,25aug11,ot  * Decode additional ESC controller types             */
/*                 * Fixed 'd' argument not optional                    */
/* 145,22aug11,ot  * Show version of remote protocol in                 */
/*                 *  PrintVersionInformation()                         */
/* 144,15jun11,ot  * Use ECM_FLAG_GET_FIRST and ECM_FLAG_GET_NEXT with  */
/*                    ecmXxxVariable() to make code more readable       */
/* 143,01jun11,ot  * Support to force ESI EEPROM access to EtherCAT     */
/*                 *  with ecmWriteEeprom()                             */
/* 142,09may11,ot  * New option "--server" to start EtherCAT master     */
/*                 *  as remote server.                                 */
/* 141,02may11,ot  * New option "--acyclic-limit" to limit # of acyclic */
/*                 *  frames per I/O cycle.                             */
/*                 * Initial support for Windows CE                     */
/*                 * New helper code GetAbortCodeText().                */
/*                 * Added sample code to demonstrate the use of        */
/*                 *  ECM_COE_ENTRY_UNIT, ECM_COE_ENTRY_DEFAULT_VALUE   */
/*                 *  ECM_COE_ENTRY_MIN_VALUE, ECM_COE_ENTRY_MAX_VALUE  */
/*                 *  in ecmTestCoeScan()                               */
/* 140,10jan11,ot  * Added sample code to demonstrate the use of        */
/*                 *  ecmCoeGetAbortCode(), ecmFormatError() and        */
/*                 *  ECM_COE_ENTRY_NAME in ecmTestCoeScan()            */
/*                 * Member ucAccessAndMapping renamed into             */
/*                 *  usObjectAccess for ECM_COE_ENTRY_DESCRIPTION      */
/*                 * Added support for ECM_EVENT_EMCY and more          */
/*                 *  details for ECM_EVENT_SLV in ecmEventHandler()    */
/*                 * Added PrintEsiInformation() to demonstrate the     */
/*                 *  ESI EEPROM data helper code which can be enabled  */
/*                 *  by the dump type 'esi' for option '--print-esc'   */
/* 131,30sep10,ot  * Added resource cleanup for some error situations   */
/*                 * Added check if all configured slaves are present   */
/*                 *  and OPERATIONAL using ecmGetMasterState()         */
/* 130,30aug10,ot  * Adapted to binary incompatible prototype change of */
/*                 *  PFN_ECM_HANDLER                                   */
/*                 * New option '--memory" to process ENI from memory   */
/*                 *  instead from file in favor of two execution loops */
/*                 * New option '--password" to provide the password    */
/*                 *  for an encrypted archive on the command line.     */
/*                 * Support for new entry ecmFormatError()             */
/* 120,24aug10,ot  * Added DoEcatIo() for application driven I/O        */
/*                 * Added ChangeEcatState() to wait for state change   */
/*                 *  using either application driven I/O or worker     */
/*                 *  task I/O                                          */
/*                 * New flag for "-i" option to disable background     */
/*                 *  worker task and use application driven I/O        */
/*                 * No cyclic data exchange if "-t" is set to a        */
/*                 *  negative value.                                   */
/*                 * Let ecmInitLibrary() be the 1st call into stack    */
/*                 * Fixed misspelled ulCyclicPeriod                    */
/* 112,16aug10,ot  * Print feature flags and mark debug build of master */
/*                 *  if called with "-v"                               */
/* 111,18may10,ot  * Description for ECM_EVENT_LOCAL events             */
/*                 * Added sample code to display Frm0WcState variable  */
/* 110,01mar10,ot  * Added platform support for RTX                     */
/*                 * Option "-d" gets (optional) argument to define     */
/*                 *  frequency of DC drift compensation and main loop  */
/*                 *  prints DC drift in ns if configured               */
/* 102,04jan10,ot  * New option "-u" |"--udp"                           */
/*                 * New option "-i" |"--io-mode"                       */
/*                 * New option "--dst-addr" as superset of option      */
/*                 *  "--no-broadcast"                                  */
/*                 * New option "--dc-clock-shift"                      */
/* 101,14dec09,ot  * Added support to initialize progname for getopt()  */
/*                 * Fixed bug in evaluation of debug mask parameter    */
/*                 * New option "--no-broadcast"                        */
/*                 * Option "--reduce-cyc-frames" becomes "--optimize"  */
/*                 * Option "-t" becomes "--tci"                        */
/* 100,22oct08,ot  * Initial release for Win32, Linux, QNX and VxWorks  */
/*----------------------------------------------------------------------*/

/* Prevent MS VC8 compiler and later warnings about deprecated CRT function */
#if (_MSC_VER >= 1400)
# define _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_WARNINGS
#endif

//EF mod: Windows requires it
#include "pch.h"


/*********************************************************************/
/*                            INCLUDES                               */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#ifdef VXWORKS
# define optind         _optind
# define opterr         _opterr
# define optopt         _optopt
#endif
#include <getopt.h>
#include <ecm.h>

//EF mod
#include "globalVar.h"
//------


/*********************************************************************/
/*                            DEFINES                                */
/*********************************************************************/

#define ECM_TEST_MAJOR    1
#define ECM_TEST_MINOR    7
#define ECM_TEST_BUILD    0

       /* TRUE and FALSE as values for the bool_t */
#ifndef TRUE
# define TRUE        1
#endif

#ifndef FALSE
# define FALSE       0
#endif

     /*
      * OS specific priority of cyclic and acyclic threads/tasks.
      *
      * Usually the priority of the cyclic thread should be better than
      * the priority of the acyclic thread.
      *
      * VxWorks: Both priorities should be worse than the one of the netTask.
      *
      * RTX:     Both priorities should be worse than the InterruptPriority
      *          and the ReceivePriority of the network driver.
      *
      * Please refer to the OS specific documentation for details.
      *
      */
#if defined(VXWORKS)
#  define PRIO_ACYCLIC      70
#  define PRIO_CYCLIC       60
#  define PRIO_REMOTE       80
#  define main              _ecmTest  /* Redefine main for VxWorks */
# elif defined(__QNXNTO__)
#  define PRIO_ACYCLIC      50
#  define PRIO_CYCLIC       100
#  define PRIO_REMOTE       40
# elif defined(_OS9000)
#  define PRIO_ACYCLIC      200
#  define PRIO_CYCLIC       500
#  define PRIO_REMOTE       150
#include <LIB/slmlib.h>      /* For optional HawkEye support */
# elif defined(_WIN32)
#  if defined(UNDER_RTSS)
#   define PRIO_ACYCLIC     50
#   define PRIO_CYCLIC      60
#   define PRIO_REMOTE      30
#  else
#   include <windows.h>
#   define PRIO_ACYCLIC     THREAD_PRIORITY_ABOVE_NORMAL
#   define PRIO_CYCLIC      THREAD_PRIORITY_ABOVE_NORMAL
#   define PRIO_REMOTE      THREAD_PRIORITY_BELOW_NORMAL
#  endif
# elif defined(LINUX)
#  define PRIO_ACYCLIC      70 /* ACYCLIC and CYCLIC prio effect depends on psys implementation: might be overruled by enable_realtime() */
#  define PRIO_CYCLIC       80
#  define PRIO_REMOTE       40
# else
#  define PRIO_ACYCLIC      ECM_PRIO_DEFAULT
#  define PRIO_CYCLIC       ECM_PRIO_DEFAULT
#  define PRIO_REMOTE       ECM_PRIO_DEFAULT
#endif

    /* Flags for ulConfigFlags (command line configuration) */
#define ECM_TEST_FLAG_PRINT_NICLIST       (1 <<  0)
#define ECM_TEST_FLAG_PRINT_VERSION       (1 <<  1)
#define ECM_TEST_FLAG_USE_DC              (1 <<  2)
#define ECM_TEST_FLAG_SLAVE_STATUS        (1 <<  3)
#define ECM_TEST_FLAG_REDUNDANCY          (1 <<  4)
#define ECM_TEST_FLAG_PROFILING           (1 <<  5)
#define ECM_TEST_FLAG_SCAN_DICTIONARY     (1 <<  6)
#define ECM_TEST_FLAG_REDUCE_CYCLIC_FRM   (1 <<  7)
#define ECM_TEST_FLAG_PRINT_VARLIST       (1 <<  8)
#define ECM_TEST_FLAG_PRINT_COPY_VECTOR   (1 <<  9)//EF mod: Default value is 1!!!
#define ECM_TEST_FLAG_PRINT_STATISTICS    (1 << 10)
#define ECM_TEST_FLAG_PRINT_ESC           (1 << 11)
#define ECM_TEST_FLAG_NO_BROADCAST        (1 << 12)
#define ECM_TEST_FLAG_FORCE_DST_ADDR      (1 << 13)
#define ECM_TEST_FLAG_UDP                 (1 << 14)
#define ECM_TEST_FLAG_DCM_CLOCK_SHIFT     (1 << 15)
#define ECM_TEST_FLAG_MEMORY_CONFIG       (1 << 16)
#define ECM_TEST_FLAG_PRINT_ESI           (1 << 17)
#define ECM_TEST_FLAG_REMOTE_CONTROL      (1 << 18)
#define ECM_TEST_FLAG_REMOTE_MONITOR      (1 << 19)
#define ECM_TEST_FLAG_DC_DRIFT_COMP       (1 << 20)
#define ECM_TEST_FLAG_SILENT              (1 << 22)
#define ECM_TEST_FLAG_CHECK_JITTER        (1 << 23)

#define ECM_TEST_FLAG_REMOTE_SERVER                                           \
    (ECM_TEST_FLAG_REMOTE_CONTROL | ECM_TEST_FLAG_REMOTE_MONITOR)

    /* Default value of event mask for application callback */
#define ECM_TEST_EVENT_MASK     (ECM_EVENT_LOCAL | ECM_EVENT_CFG          |   \
                                 ECM_EVENT_WCNT  | ECM_EVENT_STATE_CHANGE |   \
                                 ECM_EVENT_SLV   | ECM_EVENT_COE_EMCY)

    /* Flags for supported I/O modes */
#define ECM_TEST_IO_MODE_TX_BEFORE_RX      (1 << 0)
#define ECM_TEST_IO_MODE_NO_ACYC_WORKER    (1 << 1)
#define ECM_TEST_IO_MODE_NO_CYC_WORKER     (1 << 2)

    /* Flags to configure profiling options */
#define ECM_TEST_PROF_ACYLIC               (1 << ECM_PROFILE_ACYCLIC)
#define ECM_TEST_PROF_INPUT                (1 << ECM_PROFILE_INPUT)
#define ECM_TEST_PROF_OUTPUT               (1 << ECM_PROFILE_OUTPUT)
#define ECM_TEST_PROF_CYCLE_START          (1 << ECM_PROFILE_CYCLIC_START)
#define ECM_TEST_PROF_CYCLE_HANDLER        (1 << ECM_PROFILE_CYCLIC_HANDLER)
#define ECM_TEST_PROF_CYCLE_END            (1 << ECM_PROFILE_CYCLIC_END)
#define ECM_TEST_PROF_CYCLIC               (1 << ECM_PROFILE_CYCLIC_WORKER)
#define ECM_TEST_PROF_FRAME_TX             (1 << ECM_PROFILE_FRAME_TX)
#define ECM_TEST_PROF_USER1                (1 << ECM_PROFILE_USER1)
#define ECM_TEST_PROF_USER2                (1 << ECM_PROFILE_USER2)
#define ECM_TEST_PROF_MODE_NO_IO_TIME      0x80000000

    /* Maximum size for the password of a compressed configuration */
#define ECM_TEST_MAX_PASSWORD_SIZE         64

   /* Maximum size for a remote channel description */
#define ECM_TEST_MAX_CHANNEL_DESC          256

    /* Default cycle time is 10 ms */
#define ECM_TEST_DEFAULT_CYCLE_TIME        10000
    
    /* Default ID to indicate OS cycle time violations */
#define ECM_TEST_TRACE_ID                  42

#ifdef WINCE
# define rewind(fp)  fseek(fp, 0L, SEEK_SET)
#endif


/*********************************************************************/
/*                            ENUMS                                  */
/*********************************************************************/

/*
 * For long options that have no equivalent short option, we use a
 * non-character as a pseudo short option, starting with CHAR_MAX + 1.
 */
enum
{
    OPT_DEBUG_MASK = CHAR_MAX + 1,
    OPT_PRINT_COPY_VECTOR,
    OPT_PRINT_ESI,
    OPT_PRINT_ESC,
    OPT_PRINT_STATISTIC,
    OPT_PRINT_VARLIST,
    OPT_ENABLE_PROFILING,
    OPT_SCAN_DICTIONARY,
    OPT_REDUCED_CYCLIC_FRAMES,
    OPT_VLAN_TCI,
    OPT_DST_ADDR,
    OPT_DCM_CLOCK_SHIFT,
    OPT_DC_DRIFT_COMPENSATION,
    OPT_DC_EPOCH,
    OPT_COPY_TO_MEM,
    OPT_ZIP_PASSWORD,
    OPT_MAX_ACYCLIC,
    OPT_REMOTE_SERVER,
    OPT_SILENT,
    OPT_CHECK_JITTER
};

enum {
    ECM_TEST_CYCLIC_START_HANDLER = 0,
    ECM_TEST_CYCLIC_HANDLER,
    ECM_TEST_CYCLIC_END_HANDLER
};
/*********************************************************************/
/*                            LOCALS                                 */
/*********************************************************************/

/* Help text */
static const char* const ecmTestHelp[] =
{
    "-a P[,R]   --adapter=P[,R]        Primary[,Redundant] adapter number",
    "-c time    --cycletime=time       Cycle time in us",
    "-d         --dc                   Enable distributed clocks support",
    "-e hexmask --eventmask=hexmask    EtherCAT master event mask",
    "-i flags   --io-mode=flags        I/O-mode 0x1: Tx before Rx",
    "                                           0x2: No acyclic worker task",
    "                                           0x4: No cyclic worker task",
    "-n         --niclist              Print list of available network adapter",
    "-m         --monitor              Monitor slave status while operational",
    "-r         --redundancy           Enable 2nd adapter for cable redundancy",
    "-s         --silent               Suppress startup events",
    "-t         --time=runtime[,loops] Test runtime in seconds (0 = forever)",
    "-u         --udp                  EtherCAT via UDP",
    "-v         --version              Print version information",
    "           --acyclic-limit        Limit # of acyclic frames per I/O cycle",
    "           --dc-clock-shift=[s,c] Shift master clock to adjust DC drift",
    "                                  s = User Shift Master c = Ctrl cycles",
    "           --dc-drift-comp        Enable continuous drift compensation",
    "           --dc-epoch=epoch       DC epoch (0/1/2) = (DC/System/None)",
    "           --debugmask=hexmask    Trace mask (Debug build only)",
    "           --dst-addr=hwaddr      Destination MAC addr (Def: Broadcast)",
    "           --jitter=[timespan]    Check jitter of worker thread",
    "           --memory               Process ENI file from memory",
    "           --optimize             Reduce number of cyclic frames",
    "           --password=password    Password of a compressed ZIP archive",
    "           --print-copy-vector    Print optimized copy vector",
    "           --print-esc=[hex|esi]  Print ESC register and EEPROM",
    "           --print-statistics     Print statistic data",
    "           --print-varlist        Print list of process variables",
    "           --profiling=type       Print profiling data",
    "           --scan-dictionary      Scan object dictionary at startup",
    "           --server=[channel]     Initialize as remote server with channel",
    "                                  as [Stream:]IP[@Port][M|C]",
    "           --tci=TCI              VLAN TCI",
    NULL};

/* Options of getopt_long() call */
static struct option const longopts[] =
{
    {"adapter", required_argument, NULL, 'a'},
    {"cycletime", required_argument, NULL, 'c'},
    {"dc", optional_argument, NULL, 'd'},
    {"eventmask", required_argument, NULL, 'e'},
    {"io-mode", required_argument, NULL, 'i'},
    {"niclist", no_argument, NULL, 'n'},
    {"monitor", no_argument, NULL, 'm'},
    {"redundancy", no_argument, NULL, 'r'},
    {"time", required_argument, NULL, 't'},
    {"udp", no_argument, NULL, 'u'},
    {"version", no_argument, NULL, 'v'},
    {"acyclic-limit", required_argument, NULL, OPT_MAX_ACYCLIC},
    {"dc-clock-shift", optional_argument, NULL, OPT_DCM_CLOCK_SHIFT},
    {"dc-drift-comp", no_argument, NULL, OPT_DC_DRIFT_COMPENSATION},
    {"dc-epoch", required_argument, NULL, OPT_DC_EPOCH},
    {"debugmask", required_argument, NULL, OPT_DEBUG_MASK},
    {"dst-addr", optional_argument, NULL, OPT_DST_ADDR},
    {"memory", no_argument, NULL, OPT_COPY_TO_MEM},
    {"optimize", no_argument, NULL, OPT_REDUCED_CYCLIC_FRAMES},
    {"password", required_argument, NULL, OPT_ZIP_PASSWORD},
    {"print-copy-vector", no_argument, NULL, OPT_PRINT_COPY_VECTOR},
    {"print-esc", optional_argument, NULL, OPT_PRINT_ESC},
    {"print-statistics", no_argument, NULL, OPT_PRINT_STATISTIC},
    {"print-varlist", no_argument, NULL, OPT_PRINT_VARLIST},
    {"profiling", optional_argument, NULL, OPT_ENABLE_PROFILING},
    {"scan-dictionary", no_argument, NULL, OPT_SCAN_DICTIONARY},
    {"server", optional_argument, NULL, OPT_REMOTE_SERVER},
    {"tci", required_argument, NULL, OPT_VLAN_TCI},
    {"silent", no_argument, NULL, OPT_SILENT},
    {"jitter", optional_argument, NULL, OPT_CHECK_JITTER},
    {0, 0, 0, 0}
};

/* Buffer for process data image */
static uint8_t ucInputBuffer[32000], ucOutputBuffer[32000];

/* Configuration flags defined by command line arguments */
static uint32_t ulConfigFlags;

/* Mask for events */
static uint32_t g_ulEventMask = ECM_TEST_EVENT_MASK;

/* Mask for trace messages (debug build only) */
static uint32_t g_ulDebugMask = 0xC0000000;

/* Mask for profiling */
static uint32_t g_ulProfileMask = ECM_PROFILE_CYCLIC_WORKER;

static int iPrimaryNic, iRedundantNic;

/* Cycle time in microseconds */
static uint32_t g_ulCycleTime;

/* Cycle time violation for jitter check in microseconds */
static uint32_t g_ulCycleTimeViolation;

/* Clock tick of DC drift compensation */
static uint16_t g_usDcClockControl;

/* Clock tick of DC drift compensation */
static uint8_t g_ucDcSysTimeEpoch;

/* User Shift Master in us */
static int16_t g_sUserShiftMaster;

/* TCI of VLAN tagged frame if configured */
static uint16_t g_usVlanTci;

/* Example running time in seconds (Default is forever) */
static int32_t g_lRuntime;

/* INIT -> OP loops */
static int32_t g_lLoops;

/* I/O mode (Default is 0) */
static uint32_t g_ulIoMode;

/* Limit # of acyclic frames per I/O cycle (Default is 0 = No limit) */
static uint16_t g_usAcycCtrl = 0;

/* Archive password (Default is empty) */
static char g_szPassword[ECM_TEST_MAX_PASSWORD_SIZE+1];

/* Channel description for remote server (Default is Stream:127.0.0.1@ECM_REMOTING_PORT) */
static char g_szChannelDescription[ECM_TEST_MAX_CHANNEL_DESC+1];

/* Clock cycles per second of high resolution counter */
static uint32_t g_ulCyclesPerSec;

/* (Overridden) destination MAC address */
static ECM_ETHERNET_ADDRESS g_DstMac;

/* Reference to virtual variables */
static uint16_t *pSlaveCount = NULL, *pSlaveCount2 = NULL;
static uint16_t *pDevState = NULL, *pWcState = NULL;

/* Reference to process variables */
static uint8_t *pucDio = NULL;
//EF mod: extra pointers
//Writings to slave
static uint8_t *pucDio_OperationMode	= NULL;//"Operation mode" pointer
static uint8_t *pucDio_ControlWord		= NULL;//"Control Word" pointer
static uint8_t *pucDio_TargetPosition	= NULL;//"Target position" pointer
static uint8_t *pucDio_TargetVelocity	= NULL;//"Target velocity" pointer
static uint8_t *pucDio_ProfilerMaxVel	= NULL;//"Profiler max. velocity" pointer
static uint8_t *pucDio_ProfilerMaxAcc	= NULL;//"Profiler max. acceleration" pointer
static uint8_t *pucDio_ProfilerMaxDec	= NULL;//"Profiler max. deceleration" pointer
//Readings from slave
static uint8_t *pucDio_StatusWord		= NULL;//"Status mode" pointer
static uint8_t *pucDio_PositionActual	= NULL;//"Position actual" pointer
static uint8_t *pucDio_VelocityActual	= NULL;//"Velocity actual" pointer
static uint8_t *pucDio_DisplayOperationMode = NULL;//"Operation mode display" pointer
static uint8_t *pucDio_PrimareTemperature	= NULL;//"Primary temperature value" pointer

/*********************************************************************/
/*                   FORWARD DECLARATIONS                            */
/*********************************************************************/
static int ecmTestInit(ECM_LIB_INIT *pLibInit);
static int ecmTestCleanup(void);


//Temporally global, to be moved inside its class
FILE * ECMlogFile;
const char * cECMlogFile = "logFile.txt";



/*
 * Print usage information
 */
static void PrintUsage (void)
{
    char const * const *p;

    printf ("\nUsage: ecmTest [OPTIONS] ENI-file[@ZIP-File]\n\n");
    for (p = ecmTestHelp;  *p;  p++)
        printf ("  %s\n", *p);
}

/*
 * Helper code to convert a hex digit into an integer.
 *
 * Returns converted (positive) value or -1 on error.
 */
static int ConvertHexDigit(const char c)
{
    int digit;

    if ((c >= '0') && (c <= '9')) {
        digit = c - '0';
    } else if ((c >= 'a') && (c <= 'f')) {
        digit = c - 'a' + 10;
    } else if ((c >= 'A') && (c <= 'F')) {
        digit = c - 'A' + 10;
    } else {
        return -1;
    }

    return digit;
}

/*
 * Helper code to convert a MAC address string with '-' or ':' as separator
 *
 * Returns 0 on success or -1 on error
 */
static int ParseHwAddr(const char *pszBuffer, ECM_ETHERNET_ADDRESS *pHwAddr)
{
    int i, digit;

    /* Initialize MAC address */
    ECM_INIT(*pHwAddr);

    for (i = 0; i < 6; i++) {
        if (0 == *pszBuffer) {
            break;
        }

        /* Convert 1st digit and return on conversion error or end of string */
        digit = ConvertHexDigit(*pszBuffer++);
        if ((digit < 0) || (0 == *pszBuffer)) {
            break;
        }

        /* Check for separator or convert 2nd digit */
        if ((':' == *pszBuffer) || ('-' == *pszBuffer)) {
            pHwAddr->b[i] = (uint8_t)digit;
        } else {
            pHwAddr->b[i] = (uint8_t)(digit << 4);
            digit = ConvertHexDigit(*pszBuffer++);
            if (digit < 0) {
                break;
            }
            pHwAddr->b[i] = pHwAddr->b[i] + (uint8_t)digit;
        }

        /* Skip separator */
        if ((':' == *pszBuffer) || ('-' == *pszBuffer)) {
            pszBuffer++;
        }
    } /* for i */

    return(6 == i ? 0 : -1);
}

/*
 * Initialize configuration defaults and parse the command line arguments
 */
static int ParseCommandLine(int argc, char *argv[])
{
    int c;
    char *pszBuffer;

    /* Initialize configuration default */
    iPrimaryNic        = 0;      /* 1st enumerated NIC is primary adapter */
    iRedundantNic      = 1;      /* 2nd enumerated NIC is redundant adapter */
    g_ulCycleTime      = 0;      /* Say; Use ECM_TEST_DEFAULT_CYCLE_TIME */
    ulConfigFlags      = 0;
    g_lRuntime         = 0;      /* Default: Run endlessly */
    g_lLoops           = 1;      /* Default: One loop */
    g_ulIoMode         = 0;      /* Default: I/O-Mode 0 */
    g_usDcClockControl = 0;      /* Default: No local drift compensation */
	g_sUserShiftMaster = 0;      /* Default: No user shift master */
    g_ucDcSysTimeEpoch = ECM_SYSTIME_EPOCH_DC; /* Default SysTime: DC epoch     */
    g_usVlanTci        = ECM_VLAN_TAG_NONE;    /* Default: No multi master mode */
    ECM_INIT(g_szPassword);      /* Default: No password */

    /* Get the program name without the preceding path */
    if (NULL == (pszBuffer = strrchr(argv[0], '\\'))) {
        pszBuffer = strrchr(argv[0], '/');
    }
#ifdef REPLACE_GETOPT
    progname = pszBuffer ? ++pszBuffer : argv[0];
#endif

    /*
    * Parse command line using getopt_long()
    */
    while ((c = getopt_long (argc, argv, "a:c:de:i:nmrst:uv", longopts, NULL)) != -1) {
        char *pEnd;

        switch (c) {
        case 'a':
            pszBuffer  = strtok(optarg, ",");
            if (pszBuffer != NULL) {
                iPrimaryNic = strtoul(pszBuffer, &pEnd, 0);
                if(*pEnd != '\0') {
                    fprintf(ECMlogFile,"Invalid parameter >%s< for option '%c'\n", pEnd, c);
                    return -1;
                }
            }
            pszBuffer  = strtok(NULL, ",");
            if (pszBuffer != NULL) {
                iRedundantNic = strtoul(pszBuffer, &pEnd, 0);
                if(*pEnd != '\0') {
                    fprintf(ECMlogFile,"Invalid parameter >%s< for option '%c'\n", pEnd, c);
                    return -1;
                }
            }
            break;
        case 'c':
            g_ulCycleTime = (uint32_t)strtoul(optarg, &pEnd, 0);
            if(*pEnd != '\0') {
                fprintf(ECMlogFile,"Invalid parameter >%s< for option '%c'\n", pEnd, c);
                return -1;
            }
            break;
        case 'd':
            ulConfigFlags |= ECM_TEST_FLAG_USE_DC;
            break;
        case 'e':
            g_ulEventMask = strtoul(optarg, &pEnd, 16);
            if (*pEnd != '\0') {
                fprintf(ECMlogFile,"Invalid parameter %s for option '%c'\n", pEnd, c);
                g_ulEventMask = ECM_TEST_EVENT_MASK;
            }
            break;
        case 'i':
            g_ulIoMode = (uint32_t)atoi(optarg);
            if (g_ulIoMode > 7) {
                fprintf(ECMlogFile,"Invalid parameter %lx for I/O mode\n", (unsigned long)g_ulIoMode);
                g_ulIoMode = 0;
            }
            break;
        case 'm':
            ulConfigFlags |= ECM_TEST_FLAG_SLAVE_STATUS;
            break;
        case 'n':
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_NICLIST;
            break;
        case 'r':
            ulConfigFlags |= ECM_TEST_FLAG_REDUNDANCY;
            break;
        case 's':
            ulConfigFlags |= ECM_TEST_FLAG_SILENT;
            break;
        case 't':
            pszBuffer  = strtok(optarg, ",");
            if (pszBuffer != NULL) {
                g_lRuntime = (int32_t)atoi(pszBuffer);
            }
            pszBuffer  = strtok(NULL, ",");
            if (pszBuffer != NULL) {
                g_lLoops = (int32_t)atoi(pszBuffer);
            }
            break;
        case 'u':
            ulConfigFlags |= ECM_TEST_FLAG_UDP;
            break;
        case 'v':
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_VERSION;
            break;
        case OPT_DEBUG_MASK:
            if (*optarg != '\0') {
                g_ulDebugMask = strtoul(optarg, &pszBuffer, 16);
                if (*pszBuffer != '\0') {
                    fprintf(ECMlogFile,"Invalid character %c in debug mask argument '%s' !\n",
                        *pszBuffer, optarg);
                    g_ulDebugMask = 0xC0000000;
                } else if ((errno != 0) &&
                    ((0 == g_ulDebugMask) || (ULONG_MAX == g_ulDebugMask))) {
                        fprintf(ECMlogFile,"Invalid parameter in debug mask argument '%s' !\n",
                            optarg);
                        g_ulDebugMask = 0xC0000000;
                }
            }
            break;
        case OPT_DST_ADDR:
            if (optarg != NULL) {
                if (ParseHwAddr(optarg, &g_DstMac) != 0) {
                    fprintf(ECMlogFile,"Invalid hwaddr '%s' for primary adapter !\n", optarg);
                }
                ulConfigFlags |= ECM_TEST_FLAG_FORCE_DST_ADDR;
            } else {
                fprintf(ECMlogFile,"Enabling no broadcast\n");
                ulConfigFlags |= ECM_TEST_FLAG_NO_BROADCAST;
            }
            break;
        case OPT_PRINT_COPY_VECTOR:
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_COPY_VECTOR;
            break;
        case OPT_PRINT_ESC:
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_ESC;
            if (optarg != NULL) {
                if (0 == strcmp(optarg, "esi")) {
                    ulConfigFlags |= ECM_TEST_FLAG_PRINT_ESI;
                } else {
                    if (strcmp(optarg, "hex") != 0) {
                        fprintf(ECMlogFile,"Ignoring option '%s' dumping EEPROM data !\n",
                                optarg);
                    }
                }
            }
            break;
        case OPT_PRINT_STATISTIC:
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_STATISTICS;
            break;
        case OPT_PRINT_VARLIST:
            ulConfigFlags |= ECM_TEST_FLAG_PRINT_VARLIST;
            break;
        case OPT_ENABLE_PROFILING:
            ulConfigFlags |= ECM_TEST_FLAG_PROFILING;
            g_ulProfileMask = ECM_TEST_PROF_CYCLIC;
            if (optarg != NULL) {
                g_ulProfileMask = strtoul(optarg, &pszBuffer, 16);
                if (*pszBuffer != '\0') {
                    fprintf(ECMlogFile,"Invalid character %c in profiling mask argument '%s' !\n",
                           *pszBuffer, optarg);
                    g_ulProfileMask = 0;
                } else if ((errno != 0) &&
                    ((0 == g_ulProfileMask) || (ULONG_MAX == g_ulProfileMask))) {
                        fprintf(ECMlogFile,"Invalid parameter in profiling mask argument '%s' !\n",
                            optarg);
                        g_ulProfileMask = 0;
                }
            }
            break;
        case OPT_SCAN_DICTIONARY:
            ulConfigFlags |= ECM_TEST_FLAG_SCAN_DICTIONARY;
            break;
        case OPT_REDUCED_CYCLIC_FRAMES:
            ulConfigFlags |= ECM_TEST_FLAG_REDUCE_CYCLIC_FRM;
            break;
        case OPT_VLAN_TCI:
            g_usVlanTci = (uint16_t)atoi(optarg);
            break;
        case OPT_DCM_CLOCK_SHIFT:
            ulConfigFlags |= ECM_TEST_FLAG_DCM_CLOCK_SHIFT;
			if (optarg != NULL) {
				pszBuffer = strtok(optarg, ",");
                if (pszBuffer != NULL) {
                    g_sUserShiftMaster = (int16_t)atoi(pszBuffer);
                }
				pszBuffer  = strtok(NULL, ",");
                if (pszBuffer != NULL) {
                    g_usDcClockControl = (uint16_t)atoi(pszBuffer);
                }
			}
            break;
        case OPT_DC_DRIFT_COMPENSATION:
            ulConfigFlags |= ECM_TEST_FLAG_DC_DRIFT_COMP;
            break;
        case OPT_DC_EPOCH:
            g_ucDcSysTimeEpoch = (uint8_t)atoi(optarg);
            break;
        case OPT_COPY_TO_MEM:
            ulConfigFlags |= ECM_TEST_FLAG_MEMORY_CONFIG;
            break;
        case OPT_ZIP_PASSWORD:
            strncpy(g_szPassword, optarg, ECM_TEST_MAX_PASSWORD_SIZE);
            break;
        case OPT_MAX_ACYCLIC:
            g_usAcycCtrl = (uint16_t)atoi(optarg);
            break;
        case OPT_REMOTE_SERVER:
            if (optarg != NULL) {
                /* 20 = strlen("Stream:@99999999999C"), the max we could add... */
                const int tooLong = (strlen(optarg) >= ECM_TEST_MAX_CHANNEL_DESC - 20);
                const int haveType = (strchr(optarg, ':') != NULL);
                const int havePort = (strchr(optarg, '@') != NULL);
                char *pcLastChar = optarg + strlen(optarg) - 1;

                /* Distinguish between monitor or config mode */
                if(!isdigit(*pcLastChar)) {
                    if('M' == *pcLastChar) {
                        ulConfigFlags |= ECM_TEST_FLAG_REMOTE_MONITOR;
                    }
                    *pcLastChar = '\0';
                }

                if (((haveType) && (havePort)) || (tooLong)) {
                    strncat(g_szChannelDescription, optarg,
                            ECM_TEST_MAX_CHANNEL_DESC);
                } else if (haveType) {
                    sprintf(g_szChannelDescription, "%s@%d", optarg,
                            ECM_REMOTING_PORT);
                } else if (havePort) {
                    sprintf(g_szChannelDescription, "Stream:%s", optarg);
                } else if(*optarg != '\0') {
                    sprintf(g_szChannelDescription, "Stream:%s@%d", optarg,
                            ECM_REMOTING_PORT);
                } else {
                    /* Allow INADDR_ANY for remote access and ESDCP requests */
                    sprintf(g_szChannelDescription, "Stream:0.0.0.0@%d",
                        ECM_REMOTING_PORT);
                }
            } else {
                sprintf(g_szChannelDescription, "Stream:127.0.0.1@%d",
                    ECM_REMOTING_PORT);
            }
            if(0 == (ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR)) {
                ulConfigFlags |= ECM_TEST_FLAG_REMOTE_CONTROL;
            }
            break;
        case OPT_SILENT:
            ulConfigFlags |= ECM_TEST_FLAG_SILENT;
            break;
        case OPT_CHECK_JITTER:
            ulConfigFlags |= ECM_TEST_FLAG_CHECK_JITTER;
            if (optarg != NULL) {
                g_ulCycleTimeViolation = (uint32_t)atoi(optarg);
            }
            break;
        default:
            return(-1);
        } /* of switch */
    }  /* of while */

    return(0);
}


/*
 * Dump a EEPROM buffer with word addresses
 */
static int hexDump(uint8_t *pBuffer, uint16_t usDataSize, uint16_t usBase)
{
    int i, j;

    for (i = 0; i < usDataSize; i += 4) {
        fprintf(ECMlogFile,"%04x:",  usBase + i);
        for (j = 0; j < 8; j++) {
            fprintf(ECMlogFile," %02x", *(pBuffer + i*2 + j));
        }
        printf (" |");
        for (j = 0; j < 8; j++) {
            uint8_t ch = *(pBuffer + i*2 + j);
            fprintf(ECMlogFile," %c", isprint(ch) ? ch : '.');
        }
        fprintf(ECMlogFile,"\n");
    }

    return(0);
}

/*
 * Read a configuration file into memory
 */
static int ReadConfigFileIntoMemory(const char *pszConfigFile, void **ppBuffer,
                                   size_t *pSize)
{
    FILE *fp;
    int state;

    /* Try open the master configuration file */
    fp = fopen(pszConfigFile, "rb");
    if (NULL == fp) {
        fprintf(ECMlogFile,"Could not open input file '%s'!\n", pszConfigFile);
        return(-1);
    }

    /*
     * Determine file size
     */
    state = fseek(fp, 0, SEEK_END);
    if (state != 0) {
        fclose(fp);
        fprintf(ECMlogFile,"Error getting file size of '%s'\n", pszConfigFile);
        return(-1);
    }

    *pSize = ftell(fp);

    /* Allocate memory and read complete file into RAM */
    *ppBuffer = calloc(1, *pSize);
    if (NULL == *ppBuffer) {
        fclose(fp);
        fprintf(ECMlogFile,"Error allocating memory for file '%s'\n", pszConfigFile);
        return(-1);
    }

    rewind(fp);   /* Go to start of file */

    fread(*ppBuffer, 1, *pSize, fp);
    fclose(fp);

    return(0);
}

/*
 * Dump an error message and handle repeated identical errors.
 */
static void CycleHandlerErrorMessage(int iError, int iIdx)
{
    char szBuffer[128];
    static int iLastError[3], iLastErrorCount[3];

    if (iError != ECM_SUCCESS) {
        ecmFormatError(iError, ECM_ERROR_FORMAT_LONG, szBuffer, sizeof(szBuffer));
        if(0 == (ulConfigFlags & ECM_TEST_FLAG_SILENT)) {
            if(iLastError[iIdx] != iError) {
                iLastError[iIdx] = iError;
                printf("Error %d in I/O cycle: %s\n", iError, szBuffer);
            } else {
                iLastErrorCount[iIdx]++;
            }
        }
    } else {
        if(iLastErrorCount[iIdx] > 0) {
            printf("Error %d in I/O cycle repeated for %d cyles\n",
                       iLastError[iIdx], iLastErrorCount[iIdx]);
        }
        iLastError[iIdx]      = ECM_SUCCESS;
        iLastErrorCount[iIdx] = 0;
    }

    return;
};

/*
 * This handler is called with the start of each cycle before the input
 * or output data is processed.

 */
static int CycleStartHandler(ECM_HANDLE hndDevice, int error)
{
    static int iCycleCount = 0;
    static uint64_t ullLastTick = 0;
    static uint64_t ullMax = 0;
    uint64_t ullAct, ullDiff = 0;
    uint32_t ulMax = 0;

    (void)hndDevice;    /* Prevent compiler warning */

    /* 
     * The following code calculates the maximum jitter of the
     * cyclic handler and indicates this every time a new maximum
     * is detected and a configurable threshold is exceeded.
     */
    if((ulConfigFlags & ECM_TEST_FLAG_CHECK_JITTER) != 0) {
        ecmGetClockCycles(&ullAct);
        if(0 == ullLastTick) {
            ullLastTick = ullAct;
        } else {
            ullDiff = ullAct - ullLastTick;
            ullLastTick = ullAct;
            if(ullDiff > ullMax) {
                ullMax = ullDiff;
                ulMax = (uint32_t)((ullMax * 1000000) / g_ulCyclesPerSec);
                if(ulMax > g_ulCycleTimeViolation) {
                    ulMax -= g_ulCycleTime;
                    (void)ecmTraceEvent(ECM_TEST_TRACE_ID, iCycleCount,
                                       (int32_t)(ulMax));
                    if(0 == (ulConfigFlags & ECM_TEST_FLAG_SILENT)) {
                        fprintf(ECMlogFile,"Jitter (%d) %d us\n", iCycleCount, ulMax);
                    }
                }
            }
        }
    }

    /*
     * DO NOT CALL CycleHandlerErrorMessage() IN PRODUCTION CODE.
     */
    CycleHandlerErrorMessage(error, ECM_TEST_CYCLIC_START_HANDLER);

    iCycleCount++;


    return(0);
}

/*
 * This handler is called with each cycle to process the input data and
 * to setup the output data which are to be transmitted with the next cycle
 * from the application point of view.
 */
static int CycleHandler(ECM_HANDLE hndDevice, int error)
{
    static uint8_t	ucData;
	static uint64_t ucData64;
	static uint8_t	ucData_OperationMode;	//Sets Operation Mode register value
	static uint8_t	ucData_TargetPosition;	//Sets Position value
	static uint32_t cycleCounter;

	static BOOLEAN faul_reset_is_done = FALSE;//EF mod: To send a "RESET FAULT" just once

    (void)hndDevice;    /* Prevent compiler warning */

    /*
     * DO NOT CALL CycleHandlerErrorMessage() IN PRODUCTION CODE.
     */
    CycleHandlerErrorMessage(error, ECM_TEST_CYCLIC_HANDLER);


	//EF mod: Just another value in output data!
/*	if (cycleCounter < 500) {

		if (!faul_reset_is_done) {
			ucData = 128;//128: Fault reset
			faul_reset_is_done = TRUE;

			// Set output data
			if (0 == ucData_OperationMode) {
				ucData_OperationMode = 1;//Sets Operation Mode once and forever: Profile Position mode
			}

			// Update output data in process image
			if (pucDio_OperationMode != 0) {
				*pucDio_OperationMode = ucData_OperationMode;//EF mod, this pointer must be 10 bytes offset to "pucDio" --> OK!
			}
		}
		else {
			ucData = 0;//0: default
		}
	}
	else if (cycleCounter < 600) {
		ucData = 6; //6: Shutdown 
	}

	else if (cycleCounter < 700) {
		ucData = 7; //7: Switch on
	}
	else if (cycleCounter < 720) {
		ucData = 15;//15: Switch on
		//ucData_TargetPosition = 200;
		if (*pucDio_TargetPosition == 0) {
			//*pucDio_TargetPosition++ = ucData_TargetPosition;
			*pucDio_TargetPosition = 0xD0;//LSB
			*(pucDio_TargetPosition + 1) = 0x17;//MSB
		}
	}
	else {
		ucData = 31;//ON! Motor should move now!

	}
	cycleCounter++;*/
	//----------------------------

    // Update CONTROL WORD data in process image */
    /*if (pucDio != 0) {
        *pucDio = ucData;//Controlword!
    }*/

    /* If we start transmitting the data they may not be available yet */
    if ((g_ulIoMode & ECM_TEST_IO_MODE_TX_BEFORE_RX) != 0) {
#if defined(_OS9000)
	u_int32 ticks = 1;
	
	do {
        signal_code sig = SIGWAKE;
        if (_os_sleep(&ticks, &sig) || fatalsignal(sig)) {
            break;
        }
    } while (ticks);
#else	
        ecmSleep(1);
#endif		
    }

    return(0);
}

static int CycleEndHandler(ECM_HANDLE hndDevice, int error)
{
    (void)hndDevice;    /* Prevent compiler warning */

    /*
     * DO NOT CALL CycleHandlerErrorMessage() IN PRODUCTION CODE.
     */
    CycleHandlerErrorMessage(error, ECM_TEST_CYCLIC_END_HANDLER);

    /* If the acyclic worker task is not running do the call here */
    if ((g_ulIoMode & ECM_TEST_IO_MODE_NO_ACYC_WORKER) != 0) {
        ecmProcessAcyclicCommunication(hndDevice);
    }

    return(0);
}

/*
 * Sample code to use the async I/O interface of the master stack
 * to read and decode ESI EEPROM data
 */
static int PrintEsiInformation(char *buffer, uint32_t ulSzBuffer)
{
    ECM_ESI_CATEGORY cat1, cat2;
    int j, result;
    uint16_t usIdx, usEntries = 20;
    ECM_ESI_CATEGORY_HEADER header[20];

    result = ecmGetEsiCategoryList(buffer, ulSzBuffer, header, &usEntries);
    if ((result != ECM_SUCCESS) && (result != ECM_E_NOT_FOUND)) {
        fprintf(ECMlogFile,"ecmGetEsiCategoryList() failed with %s\n", ecmResultToString(result));
        return(result);
    }

    fprintf(ECMlogFile," Index | Size | Categotry type\n");
    fprintf(ECMlogFile,"-------+------+--------------------\n");
    for (j = 0; j < usEntries; j++) {
        fprintf(ECMlogFile," %04x  | %04x | ", header[j].usCategoryType,
            header[j].usCategorySize);
        switch(header[j].usCategoryType) {
        case ECM_ESI_CATEGORY_TYPE_NOP:
            fprintf(ECMlogFile,"No info\n");
            break;
        case ECM_ESI_CATEGORY_TYPE_STRING:
            fprintf(ECMlogFile,"String repository");
            break;
        case ECM_ESI_CATEGORY_TYPE_DATA_TYPE:
            fprintf(ECMlogFile,"Data types");
            break;
        case ECM_ESI_CATEGORY_TYPE_GENERAL:
            fprintf(ECMlogFile,"General information");
            break;
        case ECM_ESI_CATEGORY_TYPE_FMMU:
            fprintf(ECMlogFile,"FMMUs");
            break;
        case ECM_ESI_CATEGORY_TYPE_SYNCMAN:
            fprintf(ECMlogFile,"Sync Manager");
            break;
        case ECM_ESI_CATEGORY_TYPE_TXPDO:
            fprintf(ECMlogFile,"TxPDO description");
            break;
        case ECM_ESI_CATEGORY_TYPE_RXPDO:
            fprintf(ECMlogFile,"TxPDO description");
            break;
        case ECM_ESI_CATEGORY_TYPE_DC:
            fprintf(ECMlogFile,"Distributed Clock");
            break;
        default:
            fprintf(ECMlogFile,"Unknown");
            if (header[j].usCategoryType & ECM_ESI_VENDOR_SPECIFIC) {
                fprintf(ECMlogFile," (Vendor specific)");
            }
        }
        fprintf(ECMlogFile,"\n");
    }

    fprintf(ECMlogFile,"-------+------+--------------------\n");

    /* Dump general category */
    result = ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_GENERAL,
                               0, &cat1);
    if (ECM_SUCCESS == result) {
        ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
            cat1.general.ucGroupIdx, &cat2);
        fprintf(ECMlogFile,"Group Information : %s\n", cat2.cString);
        ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
            cat1.general.ucImgIdx, &cat2);
        fprintf(ECMlogFile,"Image Name        : %s\n", cat2.cString);
        ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
            cat1.general.ucOrderIdx, &cat2);
        fprintf(ECMlogFile,"Order Number      : %s\n", cat2.cString);
        ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
            cat1.general.ucNameIdx, &cat2);
        fprintf(ECMlogFile,"Device Name       : %s\n", cat2.cString);
    }

    /* Dump FMMU category */
    for (usIdx = 0; usIdx < 256; usIdx++) {
        result = ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_FMMU,
                                usIdx, &cat1);
        if (result != ECM_SUCCESS) {
            break;
        }
        fprintf(ECMlogFile,"FMMU%d\n", usIdx);
        fprintf(ECMlogFile," Usage            : ");
        switch(cat1.fmmu) {
            case ECM_ESI_FMMU_TYPE_NONE:
            case ECM_ESI_FMMU_TYPE_UNUSED:
                fprintf(ECMlogFile,"Unused\n");
                break;
            case ECM_ESI_FMMU_TYPE_INPUTS : fprintf(ECMlogFile,"Inputs\n"); break;
            case ECM_ESI_FMMU_TYPE_OUTPUTS: fprintf(ECMlogFile,"Outputs\n"); break;
            case ECM_ESI_FMMU_TYPE_MBOXSTATE: fprintf(ECMlogFile,"MBox state\n"); break;
            default: fprintf(ECMlogFile,"Invalid\n"); break;
        }
    }

    /* Dump Sync Manager category */
    for (usIdx = 0; usIdx < 256; usIdx++) {
        result = ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_SYNCMAN,
                                usIdx, &cat1);
        if (result != ECM_SUCCESS) {
            break;
        }
        fprintf(ECMlogFile,"SM%d\n", usIdx);
        fprintf(ECMlogFile," Physical Start   : %0#4x\n", cat1.sm.usPhysStartAddr);
        fprintf(ECMlogFile," Length           : %0#4x\n", cat1.sm.usLength);
        fprintf(ECMlogFile," Ctrl Register    : %0#2x\n", cat1.sm.ucControlReg);
        fprintf(ECMlogFile," Enable           : ");
        if (cat1.sm.ucEnable & ECM_ESI_SM_FLAG_ENABLE) {
            fprintf(ECMlogFile,"Yes");
        } else {
            fprintf(ECMlogFile,"No");
        }
        if (cat1.sm.ucEnable & ECM_ESI_SM_FLAG_FIXED) {
            fprintf(ECMlogFile," Fixed");
        }
        if (cat1.sm.ucEnable & ECM_ESI_SM_FLAG_VIRTUAL) {
            fprintf(ECMlogFile," Virtual");
        }
        if (cat1.sm.ucEnable & ECM_ESI_SM_FLAG_OP_ONLY) {
            fprintf(ECMlogFile," OpOnly");
        }
        fprintf(ECMlogFile,"\n");
        fprintf(ECMlogFile," Usage            : ");
        switch(cat1.sm.ucType) {
            case ECM_ESI_SM_TYPE_UNUSED: fprintf(ECMlogFile,"Unused\n"); break;
            case ECM_ESI_SM_TYPE_MBOX_OUT : fprintf(ECMlogFile,"MBox out\n"); break;
            case ECM_ESI_SM_TYPE_MBOX_IN : fprintf(ECMlogFile,"MBox in\n"); break;
            case ECM_ESI_SM_TYPE_DATA_OUT : fprintf(ECMlogFile,"Process data out\n"); break;
            case ECM_ESI_SM_TYPE_DATA_IN : fprintf(ECMlogFile,"Process data in\n"); break;
            default: fprintf(ECMlogFile,"Invalid\n"); break;
        }
    }

    /* Dump RxPDO category */
    for (usIdx = 0; usIdx < 256; usIdx++) {
        result = ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_RXPDO,
                                usIdx, &cat1);
        if (result != ECM_SUCCESS) {
            break;
        }
        fprintf(ECMlogFile,"RxPDO%d\n", usIdx);
        fprintf(ECMlogFile," Index   : %0#4x\n", cat1.pdo.usIndex);
        ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
            cat1.pdo.ucNameIdx, &cat2);
        fprintf(ECMlogFile," Name    : %s\n", cat2.cString);
        fprintf(ECMlogFile," SyncMan : %0#2x\n", cat1.pdo.ucSM);
        fprintf(ECMlogFile," DC Sync : %0#2x\n", cat1.pdo.ucDcSync);
        for (j = 0; j < cat1.pdo.ucNumEntries; j++) {
            fprintf(ECMlogFile," Object_%d\n", j);
            fprintf(ECMlogFile,"  Index    : %0#4x\n", cat1.pdo.entry[j].usIndex);
            fprintf(ECMlogFile,"  Subindex : %0#2x\n", cat1.pdo.entry[j].ucSubIndex);
            ecmGetEsiCategory(buffer, ulSzBuffer, ECM_ESI_CATEGORY_TYPE_STRING,
                cat1.pdo.entry[j].ucNameIdx, &cat2);
            fprintf(ECMlogFile,"  Name     : %s\n", cat2.cString);
            fprintf(ECMlogFile,"  Bitlen   : %0#2x\n", cat1.pdo.entry[j].ucBitLen);
        }
    }


    return(0);
}

/*
 * Sample code to use the async I/O interface of the master stack
 * to read ESC register and the EEPROM
 */
static int PrintEscRegister(ECM_HANDLE hndMaster)
{
    int result, port;
    int32_t iAddr = 0;
    uint16_t usCount, usStatusDL = 0;
    ECM_ESC_INFORMATION EscInfo;
    ECM_ESC_ERROR_COUNTER EscErrorCounter;
    uint16_t usBufferSz;
    uint8_t ucBuffer[2048];

    /* Return if not configured */
    if (0 == (ulConfigFlags & ECM_TEST_FLAG_PRINT_ESC)) {
      return(0);
    }

    /* Blocking operation not possible without cyclic worker task */
    if ((g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER) != 0) {
      fprintf(ECMlogFile,"Blocking ESC access not supported without cyclic worker !\n");
      return(-1);
    }

    /* Get number of slaves */
    ECM_GET_SLAVE_COUNT(hndMaster, &usCount, &result);
    fprintf(ECMlogFile,"Found %d active slaves\n", usCount);

    /* Iterate for all slaves */
    while(usCount-- != 0) {

        /* Dump ESC info */
        ECM_INIT(EscInfo);
        ECM_GET_ESC_INFO(hndMaster, iAddr, &EscInfo, &result);
        if (ECM_SUCCESS == result) {
            fprintf(ECMlogFile,"ESC Info:\n");
            fprintf(ECMlogFile,"---------\n");
            fprintf(ECMlogFile,"Type             : ");
            switch(EscInfo.ucType) {
                case ECM_ESC_TYPE_BKHF_TERMINAL_1:
                    fprintf(ECMlogFile,"Beckhoff first terminals\n");
                    break;
                case ECM_ESC_TYPE_ESC20:
                    fprintf(ECMlogFile,"Beckhoff ESC 10 or ESC 20\n");
                    break;
                case ECM_ESC_TYPE_EK1100:
                    fprintf(ECMlogFile,"Beckhoff EK1100-\n");
                    break;
                case ECM_ESC_TYPE_IP_CORE:
                    fprintf(ECMlogFile,"Beckhoff IP Core\n");
                    break;
                case ECM_ESC_TYPE_BKHF_TERMINAL_2:
                    fprintf(ECMlogFile,"Beckhoff current terminals\n");
                    break;
                case ECM_ESC_TYPE_ET1100:
                    fprintf(ECMlogFile,"Beckhoff ET1100");
                    if (0 == EscInfo.usBuild) {
                        fprintf(ECMlogFile,"0001\n");
                    } else {
                        fprintf(ECMlogFile,"%04d\n", EscInfo.usBuild);
                    }
                    break;
                case ECM_ESC_TYPE_ET1200:
                    fprintf(ECMlogFile,"Beckhoff ET1200\n");
                    break;
                default:
                    fprintf(ECMlogFile,"Unknown ESC type %02x\n", EscInfo.ucType);
                    break;
            }
            fprintf(ECMlogFile,"Version          : %d.%d\n", EscInfo.ucRevision, EscInfo.usBuild);
            fprintf(ECMlogFile,"#FMMUs           : %d\n", EscInfo.ucNumFMMU);
            fprintf(ECMlogFile,"#SM              : %d\n", EscInfo.ucNumSM);
            fprintf(ECMlogFile,"#RAM Size        : %d KByte\n", EscInfo.ucRamSize);
            fprintf(ECMlogFile,"Port Description : %x\n", EscInfo.ucPortDescription);
            fprintf(ECMlogFile,"Features         : %04lx\n", (unsigned long)EscInfo.ulFeatures);
        }

        /* Dump DL status register */
        ECM_GET_DL_STATUS(hndMaster, iAddr, &usStatusDL, &result);

        if (ECM_SUCCESS == result) {
            fprintf(ECMlogFile,"\nESC DL Status:\n");
            fprintf(ECMlogFile,"--------------\n");
            fprintf(ECMlogFile,"PDI Operational         : %s\n",
                (usStatusDL & ESC_DL_STATUS_PDI_OPERATIONAL) ? "True" : "False");
            fprintf(ECMlogFile,"PDI Watchdog            : %s\n",
                (usStatusDL & ESC_DL_STATUS_WD_RELOADED) ? "Running" : "Expired");
            fprintf(ECMlogFile,"Enhanced Link Detection : %s\n",
                (usStatusDL & ESC_DL_STATUS_ENH_LINK_DETECT) ? "Enabled" : "Disabled");
            for (port = 0; port < 4; port++) {
                fprintf(ECMlogFile,"Physicl link port %c     : %s\n", 'A'+ port,
                    (usStatusDL & ESC_DL_STATUS_ACT_LINK_PORT0 << port) ?
                    "Active" : "Inactive");
            }
            for (port = 0; port < 4; port++) {
                fprintf(ECMlogFile,"Loop state port %c       : ", 'A'+ port);
                switch(ECM_ESC_GET_PORT_LOOP_CONFIG(usStatusDL, port)) {
                        case 0x00:
                            fprintf(ECMlogFile,"Open, no stable communication\n");
                            break;
                        case 0x01:
                            fprintf(ECMlogFile,"Closed, no stable communication\n");
                            break;
                        case 0x02:
                            fprintf(ECMlogFile,"Open, communication established\n");
                            break;
                        case 0x03:
                            fprintf(ECMlogFile,"Closed, communication established\n");
                            break;
                }
            }
        }

        /* Dump and reset ESC error counter */
        ECM_GET_ESC_ERROR_COUNTER(hndMaster, iAddr, &EscErrorCounter, &result);

        for (port = 0; port < 4; port++) {
            if (usStatusDL & ESC_DL_STATUS_ACT_LINK_PORT0 << port) {
                fprintf(ECMlogFile,"Invalid frames port %c   : %d\n", 'A'+ port,
                    *((uint8_t *)&EscErrorCounter + (port << 1)));
                fprintf(ECMlogFile,"Rx errors port %c        : %d\n", 'A'+ port,
                    *((uint8_t *)&EscErrorCounter + 1 + (port << 1)));
                fprintf(ECMlogFile,"Forwarded errors port %c : %d\n", 'A'+ port,
                    *((uint8_t *)&EscErrorCounter + 8 + port));
                fprintf(ECMlogFile,"Lost link port %c        : %d\n", 'A'+ port,
                    *((uint8_t *)&EscErrorCounter + 16 + port));
            }
        }
        ECM_CLR_ESC_ERROR_COUNTER(hndMaster, iAddr, &result);

        ECM_GET_ESC_ERROR_COUNTER(hndMaster, iAddr, &EscErrorCounter, &result);

        /*
         * Assign ESI EEPROM access to EtherCAT
         */
        usBufferSz = 0;
        result = ecmWriteEeprom(hndMaster, iAddr, 0, &usBufferSz,
                                (uint16_t *)ucBuffer);
        if (result != ECM_SUCCESS) {
            ecmFormatError(result, ECM_ERROR_FORMAT_LONG, (char *)ucBuffer,
                sizeof(ucBuffer));
            fprintf(ECMlogFile,"Assigning EEPROM to ECAT failed with %d: %s\n", result, ucBuffer);
        }

        /*
         * Dump 100 words of EEPROM content as hexadecimal values starting at
         * offset 0 or use the ESI EEPROM support routines to decode the
         * EEPROM data.
         */
        if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_ESI) != 0) {
            usBufferSz = sizeof(ucBuffer) / 2;
        } else {
            usBufferSz = 100;
        }

        result = ecmReadEeprom(hndMaster, iAddr, 0, &usBufferSz,
                (uint16_t *)ucBuffer);
        if (result != ECM_SUCCESS) {
            ecmFormatError(result, ECM_ERROR_FORMAT_LONG, (char *)ucBuffer,
                           sizeof(ucBuffer));
            fprintf(ECMlogFile,"Reading EEPROM failed with %d: %s\n", result, ucBuffer);
        } else {
            if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_ESI) != 0) {
                fprintf(ECMlogFile,"\nESI EEPROM data:\n");
                fprintf(ECMlogFile,"-----------------\n");
                PrintEsiInformation((char *)ucBuffer, usBufferSz);
            } else {
                fprintf(ECMlogFile,"\nEEPROM data:\n");
                fprintf(ECMlogFile,"------------\n");
                hexDump(ucBuffer, usBufferSz, 0);
            }
            fprintf(ECMlogFile,"\n");
        }

        iAddr--;
    }

    return(0);
}

/*
 * Helper code to log detailed version information
 */
static void PrintVersionInformation(ECM_VERSION *pecmVersion)
{
    fprintf(ECMlogFile,"ESD EtherCAT master : V%x.%x.%x",
        (pecmVersion->usVersionMaster >> 12) & 0x0F,
        (pecmVersion->usVersionMaster >>  8) & 0x0F,
        (pecmVersion->usVersionMaster      ) & 0xFF);
    if (pecmVersion->ulFeatures & ECM_FEATURE_DEBUG_BUILD) {
        fprintf(ECMlogFile," (Debug build)");
    }
    if (pecmVersion->ulFeatures & ECM_FEATURE_TRIAL_VERSION) {
        fprintf(ECMlogFile," (Trial version)");
    }
    fprintf(ECMlogFile,"\n");
    fprintf(ECMlogFile,"Expat XML-Parser    : V%x.%x.%x\n",
        (pecmVersion->usVersionParser >> 12) & 0x0F,
        (pecmVersion->usVersionParser >>  8) & 0x0F,
        (pecmVersion->usVersionParser      ) & 0xFF);
    fprintf(ECMlogFile,"Zlib compression    : V%x.%x.%x\n",
        (pecmVersion->usVersionZlib >> 12) & 0x0F,
        (pecmVersion->usVersionZlib >>  8) & 0x0F,
        (pecmVersion->usVersionZlib      ) & 0xFF);
    fprintf(ECMlogFile,"HAL layer           : V%x.%x.%x\n",
        (pecmVersion->usVersionHal >> 12) & 0x0F,
        (pecmVersion->usVersionHal >>  8) & 0x0F,
        (pecmVersion->usVersionHal      ) & 0xFF);
    fprintf(ECMlogFile,"Remote protocol     : V%x.%x.%x\n",
        (pecmVersion->usVersionRemote >> 12) & 0x0F,
        (pecmVersion->usVersionRemote >>  8) & 0x0F,
        (pecmVersion->usVersionRemote      ) & 0xFF);
    fprintf(ECMlogFile,"Operating system    : V%d.%d.%d (",
        (pecmVersion->usVersionOs >> 12) & 0x0F,
        (pecmVersion->usVersionOs >>  8) & 0x0F,
        (pecmVersion->usVersionOs      ) & 0xFF);
    switch(pecmVersion->usTypeOs & ECM_OS_TYPE_MASK) {
        case ECM_OS_TYPE_WIN32:   fprintf(ECMlogFile,"Windows"); break;
        case ECM_OS_TYPE_LINUX:   fprintf(ECMlogFile,"Linux"); break;
        case ECM_OS_TYPE_NTO:     fprintf(ECMlogFile,"QNX/Neutrino"); break;
        case ECM_OS_TYPE_VXWORKS: fprintf(ECMlogFile,"VxWorks"); break;
        case ECM_OS_TYPE_RTX:     fprintf(ECMlogFile,"RTX"); break;
        case ECM_OS_TYPE_WINCE:   fprintf(ECMlogFile,"WinCE"); break;
        case ECM_OS_TYPE_OS9:     fprintf(ECMlogFile,"OS-9"); break;
        default: fprintf(ECMlogFile,"Unknown"); break;
    }
	fprintf(ECMlogFile," / %d-bit", (pecmVersion->usTypeOs & ECM_OS_64BIT) ? 64 : 32);
    fprintf(ECMlogFile," / %s endian)\n", (pecmVersion->usTypeOs & ECM_OS_BIG_ENDIAN) ?
                              "Big" : "Little");
    if(pecmVersion->pszBuildString != NULL) {
        fprintf(ECMlogFile,"Build information   : %s\n",
            pecmVersion->pszBuildString);
    }
    fprintf(ECMlogFile,"Min. cycle time     : %lu us\n",
       (unsigned long)pecmVersion->ulMinCycleTime);
    fprintf(ECMlogFile,"Highres counter     : %lu ticks/sec\n",
       (unsigned long)g_ulCyclesPerSec);
    fprintf(ECMlogFile,"Features            : 0x%08lx\n",
       (unsigned long)pecmVersion->ulFeatures);
}

/*
 * Helper code to log detailed information of a configured device
 */
static void PrintDeviceDescription(ECM_HANDLE hndDevice)
{
    ECM_DEVICE_DESC descDevice;

    if (ecmGetDeviceState(hndDevice, &descDevice, NULL) != ECM_SUCCESS) {
        return;
    }

    fprintf(ECMlogFile,"Primary adapter MAC   : %02X-%02X-%02X-%02X-%02X-%02X\n",
        descDevice.macAddr[ECM_NIC_PRIMARY].b[0],
        descDevice.macAddr[ECM_NIC_PRIMARY].b[1],
        descDevice.macAddr[ECM_NIC_PRIMARY].b[2],
        descDevice.macAddr[ECM_NIC_PRIMARY].b[3],
        descDevice.macAddr[ECM_NIC_PRIMARY].b[4],
        descDevice.macAddr[ECM_NIC_PRIMARY].b[5]);
    fprintf(ECMlogFile,"Redundant adapter MAC : %02X-%02X-%02X-%02X-%02X-%02X\n",
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[0],
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[1],
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[2],
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[3],
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[4],
        descDevice.macAddr[ECM_NIC_REDUNDANT].b[5]);
    fprintf(ECMlogFile,"Cycle time            : %d\n", descDevice.ulCycleTime);
    fprintf(ECMlogFile,"Watchdog cycle        : %d\n", descDevice.usCycleWatchdog);
    fprintf(ECMlogFile,"Link check cycle      : %d\n", descDevice.usCycleLinkState);
    fprintf(ECMlogFile,"DC control cycle      : %d\n", descDevice.usCycleDcCtrl);
    fprintf(ECMlogFile,"Max acyclic frames    : %d\n", descDevice.usAcycCtrl);
    fprintf(ECMlogFile,"\n");

    return;
}

/*
 * Helper code to log detailed information of a configured master
 */
static void PrintMasterDescription(ECM_HANDLE hndMaster)
{
    ECM_MASTER_DESC descMaster;

    ECM_INIT(descMaster);

    /* Call once to get the master device name  */
    if (ecmGetMasterState(hndMaster, &descMaster, NULL) != ECM_SUCCESS) {
        return;
    }

    fprintf(ECMlogFile,"Device Name       : %s\n", descMaster.szName);

    /* Call a 2nd time to get (optional) ENI filename */
    descMaster.ulFlags = ECM_FLAG_MASTER_ENI_FILENAME;
    if (ecmGetMasterState(hndMaster, &descMaster, NULL) != ECM_SUCCESS) {
        return;
    }
    if((descMaster.ulFlags & ECM_FLAG_MASTER_ENI_FILENAME) != 0) {
        fprintf(ECMlogFile,"ENI filename      : %s\n", descMaster.szName);
    }

    /* Call a 3rd time to get (optional) project GUID */
    descMaster.ulFlags = ECM_FLAG_MASTER_PROJECT_GUID;
    if (ecmGetMasterState(hndMaster, &descMaster, NULL) != ECM_SUCCESS) {
        return;
    }
    if((descMaster.ulFlags & ECM_FLAG_MASTER_PROJECT_GUID) != 0) {
        fprintf(ECMlogFile,"Project GUID      : %s\n", descMaster.szName);
    }

    fprintf(ECMlogFile,"Destination MAC   : %02X-%02X-%02X-%02X-%02X-%02X\n",
        descMaster.macAddr.b[0], descMaster.macAddr.b[1],
        descMaster.macAddr.b[2], descMaster.macAddr.b[3],
        descMaster.macAddr.b[4], descMaster.macAddr.b[5]);
    fprintf(ECMlogFile,"VLAN TCI          : %04x\n", descMaster.vlanTCI);
    fprintf(ECMlogFile,"Mbox slaves       : %04x\n", descMaster.usMboxCount);
    fprintf(ECMlogFile,"Mbox State Addr.  : %08lx\n",
        (unsigned long)descMaster.ulMboxStateAddr);
    fprintf(ECMlogFile,"Recv (Addr/Size)  : %p/%lu\n",
        descMaster.pInput, (unsigned long)descMaster.ulSzInput);
    fprintf(ECMlogFile,"Send (Addr/Size)  : %p/%lu\n",
        descMaster.pOutput, (unsigned long)descMaster.ulSzOutput);
    fprintf(ECMlogFile,"DC Mode           : ");
    if(descMaster.ulFlags & ECM_FLAG_MASTER_DC) {
        fprintf(ECMlogFile,"Enabled");
        if(descMaster.ulFlags & ECM_FLAG_MASTER_DC_RESYNC) {
            fprintf(ECMlogFile," + Cont. Drift Compensation");
        }
        if(descMaster.ulFlags & ECM_FLAG_MASTER_DCM_CLOCK_SHIFT) {
            fprintf(ECMlogFile," + Master Clock Sync.");
        }
        fprintf(ECMlogFile,"\nUser Shift Master : %d us\n", descMaster.sDcUserShift);
        fprintf(ECMlogFile,"DC SysTime Epoch  : ");
        if(ECM_SYSTIME_EPOCH_UNIX == descMaster.ucDcSysTimeEpoch) {
            time_t now = time(NULL);
            fprintf(ECMlogFile,"%s", asctime(gmtime(&now)));
        } else if(ECM_SYSTIME_EPOCH_NONE == descMaster.ucDcSysTimeEpoch) {
            fprintf(ECMlogFile,"Reference clock\n");
        } else {
            fprintf(ECMlogFile,"Sat Jan 01 00:00:00 2000");
        }
    } else {
        fprintf(ECMlogFile,"Disabled");
    }

    fprintf(ECMlogFile,"\n");

    return;
}

/*
 * Helper code to log detailed information of a configured slave
 */
static void PrintSlaveDescription(ECM_HANDLE hndSlave)
{
    ECM_SLAVE_DESC descSlave;

    if (ecmGetSlaveState(hndSlave, &descSlave, NULL) != ECM_SUCCESS) {
        return;
    }

    fprintf(ECMlogFile,"Device Name       : %s\n", descSlave.szName);
    fprintf(ECMlogFile,"Phys Addr.        : %04x\n", descSlave.usPhysAddr);
    fprintf(ECMlogFile,"Auto Inc Addr.    : %04x\n", descSlave.usAutoIncAddr);
    fprintf(ECMlogFile,"Vendor ID         : %08lx\n", (unsigned long)descSlave.ulVendorId);
    fprintf(ECMlogFile,"Product Code      : %08lx\n", (unsigned long)descSlave.ulProductCode);
    fprintf(ECMlogFile,"Revision No.      : %08lx\n", (unsigned long)descSlave.ulRevisionNo);
    fprintf(ECMlogFile,"Serial No.        : %08lx\n", (unsigned long)descSlave.ulSerialNo);
    fprintf(ECMlogFile,"Recv (Start/Size) : %lu/%lu\n",
            (unsigned long)descSlave.ulRecvBitStart,
            (unsigned long)descSlave.ulRecvBitLength);
    fprintf(ECMlogFile,"Send (Start/Size) : %lu/%lu\n",
            (unsigned long)descSlave.ulSendBitStart,
            (unsigned long)descSlave.ulSendBitLength);
    if(descSlave.ulFlags & ECM_FLAG_SLAVE_DC) {
        fprintf(ECMlogFile,"DC Support        : %s Bit (DC %s)\n",
            (descSlave.ulFlags & ECM_FLAG_SLAVE_DC64) ? "32" : "64",
            (descSlave.ulFlags & ECM_FLAG_SLAVE_DC_REFCLOCK) ? "Master" : "Slave");
        fprintf(ECMlogFile,"SYNC0/1 (Shift)   : %lu/%lu (%ld) ns\n",
            (unsigned long)descSlave.ulCycleTime0,
            (unsigned long)descSlave.ulCycleTime1,
            (long)descSlave.lShiftTime);
    }
    fprintf(ECMlogFile,"\n");

    return;
}

/*
 * Helper code to print the copy vector
 */
static void PrintCopyVector(ECM_HANDLE hndMaster)
{
    ECM_MASTER_DESC descMaster;
    ECM_COPY_VECTOR *pCopyVector;
    uint32_t ulEntries, ulSum = 0, i;
    int result;

	//EF mod -- Uncomment next IF
	//auto a = ulConfigFlags & ECM_TEST_FLAG_PRINT_COPY_VECTOR;
    /* Return if not configured */
    if (0 == (ulConfigFlags & ECM_TEST_FLAG_PRINT_COPY_VECTOR)) {//Comment this IF to see these prints on screen
        return;
    }

    if (ecmGetMasterState(hndMaster, &descMaster, NULL) != ECM_SUCCESS) {
        return;
    }

    /* Copy vector of input process image */
    result = ecmGetCopyVector(hndMaster, NULL, &ulEntries, ECM_INPUT_DATA);
    if (ECM_SUCCESS == result) {
        pCopyVector = (PECM_COPY_VECTOR)calloc((size_t)ulEntries, sizeof(ECM_COPY_VECTOR));//EF mod
        if (pCopyVector != NULL) {
            ulSum = 0;
            result = ecmGetCopyVector(hndMaster, pCopyVector, &ulEntries, ECM_INPUT_DATA);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"Input process image:\n");
                fprintf(ECMlogFile,"--------------------\n");
                for (i = 0; i < ulEntries; i++) {
                    fprintf(ECMlogFile,"Offset = %08lu Size: %08lx\n",
                            (unsigned long)pCopyVector[i].ulOffset,
                            (unsigned long)pCopyVector[i].ulSize);
                    ulSum += pCopyVector[i].ulSize;
                }
            }
            fprintf(ECMlogFile,"---> %lu / %lu bytes to copy (%ld %%)\n",
                   (unsigned long)ulSum, (unsigned long)descMaster.ulSzInput,
                   (long)((ulSum  * 100) / descMaster.ulSzInput));
            free(pCopyVector);
        }
    }

    /* Copy vector of output process image */
    result = ecmGetCopyVector(hndMaster, NULL, &ulEntries, ECM_OUTPUT_DATA);
    if (ECM_SUCCESS == result) {
        pCopyVector = (PECM_COPY_VECTOR)calloc((size_t)ulEntries, sizeof(ECM_COPY_VECTOR));//EF mod
        if (pCopyVector != NULL) {
            ulSum = 0;
            result = ecmGetCopyVector(hndMaster, pCopyVector, &ulEntries, ECM_OUTPUT_DATA);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"Output process image:\n");
                fprintf(ECMlogFile,"---------------------\n");
                for (i = 0; i < ulEntries; i++) {
                    fprintf(ECMlogFile,"Offset = %08lu Size: %08lu\n",
                            (unsigned long)pCopyVector[i].ulOffset,
                            (unsigned long)pCopyVector[i].ulSize);
                    ulSum += pCopyVector[i].ulSize;
                }
            }
            fprintf(ECMlogFile,"---> %lu / %lu bytes to copy (%lu %%)\n",
                    (unsigned long)ulSum, (unsigned long)descMaster.ulSzOutput,
                    (unsigned long)((ulSum  * 100) / descMaster.ulSzOutput));
            free(pCopyVector);
        }
    }

    return;
}

/*
 * Helper code to log adapter, device and master statistics
 */
static void PrintStatistic(ECM_HANDLE hndDevice, ECM_HANDLE hndMaster)
{
    ECM_NIC_STATISTIC    diagPrimary, diagRedundant;
    ECM_DEVICE_STATISTIC diagDevPri, diagDevRed;
    ECM_MASTER_STATISTIC diagMaster;
    int result;

    /* Return if not configured */
    if (0 == (ulConfigFlags & ECM_TEST_FLAG_PRINT_STATISTICS)) {
        return;
    }

    fprintf(ECMlogFile,"Adapter Statistic\n");
    fprintf(ECMlogFile,"-----------------\n");

    /* Request NIC statistic */
    result = ecmGetNicStatistic(hndDevice, &diagPrimary, &diagRedundant);
    if (ECM_SUCCESS == result) {
        fprintf(ECMlogFile,"Primary:\n");
        fprintf(ECMlogFile," RX Frames (Total / Errors / Discarded) %lu, %lu, %lu\n",
            (unsigned long)diagPrimary.ulRxFrames,
            (unsigned long)diagPrimary.ulRxErrors,
            (unsigned long)diagPrimary.ulRxDiscarded);
        fprintf(ECMlogFile," TX Frames (Total / Errors / Discarded) %lu, %lu, %lu\n",
            (unsigned long)diagPrimary.ulTxFrames,
            (unsigned long)diagPrimary.ulTxErrors,
            (unsigned long)diagPrimary.ulTxDiscarded);
        if(diagPrimary.ulSupportedMask &
            (ECM_STATISTIC_RX_BYTES | ECM_STATISTIC_TX_BYTES)) {
                fprintf(ECMlogFile," Bytes (Rx / Tx) %lu, %lu\n",
                    (unsigned long)diagPrimary.ulRxBytes,
                    (unsigned long)diagPrimary.ulTxBytes);

        }
        if ((ulConfigFlags & ECM_TEST_FLAG_REDUNDANCY) != 0) {
            fprintf(ECMlogFile,"Redundant:\n");
            fprintf(ECMlogFile," RX Frames (Total / Errors / Discarded) %lu, %lu, %lu\n",
                (unsigned long)diagRedundant.ulRxFrames,
                (unsigned long)diagRedundant.ulRxErrors,
                (unsigned long)diagRedundant.ulRxDiscarded);
            fprintf(ECMlogFile," TX Frames (Total / Errors / Discarded) %lu, %lu, %lu\n",
                (unsigned long)diagRedundant.ulTxFrames,
                (unsigned long)diagRedundant.ulTxErrors,
                (unsigned long)diagRedundant.ulTxDiscarded);
            if(diagRedundant.ulSupportedMask &
                (ECM_STATISTIC_RX_BYTES | ECM_STATISTIC_TX_BYTES)) {
                    fprintf(ECMlogFile," Bytes (Rx / Tx) %lu, %lu\n",
                        (unsigned long)diagRedundant.ulRxBytes,
                        (unsigned long)diagRedundant.ulTxBytes);
            }
        }
    }
    fprintf(ECMlogFile,"\n");


    fprintf(ECMlogFile,"Device Statistic\n");
    fprintf(ECMlogFile,"----------------\n");

    /* Request device statistic */
    result = ecmGetDeviceStatistic(hndDevice, &diagDevPri, &diagDevRed);
    if (ECM_SUCCESS == result) {
        fprintf(ECMlogFile,"Primary:\n");
        fprintf(ECMlogFile," RX Frames (Total / ECAT / Discarded) %lu, %lu, %lu\n",
                (unsigned long)diagDevPri.ulRxFrames,
                (unsigned long)diagDevPri.ulRxEcatFrames,
                (unsigned long)diagDevPri.ulRxDiscarded);
        fprintf(ECMlogFile," TX Frames (Total / Error) %lu, %lu\n",
                (unsigned long)diagDevPri.ulTxEcatFrames,
                (unsigned long)diagDevPri.ulTxError);
        fprintf(ECMlogFile," Link lost %lu\n",
                (unsigned long)diagDevPri.ulLostLink);
        if ((ulConfigFlags & ECM_TEST_FLAG_REDUNDANCY) != 0) {
      fprintf(ECMlogFile,"Redundant:\n");
      fprintf(ECMlogFile," RX Frames (Total / ECAT / Discarded) %lu, %lu, %lu\n",
              (unsigned long)diagDevRed.ulRxFrames,
              (unsigned long)diagDevRed.ulRxEcatFrames,
              (unsigned long)diagDevRed.ulRxDiscarded);
      fprintf(ECMlogFile," TX Frames (Total / Error) %lu, %lu\n",
              (unsigned long)diagDevRed.ulTxEcatFrames,
              (unsigned long)diagDevRed.ulTxError);
      fprintf(ECMlogFile," Link lost %lu\n", (unsigned long)diagDevRed.ulLostLink);
    }
    }
    fprintf(ECMlogFile,"\n");


    fprintf(ECMlogFile,"Master Statistic\n");
    fprintf(ECMlogFile,"----------------\n");

    /* Request master statistic */
    result = ecmGetMasterStatistic(hndMaster, &diagMaster);
    if (ECM_SUCCESS == result) {
        fprintf(ECMlogFile,"RX Frames (Acyclic / Cyclic / Async) %lu, %lu, %lu\n",
                (unsigned long)diagMaster.ulRxAcyclicFrames,
                (unsigned long)diagMaster.ulRxCyclicFrames,
                (unsigned long)diagMaster.ulRxAsyncFrames);
        fprintf(ECMlogFile,"RX Frames discarded (Acyclic / Cyclic / Other) %lu, %lu, %lu\n",
                (unsigned long)diagMaster.ulRxAcyclicDiscarded,
                (unsigned long)diagMaster.ulRxCyclicDiscarded,
                (unsigned long)diagMaster.ulRxDiscarded);
        fprintf(ECMlogFile,"TX Frames (Acyclic / Cyclic / Async) %lu, %lu %lu\n",
                (unsigned long)diagMaster.ulTxAcyclicFrames,
                (unsigned long)diagMaster.ulTxCyclicFrames,
                (unsigned long)diagMaster.ulTxAsyncFrames);
    }

    fprintf(ECMlogFile,"\n");
}

/*
 * Helper code to print the list of variables in ENI file
 */
static void PrintVarList(ECM_HANDLE hndMaster)
{
    ECM_VAR_DESC VarDesc;
    int          result;

    /* Return if not configured */
    if (0 == (ulConfigFlags & ECM_TEST_FLAG_PRINT_VARLIST)) {
        return;
    }

    fprintf(ECMlogFile,"Process variables (Input):\n");
    fprintf(ECMlogFile,"--------------------------\n");
    result = ecmGetVariable(hndMaster, &VarDesc, ECM_FLAG_GET_FIRST);
    while(ECM_SUCCESS == result) {
        fprintf(ECMlogFile,"%s (%s)\n\tSize: %02d - Offset:%lu\n",
            VarDesc.pszName,
            (VarDesc.usDataType & ECM_FLAG_VAR_INPUT) ? "Input" : "Output",
            VarDesc.usBitSize, (unsigned long)VarDesc.ulBitOffs);
#if 0
        if (VarDesc.pszComment != NULL) {
            fprintf(ECMlogFile,"\t%s", VarDesc.pszComment);
        }
#endif
        fprintf(ECMlogFile,"\n\n");
        result = ecmGetVariable(hndMaster, &VarDesc, ECM_FLAG_GET_NEXT);
    }

    fprintf(ECMlogFile,"Process variables:\n");
    fprintf(ECMlogFile,"------------------\n");
    result = ecmLookupVariable(hndMaster, "Dev", &VarDesc, ECM_FLAG_GET_FIRST);
    while(ECM_SUCCESS == result) {
        fprintf(ECMlogFile,"%s\n\tSize: %02d - Offset:%lu\n\n",
            VarDesc.pszName, VarDesc.usBitSize, (unsigned long)VarDesc.ulBitOffs);
        result = ecmLookupVariable(hndMaster, "Dev", &VarDesc, ECM_FLAG_GET_NEXT);
    }
     
    fprintf(ECMlogFile,"Here\n");
}

/*
 * Helper code to dump and reset the profiling data
 */
static void PrintProfilingData(ECM_HANDLE hndDevice)
{
    ECM_PROFILING_DATA profData;
    int                result;

    if(g_ulProfileMask & (ECM_TEST_PROF_INPUT | ECM_TEST_PROF_OUTPUT | ECM_TEST_PROF_FRAME_TX)) {
        /* Get profiling data of ecmProcessInputData() */
        if(g_ulProfileMask & ECM_TEST_PROF_INPUT) {
            result = ecmGetProfilingData(hndDevice, &profData, ECM_PROFILE_INPUT);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"IN: (%lu / %lu / %lu)  ",
                    (unsigned long)profData.ulMin,
                    (unsigned long)profData.ulAvg,
                    (unsigned long)profData.ulMax);
                ecmGetProfilingData(hndDevice, NULL, ECM_PROFILE_INPUT);
            }
        }

        /* Get profiling data of ecmProcessOutputData() */
        if(g_ulProfileMask & ECM_TEST_PROF_OUTPUT) {
            result = ecmGetProfilingData(hndDevice, &profData, ECM_PROFILE_OUTPUT);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"OUT: (%lu / %lu / %lu)  ",
                    (unsigned long)profData.ulMin,
                    (unsigned long)profData.ulAvg,
                    (unsigned long)profData.ulMax);
                ecmGetProfilingData(hndDevice, NULL, ECM_PROFILE_OUTPUT);
            }
        }

        /* Get profiling data of Tx transmission time */
        if(g_ulProfileMask & ECM_TEST_PROF_FRAME_TX) {
            result = ecmGetProfilingData(hndDevice, &profData, ECM_PROFILE_FRAME_TX);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"TX: (%lu / %lu / %lu)",
                    (unsigned long)profData.ulMin,
                    (unsigned long)profData.ulAvg,
                    (unsigned long)profData.ulMax);
                ecmGetProfilingData(hndDevice, NULL, ECM_PROFILE_FRAME_TX);
            }
        }

        fprintf(ECMlogFile,"\n");
    }


    if(g_ulProfileMask & (ECM_TEST_PROF_ACYLIC | ECM_TEST_PROF_CYCLIC)) {

        /* Get profiling data of ecmProcessAcylicCommunication() */
        if(g_ulProfileMask & ECM_TEST_PROF_CYCLIC) {
            if(g_ulProfileMask & ECM_TEST_PROF_MODE_NO_IO_TIME) {
                fprintf(ECMlogFile,"*");
            }
            /* Get profiling data of ecmProcessInputData() */
            result = ecmGetProfilingData(hndDevice, &profData, ECM_PROFILE_CYCLIC_WORKER);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"CYC: (%lu / %lu / %lu)  ",
                    (unsigned long)profData.ulMin,
                    (unsigned long)profData.ulAvg,
                    (unsigned long)profData.ulMax);
                ecmGetProfilingData(hndDevice, NULL, ECM_PROFILE_CYCLIC_WORKER);
            }
        }

        /* Get profiling data of ecmProcessAcylicCommunication() */
        if(g_ulProfileMask & ECM_TEST_PROF_ACYLIC) {
            result = ecmGetProfilingData(hndDevice, &profData, ECM_PROFILE_ACYCLIC);
            if (ECM_SUCCESS == result) {
                fprintf(ECMlogFile,"ACYC: (%lu / %lu / %lu)",
                    (unsigned long)profData.ulMin,
                    (unsigned long)profData.ulAvg,
                    (unsigned long)profData.ulMax);
                ecmGetProfilingData(hndDevice, NULL, ECM_PROFILE_ACYCLIC);
            }
        }

        fprintf(ECMlogFile,"\n");
    }

    return;
}


#define ECM_OD_LIST_SIZE(entries)  (sizeof(ECM_COE_OD_LIST) + \
                                    ((entries - 1) * sizeof(uint16_t)))

#define ECM_OD_ENTRY_SIZE(payload) (sizeof(ECM_COE_ENTRY_DESCRIPTION) + payload)

static const char *GetAbortCodeText(uint32_t ulAbortCode)
{
    static char buffer[40];
    int iResult;

    iResult = ecmFormatError((int)ulAbortCode, ECM_ERROR_COE_ABORT_CODE,
                             buffer, sizeof(buffer));
    if (iResult != ECM_SUCCESS) {
        strcpy(buffer, "???");
    }

    return(buffer);
}

/*
 * Dump the data element defined by an IDN of a SoE slave.
 */
static void ecmTestSoeDump(ECM_HANDLE hndSlave, uint16_t usIDN)
{
    ECM_MBOX_SPEC              mboxHead;
    char                       buffer[80];
    PECM_SOE_STRING            pSoeString;
    uint32_t                   szBuffer, ulAttributes;
    int                        result;

    ecmSoeIdnToStr(usIDN, buffer);
    fprintf(ECMlogFile,"%s ", buffer);


    /* Request element name of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_NAME;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            fprintf(ECMlogFile,"*E:%04x*          ", mboxHead.soe.u.usError);
        } else {
            fprintf(ECMlogFile,"!E:%04x!          ", result);
        }
    } else {
        pSoeString = (PECM_SOE_STRING)buffer;
        if(pSoeString->ucLength > 20) {
            pSoeString->cString[20] = '\0';
        } else {
            pSoeString->cString[pSoeString->ucLength] = '\0';
        }
        fprintf(ECMlogFile,"%-20s ", pSoeString->cString);
    }

    /* Request unit of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_UNIT;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            fprintf(ECMlogFile,"*E:%04x*  ", mboxHead.soe.u.usError);
        } else {
            fprintf(ECMlogFile,"!E:%04x!  ", result);
        }
    } else {
        pSoeString = (PECM_SOE_STRING)buffer;
        pSoeString->cString[pSoeString->ucLength] = '\0';
        fprintf(ECMlogFile,"%-10s", pSoeString->cString);
    }

    /* Request attribute of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_ATTRIBUTE;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        fprintf(ECMlogFile,"! %04x ! ", result);
    } else {
        ecmCpuToLe(&ulAttributes, buffer, (const uint8_t *)"\x04");
        fprintf(ECMlogFile,"%08x ", ulAttributes);
    }

    /* Request minimum value of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_MIN;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            fprintf(ECMlogFile,"*E:%04x* ", mboxHead.soe.u.usError);
        } else {
            fprintf(ECMlogFile,"!E:%04x! ", result);
        }
    } else {
        fprintf(ECMlogFile,"%-8d ", *(uint16_t *)&buffer);
    }

    /* Request maximum value of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_MAX;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            fprintf(ECMlogFile,"*E:%04x* ", mboxHead.soe.u.usError);
        } else {
            fprintf(ECMlogFile,"!E:%04x! ", result);
        }
    } else {
        fprintf(ECMlogFile,"%-8d ", *(uint16_t *)&buffer);
    }

    /* Request value of data block */
    ECM_INIT(mboxHead);
    mboxHead.soe.u.usIDN = usIDN;
    mboxHead.soe.ucElements = ECM_SOE_ELEM_VALUE;
    szBuffer = sizeof(buffer);
    result = ecmSoeUpload(hndSlave, &mboxHead, buffer, &szBuffer);
    if (result != ECM_SUCCESS) {
        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            fprintf(ECMlogFile,"*E:%04x*\n", mboxHead.soe.u.usError);
        } else {
            fprintf(ECMlogFile,"!E:%04x!\n", result);
        }
    } else {
        fprintf(ECMlogFile,"%d\n", *(uint16_t *)&buffer);
    }

    return;
}

/*
 * Scan and dump the IDNs of complex slave supporting the SoE protocol.
 */
static void ecmTestSoeScan(ECM_HANDLE hndSlave)
{
    ECM_MBOX_SPEC              mboxHead;
    char                       buffer[400];
    PECM_SOE_ARRAY_16          pIdnList = NULL;
    uint32_t                   szBuffer;
    int                        i, result;
    uint16_t                   usSzList = 0, usCount;

    memset(buffer, 0, sizeof(buffer));
    ECM_INIT(mboxHead);
    /* ecmSoeStrToIDN(&mboxHead.soe.u.usIDN, "S-0-0187"); */
    ecmSoeStrToIDN(&mboxHead.soe.u.usIDN, "P-0-0030");
    mboxHead.soe.ucElements = ECM_SOE_ELEM_VALUE;
    ecmSoeDownload(hndSlave, &mboxHead, buffer, sizeof(buffer));
    /*
     * Request the list of all supported IDNs which size is unknown.
     */
    for(pIdnList = (PECM_SOE_ARRAY_16)buffer, usSzList = sizeof(buffer);;) {
        ECM_INIT(mboxHead);
        ecmSoeStrToIDN(&mboxHead.soe.u.usIDN, "S-0-0017");
        mboxHead.soe.ucElements = ECM_SOE_ELEM_VALUE;
        szBuffer = usSzList;
        result = ecmSoeUpload(hndSlave, &mboxHead, pIdnList, &szBuffer);
        if(ECM_SUCCESS == result) {
            /* IDN list fits into default buffer */
            break;
        }

        if(mboxHead.soe.ucCommand & ECM_SOE_FLAG_ERROR) {
            ecmFormatError(mboxHead.soe.u.usError, ECM_ERROR_SOE_ERROR_CODE,
                           buffer, sizeof(buffer));
            fprintf(ECMlogFile,"SoE upload returned with SoE error 0x%04x (%s)",
                           mboxHead.soe.u.usError, buffer);
        } else {
            /* Buffer too small */
            if((ECM_E_SIZE_MISMATCH == result) && (szBuffer >= 2)) {
                ecmCpuToLe(&usSzList, buffer, (const uint8_t *)"\x02");
                usSzList += 4;  /* Add header size */
                pIdnList = (PECM_SOE_ARRAY_16)calloc(1, usSzList);
                if(pIdnList != NULL) {
                    continue;   /* Retry request with returned size */
                } else {
                    break;      /* Stop on resource error */
                }
            } else {
                ecmFormatError(result, ECM_ERROR_FORMAT_LONG, buffer, sizeof(buffer));
                fprintf(ECMlogFile,"SoE upload returned with error %04x (%s)",
                       result, buffer);
                break;
            }
        }
    } /* of for */

    usCount = (usSzList - 4) / 2;
    fprintf(ECMlogFile,"SoE Device with %d IDNs\n\n", usCount);
    fprintf(ECMlogFile,"   IDN   Name                 Unit      Attrs    Min      Max      Data\n");
    fprintf(ECMlogFile,"-------------------------------------------------------------------------------\n");

    /* Iterate the list of all IDNs of the device */
    for(i = 0; i < usCount; i++) {
        uint16_t usIDN;

        ecmCpuToLe(&usIDN, &pIdnList->usData[i], (const uint8_t *)"\x02");
        ecmTestSoeDump(hndSlave, usIDN);
    }

    return;
}

/*
 * Helper code to dump the object dictionary of a complex slave supporting
 * CoE using the CoE support API of the master stack.
 */
static void ecmTestCoeScan(ECM_HANDLE hndSlave)
{
    ECM_SLAVE_DESC             DescSlave;
    ECM_COE_OD_LIST_COUNT      OdListCount;
    ECM_COE_OBJECT_DESCRIPTION ObjDesc;
    ECM_COE_OD_LIST            *pOdList;
    ECM_COE_ENTRY_DESCRIPTION  *pEntryDesc;
    ECM_MBOX_SPEC              mboxHead;
    char                       buffer[80];
    unsigned char              *pucData;
    uint32_t                   szBuffer, ulAbortCode, ulTemp;
    int                        i, j, k, result;
    uint16_t                   usTemp;

    /* Request number of object dictionary entries from slave */
    result = ecmCoeGetOdList(hndSlave, &OdListCount);
    if (result != ECM_SUCCESS) {
        fprintf(ECMlogFile,"Requesting # of OD entries for %s failed with %s\n",
               DescSlave.szName, ecmResultToString(result));
        return;
    }

    fprintf(ECMlogFile,"CoE object dictionary entries (%d) of: %s\n",
           OdListCount.usAll, DescSlave.szName);

    /* Request list with indexes of all dictionary entries from slave */
    pOdList = (PECM_COE_OD_LIST)calloc(1, ECM_OD_LIST_SIZE(OdListCount.usAll));
    if (NULL == pOdList) {
        fprintf(ECMlogFile,"Failed to allocate memory for OD index list\n");
        return;
    }

    pOdList->ulType  = ECM_LIST_TYPE_ALL;
    pOdList->usCount = OdListCount.usAll;
    result = ecmCoeGetOdEntries(hndSlave, pOdList);
    if (result != ECM_SUCCESS) {
        fprintf(ECMlogFile,"Requesting list of OD indexes failed with %s\n", ecmResultToString(result));
        free(pOdList);
        return;
    }

    /* Allocate memory for an object dictionary entry description */
    pEntryDesc = (PECM_COE_ENTRY_DESCRIPTION)calloc(1, ECM_OD_ENTRY_SIZE(256));
    if (NULL == pEntryDesc) {
        fprintf(ECMlogFile,"Failed to allocate memory for OD entry description\n");
        free(pOdList);
        return;
    }

    /*
     * Iterate list of object dictionary entries and request the description
     * for each object from the slave
     */
    for (i = 0; i < pOdList->usCount; i++) {
        ObjDesc.usIndex = pOdList->usIndex[i];
        result = ecmCoeGetObjDescription(hndSlave, &ObjDesc);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"  Requesting description for index %04x failed with %s\n",
                   pOdList->usIndex[i], ecmResultToString(result));
            if (ECM_E_ABORTED == result) {
                uint32_t ulAbortCode;
                ecmCoeGetAbortCode(hndSlave, &ulAbortCode);
                fprintf(ECMlogFile," Aborted: %s (0x%08lx)",
                    GetAbortCodeText(ulAbortCode), (unsigned long)ulAbortCode);
            }
            continue;
        }

        /* Print the object description */
        fprintf(ECMlogFile,"%04X (", ObjDesc.usIndex);
        switch(ObjDesc.ucObjCodeAndCategory & 0xF)
        {
        case ECM_COE_OBJ_ARRAY:
            fprintf(ECMlogFile,"ARR");
            break;
        case ECM_COE_OBJ_VAR:
            fprintf(ECMlogFile,"VAR");
            break;
        case ECM_COE_OBJ_RECORD:
            fprintf(ECMlogFile,"REC");
            break;
        default:
            fprintf(ECMlogFile,"???");
            break;
        }
        fprintf(ECMlogFile," / %c / ", (ObjDesc.ucObjCodeAndCategory & ECM_COE_OBJCAT_MANDATORY) ? 'M' : 'O');
        fprintf(ECMlogFile,"%2d) - %s\n", ObjDesc.usDataType, ObjDesc.szName);

        /*
         * Iterate the list of object dictionary sub-entries and request the
         * description for each sub-entry from slave
         */
        for (j = 0; j <= ObjDesc.ucMaxSubIndex; j++) {
            pEntryDesc->usIndex       = pOdList->usIndex[i];
            pEntryDesc->ucSubindex    = (uint8_t)j;
            pEntryDesc->ucRequestData = 0xFF;
            pEntryDesc->usSize        = ECM_OD_ENTRY_SIZE(256);

            result = ecmCoeGetEntryDescription(hndSlave, pEntryDesc);
            if (result != ECM_SUCCESS) {
                fprintf(ECMlogFile,"  Requesting description of entry (%04x/%02x) failed with %s\n",
                       pOdList->usIndex[i], j, ecmResultToString(result));
                if (ECM_E_ABORTED == result) {
                    ecmCoeGetAbortCode(hndSlave, &ulAbortCode);
                    fprintf(ECMlogFile,"  Aborted: %s (0x%08lx)",
                        GetAbortCodeText(ulAbortCode), (unsigned long)ulAbortCode);
                }
                continue;
            }

            /*
             * Some EtherCAT devices return an entry description with data size
             * and/or data type set to 0 which means that these entries do not
             * exit returning an abort code if the default value is requested.
             */
            if ((0 == pEntryDesc->usBitLen) || (0 == pEntryDesc->usDataType)) {
                continue;
            }

            /* Print the object dictionary entry description */
            fprintf(ECMlogFile,"  %02X: Bits = %2d Type = %2d ", j, pEntryDesc->usBitLen,
                   pEntryDesc->usDataType);
            if (pEntryDesc->usObjectAccess &
                (ECM_COE_ATTRIB_RP | ECM_COE_ATTRIB_RS | ECM_COE_ATTRIB_RO)) {
                    fprintf(ECMlogFile,"(R");
            } else {
                fprintf(ECMlogFile,"( ");
            }
            if (pEntryDesc->usObjectAccess &
                (ECM_COE_ATTRIB_WP | ECM_COE_ATTRIB_WS | ECM_COE_ATTRIB_WO)) {
                    fprintf(ECMlogFile,"W");
            } else {
                fprintf(ECMlogFile," ");
            }
            if (pEntryDesc->usObjectAccess & (ECM_COE_ATTRIB_RX | ECM_COE_ATTRIB_TX)) {
                fprintf(ECMlogFile,"M");
            } else {
                fprintf(ECMlogFile," ");
            }
            if (pEntryDesc->usObjectAccess & ECM_COE_ATTRIB_BU) {
                fprintf(ECMlogFile,"B");
            } else {
                fprintf(ECMlogFile," ");
            }
            if (pEntryDesc->usObjectAccess & ECM_COE_ATTRIB_ST) {
                fprintf(ECMlogFile,"S");
            } else {
                fprintf(ECMlogFile," ");
            }
            fprintf(ECMlogFile,") ");

            /* Read the current value of this entry */
            mboxHead.coe.usIndex    = pOdList->usIndex[i];
            mboxHead.coe.ucSubindex = (uint8_t)j;
            mboxHead.coe.ucFlags    = 0;
            szBuffer                = sizeof(buffer);
            result = ecmCoeSdoUpload(hndSlave, &mboxHead, buffer, &szBuffer);
            if (result != ECM_SUCCESS) {
                fprintf(ECMlogFile,"SDO upload error %s", ecmResultToString(result));
                if ((mboxHead.coe.ucFlags & ECM_COE_FLAG_ABORT_CODE) != 0) {
                    fprintf(ECMlogFile," (Abort code: 0x%08lx)", *(unsigned long *)buffer);
                }
                fprintf(ECMlogFile,"\n");
                continue;
            }

            /* Print the result (All data is little endian !!)*/
            fprintf(ECMlogFile,"Data = ");
            switch(pEntryDesc->usDataType)
            {
            case ECM_COE_TYP_BOOLEAN:
                fprintf(ECMlogFile,"%s", (*(uint8_t *)buffer & 0x1) ? "TRUE" : "FALSE");
                break;
            case ECM_COE_TYP_INT8:
            case ECM_COE_TYP_UINT8:
                fprintf(ECMlogFile,"%02x", *(uint8_t *)buffer);
                break;
            case ECM_COE_TYP_INT16:
            case ECM_COE_TYP_UINT16:
                ecmCpuToLe(&usTemp, buffer, (const uint8_t *)"\x02\0");
                fprintf(ECMlogFile,"%04x", usTemp);
                break;
            case ECM_COE_TYP_INT32:
            case ECM_COE_TYP_UINT32:
                ecmCpuToLe(&ulTemp, buffer, (const uint8_t *)"\x04\0");
                fprintf(ECMlogFile,"%08lx", (unsigned long)ulTemp);
                break;
            case ECM_COE_TYP_VIS_STRING:
                buffer[szBuffer] = '\0';
                fprintf(ECMlogFile,"%s", buffer);
                break;
            default:
                fprintf(ECMlogFile,"Unsupported");
                break;
            } /* of switch */

            fprintf(ECMlogFile,"\n");

            /* Print (optional) unit definition */
            pucData = ECM_COE_ENTRY_UNIT(pEntryDesc);
            if (pucData != NULL) {
                fprintf(ECMlogFile,"     Unit = 0x");
                for (k = 0; k < 4; k++) {
                    fprintf(ECMlogFile,"%02x", *pucData++);
                }
                fprintf(ECMlogFile,"\n");
            }

            /* Print (optional) default value definition */
            pucData = ECM_COE_ENTRY_DEFAULT_VALUE(pEntryDesc);
            if (pucData != NULL) {
                fprintf(ECMlogFile,"     Default Value = 0x");
                for (k = 0; k < ECM_DATA_BYTES(pEntryDesc->usBitLen); k++) {
                    fprintf(ECMlogFile,"%02x", *pucData++);
                }
                fprintf(ECMlogFile,"\n");
            }

            /* Print (optional) min value definition */
            pucData = ECM_COE_ENTRY_MIN_VALUE(pEntryDesc);
            if (pucData != NULL) {
                fprintf(ECMlogFile,"     Min Value = 0x");
                for (k = 0; k < ECM_DATA_BYTES(pEntryDesc->usBitLen); k++) {
                    fprintf(ECMlogFile,"%02x", *pucData++);
                }
                fprintf(ECMlogFile,"\n");
            }

            /* Print (optional) max value definition */
            pucData = ECM_COE_ENTRY_MAX_VALUE(pEntryDesc);
            if (pucData != NULL) {
                fprintf(ECMlogFile,"     Max Value = 0x");
                for (k = 0; k < ECM_DATA_BYTES(pEntryDesc->usBitLen); k++) {
                    fprintf(ECMlogFile,"%02x", *pucData++);
                }
                fprintf(ECMlogFile,"\n");
            }

            fprintf(ECMlogFile,"  %s\n", ECM_COE_ENTRY_NAME(pEntryDesc));
        } /* for j */
    } /* for i */

    /* Return dynamically allocated memory */
    free(pEntryDesc);
    free(pOdList);
}

/*
 * Helper code for application driven I/O returning current network state.
 *
 * This code does the work of the cyclic worker task.
 */
static int DoEcatIo(ECM_HANDLE hndDevice)
{
  int      result;
  uint32_t ulRuntime;

  /* Indicate the cycle start to the stack */
  (void)ecmGetCycleRuntime(hndDevice, NULL, NULL);

  /* Call start of cycle handler */
  CycleStartHandler(hndDevice, ECM_SUCCESS);

  /* Process I/O data */
  if ((g_ulIoMode & ECM_TEST_IO_MODE_TX_BEFORE_RX) != 0) {
      result = ecmProcessOutputData(hndDevice);
  } else {
      result = ecmProcessInputData(hndDevice);
  }
  if (result != ECM_SUCCESS) {
      fprintf(ECMlogFile,"I/O (1) error %s\n", ecmResultToString(result));
  }

  /* Call the I/O handler (which might also do the asynchronous communication) */
  CycleHandler(hndDevice, result);

  /* Process I/O data */
  if ((g_ulIoMode & ECM_TEST_IO_MODE_TX_BEFORE_RX) != 0) {
      result = ecmProcessInputData(hndDevice);
  } else {
      result = ecmProcessOutputData(hndDevice);
  }
  if (result != ECM_SUCCESS) {
      fprintf(ECMlogFile,"I/O (1) error %s\n", ecmResultToString(result));
  }

  /* Call end of cycle handler */
  CycleEndHandler(hndDevice, result);

  /* Indicate the cycle end to the stack */
  (void)ecmGetCycleRuntime(hndDevice, &ulRuntime, NULL);

  return(ECM_SUCCESS);
}

/*
 * Helper code to change the network state and wait for completion.
 */
static int ChangeEcatState(ECM_HANDLE hndDevice, ECM_HANDLE hndMaster,
                           uint16_t usState)
{
    int i, result;
    uint16_t usActState;

    /*
     * If the cyclic background worker task is running we just have
     * wait for the state OPERATIONAL. Without the worker task we
     * have to drive our own I/O cycle and have to poll the state
     * until we reached this state.
     */
    if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER)) {
        /* Wait for OPERATIONAL if we have worker tasks */
        result = ecmRequestState(hndMaster, usState, 150000);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed with %s to change network state into state %d !\n", ecmResultToString(result), usState);
        }
        return (result);
    } else {
        /* Request state without waiting for completion */
        result = ecmRequestState(hndMaster, ECM_DEVICE_STATE_OP, 0);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to change network state into operational !\n");
            return (result);
        }

        /* Drive the I/O cycle and poll the state */
        for (i = 0; i < 15000; i++) {

            /* Perform one I/O cycle */
            result = DoEcatIo(hndDevice);

            /* Get current network state */
            result = ecmGetState(hndMaster, &usActState);
            if (result != ECM_SUCCESS) {
                fprintf(ECMlogFile,"Failed to get network state ! (%s)\n", ecmResultToString(result));
                return (-1);
            }

            /* Leave loop on success otherwise wait some time */
            if (usActState == usState) {
                return (ECM_SUCCESS);
            }

            ecmSleep(1);
        }
    }

    return (ECM_E_FAIL);
}

/*
 * Helper code to get references to virtual and real process variables.
 */
static void ecmTestSetupProcesData(ECM_HANDLE hndMaster)
{
    ECM_VAR_DESC VarDesc;
    int result;

    /*
     * Get reference to virtual variables
     */
    result = ecmLookupVariable(hndMaster, ".DevState", &VarDesc, ECM_FLAG_GET_FIRST);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pDevState);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable DevState\n");
        }
    }

    /*
     * Get reference with a regular expression as variable name. The example
     * below will match variables with '.SlaveCount' and '.SlaveCount2' at the
     * end of the variable names which are the virtual variables for the
     * number of slaves controlled via the primary and redundant NIC.
     */
    result = ecmLookupVariable(hndMaster, "\\.SlaveCount[2]?$", &VarDesc,
                               ECM_FLAG_GET_FIRST | ECM_FLAG_REGEXP);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pSlaveCount);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable SlaveCount\n");
        }
    }

    result = ecmLookupVariable(hndMaster, NULL, &VarDesc,
                               ECM_FLAG_GET_NEXT | ECM_FLAG_REGEXP);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pSlaveCount2);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable SlaveCount2\n");
        }
    }

    result = ecmLookupVariable(hndMaster, ".SlaveCount", &VarDesc, ECM_FLAG_GET_FIRST);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pSlaveCount);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable SlaveCount\n");
        }
    }

    result = ecmLookupVariable(hndMaster, ".SlaveCount2", &VarDesc, ECM_FLAG_GET_FIRST);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pSlaveCount2);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable SlaveCount2\n");
        }
    }

    /* Get variable ignoring case */
    result = ecmLookupVariable(hndMaster, "Inputs.Frm0wcstate", &VarDesc,
                               ECM_FLAG_GET_FIRST | ECM_FLAG_IGNORE_CASE);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_INPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pWcState);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable WcState\n");
        }
    }

    /*
     * Get reference to process variables
     */
    result = ecmLookupVariable(hndMaster, "Channel 1.Output",
                               &VarDesc, ECM_FLAG_GET_FIRST);
    if (ECM_SUCCESS == result) {
        result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
            VarDesc.ulBitOffs / 8, 2, (void **)&pucDio);
        if (result != ECM_SUCCESS) {
            fprintf(ECMlogFile,"Failed to get reference to virtual variable Output\n");
        }
    }


	//EF mod
	/*
	* This example tries to read a PDO mapping variable
	*/

	/*
	* Get reference to process variables
	*/
	result = ecmLookupVariable(hndMaster, "Controlword",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&pucDio);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile,"Failed to get reference to Controlword Output\n");
		}
	}
	//Get Control Word pointer (done just before)
	pucDio_ControlWord = pucDio;//See upper code

	//Get Operation Mode pointer
	result = ecmLookupVariable(hndMaster, "Operation mode",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&pucDio_OperationMode);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile,"Failed to get reference to Controlword Output\n");
		}
	}

	//Get Target Position pointer
	result = ecmLookupVariable(hndMaster, "Target position",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&	pucDio_TargetPosition
		);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile,"Failed to get reference to Controlword Output\n");
		}
	}

	//Get Target Velocity pointer
	result = ecmLookupVariable(hndMaster, "Target velocity",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&	pucDio_TargetVelocity
		);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile,"Failed to get reference to Controlword Output\n");
		}
	}

	//Get Profiler Max Velocity pointer
	result = ecmLookupVariable(hndMaster, "Profiler max. velocity",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&	pucDio_ProfilerMaxVel
		);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile, "Failed to get reference to Controlword Output\n");
		}
	}

	//Get Profiler Max Acceleration pointer
	result = ecmLookupVariable(hndMaster, "Profiler max. acceleration",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&	pucDio_ProfilerMaxAcc
		);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile, "Failed to get reference to Controlword Output\n");
		}
	}

	//Get Profiler Max Deceleration pointer
	result = ecmLookupVariable(hndMaster, "Profiler max. deceleration",
		&VarDesc, ECM_FLAG_GET_FIRST);
	if (ECM_SUCCESS == result) {
		result = ecmGetDataReference(hndMaster, ECM_OUTPUT_DATA,
			VarDesc.ulBitOffs / 8, 2, (void **)&	pucDio_ProfilerMaxDec
		);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile, "Failed to get reference to Controlword Output\n");
		}
	}
	//Pointer to the memory location the reference pointer is stored
	//(void **)&pucDio;
	//----------------------------------------------
}

void ecmTestEventToString(uint32_t ulEvent)
{
    if (ulEvent & ECM_LOCAL_STATE_LINK_ERROR_NIC1) {
        printf("No link primary |");
    }
    if (ulEvent & ECM_LOCAL_STATE_LINK_ERROR_NIC2) {
        printf("No link redundant |");
    }
    if (ulEvent & ECM_LOCAL_STATE_LOST_FRAME) {
        printf("Redundancy mode |");
    }
    if (ulEvent & ECM_LOCAL_STATE_ERROR_RESOURCE) {
        printf("No Tx resources |");
    }
    if (ulEvent & ECM_LOCAL_STATE_WATCHDOG) {
        printf("Watchdog |");
    }
    if (ulEvent & ECM_LOCAL_STATE_ERROR_ADAPTER) {
        printf("Open adapter |");
    }
    if (ulEvent & ECM_LOCAL_STATE_DEVICE_INIT) {
        printf("INIT |");
    }
    if (ulEvent & ECM_LOCAL_STATE_DEVICE_PREOP) {
        printf("PREOP |");
    }
    if (ulEvent & ECM_LOCAL_STATE_DEVICE_SAFE_OP) {
        printf("SAFEOP |");
    }
    if (ulEvent & ECM_LOCAL_STATE_DEVICE_ERROR) {
        printf("ERROR |");
    }
    if (ulEvent & ECM_LOCAL_STATE_DC_OUT_OF_SYNC) {
        printf("DC |");
    }
    if (ulEvent & ECM_LOCAL_STATE_ERROR_WCNT) {
        printf("WKC |");
    }
    if (ulEvent & ECM_LOCAL_STATE_NO_DATA_NIC1) {
        printf("No data (Primary) |");
    }
    if (ulEvent & ECM_LOCAL_STATE_NO_DATA_NIC2) {
        printf("No Data (Redundant) |");
    }
    if (ulEvent & ECM_LOCAL_STATE_TRIAL_EXPIRED) {
        printf("Trial period expired |");
    }
    if(ulEvent & ECM_LOCAL_STATE_CTIME_EXCEEDED) {
        printf("Cycle time exceeded |");
    }

    return;
}

/*
 * Helper code to decode an asynchronous event
 *
 * !! The code in this handler is time critical, so never do a printf()
 *    in your production code !!
 */
static int ecmEventHandler(ECM_EVENT *pEvent)
{
    uint32_t ulFlagSet, ulFlagReset;
    char szBuffer[40];

    if((ulConfigFlags & ECM_TEST_FLAG_SILENT) != 0) {
        return(0);
    }

    switch(pEvent->ulEvent)
    {
    case ECM_EVENT_CFG:
        switch(pEvent->ulArg1)
        {
        case ECM_EVENT_CFG_INTERNAL:
            printf("Internal parser error (%lu)\n",
                    (unsigned long)pEvent->ulArg2);
            break;
        case ECM_EVENT_CFG_MEMORY:
			printf("Out of memory parsing line %lu\n",
                    (unsigned long)pEvent->ulArg2);
            break;
        case ECM_EVENT_CFG_IO:
			printf("Input buffer I/O error\n");
            break;
        case ECM_EVENT_CFG_SYNTAX:
            ecmFormatError(ECM_ENI_ERROR_REASON(pEvent->ulArg2),                                        ECM_ERROR_ENI_PARSER_ERROR_CODE, szBuffer,
                           sizeof(szBuffer));
			printf("Parse error at line %lu of ENI file: %s\n",
                   (unsigned long)ECM_ENI_ERROR_LINE(pEvent->ulArg2),
                   szBuffer);
            break;
        case ECM_EVENT_CFG_DISCARD:
            /*
             * No warnings for elements currently not elevated by the ENI parser
             *
             * printf("Skip unsupported section starting in line %lu\n",
             *      (unsigned long)pEvent->ulArg2);
             */
            break;
        case ECM_EVENT_CFG_PARAMETER:
			printf("Error processing ENI element text in line %lu\n",
                   (unsigned long)pEvent->ulArg2);
            break;
        case ECM_EVENT_CFG_CYCLE_TIME:
			printf("Inconsistent cycle time definitions");
            if(pEvent->ulArg2 != 0) {
				printf("for slave %4x", pEvent->ulArg2);
            }
			printf("\n");
            break;
        default:
			printf("Unhandled configuration event 0x%lx\n", (unsigned long)pEvent->ulEvent);
            break;
        }
        break;
    case ECM_EVENT_LOCAL:
        ulFlagSet      = pEvent->ulArg1 & (pEvent->ulArg1 ^ pEvent->ulArg2);
        ulFlagReset    = pEvent->ulArg2 & (pEvent->ulArg1 ^ pEvent->ulArg2);
#if 0
        printf("Internal event SET: %lx RESET: %lx (",
                (unsigned long)ulFlagSet, (unsigned long)ulFlagReset);
#endif
        if(ulFlagSet != 0) {
			printf("+ (");
            ecmTestEventToString(ulFlagSet);
			printf(")\n");
        }

        if(ulFlagReset != 0) {
			printf("- (");
            ecmTestEventToString(ulFlagReset);
			printf(")\n");
        }

        break;
    case ECM_EVENT_WCNT:
		printf("WC error (%08lx) in frame %lu\n",
                (unsigned long)pEvent->ulArg1, (unsigned long)pEvent->ulArg2);
        break;
    case ECM_EVENT_STATE_CHANGE:
		printf("Master state change to ");
        switch(pEvent->ulArg1)
        {
        case ECM_DEVICE_STATE_UNKNOWN:
			printf("UNKNOWN\n");
            break;
        case ECM_DEVICE_STATE_INIT:
			printf("INIT\n");
            break;
        case ECM_DEVICE_STATE_PREOP:
			printf("PREOP\n");
            break;
        case ECM_DEVICE_STATE_SAFEOP:
			printf("SAFEOP\n");
            break;
        case ECM_DEVICE_STATE_OP:
			printf("OP\n");
            break;
        default:
			printf("???\n");
            break;
        }
        break;
    case ECM_EVENT_SLV:
		printf("Slave %04lu: State ", (unsigned long)pEvent->ulArg2);
        switch (pEvent->ulArg1 & 0xF)
        {
        case ECM_ESC_AL_STATUS_INIT:
			printf("INIT");
            break;
        case ECM_ESC_AL_STATUS_PREOP:
			printf("PREOP");
            break;
        case ECM_ESC_AL_STATUS_BOOTSTRAP:
            printf("BOOTSTRAP");
            break;
        case ECM_ESC_AL_STATUS_SAFEOP:
            printf("SAFEOP");
            break;
        case ECM_ESC_AL_STATUS_OP:
            printf("OP");
            break;
        default:
            printf("???");
            break;
        }
        if ((pEvent->ulArg1 & ECM_ESC_AL_STATUS_ERROR) != 0) {
            ecmFormatError((pEvent->ulArg1 >> 16) & 0xFFFF, ECM_ERROR_AL_STATUS,
                szBuffer, sizeof(szBuffer));
            printf(" - AL status: %s\n", szBuffer);
        } else {
            printf("\n");
        }
        if (pEvent->ulArg1 & 0x0000FFE0) {
            if (pEvent->ulArg1 & ECM_EVENT_SLV_ID_ERROR) {
                printf(" ID verification failed");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_INIT_ERROR) {
                printf(" Init error");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_NOT_PRESENT) {
                printf(" Not present");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_ERROR_LINK) {
                printf(" Link error");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_MISSING_LINK) {
                printf(" Missing link");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_COMM_PORT_A) {
                printf(" Communication (Port 0)");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_COMM_PORT_B) {
                printf(" Communication (Port 1)");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_COMM_PORT_C) {
                printf(" Communication (Port 2)");
            }
            if (pEvent->ulArg1 & ECM_EVENT_SLV_COMM_PORT_D) {
                printf(" Communication (Port 3)");
            }
            printf("\n");
        }
        break;
    case ECM_EVENT_COE_EMCY:
        printf("Slave %04lx: EMCY (%04x/%02x)", (unsigned long)pEvent->ulArg2,
           (uint16_t)((pEvent->ulArg1 >> 8) & 0xFFFF),
           (uint8_t)(pEvent->ulArg1 & 0xFF));
        break;
    case ECM_EVENT_REMOTE:
        printf("Remote client connection ");
        switch(pEvent->ulArg1)
        {
        case ECM_EVENT_REM_CONNECT:
            printf("established\n");
            break;
        case ECM_EVENT_REM_DISCONNECT:
            printf("shutdown\n");
            break;
        default:
            printf("?!?\n");
            break;
        }
        break;
    default:
        printf("Unhandled event %lu\n", (unsigned long)pEvent->ulEvent);
        break;
    }

    return(0);
}

#if defined(_WIN32)
static int ecmTestAdjustClock(int32_t lDelta, uint32_t ulPeriod)
{
    fprintf(ECMlogFile,"Delta: %ld ns (%uld us)\n", lDelta, ulPeriod);
    return(0);
}
#endif

/*
 * Helper code to print DC related monitoring and diagnostic data
 */
static void ecmTestDcMonitoring(ECM_HANDLE hndDevice, ECM_HANDLE hndMaster)
{
    ECM_MASTER_STATE state;
    ECM_DEVICE_STATE stateDev;

    if (ECM_SUCCESS == ecmGetMasterState(hndMaster, NULL, &state)) {
        fprintf(ECMlogFile," DC drift: %d ns / SM<->SYNC0: %d ns / SYNC Window: %d ns\n",
            (int)state.lDeviation, (int)state.lSmToSync0Delay,
            (int)state.ulDcSysTimeDiff);
    }

    /*
     * If DC epoch is configured to the local time get the DC
     * system time, convert it into a UNIX time and contrast
     * it with the current local time
     */
    if(ECM_SYSTIME_EPOCH_UNIX == g_ucDcSysTimeEpoch) {
        if(ECM_SUCCESS == ecmGetDeviceState(hndDevice, NULL, &stateDev)) {
            char cBuffer[32];
            int64_t lTime;
            time_t unixTime;
            uint64_t ullSysTime;

            fprintf(ECMlogFile," DC time : ");
            ullSysTime = (uint64_t)stateDev.ulDcRefClockHigh << 32 |
                (uint64_t)stateDev.ulDcRefClockLow;

            (void)ecmDcToUnixTime(ullSysTime, &lTime, NULL);
            unixTime = (time_t)lTime;
            strftime(cBuffer, sizeof(cBuffer),
                "%y/%m/%d - %X", gmtime(&unixTime));
            fprintf(ECMlogFile,"%s", cBuffer);
            unixTime = time(NULL);
            strftime(cBuffer, sizeof(cBuffer)," (%X)\n", gmtime(&unixTime));
            fprintf(ECMlogFile,"%s", cBuffer);
        }
    }

    return;
}

CecmTest::CecmTest() {

	//Open log file with writting permissions
	ECMlogFile = fopen(cECMlogFile, "w");
	//pFile = fopen("C:\\workspace\\Base_project_Master_Config_sample.xml", "r");
	if (ECMlogFile != NULL)
	{
		fprintf(ECMlogFile, "Creating log file %s\n", cECMlogFile);
		fflush(ECMlogFile);
	}


}

CecmTest::~CecmTest() {
	//TODO
	fclose(ECMlogFile);

}

int CecmTest::Init(int argc, char *argv[], bool is_running)
{
	ECM_VERSION  ecmVersion;
	ECM_LIB_INIT libInitData;
	ECM_HANDLE   hndDevice, hndMaster, hndSlave;
	ECM_ETHERNET_ADDRESS macPrimary, macRedundant;
	ECM_CFG_INIT cfgInitData;
	const char   *pszArchiveFile, *pszEniFile = NULL;
	int i, j, result;
	uint32_t numAdapter;
	PECM_NIC pNicList;
	ECM_PROC_CTRL ProcCtrl;
	ECM_MASTER_STATE masterState;

	/* Print the startup banner */
	fprintf(ECMlogFile, "EtherCAT master test application V%d.%d.%d\n",
		ECM_TEST_MAJOR, ECM_TEST_MINOR, ECM_TEST_BUILD);
	fprintf(ECMlogFile, "(c) 2008-2015 esd electronic system design gmbh\n\n");
	fflush(ECMlogFile);

	/* Parse the command line arguments and adjust args to last argument */
	ParseCommandLine(argc, argv);

	argc -= optind;
	argv += optind;

	/* OS dependent initialization of the runtime environment */
	result = ecmTestInit(&libInitData);
	if (result != 0) {
		fprintf(ECMlogFile,"ecmTestInit() returned with %d\n", result);
		return(-1);
	}

	/* Initialization of master library */
	libInitData.ulEventMask = g_ulEventMask;
	libInitData.ulEventMask |= ECM_EVENT_REMOTE;
	libInitData.pfnEventHandler = ecmEventHandler;
	libInitData.ulDbgMask = g_ulDebugMask;
	result = ecmInitLibrary(&libInitData);
	if (result != ECM_SUCCESS) {
		fprintf(ECMlogFile,"Initializing master stack failed with %s\n",
			ecmResultToString(result));
		(void)ecmTestCleanup();
		return(-1);
	}

	/* Make of copy of the high resolution counter frequency */
	g_ulCyclesPerSec = (uint32_t)libInitData.ullCyclesPerSec;

	/* Print revision of master modules and validate ABI */
	ecmVersion.usVersionMaster = (ECM_LEVEL << 12) | (ECM_REVI << 8) | ECM_BUILD;
	if (ecmGetVersion(&ecmVersion) != ECM_SUCCESS) {
		fprintf(ECMlogFile,"Getting EtherCAT master version information failed\n");
		(void)ecmTestCleanup();
		return(-1);
	}
	if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_VERSION) != 0) {
		PrintVersionInformation(&ecmVersion);
	}

	/* Get number of available NICs */
	if (ecmGetNicList(NULL, &numAdapter) != ECM_SUCCESS) {
		fprintf(ECMlogFile,"Getting number of available adapter failed\n");
		(void)ecmTestCleanup();
		return(-1);
	}
	else {
		pNicList = (PECM_NIC)calloc(numAdapter, sizeof(ECM_NIC));
		if (pNicList != NULL) {
			ecmGetNicList(pNicList, &numAdapter);
			if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_NICLIST) != 0) {
				fprintf(ECMlogFile,"\nAvailable NICs for EtherCAT\n");
				fprintf(ECMlogFile,"---------------------------\n");
			}
			for (i = 0; i < (int)numAdapter; i++) {
				if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_NICLIST) != 0) {
					fprintf(ECMlogFile,"%d. %s\n   MAC: %02x.%02x.%02x.%02x.%02x.%02x\n",
						i, pNicList[i].szName,
						pNicList[i].macAddr.b[0], pNicList[i].macAddr.b[1],
						pNicList[i].macAddr.b[2], pNicList[i].macAddr.b[3],
						pNicList[i].macAddr.b[4], pNicList[i].macAddr.b[5]);
				}
				if (iPrimaryNic == i) {
					ECM_INIT_MAC(macPrimary, pNicList[i].macAddr);
				}
				if (iRedundantNic == i) {
					ECM_INIT_MAC(macRedundant, pNicList[i].macAddr);
				}
			}
			free(pNicList);
		}
	}

	/* Start remote server */
	if ((ulConfigFlags & ECM_TEST_FLAG_REMOTE_SERVER) != 0) {
		uint32_t ulFlags;

		fprintf(ECMlogFile,"Start remote server (%s) in ", g_szChannelDescription);
		if ((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
			ulFlags = ECM_FLAG_REMOTE_MONITOR_MODE;
			fprintf(ECMlogFile,"monitoring mode ...\n");
		}
		else {
			ulFlags = ECM_FLAG_REMOTE_CONTROL_MODE;
			fprintf(ECMlogFile,"control mode ...\n");
		}
		ECM_SET_REMOTE_SERVER_PRIO(ulFlags, PRIO_REMOTE);
		ECM_SETUP_REMOTE_WATCHDOG(ulFlags, 5);
		result = ecmStartRemotingServer(g_szChannelDescription, ulFlags);
		if (result != ECM_SUCCESS) {
			fprintf(ECMlogFile," - Failed with %s\n", ecmResultToString(result));
			(void)ecmTestCleanup();
			return(-1);
		}
	}

	/* Clear the init data structure */
	ECM_INIT(cfgInitData);

	/* Check if ENI file is defined */
	if (argc != 1) {
		PrintUsage();
		(void)ecmTestCleanup();
		return(0);
	}
	else {
		/* Get ENI configuration file and (optional) archive file */
		pszEniFile = strtok(argv[0], "@");
		pszArchiveFile = strtok(NULL, "@");
		/* Return without configuration file */
		if (NULL == pszEniFile) {
			fprintf(ECMlogFile,"No EtherCAT network configuration specified\n");
			(void)ecmTestCleanup();
			return(-1);
		}

		/* Get configuration data either from file or from memory */
		if (0 == (ulConfigFlags & ECM_TEST_FLAG_MEMORY_CONFIG)) {
			cfgInitData.Config.File.pszEniFile = pszEniFile;
			cfgInitData.Config.File.pszArchiveFile = pszArchiveFile;
			if (strlen(g_szPassword) > 0) {
				cfgInitData.Config.File.pszPassword = g_szPassword;
			}
		}
		else {
			result = ReadConfigFileIntoMemory(pszArchiveFile ? pszArchiveFile : pszEniFile,
				(void **)&cfgInitData.Config.Buffer.pAddress,
				&cfgInitData.Config.Buffer.size);
			if (result != 0) {
				free((void *)cfgInitData.Config.Buffer.pAddress);
				(void)ecmTestCleanup();
				return(-1);
			}
			cfgInitData.ulFlags = ECM_FLAG_CFG_PARSE_BUFFER;
			cfgInitData.Config.Buffer.pszEniFile = pszEniFile;
			if (strlen(g_szPassword) > 0) {
				cfgInitData.Config.Buffer.pszPassword = g_szPassword;
			}
		}

		/* Indicate compressed configuration */
		if (pszArchiveFile != NULL) {
			cfgInitData.ulFlags |= ECM_FLAG_CFG_COMPRESSED;
		}
	} /* of if (argc != 1) */


	/* Store parser status in MSB of payload from ECM_EVENT_CFG_SYNTAX */
	cfgInitData.ulFlags |= ECM_FLAG_CFG_ENI_ERR_REASON;

	/* Copy the VLAN TCI */
	cfgInitData.cfgDataMaster.vlanTCI = g_usVlanTci;

	/* Define virtual port descriptor for EoE */
	/* cfgInitData.cfgDataDevice.ucVport = 2; */

	/* Mark master instance as monitored instance in remote monitoring mode */
	if ((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
		cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_REMOTE_INSTANCE;
	}

	/* Overwrite MAC of configuration file with own adapter MAC */
	cfgInitData.ulFlags |= ECM_FLAG_CFG_IGNORE_SRC_MAC;
	ECM_INIT_MAC(cfgInitData.cfgDataDevice.macAddr[ECM_NIC_PRIMARY], macPrimary);
	if ((ulConfigFlags & ECM_TEST_FLAG_REDUNDANCY) != 0) {
		cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_REDUNDANCY;
		ECM_INIT_MAC(cfgInitData.cfgDataDevice.macAddr[ECM_NIC_REDUNDANT],
			macRedundant);
	}
	/* Use EtherCAT via UDP */
	if ((ulConfigFlags & ECM_TEST_FLAG_UDP) != 0) {
		cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_UDP;
	}

	/* Do not include I/O time profiling ECM_PROFILE_CYCLIC_WORKER */
	if (g_ulProfileMask & ECM_TEST_PROF_MODE_NO_IO_TIME) {
		cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_PROFILE_NO_IO;
	}

	/* Use destination MAC of ENI file instead of broadcast */
	if ((ulConfigFlags & ECM_TEST_FLAG_NO_BROADCAST) != 0) {
		cfgInitData.ulFlags |= ECM_FLAG_CFG_USE_DST_MAC;
	}

	/* Enable optimized cyclic frames */
	if ((ulConfigFlags & ECM_TEST_FLAG_REDUCE_CYCLIC_FRM) != 0) {
		cfgInitData.ulFlags |= ECM_FLAG_CFG_REDUCE_FRAMES;
	}

	/* Setup number of cycles after NIC link state is checked */
	cfgInitData.cfgDataDevice.usCycleLinkState = 50;

	/* Setup number of cycles after watchdog is triggered */
	cfgInitData.cfgDataDevice.usCycleWatchdog = 0;    /* 50; */

	/* Limit # of acyclic frames per I/O cycle */
	cfgInitData.cfgDataDevice.usAcycCtrl = g_usAcycCtrl;

	/* Use configured destination MAC instead of broadcast */
	if ((ulConfigFlags & ECM_TEST_FLAG_FORCE_DST_ADDR) != 0) {
		ECM_INIT_MAC(cfgInitData.cfgDataMaster.macAddr, g_DstMac);
		cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DST_ADDR_VALID;
	}

	/* Use the stack for process data */
	cfgInitData.cfgDataMaster.pInput = &ucInputBuffer;
	cfgInitData.cfgDataMaster.ulSzInput = sizeof(ucInputBuffer);
	cfgInitData.cfgDataMaster.pOutput = &ucOutputBuffer;
	cfgInitData.cfgDataMaster.ulSzOutput = sizeof(ucOutputBuffer);

	/* Setup DC configuration if enabled */
	if ((ulConfigFlags & ECM_TEST_FLAG_USE_DC) != 0) {
		cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DC;
		if ((ulConfigFlags & ECM_TEST_FLAG_DC_DRIFT_COMP) != 0) {
			cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DC_RESYNC;
		}
		cfgInitData.cfgDataMaster.ucDcSysTimeEpoch = g_ucDcSysTimeEpoch;
		/*
		 * Enable master clock synchronization if configured and set the
		 * configured parameter for control loop and local DC shift.
		 */
		if ((ulConfigFlags & ECM_TEST_FLAG_DCM_CLOCK_SHIFT) != 0) {
			cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DCM_CLOCK_SHIFT;
			cfgInitData.cfgDataMaster.sDcUserShift = g_sUserShiftMaster;
			cfgInitData.cfgDataDevice.usCycleDcCtrl = g_usDcClockControl;
		}
	}


	/* Enable slave monitoring support */
	if ((ulConfigFlags & ECM_TEST_FLAG_SLAVE_STATUS) != 0) {
		cfgInitData.ulFlags |= (ECM_FLAG_CFG_DIAG_STATUS |
			ECM_FLAG_CFG_DIAG_ERRCNT);
		cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DIAG;
	}

	/* Set alignment to 4 */
	cfgInitData.cfgDataMaster.ucAlignment = 4;

	/* Say: Support virtual variables and keep process variable description */
	cfgInitData.ulFlags |= ECM_FLAG_CFG_VIRTUAL_VARS |
		ECM_FLAG_CFG_KEEP_PROCVARS;

	/* Say: Want to create device and master */
	hndDevice = ECM_INVALID_HANDLE;

	//EF modifications
	FILE * pFile;
	pFile = fopen(argv[0], "r");
	//pFile = fopen("C:\\workspace\\Base_project_Master_Config_sample.xml", "r");
	if (pFile != NULL)
	{
		fprintf(ECMlogFile, "ENI configuration file selected: %s\n", argv[0]);
		fclose(pFile);
	}
	//-----------------

	/* Create configuration based on ENI configuration file */
	result = ecmReadConfiguration(&cfgInitData, &hndDevice, &hndMaster);
	/* Free resources if configuration is stored in memory */
	if ((ulConfigFlags & ECM_TEST_FLAG_MEMORY_CONFIG) != 0) {
		free((void *)cfgInitData.Config.Buffer.pAddress);
	}
	if (result != ECM_SUCCESS) {
		fprintf(ECMlogFile,"Reading configuration failed with %s\n", ecmResultToString(result));
		(void)ecmTestCleanup();
		return(result);
	}

	/*
	 * Cycle time precedence:
	 *      1. Command line parameter
	 *      2. esd EtherCAT Workbench specific keyword <CycleTime>
	 *      3. Default cycle time
	 */
	if (0 == g_ulCycleTime) {
		if (0 == cfgInitData.cfgDataDevice.ulCycleTime) {
			g_ulCycleTime = ECM_TEST_DEFAULT_CYCLE_TIME;
		}
		else {
			g_ulCycleTime = cfgInitData.cfgDataDevice.ulCycleTime;
		}
	}

	/*
	 * In case of cyclic handler jitter check without allowed absolute value
	 * use 10 percent of configured cycle time as threshold for an indication.
	 */
	if ((ulConfigFlags & ECM_TEST_FLAG_CHECK_JITTER) != 0) {
		if (0 == g_ulCycleTimeViolation) {
			g_ulCycleTimeViolation = g_ulCycleTime / 10;
		}
		g_ulCycleTimeViolation += g_ulCycleTime;
	}

	/* Print device description */
	bool printInfoToFile = false;
	if (printInfoToFile) {
		//fflush(ECMlogFile);
		printf("Device:\n-------\n");
		PrintDeviceDescription(hndDevice);

		/* Print master description */
		//fflush(ECMlogFile);
		printf("Master:\n-------\n");
		PrintMasterDescription(hndMaster);

		/* Iterate list of slaves */
		//fflush(ECMlogFile);
		printf("Slaves:\n-------\n");
		for (hndSlave = hndMaster; hndSlave;) {
			if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
				break;
			}

			/* Print slave description */
			PrintSlaveDescription(hndSlave);
		}

		/* Print the list of process variables (defined in ENI file) */
		PrintVarList(hndMaster);

		/* Print copy vector */
		PrintCopyVector(hndMaster);//EF mod

		/* Print statistic information */
		PrintStatistic(hndDevice, hndMaster);
	}
	/* Setup reference to variables before attaching the master */
	ecmTestSetupProcesData(hndMaster);

	/* Attach the master to the device */
	result = ecmAttachMaster(hndMaster);
	if (result != ECM_SUCCESS) {
		printf("Attaching master failed with %s\n", ecmResultToString(result));
		ecmDeleteMaster(hndMaster);
		ecmDeleteDevice(hndDevice);
		(void)ecmTestCleanup();
		return(-1);
	}

	ecmSleep(1000);

	/* Reset error handling in cyclic handler */
	CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_HANDLER);
	CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_START_HANDLER);
	CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_END_HANDLER);

	/* Setup background worker tasks parameter */
	ECM_INIT(ProcCtrl);
	if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_ACYC_WORKER)) {
		ProcCtrl.ulAcyclicPeriod = 1000;
		ProcCtrl.ulAcyclicPrio = PRIO_ACYCLIC;
	}
	if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER)) {
		ProcCtrl.ulCyclicPeriod = g_ulCycleTime;
		ProcCtrl.ulCyclicPrio = PRIO_CYCLIC;
		ProcCtrl.pfnHandler = CycleHandler;
		ProcCtrl.pfnBeginCycle = CycleStartHandler;
		ProcCtrl.pfnEndCycle = CycleEndHandler;
		/* Configure I/O processing order */
		if ((g_ulIoMode & ECM_TEST_IO_MODE_TX_BEFORE_RX) != 0) {
			ProcCtrl.ulFlags = ECM_FLAG_CTRL_OUT_BEFORE_IN;
		}
	}

	result = ecmProcessControl(hndDevice, &ProcCtrl);
	if (result != ECM_SUCCESS) {
		printf("Creating background worker threads failed with %s\n",
			ecmResultToString(result));
		ecmDetachMaster(hndMaster);
		ecmDeleteMaster(hndMaster);
		ecmDeleteDevice(hndDevice);
		(void)ecmTestCleanup();
		return(-1);
	}
	else {
		printf("Worker thread cycle times (microseconds) (Cyclic: %d / Acyclic %d)\n",
			ProcCtrl.ulCyclicPeriod, ProcCtrl.ulAcyclicPeriod);
	}

	/* INIT -> OP loop */
	do {
		/* Wait some cycles */
		ecmSleep(100);//EF mod, default value is 100 ms

		/* Do some I/O using async services in INIT state */
		PrintEscRegister(hndMaster);

		/*
		* Change EtherCAT network state to into OPERATIONAL.
		*/
		result = ChangeEcatState(hndDevice, hndMaster, ECM_DEVICE_STATE_OP);
		if (result != ECM_SUCCESS) {
			printf("Network failed to change into OPERATIONAL with %s!\n",
				ecmResultToString(result));
			ecmDetachMaster(hndMaster);
			ecmDeleteMaster(hndMaster);
			ecmDeleteDevice(hndDevice);
			(void)ecmTestCleanup();
			return(-1);
		}

		/*
		* Validate if all configured slaves are present and OPERATIONAL
		*/
		result = ecmGetMasterState(hndMaster, NULL, &masterState);
		if (result != ECM_SUCCESS) {
			printf("Requesting master state failed with %s\n", ecmResultToString(result));
		}
		else {
			if (masterState.usNumSlaves != masterState.usActiveSlaves) {
				printf("Warning: Number of configured slaves (%d) "
					"differ from active slaves (%d)\n",
					masterState.usNumSlaves, masterState.usActiveSlaves);
			}
			if ((masterState.ulFlags & ECM_LOCAL_STATE_MASK) != 0) {
				printf("Warning: Not all slaves changed into operational !!\n");
			}
		}

		/* Scan and CoE object dictionary |SoE data elements */
		if ((ulConfigFlags & ECM_TEST_FLAG_SCAN_DICTIONARY) != 0) {
			/* Blocking operation not possible without cyclic worker task */
			if ((g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER) != 0) {
				printf("Blocking ESC access not supported without cyclic worker !\n");
			}
			else {
				for (hndSlave = hndMaster; hndSlave;) {
					ECM_SLAVE_DESC descSlave;

					if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
						break;
					}
					/* Gets slave state and do an object scan for CoE/SoE slaves */
					result = ecmGetSlaveState(hndSlave, &descSlave, NULL);
					if (result != ECM_SUCCESS) {
						printf("Requesting slave description failed with %s\n",
							ecmResultToString(result));
						continue;
					}
					if ((descSlave.ulFlags & ECM_FLAG_SLAVE_COE) != 0) {
						ecmTestCoeScan(hndSlave);
						printf("\t\t-----------\n");
						ecmSleep(1000);
					}
					else if ((descSlave.ulFlags & ECM_FLAG_SLAVE_SOE) != 0) {
						ecmTestSoeScan(hndSlave);
						ecmSleep(1000);
					}
				}
			}
		}

		/* Initialize to request slave diagnostic data */
		hndSlave = hndMaster;

		/*
		* Print device state and active slaves while running.
		* Cyclic I/O data is handled in the callback handler
		*/
		for (i = 0; ; i++) {
			uint16_t usData;

			/* Check exit condition */
			if ((g_lRuntime != 0) && i >= (int)g_lRuntime) {
				break;
			}

			/* Print device state (virtual variable) */
			if (pDevState != NULL) {
				ecmCpuToLe(&usData, pDevState, (const uint8_t *)"\x2\x0");
				printf("State: %04x", usData);
			}
			/* Print # of slaves on primary and redundant adapter */
			if (pSlaveCount != NULL) {
				ecmCpuToLe(&usData, pSlaveCount, (const uint8_t *)"\x2\x0");
				printf(" / Pri: %d", usData);
				if (pSlaveCount2 != NULL) {
					ecmCpuToLe(&usData, pSlaveCount2, (const uint8_t *)"\x2\x0");
					printf(" / Red: %d", usData);
				}
			}

			/* Print WC state of 1st frame */
			if (pWcState != NULL) {
				ecmCpuToLe(&usData, pWcState, (const uint8_t *)"\x2\x0");
				printf(" / Frm0WC: %04x", usData);
			}

			/* Request diagnostic data in round robin mode */
			if ((ulConfigFlags & ECM_TEST_FLAG_SLAVE_STATUS) != 0) {
				if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
					hndSlave = hndMaster;
				}
				else {
					ECM_SLAVE_DIAG diag;
					diag.usControl = 0;
					if (ECM_SUCCESS == ecmGetSlaveDiag(hndSlave, &diag)) {
						printf(" Link lost (Slave %x): %d", diag.usAddr,
							diag.counter.ucLostLinkErrorPort0);
					}
				}
			}

			printf("\n");

			/* Optionally print DC statistic */
			if ((ulConfigFlags & ECM_TEST_FLAG_USE_DC) != 0) {
				ecmTestDcMonitoring(hndDevice, hndMaster);
			}

			/* If the background worker task is running we just wait and
			* get called in our handler with each cycle. Without worker
			* task we have to drive our own I/O cycle and call the handler
			* in between.
			*/
			if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER)) {
				ecmSleep(100);//EF mod, default is 1000 ms
			}
			else {
				for (j = 0; j < 100; j++) {
					DoEcatIo(hndDevice);
					ecmSleep(10);
				}
			}

			/* Print collected profiling data if configured */
			if ((ulConfigFlags & ECM_TEST_FLAG_PROFILING) != 0) {
				PrintProfilingData(hndDevice);
			}
		}

		printf("Change state to INIT...\n");

		/* Change EtherCAT state back to INIT */
		result = ChangeEcatState(hndDevice, hndMaster, ECM_DEVICE_STATE_INIT);
		if (result != ECM_SUCCESS) {
			printf("Network failed to change into INIT ! (%s)\n",
				ecmResultToString(result));
			(void)ecmTestCleanup();
			return(-1);
		}

		PrintStatistic(hndDevice, hndMaster);

	//} while ( (--g_lLoops > 0) );
	} while ( (--g_lLoops > 0) && (is_running == true) );

	printf("Detaching master...\n");
	result = ecmDetachMaster(hndMaster);
	if (result != ECM_SUCCESS) {
		printf("Failed with %s\n", ecmResultToString(result));
	}

	printf("Deleting master...\n");
	result = ecmDeleteMaster(hndMaster);
	if (result != ECM_SUCCESS) {
		printf("Failed with %s\n", ecmResultToString(result));
	}

	fprintf(ECMlogFile,"Deleting device...\n");
	result = ecmDeleteDevice(hndDevice);
	if (result != ECM_SUCCESS) {
		fprintf(stdout, "Failed with %s\n", ecmResultToString(result));
	}

	/* Terminate remote server thread if remote monitor mode is configured */
	if ((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
		fprintf(stdout, "Terminate remote server ...\n");
		ecmStopRemotingServer();
	}

	(void)ecmTestCleanup();
	fprintf(stdout, "End\n");

	return 0;
}

DWORD WINAPI CecmTest::emcThreadFunction(LPVOID lpParam) {
	
	PMYDATA arg = (PMYDATA)lpParam;


	//old functional call for base version
//	main(arg->argc, arg->argv);

	//mod for compilation success while code is being modified but not finished yet
	CecmTest test;
	test.main(arg->argc, arg->argv);

	return 0;
}


int CecmTest::main(int argc, char *argv[])
{
    ECM_VERSION  ecmVersion;
    ECM_LIB_INIT libInitData;
    ECM_HANDLE   hndDevice, hndMaster, hndSlave;
    ECM_ETHERNET_ADDRESS macPrimary, macRedundant;
    ECM_CFG_INIT cfgInitData;
    const char   *pszArchiveFile, *pszEniFile = NULL;
    int i, j, result;
    uint32_t numAdapter;
    PECM_NIC pNicList;
    ECM_PROC_CTRL ProcCtrl;
    ECM_MASTER_STATE masterState;

    /* Print the startup banner */
	printf("EtherCAT master test application V%d.%d.%d\n",
       ECM_TEST_MAJOR, ECM_TEST_MINOR, ECM_TEST_BUILD);
    printf("(c) 2008-2015 esd electronic system design gmbh\n\n");

    /* Parse the command line arguments and adjust args to last argument */
    ParseCommandLine(argc, argv);

    argc -= optind;
    argv += optind;

    /* OS dependent initialization of the runtime environment */
    result = ecmTestInit(&libInitData);
    if (result != 0) {
      printf("ecmTestInit() returned with %d\n", result);
      return(-1);
    }

    /* Initialization of master library */
    libInitData.ulEventMask     = g_ulEventMask;
    libInitData.ulEventMask    |= ECM_EVENT_REMOTE;
    libInitData.pfnEventHandler = ecmEventHandler;
    libInitData.ulDbgMask       = g_ulDebugMask;
    result = ecmInitLibrary(&libInitData);
    if (result != ECM_SUCCESS) {
        printf("Initializing master stack failed with %s\n",
            ecmResultToString(result));
        (void)ecmTestCleanup();
        return(-1);
    }

    /* Make of copy of the high resolution counter frequency */
    g_ulCyclesPerSec = (uint32_t)libInitData.ullCyclesPerSec;

    /* Print revision of master modules and validate ABI */
    ecmVersion.usVersionMaster = (ECM_LEVEL << 12) | (ECM_REVI << 8) | ECM_BUILD;
    if (ecmGetVersion(&ecmVersion) != ECM_SUCCESS) {
        printf("Getting EtherCAT master version information failed\n");
        (void)ecmTestCleanup();
        return(-1);
    }
    if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_VERSION) != 0) {
        PrintVersionInformation(&ecmVersion);
    }

    /* Get number of available NICs */
    if (ecmGetNicList(NULL, &numAdapter) != ECM_SUCCESS) {
        printf("Getting number of available adapter failed\n");
        (void)ecmTestCleanup();
        return(-1);
    } else {
        pNicList = (PECM_NIC)calloc(numAdapter, sizeof(ECM_NIC));
        if (pNicList != NULL) {
            ecmGetNicList(pNicList, &numAdapter);
            if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_NICLIST) != 0) {
                printf("\nAvailable NICs for EtherCAT\n");
                printf("---------------------------\n");
            }
            for (i = 0; i < (int)numAdapter; i++) {
                if ((ulConfigFlags & ECM_TEST_FLAG_PRINT_NICLIST) != 0) {
                    printf("%d. %s\n   MAC: %02x.%02x.%02x.%02x.%02x.%02x\n",
                        i, pNicList[i].szName,
                        pNicList[i].macAddr.b[0], pNicList[i].macAddr.b[1],
                        pNicList[i].macAddr.b[2], pNicList[i].macAddr.b[3],
                        pNicList[i].macAddr.b[4], pNicList[i].macAddr.b[5]);
                }
                if (iPrimaryNic == i) {
                    ECM_INIT_MAC(macPrimary, pNicList[i].macAddr);
                }
                if (iRedundantNic == i) {
                    ECM_INIT_MAC(macRedundant, pNicList[i].macAddr);
                }
            }
            free(pNicList);
        }
    }

    /* Start remote server */
    if ((ulConfigFlags & ECM_TEST_FLAG_REMOTE_SERVER) != 0) {
        uint32_t ulFlags;

        printf("Start remote server (%s) in ", g_szChannelDescription);
        if((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
            ulFlags = ECM_FLAG_REMOTE_MONITOR_MODE;
            printf("monitoring mode ...\n");
        } else {
            ulFlags = ECM_FLAG_REMOTE_CONTROL_MODE;
            printf("control mode ...\n");
        }
        ECM_SET_REMOTE_SERVER_PRIO(ulFlags, PRIO_REMOTE);
        ECM_SETUP_REMOTE_WATCHDOG(ulFlags, 5);
        result = ecmStartRemotingServer(g_szChannelDescription, ulFlags);
        if (result != ECM_SUCCESS) {
            printf(" - Failed with %s\n", ecmResultToString(result));
            (void)ecmTestCleanup();
            return(-1);
        }
    }

    /* Clear the init data structure */
    ECM_INIT(cfgInitData);
    
    /* Check if ENI file is defined */
    if (argc != 1) {
        PrintUsage();
        (void)ecmTestCleanup();
        return(0);
    } else {
        /* Get ENI configuration file and (optional) archive file */
        pszEniFile     = strtok(argv[0], "@");
        pszArchiveFile = strtok(NULL,"@");
        /* Return without configuration file */
        if (NULL == pszEniFile) {
            printf("No EtherCAT network configuration specified\n");
            (void)ecmTestCleanup();
            return(-1);
        }

        /* Get configuration data either from file or from memory */
        if (0 == (ulConfigFlags & ECM_TEST_FLAG_MEMORY_CONFIG)) {
            cfgInitData.Config.File.pszEniFile     = pszEniFile;
            cfgInitData.Config.File.pszArchiveFile = pszArchiveFile;
            if (strlen(g_szPassword) > 0) {
                cfgInitData.Config.File.pszPassword    = g_szPassword;
            }
        } else {
            result = ReadConfigFileIntoMemory(pszArchiveFile ? pszArchiveFile : pszEniFile,
                (void **)&cfgInitData.Config.Buffer.pAddress,
                &cfgInitData.Config.Buffer.size);
            if (result != 0) {
                free((void *)cfgInitData.Config.Buffer.pAddress);
                (void)ecmTestCleanup();
                return(-1);
            }
            cfgInitData.ulFlags                    = ECM_FLAG_CFG_PARSE_BUFFER;
            cfgInitData.Config.Buffer.pszEniFile   = pszEniFile;
            if (strlen(g_szPassword) > 0) {
                cfgInitData.Config.Buffer.pszPassword    = g_szPassword;
            }
        }

        /* Indicate compressed configuration */
        if (pszArchiveFile != NULL) {
            cfgInitData.ulFlags |= ECM_FLAG_CFG_COMPRESSED;
        }
    } /* of if (argc != 1) */


    /* Store parser status in MSB of payload from ECM_EVENT_CFG_SYNTAX */
    cfgInitData.ulFlags |= ECM_FLAG_CFG_ENI_ERR_REASON;

    /* Copy the VLAN TCI */
    cfgInitData.cfgDataMaster.vlanTCI = g_usVlanTci;

    /* Define virtual port descriptor for EoE */
    /* cfgInitData.cfgDataDevice.ucVport = 2; */

    /* Mark master instance as monitored instance in remote monitoring mode */
    if((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
        cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_REMOTE_INSTANCE;
    }

    /* Overwrite MAC of configuration file with own adapter MAC */
    cfgInitData.ulFlags |= ECM_FLAG_CFG_IGNORE_SRC_MAC;
    ECM_INIT_MAC(cfgInitData.cfgDataDevice.macAddr[ECM_NIC_PRIMARY], macPrimary);
    if ((ulConfigFlags & ECM_TEST_FLAG_REDUNDANCY) != 0) {
        cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_REDUNDANCY;
        ECM_INIT_MAC(cfgInitData.cfgDataDevice.macAddr[ECM_NIC_REDUNDANT],
            macRedundant);
    }
    /* Use EtherCAT via UDP */
    if ((ulConfigFlags & ECM_TEST_FLAG_UDP) != 0) {
        cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_UDP;
    }

    /* Do not include I/O time profiling ECM_PROFILE_CYCLIC_WORKER */
    if(g_ulProfileMask & ECM_TEST_PROF_MODE_NO_IO_TIME) {
        cfgInitData.cfgDataDevice.ulFlags |= ECM_FLAG_DEVICE_PROFILE_NO_IO;
    }

    /* Use destination MAC of ENI file instead of broadcast */
    if ((ulConfigFlags & ECM_TEST_FLAG_NO_BROADCAST) != 0) {
        cfgInitData.ulFlags |= ECM_FLAG_CFG_USE_DST_MAC;
    }

    /* Enable optimized cyclic frames */
    if ((ulConfigFlags & ECM_TEST_FLAG_REDUCE_CYCLIC_FRM) != 0) {
        cfgInitData.ulFlags |= ECM_FLAG_CFG_REDUCE_FRAMES;
    }

    /* Setup number of cycles after NIC link state is checked */
    cfgInitData.cfgDataDevice.usCycleLinkState = 50;

    /* Setup number of cycles after watchdog is triggered */
    cfgInitData.cfgDataDevice.usCycleWatchdog  = 0;    /* 50; */

    /* Limit # of acyclic frames per I/O cycle */
    cfgInitData.cfgDataDevice.usAcycCtrl       = g_usAcycCtrl;

    /* Use configured destination MAC instead of broadcast */
    if ((ulConfigFlags & ECM_TEST_FLAG_FORCE_DST_ADDR) != 0) {
        ECM_INIT_MAC(cfgInitData.cfgDataMaster.macAddr, g_DstMac);
        cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DST_ADDR_VALID;
    }

    /* Use the stack for process data */
    cfgInitData.cfgDataMaster.pInput     = &ucInputBuffer;
    cfgInitData.cfgDataMaster.ulSzInput  = sizeof(ucInputBuffer);
    cfgInitData.cfgDataMaster.pOutput    = &ucOutputBuffer;
    cfgInitData.cfgDataMaster.ulSzOutput = sizeof(ucOutputBuffer);

    /* Setup DC configuration if enabled */
    if ((ulConfigFlags & ECM_TEST_FLAG_USE_DC) != 0) {
        cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DC;
        if ((ulConfigFlags & ECM_TEST_FLAG_DC_DRIFT_COMP) != 0) {
            cfgInitData.cfgDataMaster.ulFlags |= ECM_FLAG_MASTER_DC_RESYNC;
        }
        cfgInitData.cfgDataMaster.ucDcSysTimeEpoch = g_ucDcSysTimeEpoch;
        /*
         * Enable master clock synchronization if configured and set the
         * configured parameter for control loop and local DC shift.
         */
        if ((ulConfigFlags & ECM_TEST_FLAG_DCM_CLOCK_SHIFT) != 0) {
            cfgInitData.cfgDataMaster.ulFlags      |= ECM_FLAG_MASTER_DCM_CLOCK_SHIFT;
            cfgInitData.cfgDataMaster.sDcUserShift  = g_sUserShiftMaster;
            cfgInitData.cfgDataDevice.usCycleDcCtrl = g_usDcClockControl;
        }
    }


    /* Enable slave monitoring support */
    if ((ulConfigFlags & ECM_TEST_FLAG_SLAVE_STATUS) != 0) {
        cfgInitData.ulFlags                |= (ECM_FLAG_CFG_DIAG_STATUS |
                                               ECM_FLAG_CFG_DIAG_ERRCNT);
        cfgInitData.cfgDataMaster.ulFlags  |= ECM_FLAG_MASTER_DIAG;
    }

    /* Set alignment to 4 */
    cfgInitData.cfgDataMaster.ucAlignment = 4;

    /* Say: Support virtual variables and keep process variable description */
    cfgInitData.ulFlags |= ECM_FLAG_CFG_VIRTUAL_VARS  |
                           ECM_FLAG_CFG_KEEP_PROCVARS;

    /* Say: Want to create device and master */
    hndDevice = ECM_INVALID_HANDLE;

	//EF modifications
	FILE * pFile;
	pFile = fopen(argv[0], "r");
	//pFile = fopen("C:\\workspace\\Base_project_Master_Config_sample.xml", "r");
	if (pFile != NULL)
	{
		fprintf(ECMlogFile, "ENI configuration file selected: %s\n", argv[0]);
		fclose(pFile);
	}
	//-----------------

    /* Create configuration based on ENI configuration file */
    result = ecmReadConfiguration(&cfgInitData, &hndDevice, &hndMaster);
    /* Free resources if configuration is stored in memory */
    if ((ulConfigFlags & ECM_TEST_FLAG_MEMORY_CONFIG) != 0) {
        free((void *)cfgInitData.Config.Buffer.pAddress);
    }
    if (result != ECM_SUCCESS) {
        printf("Reading configuration failed with %s\n", ecmResultToString(result));
        (void)ecmTestCleanup();
        return(result);
    }

    /*
     * Cycle time precedence:
     *      1. Command line parameter
     *      2. esd EtherCAT Workbench specific keyword <CycleTime>
     *      3. Default cycle time
     */
    if(0 == g_ulCycleTime) {
        if(0 == cfgInitData.cfgDataDevice.ulCycleTime) {
            g_ulCycleTime = ECM_TEST_DEFAULT_CYCLE_TIME;
        } else {
            g_ulCycleTime = cfgInitData.cfgDataDevice.ulCycleTime;
        }
    }

    /*
     * In case of cyclic handler jitter check without allowed absolute value
     * use 10 percent of configured cycle time as threshold for an indication.
     */
    if ((ulConfigFlags & ECM_TEST_FLAG_CHECK_JITTER) != 0) {
        if(0 == g_ulCycleTimeViolation) {
            g_ulCycleTimeViolation = g_ulCycleTime / 10;
        }
        g_ulCycleTimeViolation += g_ulCycleTime;
    }

    /* Print device description */
    printf("Device:\n-------\n");
    PrintDeviceDescription(hndDevice);

    /* Print master description */
    printf("Master:\n-------\n");
    PrintMasterDescription(hndMaster);

    /* Iterate list of slaves */
    printf("Slaves:\n-------\n");
    for (hndSlave = hndMaster; hndSlave;) {
        if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
            break;
        }

        /* Print slave description */
        PrintSlaveDescription(hndSlave);
    }

    /* Print the list of process variables (defined in ENI file) */
    PrintVarList(hndMaster);

    /* Print copy vector */
    PrintCopyVector(hndMaster);//EF mod

    /* Print statistic information */
    PrintStatistic(hndDevice, hndMaster);

    /* Setup reference to variables before attaching the master */
    ecmTestSetupProcesData(hndMaster);

    /* Attach the master to the device */
    result = ecmAttachMaster(hndMaster);
    if (result != ECM_SUCCESS) {
        printf("Attaching master failed with %s\n", ecmResultToString(result));
        ecmDeleteMaster(hndMaster);
        ecmDeleteDevice(hndDevice);
        (void)ecmTestCleanup();
        return(-1);
    }

    ecmSleep(1000);

    /* Reset error handling in cyclic handler */
    CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_HANDLER);
    CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_START_HANDLER);
    CycleHandlerErrorMessage(ECM_SUCCESS, ECM_TEST_CYCLIC_END_HANDLER);

    /* Setup background worker tasks parameter */
    ECM_INIT(ProcCtrl);
    if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_ACYC_WORKER)) {
        ProcCtrl.ulAcyclicPeriod = 1000;
        ProcCtrl.ulAcyclicPrio   = PRIO_ACYCLIC;
    }
    if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER)) {
        ProcCtrl.ulCyclicPeriod = g_ulCycleTime;
        ProcCtrl.ulCyclicPrio   = PRIO_CYCLIC;
        ProcCtrl.pfnHandler     = CycleHandler;
        ProcCtrl.pfnBeginCycle  = CycleStartHandler;
        ProcCtrl.pfnEndCycle    = CycleEndHandler;
        /* Configure I/O processing order */
        if ((g_ulIoMode & ECM_TEST_IO_MODE_TX_BEFORE_RX) != 0) {
            ProcCtrl.ulFlags = ECM_FLAG_CTRL_OUT_BEFORE_IN;
        }
    }

    result = ecmProcessControl(hndDevice, &ProcCtrl);
    if (result != ECM_SUCCESS) {
        printf("Creating background worker threads failed with %s\n",
            ecmResultToString(result));
        ecmDetachMaster(hndMaster);
        ecmDeleteMaster(hndMaster);
        ecmDeleteDevice(hndDevice);
        (void)ecmTestCleanup();
        return(-1);
    } else {
        printf("Worker thread cycle times (microseconds) (Cyclic: %d / Acyclic %d)\n",
            ProcCtrl.ulCyclicPeriod, ProcCtrl.ulAcyclicPeriod);
    }

	//EF mod ----------------------------------------
	uint16_t value = 0;//This is the value
	void* pData = &value;//Pointer to the value
	/*uint16_t pucCnt_val = 0;
	uint16_t* pucCnt = &pucCnt_val;
	
	ECM_SLAVE_ADDR addr_slv[NUMCOMMANDS];
	//addr_slv[0].p.ado = 0x010;//Just next node, single  slave(which register it is?)
	//addr_slv[0].p.adp = 0x0000;//Just next node, single  slave (first salve, the only one available by now)
	for (i = 0; i < NUMCOMMANDS; i++) {
		addr_slv[i].p.adp = 0x0000;//it could be addr_slv[i].p.adp = 0x03E9, but is not working yet. Just keep 0x0000 by now
	}
	//NOTE: addresses are related to Slave::Memory (as seen in ESD Workbench)
	//Other functions must be used for CoE communication (just like VS does)
	//So, an offset value of 0x2000 may be a fast solution to aim to an specific registers, related to AXIS1 of SLAVE1 (there are no more axes neither slaves)

	//uint16_t usSize_data[NUMCOMMANDS];
	   
	//ADO value has an offset of 0x2000 (axis-1) as Ingenia documentation specifies.
	//By default, addres_slv[any].p.ado = 0x0000
	//Set previously in a for loop
	
	//1.BRD
	addr_slv[0].p.ado = 0x2010;//0x010 (Control Word - RW)
	usSize_data[0] = sizeof(uint16_t);//4bytes == 32bits
	//2.BRD
	addr_slv[1].p.ado = 0x2011;//0x011 (Status Word - RO)
	usSize_data[1] = sizeof(uint16_t);
	//3.BWR
	addr_slv[2].p.ado = 0x2014;//0x014 (Operation mode - RW)
	usSize_data[2] = sizeof(int8_t);
	//4.FPRD
	addr_slv[3].p.adp = 0x03E9;//Slave address
	addr_slv[3].p.ado = 0x2014;//0x015 (Operation mode display - RO) NOT WORKING YET! So it is changed to 0x2014
	usSize_data[3] = sizeof(int8_t);
	//5.BRD
	addr_slv[4].p.ado = 0x2014;//0x015 (Operation mode display - RO) NOT WORKING YET! So it is changed to 0x2014
	usSize_data[4] = sizeof(int8_t);
	//6.FPWR
	addr_slv[5].p.adp = 0x03E9;//Slave address
	addr_slv[5].p.ado = 0x2010;//0x010 (Control Word - RW)
	usSize_data[5] = sizeof(int16_t);
	//7.BRD
	addr_slv[6].p.ado = 0x2010;//0x010 (Control Word - RW)
	usSize_data[6] = sizeof(int8_t);
	//8.BRD
	addr_slv[7].p.ado = 0x2030;//0x030 (Actual Position - RO)
	usSize_data[7] = sizeof(int32_t);*/
	//------------------------------------

    /* INIT -> OP loop */
    do {
        /* Wait some cycles */
        ecmSleep(100);//EF mod, default value is 100 ms

        /* Do some I/O using async services in INIT state */
        PrintEscRegister(hndMaster);

        /*
        * Change EtherCAT network state to into OPERATIONAL.
        */
        result = ChangeEcatState(hndDevice, hndMaster, ECM_DEVICE_STATE_OP);
        if (result != ECM_SUCCESS) {
            printf("Network failed to change into OPERATIONAL with %s!\n",
                ecmResultToString(result));
            ecmDetachMaster(hndMaster);
            ecmDeleteMaster(hndMaster);
            ecmDeleteDevice(hndDevice);
            (void)ecmTestCleanup();
            return(-1);
        }

        /*
        * Validate if all configured slaves are present and OPERATIONAL
        */
        result = ecmGetMasterState(hndMaster, NULL, &masterState);
        if (result != ECM_SUCCESS) {
            printf("Requesting master state failed with %s\n", ecmResultToString(result));
        } else {
            if (masterState.usNumSlaves != masterState.usActiveSlaves) {
                printf("Warning: Number of configured slaves (%d) "
                    "differ from active slaves (%d)\n",
                    masterState.usNumSlaves, masterState.usActiveSlaves);
            }
            if ((masterState.ulFlags & ECM_LOCAL_STATE_MASK) != 0) {
                printf("Warning: Not all slaves changed into operational !!\n");
            }
        }

        /* Scan and CoE object dictionary |SoE data elements */
        if ((ulConfigFlags & ECM_TEST_FLAG_SCAN_DICTIONARY) != 0) {
            /* Blocking operation not possible without cyclic worker task */
            if ((g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER) != 0) {
                printf("Blocking ESC access not supported without cyclic worker !\n");
            } else {
                for (hndSlave = hndMaster; hndSlave;) {
                    ECM_SLAVE_DESC descSlave;

                    if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
                        break;
                    }
                    /* Gets slave state and do an object scan for CoE/SoE slaves */
                    result = ecmGetSlaveState(hndSlave, &descSlave, NULL);
                    if (result != ECM_SUCCESS) {
                        printf("Requesting slave description failed with %s\n",
                            ecmResultToString(result));
                        continue;
                    }
                    if ((descSlave.ulFlags & ECM_FLAG_SLAVE_COE) != 0) {
                        ecmTestCoeScan(hndSlave);
                        printf("\t\t-----------\n");
                        ecmSleep(1000);
                    } else if ((descSlave.ulFlags & ECM_FLAG_SLAVE_SOE) != 0) {
                        ecmTestSoeScan(hndSlave);
                        ecmSleep(1000);
                    }
                }
            }
        }

        /* Initialize to request slave diagnostic data */
        hndSlave = hndMaster;

        /*
        * Print device state and active slaves while running.
        * Cyclic I/O data is handled in the callback handler
        */
        for (i = 0; ; i++) {
            uint16_t usData;

            /* Check exit condition */
            if ((g_lRuntime != 0) && i >= (int)g_lRuntime) {
                break;
            }

            /* Print device state (virtual variable) */
            if (pDevState != NULL) {
                ecmCpuToLe(&usData, pDevState, (const uint8_t *)"\x2\x0");
                printf("State: %04x", usData);
            }
            /* Print # of slaves on primary and redundant adapter */
            if (pSlaveCount != NULL) {
                ecmCpuToLe(&usData, pSlaveCount, (const uint8_t *)"\x2\x0");
                printf(" / Pri: %d", usData);
                if (pSlaveCount2 != NULL) {
                    ecmCpuToLe(&usData, pSlaveCount2, (const uint8_t *)"\x2\x0");
                    printf(" / Red: %d", usData);
                }
            }

            /* Print WC state of 1st frame */
            if (pWcState != NULL) {
                ecmCpuToLe(&usData, pWcState, (const uint8_t *)"\x2\x0");
                printf(" / Frm0WC: %04x", usData);
            }

            /* Request diagnostic data in round robin mode */
            if ((ulConfigFlags & ECM_TEST_FLAG_SLAVE_STATUS) != 0) {
                if (ecmGetSlaveHandle(hndSlave, &hndSlave) != ECM_SUCCESS) {
                    hndSlave = hndMaster;
                } else {
                    ECM_SLAVE_DIAG diag;
                    diag.usControl = 0;
                    if(ECM_SUCCESS == ecmGetSlaveDiag(hndSlave, &diag)) {
                        printf(" Link lost (Slave %x): %d", diag.usAddr,
                            diag.counter.ucLostLinkErrorPort0);
                    }
                }
            }

            printf("\n");

            /* Optionally print DC statistic */
            if ((ulConfigFlags & ECM_TEST_FLAG_USE_DC) != 0) {
                ecmTestDcMonitoring(hndDevice, hndMaster);
            }

			//EF mod
			/*
			* This is just a sample code
			* Each step is done only once
			* Each step is done fast and simple, to not take so much time on this
			* Its main purpose is to:
			*	1) Set operation mode to 1 == Profile Position
			*	2) Set Control Word to 6,7 and 15. Check values (W-->R)
			*	3) Set a new position
			*	4) Set motor in motion to reach the new position. Control Word == 31
            */

			//Sends and async request to a slave
			//ecmAsyncRequest(ECM_HANDLE hndMaster, uint8_t ucCmd, ECM_SLAVE_ADDR addr, uint16_t usSize,void *pData, uint16_t *pucCnt);

			//Note that variables declaration and initialization is done before "OP" mode begins
			/*int i = ecmAsyncRequest_cnt;
			if(i > NUMCOMMANDS-1)
				i = NUMCOMMANDS-1;

			switch (ecmAsyncRequest_cnt) {

				case 0:
					break;
				case 1:
					break;
				case 2:
					value = 1;//Operation mode: Profile position
					break;
				case 3:
					value = 0;//dummy value, is RD operation so it will be overwritten. Just to avoid "misunderstandings"
					break;
				case 4:
					break;
				case 5:
					value = 1;//Testing a Logical read/write operation
					//value = 6; //Shutdown --> Control Word 0x010
					break;
				case 6:
					//Testing a Logical read operation
					//value = 7; //Switch on --> Control Word 0x010
					break;
				case 7:
					value = 6; //Shutdown --> Control Word 0x010
					//value = 15; //Switch on + operation enabled --> Control Word 0x010
					break;
				case 8:
					break;
				case 9:
					break;
				default:
					break;
			}

			if (ECM_SUCCESS == ecmAsyncRequest(hndMaster, newCommands[i].cmdType, newCommands[i].address, newCommands[i].numBytes, pData, pucCnt))
			{
				//fprintf(ECMlogFile, "OK! Value BRD = 0x%04X\n", value);
			}
			else 
			{
				fprintf(ECMlogFile, "Error\n");
			}

			ecmAsyncRequest_cnt++;//increase value for next iteration
			*/
			/*pucCnt++;//increase pointer to WC counter

			if (pucCnt > &pucCnt_val[NUMCOMMANDS-1]) {
				pucCnt = &pucCnt_val[NUMCOMMANDS-1];//DON'T OVERFLOW!
			}*/
			//---------------------------------------

            /* If the background worker task is running we just wait and
            * get called in our handler with each cycle. Without worker
            * task we have to drive our own I/O cycle and call the handler
            * in between.
            */
            if (0 == (g_ulIoMode & ECM_TEST_IO_MODE_NO_CYC_WORKER)) {
                ecmSleep(100);//EF mod, default is 1000 ms
            } else {
                for (j = 0; j < 100; j++) {
                    DoEcatIo(hndDevice);
                    ecmSleep(10);
                }
            }

            /* Print collected profiling data if configured */
            if ((ulConfigFlags & ECM_TEST_FLAG_PROFILING) != 0) {
                PrintProfilingData(hndDevice);
            }
        }

        fprintf(ECMlogFile,"Change state to INIT...\n");

        /* Change EtherCAT state back to INIT */
        result = ChangeEcatState(hndDevice, hndMaster, ECM_DEVICE_STATE_INIT);
        if (result != ECM_SUCCESS) {
            printf("Network failed to change into INIT ! (%s)\n",
                ecmResultToString(result));
            (void)ecmTestCleanup();
            return(-1);
        }

        PrintStatistic(hndDevice, hndMaster);

    } while(--g_lLoops > 0);

    printf("Detaching master...\n");
    result = ecmDetachMaster(hndMaster);
    if (result != ECM_SUCCESS) {
        printf("Failed with %s\n", ecmResultToString(result));
    }

    printf("Deleting master...\n");
    result = ecmDeleteMaster(hndMaster);
    if (result != ECM_SUCCESS) {
        printf("Failed with %s\n", ecmResultToString(result));
    }

    printf("Deleting device...\n");
    result = ecmDeleteDevice(hndDevice);
    if (result != ECM_SUCCESS) {
        printf("Failed with %s\n", ecmResultToString(result));
    }

    /* Terminate remote server thread if remote monitor mode is configured */
    if((ulConfigFlags & ECM_TEST_FLAG_REMOTE_MONITOR) != 0) {
        printf("Terminate remote server ...\n");
        ecmStopRemotingServer();
    }

    (void)ecmTestCleanup();
    printf("End\n");

    return 0;
}

void CecmTest::writePDRampSend(float fTargetPos) {

	//A ramp motion requires up to 7 consecutive commands
	//1. Operation mode = 1
	//2. Control word = 6
	//3. Control word = 7
	//4. Control word = 15
	//5. Position target = X (read on other function)
	//6. Control word = 31 (motion starts!)
	//7. Control word = 15 (still mode) -> better wait until 'position target' is achieved
		//5. Position target = X (read on other function)
}

void CecmTest::writePDRampSend(int iTargetPos) {

	//A ramp motion requires up to 7 consecutive commands
	//1. Operation mode = 1
	//2. Control word = 6 : Shutdown (enable voltage)
	//3. Control word = 7 : Switch on
	//4. Control word = 15 : Enable operation
	//5. Position target = X (read on other function)
	//6. Control word = 31 (motion starts!)
	//7. Control word = 15 (still mode) -> better wait until 'position target' is achieved
		//5. Position target = X (read on other function)

	//STATES:
	//Shutdown (enable voltage)
	//Switch on
	//Enable operation (a new position can be set!)
	//Motion
	//Enable operation (a new position can be set!)
	//Motion

}

//Sets Profiler
void CecmTest::setProfiler(float *fLoopMaxVel, float *fLoopMaxAcc) {
	static float maxVel = 0, maxAcc = 0;//old values

	if (maxVel != *fLoopMaxVel) {//New value
		ecmCpuToLe(pucDio_ProfilerMaxVel, fLoopMaxVel, (const uint8_t *)"\x04\x00");
		maxVel = *fLoopMaxVel;
	}
	
	if (maxAcc != *fLoopMaxAcc) {//New value
		ecmCpuToLe(pucDio_ProfilerMaxAcc, fLoopMaxAcc, (const uint8_t *)"\x04\x00");
		ecmCpuToLe(pucDio_ProfilerMaxDec, fLoopMaxAcc, (const uint8_t *)"\x04\x00");//Re-uses "MaxAcc"
		maxAcc = *fLoopMaxAcc;
	}
}
//Writes selected values to execute a Ramp Motion


//bool CecmTest::MotionFRampMode(float fLoopTargetPos) {
bool CecmTest::MotionFRampMode(int iLoopTargetPos) {

	static uint8_t	ucData;
	static uint64_t ucData64;
	static uint8_t	ucData_OperationMode;	//Sets Operation Mode register value
	static uint8_t	ucData_TargetPosition;	//Sets Position value
	static uint32_t cycleCounter;

	static BOOLEAN faul_reset_is_done = FALSE;//EF mod: To send a "RESET FAULT" just once
   
	//Ramp mode control variables
	static int	decimator = 0;
	const int	numCommands = 8;
	static int	iCommand = 0;
	//Little-Endian buffer
	uint8_t LEbuffer[4] = { 0,0,0,0 };
	//address (PDO) | value
	struct shexadecimal_matrix {
		uint8_t *PDOpointer;
		uint8_t value;
	};
	//Commands matrix
	static shexadecimal_matrix hexadecimal_matrix[numCommands] = {
		pucDio_ControlWord,		0x80,//FAULT RESET
		pucDio_OperationMode,	0x01,
		pucDio_ControlWord,		0x06,
		pucDio_ControlWord,		0x07,
		pucDio_ControlWord,		0x0F,
		pucDio_TargetPosition,	0x00,//POSITION TARGET
		pucDio_ControlWord,		0X1F,
		pucDio_ControlWord,		0X0F,
	};
	
	ecmCpuToLe(&LEbuffer[0], &iLoopTargetPos, (const uint8_t *)"\x04\x00");
	uint8_t * ppos = &LEbuffer[0];

	if (iCommand < (numCommands-1) ) {

		if (decimator > 50) {


			if (iCommand != 5) {
				*hexadecimal_matrix[iCommand].PDOpointer = hexadecimal_matrix[iCommand].value;//write value to memory (PDO)

				//
				/*if (iCommand == 1) {//At same time than Operation Mode

					float fLoopMaxVel = 20.0;
					ecmCpuToLe(pucDio_ProfilerMaxVel, &fLoopMaxVel, (const uint8_t *)"\x04\x00");
					//maxVel = *fLoopMaxVel;
				}*/

			}
			else {
				//ecmCpuToLe(hexadecimal_matrix[iCommand].PDOpointer, &iLoopTargetPos, (const uint8_t *)"\x04\x00");
				
				*hexadecimal_matrix[iCommand].PDOpointer		= LEbuffer[0];
				*(hexadecimal_matrix[iCommand].PDOpointer + 1)	= LEbuffer[1];
				*(hexadecimal_matrix[iCommand].PDOpointer + 2)	= LEbuffer[2];
				*(hexadecimal_matrix[iCommand].PDOpointer + 3)	= LEbuffer[3];
			}
			iCommand++;
			decimator = 0;
		}
		else
			decimator++;
		return false;
	}
	else {
		iCommand = 4;//for next iteration
		return true;//END
	}

}

/******************************************************************************/
/*             OS and/or platform specific initialization code                */
/******************************************************************************/

#if defined(LINUX)

# include <pthread.h>
# include <sched.h>
# include <sys/mman.h>
/*
 * Maximum stack size which is guaranteed safe to access without faulting
 */
#define MAX_SAFE_STACK  (16 * 1024)

void stack_prefault(void)
{
    unsigned char dummy[MAX_SAFE_STACK];

    memset(&dummy, 0, MAX_SAFE_STACK);

    return;
}

static int enable_realtime()
{
    struct sched_param p;

    ECM_INIT(p);

    p.sched_priority = sched_get_priority_max(SCHED_FIFO);

    if (sched_setscheduler(0, SCHED_FIFO, &p) != 0)
        return 1;

    printf("New sched policy is SCHED_FIFO, priority is %d\n", p.sched_priority);

    return 0;
}

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Lock memory */
    if (-1 == mlockall(MCL_CURRENT | MCL_FUTURE)) {
        perror("mlockall() failed");
        return(-1);
    }

    /* Pre-fault our stack */
    stack_prefault();

    if (enable_realtime() != 0) {
        printf("enable_realtime() failed!\n");
    }

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    return(0);
}

static int ecmTestCleanup()
{
    return(0);        
}

#elif defined(VXWORKS)

# define MAX_CMDLINE_ARGS    64

/*
 * VxWorks helper code to convert a string into standard argc/argv arguments
 */
STATUS ecmTest(char *pArgs)
{
    char *pcLast, *p =NULL;
    int argc = 0;
    char * argv[MAX_CMDLINE_ARGS];
    char* pszProgname = "ecmTest";

    argv[argc++] = pszProgname;

    /* Tokenize the input string */
    if (pArgs != NULL) {
        p = strtok_r (pArgs, " ", &pcLast);

        while ((p != NULL) && (argc < MAX_CMDLINE_ARGS)) {
            argv[argc++] = p;
            p = strtok_r (NULL, " ", &pcLast);
        }
    }

    /* Check for too many arguments */
    if (p != NULL) {
        printf("Too many arguments in command line !!\n");
        return ERROR;
    } else {
        optreset = 1;
        return main(argc, argv);
    }
}

/*
 * VxWorks has no common API to return the link state of the Ethernet PHYs
 * For this reason we have to implement a special handler for this purpose
 * which handles this in a BSP specific way.
 *
 * The 2nd argument of the callback is either ECM_NIC_PRIMARY or
 * ECM_NIC_REDUNDANT.
 *
 * Below is an example for the esd PMC-CPU/440 BSP which exports a function
 * phyRead() to access a PHY register. As this is a very time consuming
 */

# if (CPU==PPC440)
extern STATUS phyRead(int emacNumber, UINT8 phyReg, UINT16 *value);

static ECM_LINK_STATE GetLinkState(ECM_HANDLE hndDevice, ECM_NIC_TYPE nic)
{
    UINT16 value = 0;
    int iPhy = (ECM_NIC_PRIMARY == nic) ? iPrimaryNic : iRedundantNic;

    /* Read PHY register 1 and check Bit 2 which indicates a link if set */
    if (OK == phyRead(iPhy, 1, &value)) {
        if ((value & 0x0004) != 0) {
            return(ECM_LINK_STATE_CONNECTED);
        } else {
            return(ECM_LINK_STATE_DISCONNECTED);
        }
    } else {
        return(ECM_LINK_STATE_UNKNOWN);
    }
}
# else

/*
static ECM_LINK_STATE GetLinkState(ECM_HANDLE hndDevice, ECM_NIC_TYPE nic)
{
  return(ECM_LINK_STATE_CONNECTED);
}
*/

# endif /* of (CPU == PPC440) */

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Comment out the code below for HW specific link state implementation */
    /* pLibInit->pfnLinkState = GetLinkState; */

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    return(0);
}

static int ecmTestCleanup()
{
    return(0);        
}

#elif defined (UNDER_RTSS)

extern int ecmInitRtx(void);
extern int ecmCleanupRtx(void);

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    /* Return result of RTX initialization */
    return(ecmInitRtx());
}

static int ecmTestCleanup()
{
    int result;

    result = ecmInitLibrary(NULL);
    if(ECM_SUCCESS == result) {
        result = ecmCleanupRtx();
    }

    return(result);
}

#elif defined(WINCE)
int _tmain(int argc, _TCHAR* argv[])
{
    char cAsciiStr[1024];
    int i, nBytes;
    char *pBuffer = cAsciiStr;
    char *_argv[30];

    //argc = CreateArgvArgc(TEXT("cantest.exe"), argv, pCmdLine);

    for (i = 0; i < argc; i++) {
        nBytes = WideCharToMultiByte(CP_ACP, 0, argv[i], -1, pBuffer,
                                     sizeof(cAsciiStr) - (pBuffer - cAsciiStr),
                                     NULL, NULL);
        _argv[i] = pBuffer;
        pBuffer += nBytes;
    }

    return main(argc, _argv);
}

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    return(0);
}

static int ecmTestCleanup()
{
    return(0);        
}

# elif defined(_WIN32)

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Change priority class to high */
    if(!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        fprintf(ECMlogFile,"Failed to change priority call to HIGH with %d\n", GetLastError());
    }

    /* Comment out the code below for HW specific DC clock synchronization */
    pLibInit->pfnAdjustClock  = ecmTestAdjustClock;

    return(0);
}

static int ecmTestCleanup()
{
    ecmInitLibrary(NULL);   /* Do final cleanup */

    return(0);        
}

# elif defined(_OS9000)

#include <modes.h>
#include <sysglob.h>
#include <LIB/dbglog.h>

    /* Change the OS-9 system tick */
static int os9ChangeTick(int offset, int absolute)
{
    Sysglobs sg;
    glob_buff buf;
    int err;
    int tweak;

    if(absolute != 0) {
        tweak = offset;
    } else {
        err = _os_getsys(OFFSET(Sysglobs, d_globs[4]), sizeof(sg->d_globs[4]), &buf);
        if(0 != err) {
            printf( "_os_getssys returned %d\n", err );
        }
        tweak  = (int32)buf.lng;
        tweak += offset;
    }

    buf.lng = (u_int32)tweak;
    err = _os_setsys(OFFSET(Sysglobs, d_globs[4]), sizeof(sg->d_globs[4]), buf);
    if(0 != err) {
        printf( "_os_setssys returned %d\n", err );
    }

    return(tweak);
}

    /* Return a CPU architecture specific high resolution timestamp */
static uint64_t os9GetClockCycles(void)
{
    uint64_t tick;
#ifdef _MPFPOWERPC
    u_int32 tbhi1, tbhi2, tblo;

    /* Get a correct tblo value */
    do {
        _asm(-100, "\n"
            " mftbu %0\n"
            " mftb %1\n"
            " mftbu %2\n",
            __reg_gen(__obj_assign(tbhi1)),
            __reg_gen(__obj_assign(tblo)),
            __reg_gen(__obj_assign(tbhi2)));
    } while (tbhi1 != tbhi2);

    tick = (((u_int64)tbhi2) <<32) | tblo;
#else
# error Unsupported CPU architecture
#endif

    return tick;
}

    /*
     * Application must be User 0.0 to allow call to _os_permit()
     * in EtherCAT library
     */
_asm("_m_grpusr: equ 0");

    /* Device name of the NIC which is used for EtherCAT */
#define OS9_ECAT_DEVICE_NAME        "spee1"

static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize the network interface for EtherCAT */
    if((dev_list *)-1 == attach(OS9_ECAT_DEVICE_NAME, FAM_READ | FAM_WRITE)) {
        printf("Network interface '%s' failed to attach with error %d\n",
                OS9_ECAT_DEVICE_NAME, errno);
        return(-1);
    }

   
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Enable system state preemption for the worker threads. */
    pLibInit->ulPlatformFlags = ECM_FLAG_ENABLE_PREEMTION;

    /*
     * NIC device names available for EtherCAT (without IP configuration)
     * given as 'NIC=<device1>:NIC=<device2>:SPF=<instance>:STACK=<size>'.
     * Before EtherCAT master V1.9.0 the SPF=<instance> is ignored.
     * Before EtherCAT master V1.9.4 the STACK=<size> is ignored.
     */
    pLibInit->pszPlatformConfig = "NIC="OS9_ECAT_DEVICE_NAME":SPF=1:STACK=16384";

    /* Callback to capture a high resolution counter */
    pLibInit->pfnClockCycles  = os9GetClockCycles;

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    /* Clock frequency for esd NGCC with 553 MHz */
    pLibInit->ullCyclesPerSec = 33333333 * 16;

    /* Reset current tick modification */
    (void)os9ChangeTick(0, 1);

#if defined (_OS9DEBUG)
    dbglog_reset();

    /* set the proper env vars for memory leak detection */
    dbglog_setenv("CRT_MASK", "err,warn,mem,mem_leak,mem_leakd");
#endif

    return(0);
}

static int ecmTestCleanup()
{
#if defined (_OS9DEBUG)
    _d_mem_info();
#endif

    return(0);        
}

# else
static int ecmTestInit(ECM_LIB_INIT *pLibInit)
{
    /* Initialize data structure */
    ECM_INIT(*pLibInit);

    /* Comment out the code below for HW specific DC clock synchronization */
    /* pLibInit->pfnAdjustClock  = ecmTestAdjustClock; */

    /*
     * Comment out the code below to adapt default worker task stack size
     * Note: Before EtherCAT master V1.9.4 the STACK=<size> is ignored.
     */
    /* pLibInit->pszPlatformConfig = "STACK=32768"; */

    return (0);
}

static int ecmTestCleanup()
{
    return(0);        
}

# endif /* of defined(LINUX) */
