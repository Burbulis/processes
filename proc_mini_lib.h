#ifndef __PROCESS_DEF__
#define __PROCESS_DEF__

#include <windows.h> 
#include <Psapi.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <iterator>
#include <iostream>
#include "process_mem_man.h"
#include "peb.hpp"
#include "md5.h"
//#include "memorizer.hpp"
#include "pe_tools.h"
#include "helpers.hpp"
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)



typedef struct _CLIENT_ID {
  DWORD UniqueProcess;
  DWORD UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

  typedef struct _THREAD_BASIC_INFORMATION {
  typedef PVOID KPRIORITY;
  NTSTATUS ExitStatus; 
  PVOID TebBaseAddress; 
  CLIENT_ID ClientId; 
  KAFFINITY AffinityMask; 
  KPRIORITY Priority; 
  KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct _UNICODE_STRING {     
	USHORT Length;     
	USHORT MaximumLength;     
	PWSTR Buffer; } 
UNICODE_STRING, *PUNICODE_STRING;

typedef long NTSTATUS;
typedef NTSTATUS *PNTSTATUS;

typedef ULONG KAFFINITY;
typedef KAFFINITY *PKAFFINITY;

typedef LONG KPRIORITY;
/*
typedef struct _PROCESS_PARAMETERS
{
  ULONG   AllocationSize;
  ULONG   ActualSize;
  ULONG   Flags;
  ULONG   Unknown1;
  HANDLE  InputHandle;
  HANDLE  OutputHandle;
  HANDLE  ErrorHandle;
  UNICODE_STRING Unknown3;         
  UNICODE_STRING CurrentDirectory;
  HANDLE  CurrentDir;
  UNICODE_STRING SearchPath;       
  UNICODE_STRING ApplicationName;   
  UNICODE_STRING CommandLine;
  PVOID   EnvironmentBlock;
  ULONG   Unknown[9];
  UNICODE_STRING Unknown4;
  UNICODE_STRING Unknown5;
  UNICODE_STRING Unknown6;
  UNICODE_STRING Unknown7;
} PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;
*/
typedef struct _LDR_MODULE

{
   LIST_ENTRY     InLoadOrderModuleList;
   LIST_ENTRY     InMemoryOrderModuleList;// not used
   LIST_ENTRY     InInitializationOrderModuleList;// not used
   PVOID          BaseAddress;
   ULONG          EntryPoint;
   ULONG          SizeOfImage;
   UNICODE_STRING FullDllName;
   UNICODE_STRING BaseDllName;
   ULONG          Flags;
   SHORT          LoadCount;
   SHORT          TlsIndex;
   HANDLE         SectionHandle;
   ULONG          CheckSum;
   ULONG          TimeDateStamp;
#ifdef KDBG
  IMAGE_SYMBOL_INFO SymbolInfo;
#endif /* KDBG */
} LDR_MODULE, *PLDR_MODULE;


typedef struct _PEB_LDR_DATA

{
   ULONG Length;
   BOOLEAN Initialized;
   PVOID SsHandle;
   LIST_ENTRY InLoadOrderModuleList;
   LIST_ENTRY InMemoryOrderModuleList;
   LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;
/*
typedef struct _RTL_USER_PROCESS_PARAMETERS {
  BYTE           Reserved1[16];
  PVOID          Reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _UNICODE_STRING_WOW64 {
	USHORT Length;
	USHORT MaximumLength;
	PVOID64 Buffer;
} UNICODE_STRING_WOW64 , *PUNICODE_STRING_WOW64;
*/
typedef struct _CURDIR64
{
	UNICODE_STRING_WOW64 DosPath;
	HANDLE Handle;
} CURDIR64, *PCURDIR64;

/*
typedef struct _RTL_USER_PROCESS_PARAMETERS64
{
	ULONG MaximumLength;                            // Should be set before call RtlCreateProcessParameters
	ULONG Length;                                   // Length of valid structure
	ULONG Flags;                                    // Currently only PPF_NORMALIZED (1) is known:
													//  - Means that structure is normalized by call RtlNormalizeProcessParameters
	ULONG DebugFlags;
	PVOID64 ConsoleHandle;                            // HWND to console window associated with process (if any).
	ULONG ConsoleFlags;
	DWORD64 StandardInput;
	DWORD64 StandardOutput;
	DWORD64 StandardError;
	CURDIR64 CurrentDirectory;                        // Specified in DOS-like symbolic link path, ex: "C:/WinNT/SYSTEM32"

	UNICODE_STRING_WOW64 DllPath;                         // DOS-like paths separated by ';' where system should search for DLL files.
	UNICODE_STRING_WOW64 ImagePathName;                   // Full path in DOS-like format to process'es file image.
	UNICODE_STRING_WOW64 CommandLine;                     // Command line
	PVOID64 Environment;                              // Pointer to environment block (see RtlCreateEnvironment)
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;                            // Fill attribute for console window
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING_WOW64 WindowTitle;
	UNICODE_STRING_WOW64 DesktopInfo;                     // Name of WindowStation and Desktop objects, where process is assigned
	UNICODE_STRING_WOW64 ShellInfo;
	UNICODE_STRING_WOW64 RuntimeData;
	//RTL_DRIVE_LETTER_CURDIR CurrentDirectores[0x20];
	//ULONG EnvironmentSize;
} RTL_USER_PROCESS_PARAMETERS64, *PRTL_USER_PROCESS_PARAMETERS64;
*/


typedef struct _PEBx64 {
  BYTE Reserved1[2];
  BYTE BeingDebugged;
  BYTE Reserved2[21];
  PPEB_LDR_DATA LoaderData;
  PVOID64 ProcessParameters;
  BYTE Reserved3[520];
  void *PostProcessInitRoutine;
  BYTE Reserved4[136];
  ULONG SessionId;
} PEBx64;


/*
typedef struct _PROCESS_ENVIRONMENT_BLOCK
{

  BOOLEAN                 InheritedAddressSpace;
  BOOLEAN                 ReadImageFileExecOptions;
  BOOLEAN                 BeingDebugged;
  BOOLEAN                 Spare;
  HANDLE                  Mutant;
  PVOID                   ImageBaseAddress;
  PPEB_LDR_DATA Ldr;                             // 0Ch
  PPROCESS_PARAMETERS    pp;
  HANDLE  hHeap;     
  DWORD   lock;      
  void    *ack_lock; 
  void    *rel_lock; 
  DWORD   *lock_cnt; 
  void    *user_cb;  
  HANDLE  *heaps;    
  void    *cs;
  DWORD   ver;       
} PROCESS_ENVIRONMENT_BLOCK, *PPROCESS_ENVIRONMENT_BLOCK;
*/

/*
typedef struct _PEB
{
	BOOLEAN                 InheritedAddressSpace;
	BOOLEAN                 ReadImageFileExecOptions;
	BOOLEAN                 BeingDebugged;
	BOOLEAN                 Spare;
	HANDLE                  Mutant;
	PVOID                   ImageBaseAddress;
	PPEB_LDR_DATA           LoaderData;
	PROCESS_PARAMETERS      *ProcessParameters;
	PVOID                   SubSystemData;
	PVOID                   ProcessHeap;
	PVOID                   FastPebLock;
	//  PPEBLOCKROUTINE         FastPebLockRoutine;
	PVOID         FastPebLockRoutine;
	//  PPEBLOCKROUTINE         FastPebUnlockRoutine;
	PVOID         FastPebUnlockRoutine;
	ULONG                   EnvironmentUpdateCount;
	PVOID                  *KernelCallbackTable;
	PVOID                   EventLogSection;
	PVOID                   EventLog;
	//  PPEB_FREE_BLOCK         FreeList;
	PVOID         FreeList;
	ULONG                   TlsExpansionCounter;
	PVOID                   TlsBitmap;
	ULONG                   TlsBitmapBits[0x2];
	PVOID                   ReadOnlySharedMemoryBase;
	PVOID                   ReadOnlySharedMemoryHeap;
	PVOID                   *ReadOnlyStaticServerData;
	PVOID                   AnsiCodePageData;
	PVOID                   OemCodePageData;
	PVOID                   UnicodeCaseTableData;
	ULONG                   NumberOfProcessors;
	ULONG                   NtGlobalFlag;
	BYTE                    Spare2[0x4];
	LARGE_INTEGER           CriticalSectionTimeout;
	ULONG                   HeapSegmentReserve;
	ULONG                   HeapSegmentCommit;
	ULONG                   HeapDeCommitTotalFreeThreshold;
	ULONG                   HeapDeCommitFreeBlockThreshold;
	ULONG                   NumberOfHeaps;
	ULONG                   MaximumNumberOfHeaps;
	PVOID                  **ProcessHeaps;
	PVOID                   GdiSharedHandleTable;
	PVOID                   ProcessStarterHelper;
	PVOID                   GdiDCAttributeList;
	PVOID                   LoaderLock;
	ULONG                   OSMajorVersion;
	ULONG                   OSMinorVersion;
	ULONG                   OSBuildNumber;
	ULONG                   OSPlatformId;
	ULONG                   ImageSubSystem;
	ULONG                   ImageSubSystemMajorVersion;
	ULONG                   ImageSubSystemMinorVersion;
	ULONG                   GdiHandleBuffer[0x22];
	ULONG                   PostProcessInitRoutine;
	ULONG                   TlsExpansionBitmap;
	BYTE                    TlsExpansionBitmapBits[0x80];
	ULONG                   SessionId;
} PEB, *PPEB;
*/

//typedef PROCESS_ENVIRONMENT_BLOCK PEB, *PPEB;

typedef struct _TEB {
  PVOID Reserved1[12];
  PPEB32  ProcessEnvironmentBlock;
  PVOID Reserved2[399];
  BYTE  Reserved3[1952];
  PVOID TlsSlots[64];
  BYTE  Reserved4[8];
  PVOID Reserved5[26];
  PVOID ReservedForOle;
  PVOID Reserved6[4];
  PVOID TlsExpansionSlots;
} TEB, *PTEB;


typedef enum _PROCESSINFOCLASS 
{
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,          
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    MaxProcessInfoClass
} PROCESSINFOCLASS;

typedef   enum   _THREADINFOCLASS
{
  ThreadBasicInformation,
  ThreadTimes,
  ThreadPriority,
  ThreadBasePriority,
  ThreadAffinityMask,
  ThreadImpersonationToken,
  ThreadDescriptorTableEntry,
  ThreadEnableAlignmentFaultFixup,
  ThreadEventPair_Reusable,
  ThreadQuerySetWin32StartAddress,
  ThreadZeroTlsCell,
  ThreadPerformanceCount,
  ThreadAmILastThread,
  ThreadIdealProcessor,
  ThreadPriorityBoost,
  ThreadSetTlsArrayAddress,
  ThreadIsIoPending,
  ThreadHideFromDebugger,
  ThreadBreakOnTermination,
  MaxThreadInfoClass
}   THREADINFOCLASS;
typedef THREADINFOCLASS   ThreadInformationClass;

/*typedef struct _PROCESS_BASIC_INFORMATION 
{
  NTSTATUS ExitStatus;
  PPEB PebBaseAddress;
  KAFFINITY AffinityMask;
  KPRIORITY BasePriority;
  ULONG UniqueProcessId;
  ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;
*/
typedef struct _PROCESS_BASIC_INFORMATION
{
	PVOID Reserved1;
	PPEB32 PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;


  typedef struct NT_FILETIME 
  { // ft 
    DWORD dwLowDateTime; 
    DWORD dwHighDateTime; 
  }; 

  typedef struct _PROCESS_INFO
  {
    DWORD         dwOffset; // an ofset to the next Process structure // 00h
    DWORD         dwThreadCount;                                      // 04h
    DWORD         dwUnknown1[6];                                       // 08h
    NT_FILETIME   ftCreationTime;                                     // 20h
    NT_FILETIME   ftUserTime;
    NT_FILETIME   ftKernelTime;
    UNICODE_STRING ProcessName;                                       // 38h
    DWORD         dwBasePriority;                                     // 40h
    DWORD         dwProcessID;                                        // 44h
    DWORD         dwParentProcessID;                                  // 48h
    DWORD         dwHandleCount;                                      // 4Ch
    DWORD         dwUnkown7;                                          // 50h
    DWORD         dwUnkown8;                                          // 54h
    DWORD         dwVirtualBytesPeak;
    DWORD         dwVirtualBytes;     //dwVirtualSize
    DWORD         dwPageFaultsCountPerSec;
    DWORD         dwWorkingSetPeak;   //PeakWorkingSetSize
    DWORD         dwWorkingSet;       //WorkingSetSize
    DWORD         dwPeekPagedPoolUsage;
    DWORD         dwPagedPool; // kbytes PagedPoolUsage
    DWORD         dwPeekNonPagedPoolUsage;
    DWORD         dwNonPagedPool; // kbytes NonPagedPoolUsage
    DWORD         dwPageFileBytesPeak;
    DWORD         dwPageFileBytes;
    DWORD         dwPrivateBytes;
    NT_FILETIME   ProcessorTime;
    DWORD         dwUnknown13;
    DWORD         dwUnknown14;
} PROCESS_INFO, *PPROCESS_INFO;              
typedef enum _SYSTEM_INFORMATION_CLASS
{
  SystemBasicInformation = 0,
  SystemProcessorInformation,
  SystemTimeZoneInformation,
  SystemTimeInformationInformation,
  SystemUnk4Information, 
  SystemProcessesInformation,
  SystemUnk6Information,
  SystemConfigurationInformation,
  SystemUnk8Information,
  SystemUnk9Information,
  SystemUnk10Information,
  SystemDriversInformation,
  SystemLoadImageInformation = 26,
  SystemUnloadImageInformation = 27,
  SystemLoadAndCallImageInformation = 38
} SYSTEM_INFORMATION_CLASS;
/*
typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;
*/

typedef struct _VM_COUNTERS {
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} VM_COUNTERS;
typedef VM_COUNTERS *PVM_COUNTERS;

typedef struct _SYSTEM_THREAD_INFORMATION {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
	ULONG ContextSwitchCount;
	LONG State;
	LONG WaitReason;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;


typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters;
	SYSTEM_THREAD_INFORMATION Threads[1];
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;




#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)



typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
	PVOID Reserved1[2];
	PVOID64 PebBaseAddress;
	PVOID Reserved2[4];
	ULONG_PTR UniqueProcessId[2];
	PVOID Reserved3[2];
} PROCESS_BASIC_INFORMATION_WOW64;


typedef struct _THREAD_INFO 
{ 
  NT_FILETIME   ftCreationTime; 
  DWORD         dwUnknown1; 
  DWORD         dwStartAddress; 
  DWORD         dwOwningPID; 
  DWORD         dwThreadID; 
  DWORD         dwCurrentPriority; 
  DWORD         dwBasePriority; 
  DWORD         dwContextSwitches; 
  DWORD         dwThreadState; 
  DWORD         dwThreadWaitReason; 
  DWORD         dwUnknown3; 
  DWORD         dwUnknown4; 
  DWORD         dwUnknown5; 
  DWORD         dwUnknown6; 
  DWORD         dwUnknown7; 
} THREAD_INFO, *PTHREAD_INFO; 



typedef long (__stdcall *PNTQUERYSYSTEMINFORMATION)
(IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
 OUT PVOID SystemInformation, // pointer to buffer
 IN DWORD SystemInformationLength, // buffer size in bytes       
 OUT PDWORD LehgthReturned OPTIONAL // number of bytes written to the buffer
  	);
typedef LONG(__stdcall *PNTQUERYINFORMATIONPROCESS)(HANDLE , PROCESSINFOCLASS, PVOID, ULONG, PULONG);
typedef LONG(__stdcall *PNTQUERYINFORMATIONTHREAD)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);
static PNTQUERYSYSTEMINFORMATION NtQuerySystemInformation = NULL;
static PNTQUERYINFORMATIONPROCESS NtQueryInformationProcess = NULL;
static PNTQUERYINFORMATIONTHREAD NtQueryInformationThread = NULL;


PTHREAD_INFO 
GetThreadInfoPtr(
       PPROCESS_INFO pProcessInfo
       );

std::string GetProcessName(DWORD dwPid);



struct EmptyState
{
  
  EmptyState(DWORD dwThreadID)
  {
    this->dwThreadID = dwThreadID;
  }

  bool
  operator()()
  { 
    return ( dwThreadID != GetCurrentThreadId() );
  }
private:
  DWORD dwThreadID;
};

struct  SuspendPolicy
{
  SuspendPolicy(DWORD dwThreadID)
  {
    ThreadHandle = OpenThread(THREAD_ALL_ACCESS,false,dwThreadID); 
    char tmp_[MAX_PATH];
  //  sprintf(tmp_,"ThreadHandle = %d\n",ThreadHandle);
  //  OutputDebugStringA(tmp_);     
 
  }
  bool operator ()()
  {
    if (!ThreadHandle)
      return (false); 
    return ( -1 != SuspendThread(
        ThreadHandle  
      ));
  }
  ~SuspendPolicy()
  {
    CloseHandle( ThreadHandle );
  }
private:
  HANDLE ThreadHandle;
};

struct  ResumePolicy
{
  ResumePolicy(DWORD dwThreadID)
  {
    ThreadHandle = OpenThread(THREAD_ALL_ACCESS,false,dwThreadID); 
  }
  bool operator ()()
  {
    if (!ThreadHandle)
      return (false); 
    return ( -1 != ResumeThread(
        ThreadHandle  
      ));
  }
  ~ResumePolicy()
  {
    CloseHandle( ThreadHandle );
  }
private:
  HANDLE ThreadHandle;
};


template
<
  typename SetStatePolicy,
  typename StateCmpPolicy
>
void 
manipulate_threads()
{
  char tmp_[MAX_PATH];
  enum{PROC_INFO_COUNT = 500};
  HINSTANCE hinst_ntdll;
  hinst_ntdll = GetModuleHandle("ntdll.dll");
  if (!hinst_ntdll) 
    hinst_ntdll=LoadLibrary("ntdll.dll"); 
  if (!hinst_ntdll)
    return ;  
  
  sprintf(tmp_,"hinst_ntdll == %X\n",hinst_ntdll);
  OutputDebugStringA(tmp_);   

  NtQuerySystemInformation= 
    reinterpret_cast<NTQUERYSYSTEMINFORMATION>( 
    GetProcAddress(hinst_ntdll,"NtQuerySystemInformation")
    );

  sprintf(tmp_,"NtQuerySystemInformation == %X\n",NtQuerySystemInformation);
  OutputDebugStringA(tmp_);   

  if (!NtQuerySystemInformation)
    return;
  
  DWORD SysInfoLen;
  unsigned long NtStatus = 0;
  std::vector<PROCESS_INFO> pid;
  pid.resize(PROC_INFO_COUNT);
	SysInfoLen = sizeof(PROCESS_INFO)*PROC_INFO_COUNT;

  OutputDebugStringA("NtQuerySystemInformation");     

  NtStatus =
    NtQuerySystemInformation(SystemProcessesInformation,
       &pid[0],SysInfoLen,&SysInfoLen);

  sprintf(tmp_,"NtStatus == %d\n",NtStatus);
  OutputDebugStringA(tmp_);     
 
  PPROCESS_INFO mem_ptr_pid = &pid[0];
  while (true)
  {
    sprintf(tmp_,"mem_ptr_pid->dwProcessID == %d\n",mem_ptr_pid->dwProcessID);
    OutputDebugStringA(tmp_);     
    if (GetCurrentProcessId() == mem_ptr_pid->dwProcessID)
    {
      OutputDebugStringA("GetCurrentProcessId() == mem_ptr_pid->dwProcessID");     
      std::vector<THREAD_INFO> ThreadInfo;
      ThreadInfo.resize(mem_ptr_pid->dwThreadCount);
 			memcpy   (
          &ThreadInfo[0],
          GetThreadInfoPtr(mem_ptr_pid),
          sizeof(THREAD_INFO)*mem_ptr_pid->dwThreadCount
         );
      for (size_t i = 0; i < mem_ptr_pid->dwThreadCount; ++i)
      {
        sprintf(tmp_,"i = %d : threadId = %X\n",i,ThreadInfo[i].dwThreadID);
        OutputDebugStringA(tmp_);     
        SetStatePolicy SetPolicy( ThreadInfo[i].dwThreadID );
        StateCmpPolicy StateCmpPolicy ( ThreadInfo[i].dwThreadID );
        if (StateCmpPolicy())
        {
          if (SetPolicy())
          {
            OutputDebugStringA("Set Policy OK!\n");
          }
        }
			}
    }
    if (!mem_ptr_pid->dwOffset)
      break;
    mem_ptr_pid =
      (
        reinterpret_cast<PPROCESS_INFO>(
          (reinterpret_cast< char* > (mem_ptr_pid) + mem_ptr_pid->dwOffset)
         )
      );
  }
}

PVOID
GetModuleBase(HANDLE prc_hndl_,::std::string mod_name);

::std::string 
GetProcessName( void );

struct _Thread
{
  DWORD Tid;
  LONG State;
  LONG BasePriority;
  LONG Priority;
  LARGE_INTEGER KernelTime;
  LARGE_INTEGER UserTime;
  LARGE_INTEGER CreateTime;
  _Thread(
     DWORD Tid_,   
	 LARGE_INTEGER _KernelTime,
     LARGE_INTEGER _UserTime,
     LARGE_INTEGER _CreateTime,  
	 LONG State_,LONG BasePriority_,
	 LONG Priority_)
	  :State(State_),BasePriority(BasePriority_),Priority(Priority_),
	  KernelTime(_KernelTime),UserTime(_UserTime),CreateTime(_CreateTime),
	  Tid(Tid_)
  {};
};

struct Counters
{
  Counters() {};
  Counters(
    size_t PeakVirtualSize_,
    size_t VirtualSize_,
    ULONG PageFaultCount_,
    size_t PeakWorkingSetSize_,
    size_t WorkingSetSize_,
    size_t QuotaPeakPagedPoolUsage_,
    size_t QuotaPagedPoolUsage_,
    size_t QuotaPeakNonPagedPoolUsage_,
    size_t QuotaNonPagedPoolUsage_,
    size_t PagefileUsage_,
    size_t PeakPagefileUsage_
  ) 
  :PeakVirtualSize(PeakVirtualSize_),
  VirtualSize(VirtualSize_),
  PageFaultCount(PageFaultCount_),
  PeakWorkingSetSize(PeakWorkingSetSize_),
  WorkingSetSize(WorkingSetSize_),
  QuotaPeakPagedPoolUsage(QuotaPeakPagedPoolUsage_),
  QuotaPagedPoolUsage(QuotaPagedPoolUsage_),
  QuotaPeakNonPagedPoolUsage(QuotaPeakNonPagedPoolUsage_),
  QuotaNonPagedPoolUsage(QuotaNonPagedPoolUsage_),
  PagefileUsage(PagefileUsage_),
  PeakPagefileUsage(PeakPagefileUsage_)
  {};

  size_t PeakVirtualSize;
  size_t VirtualSize;
  ULONG PageFaultCount;
  size_t PeakWorkingSetSize;
  size_t WorkingSetSize;
  size_t QuotaPeakPagedPoolUsage;
  size_t QuotaPagedPoolUsage;
  size_t QuotaPeakNonPagedPoolUsage;
  size_t QuotaNonPagedPoolUsage;
  size_t PagefileUsage;
  size_t PeakPagefileUsage;

  std::string
    get_command()
  {
    std::string insert_cmd = "INSERT INTO VM_COUNTERS(?,?,?,?,?,?,?,?,?,?,?)";


  }

};


struct _Process 
{	 
  _Process() {};
  Counters counter_obj;
  ::std::string hash_in_disk;
  ::std::string ProcessName;
  ::std::string image_path;
  ::std::string commandline;
  ULONG Uid;
  ::std::vector<_Thread>  Thread_Seq;
};


template
<
	typename T
>
struct EmptyFunctor
{
  T& operator()(T& t)
  {
	return (t);
  }
};


/*
struct NtQueryWapper
{



}

struct GetProcessBasicInfo
	:protected mem::_handle
{

	typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
		PVOID Reserved1[2];
		PVOID64 PebBaseAddress;
		PVOID Reserved2[4];
		ULONG_PTR UniqueProcessId[2];
		PVOID Reserved3[2];
	} PROCESS_BASIC_INFORMATION_WOW64;

	GetProcessBasicInfo(DWORD pid)
	{
		mem::_handle(PROCESS_ALL_ACCESS, pid);
		PROCESS_BASIC_INFORMATION_WOW64 pbi;
		DWORD NtStatus = NtQueryInformationProcess(mem::_handle::get(), 0, &pbi, sizeof(pbi), NULL);

	}


};

*/
struct GetThreadBasicInfo
{
  GetThreadBasicInfo(HANDLE ThreadId, ULONG Pid) {
    if (!ThreadId)
      return;
    SIZE_T Readed = 0;
    THREAD_BASIC_INFORMATION tbi;
    P64 pp;
    HINSTANCE hinst_ntdll = LoadLibraryA("ntdll.dll");
    NtQueryInformationThread = reinterpret_cast<PNTQUERYINFORMATIONTHREAD>(GetProcAddress(hinst_ntdll, "NtQueryInformationThread"));
//    HANDLE proc_Handle = ::OpenProcess(PROCESS_ALL_ACCESS, true, Pid);
   
    //mem::protector<>
    DWORD NtStatus = NtQueryInformationThread(ThreadId, ThreadBasicInformation, &tbi, sizeof(tbi), 0);
   // printf("", NtStatus);
    if (tbi.TebBaseAddress)
    {
     // printf("NtStatus == %x\n,ThreadId = %x\n", NtStatus, ThreadId);
     // printf("TebBaseAddress == %x\n", tbi.TebBaseAddress);
	 // DWORD ProcessParametersOffset = si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x20 : 0x10;
	  TEB teb;
      PEB32 _peb; 
      mem::_handle open(PROCESS_ALL_ACCESS, Pid);
      mem::ObjectList< mem::Universal_Type > *ol = new mem::ObjectList< mem::Universal_Type >;
      DWORD _old_protect;
	  
	  {
      //  mem::protector<TEB> teb_(reinterpret_cast<TEB *>( tbi.TebBaseAddress ));
      //  teb_._rd(&teb);
      //  mem::protector<PEB> peb_(teb.ProcessEnvironmentBlock);
      //  peb_._rd(&_peb);
      //  mem::protector<PROCESS_PARAMETERS> pp_(_peb.ProcessParameters);
      //  pp_._rd(&pp);
        //mem::Universal_Type tbi.TebBaseAddress,sizeof(TEB),typeid(TEB).hash_code();
		  DWORD dw = 0;
     //   mem::read(ol,reinterpret_cast<TEB *>(tbi.TebBaseAddress), &teb);
		//PRTL_USER_PROCESS_PARAMETERS64 _pp = reinterpret_cast<PRTL_USER_PROCESS_PARAMETERS64>(teb.ProcessEnvironmentBlock + 0x20);
		//DWORD dw_peb = reinterpret_cast<DWORD>(peb);
	//	mem::read(ol,_pp, &dw);
		UNICODE_STRING cmdline;
		PCHAR dw_cmd = reinterpret_cast<PCHAR>(dw);
	//	mem::read(ol, dw_cmd + 112,reinterpret_cast<PCHAR>(&cmdline));
		//printf("cmd = %ws\n", cmdline.Buffer);
      //  mem::read(ol, reinterpret_cast<PROCESS_PARAMETERS *>(_peb.ProcessParameters), &pp);
      //  wchar_t *CommandLine = new wchar_t[pp.CommandLine.Length * 2];
      //  mem::read(ol,reinterpret_cast<wchar_t *> (pp.CommandLine.Buffer), CommandLine, pp.CommandLine.Length * 2);
      //  wchar_t *appName = new wchar_t[pp.ApplicationName.Length * 2];
      //  RtlZeroMemory(appName, pp.ApplicationName.Length * 2);
      //  size_t sz_ = pp.ApplicationName.Length * 2;
      //  mem::read(ol,reinterpret_cast<wchar_t *>(pp.ApplicationName.Buffer), appName, sz_);
	//	if (appName[0] != 0)
	//	{
	//		printf("appName = %ws\n", appName);
	//		_asm {nop}
	//	}
		//wchar_t *cmd = new wchar_t[cmdline.Length + 2];
		//mem::read(ol, reinterpret_cast<PWCHAR>(cmdline.Buffer), reinterpret_cast<PWCHAR>(cmd));
	  }

      delete ol;
     // ReadProcessMemory(proc_Handle, tbi.TebBaseAddress ,reinterpret_cast<PTEB>(&teb), sizeof(TEB), &Readed);
     // printf("PEB = %x\n", teb.ProcessEnvironmentBlock);
     // {
     // }
     // Readed = 0;
     /* ReadProcessMemory(proc_Handle, _peb.ProcessParameters, reinterpret_cast<PPROCESS_PARAMETERS>(&pp), sizeof(pp), &Readed);
      wchar_t *CommandLine = new wchar_t[pp.CommandLine.Length * 2];
      char *prcName = new char[pp.CommandLine.Length*2];
      RtlZeroMemory(prcName, pp.CommandLine.Length * 2);
      ReadProcessMemory(proc_Handle, pp.CommandLine.Buffer, CommandLine, pp.CommandLine.Length, &Readed);
      wcstombs(prcName, CommandLine, pp.CommandLine.Length);
      printf("prcName =%s\n", prcName);
      //pp.ApplicationName
      wchar_t *ApplicationName = new wchar_t[pp.ApplicationName.Length * 2];
      char *appName = new char[pp.ApplicationName.Length * 2];
      RtlZeroMemory(appName, pp.ApplicationName.Length * 2);
      ReadProcessMemory(proc_Handle, pp.ApplicationName.Buffer, ApplicationName, pp.ApplicationName.Length, &Readed);
      wcstombs(appName, ApplicationName, pp.ApplicationName.Length);
      printf("appName =%s\n", appName);
      printf("ImageBase = %x\n", _peb.ImageBaseAddress);
      std::vector< char > buffer;
      buffer.resize(100);
      VirtualProtectEx(proc_Handle, _peb.ImageBaseAddress, 100, PAGE_EXECUTE_READ, &_old_protect);
      ReadProcessMemory(proc_Handle, _peb.ImageBaseAddress, reinterpret_cast<LPVOID>(&buffer[0]), buffer.size(), &Readed);

      delete prcName;
      delete CommandLine;*/
    }
  //  ::CloseHandle(proc_Handle);
  }

};


struct _handle
{
	_handle(DWORD _Access, DWORD Pid) :_pid(Pid), _access(_Access)
	{
		init();
	};

	void init()
	{
		HINSTANCE load_dll = NULL;
		load_dll = GetModuleHandle("ntdll.dll");
		if (!load_dll)
			load_dll = LoadLibrary("ntdll.dll");
		//if (!hinst_ntdll)
		//	return;
	//	NtWow64ReadVirtualMemory64 = reinterpret_cast<PNTWOW64READVIRTUALMEMORY64>(GetProcAddress(load_dll, "NtWow64ReadVirtualMemory64"));

		if (NULL != handle)
		{
			return;
		}
		isWow64 = FALSE;
		handle = ::OpenProcess(_access, true, _pid);
		IsWow64Process(handle, &isWow64);
		if (isWow64)
		{
			__asm {nop}
		}
		if (handle)
		{
			printf("wow= %d pid = %d opened for read\n", isWow64, _pid);
		}
	}

	

	_handle()
	{};

	~_handle()
	{
		destroy();
	}

protected:
	void destroy()
	{
		if (handle)
		{
			::CloseHandle(handle);
		}
		handle = NULL;
	}

	static
	bool isWin32(void)
	{
		return (!!isWow64);
	}
  static
  bool 
  IsWin32Mode(void)
  {
    BOOL isWow64_ = false;
    HANDLE current_process = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
    IsWow64Process(current_process , &isWow64_);
    printf("isWow64_ = %d\n", isWow64_);
    CloseHandle(current_process);
    return (!isWow64_);
  }

	static
	HANDLE get(void)
	{
		return (handle);
	}

private:
	static BOOL isWow64;
	DWORD _pid;
	DWORD _access;
	static HANDLE handle;
};


template
<
	typename T
>
struct	single_traits
{
public:
	single_traits(char *_buffer) 
	{
		obj_ptr = reinterpret_cast<T>(_buffer);
	}

	T get()
	{
		return (obj_ptr);
	}

	typedef T type;
	T obj_ptr;
};





struct cnull
{};


enum type_tags{NONE,tt_WIN32=1,tt_WIN64=2};


struct	TotalParent
{
	virtual void set(PIMAGE_NT_HEADERS32* pimg_) {};
	virtual void set(PIMAGE_NT_HEADERS64* pimg_) {};

};



template <int type_id>
struct	getTypeById
	:public single_traits<cnull>
{
};

//PIMAGE_NT_HEADERS32, PIMAGE_NT_HEADERS64
template <>
struct	getTypeById<type_tags::tt_WIN32>
	:public single_traits< PIMAGE_NT_HEADERS32 > ,TotalParent
{	getTypeById(char *_buffer):single_traits<PIMAGE_NT_HEADERS32>(_buffer){}
	void set(PIMAGE_NT_HEADERS32* pimg_)
	{
		(*pimg_) = single_traits<PIMAGE_NT_HEADERS32>::get();
	}
  typedef IMAGE_NT_HEADERS32  NT_HEADER;
  typedef PIMAGE_NT_HEADERS32 PNT_HEADER;
};
//

//	
template <>
struct	getTypeById<type_tags::tt_WIN64>
	:public single_traits< PIMAGE_NT_HEADERS64 >, TotalParent
{	getTypeById(char *_buffer):single_traits<PIMAGE_NT_HEADERS64>(_buffer){}	
	
	void set(PIMAGE_NT_HEADERS64* pimg_)
	{	
		(*pimg_) = single_traits<PIMAGE_NT_HEADERS64>::get();
	}
  typedef IMAGE_NT_HEADERS64  NT_HEADER;
  typedef PIMAGE_NT_HEADERS64 PNT_HEADER;
};
//
//	



struct GetProcessBasicInfo
{
  _Process _process;
	
  LONG 
	ImageSize(std::string Name)
	{
		HANDLE hF = CreateFile(Name.c_str(), GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD Error = GetLastError();
		if (0 != Error)
		{
			return (0);
		}
		LARGE_INTEGER filesize;
    filesize.QuadPart = 0;
    filesize.HighPart = 0;
		bool result = GetFileSizeEx(hF, &filesize);
		CloseHandle(hF);
    if (!result)
    {
      return (0);
    }
		//ltoa(filesize.QuadPart, str, 10);
		//MessageBox(0, str, "11", 0);
		return (filesize.QuadPart);
	}

 // LONG ImageSize_(std::string Name)

	//HANDLE proc_Handle;
	GetProcessBasicInfo(const _Process& process)
	{
    _process = process;
		HINSTANCE hinst_ntdll = GetModuleHandle("ntdll.dll");
		if (NULL == hinst_ntdll)
		{
				hinst_ntdll = LoadLibrary("ntdll.dll");
		}
    
		NtQueryInformationProcess = reinterpret_cast<PNTQUERYINFORMATIONPROCESS>(GetProcAddress(hinst_ntdll, "NtWow64QueryInformationProcess64"));
		//PROCESS_BASIC_INFORMATION  ProcessInfo;
		PROCESS_BASIC_INFORMATION_WOW64 ProcessInfo;
		if (hinst_ntdll)
		{
      PEB64 _peb;
	  P64 pp;
			ULONG RLength = 0;
			ULONG64 Readed;
      InitAll();
			memoriser<_handle> mmp(PROCESS_ALL_ACCESS, process.Uid);
			NTSTATUS NtStatus = NtQueryInformationProcess(mmp.get(),
				ProcessBasicInformation,
				(PVOID)&ProcessInfo,
				sizeof(ProcessInfo),
				&RLength);
			if (STATUS_SUCCESS == NtStatus)
			{
				mmp.read<PVOID64, ULONG64, PEB64>(ProcessInfo.PebBaseAddress, sizeof(PEB64), _peb);
				mmp.read<PVOID64, ULONG64, P64>(_peb.ProcessParameters, sizeof(P64), pp);
				if ((pp.ImagePathName._Buffer == NULL)||(pp.ImagePathName.Length == 0))
					return;
				wchar_t ImageName[MAX_PATH];
				universal_type<wchar_t> ut;

				size_t	readed = static_cast<size_t>(mmp.read<PVOID64, ULONG64, wchar_t>(pp.ImagePathName._Buffer, pp.ImagePathName.Length, ut ));
        std::vector<char> rpc_img_char;
        rpc_img_char.resize(pp.ImagePathName.Length / sizeof(wchar_t));
          
          memset(&rpc_img_char[0], 0, rpc_img_char.size());
					wcstombs(&rpc_img_char[0], ut.get(), rpc_img_char.size());
          ULONG64 size_ = 0;
                   
          _process.image_path = helpers::to_str(rpc_img_char);
          size_ = ImageSize(_process.image_path);
          if (0 == size_)
					{
           // __asm {int 3}
            _CURDIR cd = pp.CurrentDirectory;
            universal_type<wchar_t> ut_x;
            size_t readed = static_cast<size_t>(mmp.read<PVOID64, ULONG64, wchar_t>(cd.DosPath._Buffer, cd.DosPath.Length, ut_x));
            std::vector<char> dir;
            dir.resize(cd.DosPath.Length / sizeof(wchar_t));
            memset(&dir[0], 0, dir.size());
            wcstombs(&dir[0], ut_x.get(), dir.size());
            std::string str_dir = helpers::to_str(dir);

            copy_to<mem_mapper> _copy(_process.image_path);
         //   BOOL result = CopyFile(_process.image_path.c_str(), "d:\\tmp\\test.bin", true);
         //   if (!result)
         //   {
         //     DWORD err = GetLastError();
         //     printf("copy error code = %d\n", err);
         //   }
            return;
					}
          std::string rpc_img_str = helpers::to_str(rpc_img_char);
					printf("%s:%d \n", rpc_img_str.c_str() , size_);
          md5_generator  md5_on_disk(rpc_img_str, size_);
          _process.hash_in_disk = md5_on_disk.get();
          if (_process.hash_in_disk.empty())
          {
           //TODO
           
            
          } 
         // char rpc_cmd_char[MAX_PATH];
//          std::vector<char> rpc_cmd_char;t_ptr = 0x00fa0a90 L"\"C:\\Program Files\\Common Files\\McAfee\\Platform\\mcuicnt.exe\" /platui -Embedding"
          	readed = static_cast<size_t>(mmp.read<PVOID64, ULONG64, wchar_t>(pp.CommandLine._Buffer, pp.CommandLine.Length /* sizeof(wchar_t)*/, ut));
            std::vector<char> rpc_cmd_char;
			if (ut.count() > 800)
			{
				__asm {nop}
			}

            rpc_cmd_char.resize(ut.count()/sizeof(wchar_t));
            memset(&rpc_cmd_char[0], 0, rpc_cmd_char.size());
            wcstombs(&rpc_cmd_char[0], ut.get(), rpc_cmd_char.size());
			if (ut.count() > 40)
			{
				std::vector<char>::iterator first = std::remove(rpc_cmd_char.begin(), rpc_cmd_char.end(), 0);
				rpc_cmd_char.erase(first, rpc_cmd_char.end());
				_process.commandline = helpers::to_str(rpc_cmd_char);
			}
          
          universal_type<char> ut_c;
          printf("size = %d\n", size_);
					readed = static_cast<size_t>(mmp.read<PVOID64, ULONG64, char>(_peb.ImageBaseAddress, size_, ut_c));
					if (0 == readed)
					{
						return;
					}
					PBYTE ImageBase = reinterpret_cast<PBYTE>( ut_c.get() );
          //result_ = "dec1771867fd50a0a8d113741a96816b"{result_="4d099c9a1818af9fc9628efff8b3a137" ProcessBuffer=0xcccccccc <Ошибка при чтении символов строки.> }
					PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase);
					printf("dos_magic  = %x\n", dos_header->e_magic);
					TotalParent *ptr = NULL;
					PIMAGE_NT_HEADERS64 image_nt = NULL;
          std::pair<PIMAGE_NT_HEADERS32, PIMAGE_NT_HEADERS64> header_pair;
          header_pair.first = NULL;
          header_pair.second = NULL;
          /*if (mmp.IsWin32())
					{
						ptr = new getTypeById<type_tags::tt_WIN32>(ImageBase + dos_header->e_lfanew);
						ptr->set(&header_pair.first);
						__asm {nop}
					}
					else
					{
						ptr = new getTypeById<type_tags::tt_WIN64>(ImageBase + dos_header->e_lfanew);
						ptr->set(&header_pair.second);
					}*/
       //   getTypeById<type_tags::tt_WIN32>::PNT_HEADER hdr;
       //   IMAGE_DATA_DIRECTORY pimport = hdr->;
          get_pe_ptr pe_(ImageBase);
          GetPeObject<getTypeById<type_tags::tt_WIN32>::PNT_HEADER> obj(pe_.get());
          obj.get_section(".idata");
          obj.import();
        //  PIMAGE_SECTION_HEADER sections = NULL;
        //  size_t sections_count = 0;
 
        /*    if (header_pair.first)
            {
              printf("machine = %x\n", header_pair.first->FileHeader.Machine);
              printf("Pe_sign = %x\n", header_pair.first->Signature);             
              printf("Entry = %X\n", header_pair.first->OptionalHeader.AddressOfEntryPoint);
              printf("BaseOfCode = %X\n", header_pair.first->OptionalHeader.BaseOfCode);
              unsigned int sz = header_pair.first->OptionalHeader.SizeOfCode;
              md5_on_disk.set_ptr(header_pair.first->OptionalHeader.BaseOfCode, sz);
              std::string on_disk = md5_on_disk.get();
              printf("size_of_code = %X\n", sz);
             // md5_generator  md(reinterpret_cast<BYTE *>(ImageBase) + header_pair.first->OptionalHeader.BaseOfCode, sz);
             // process.hash_in_ram = md.get();
             // printf("md5 = %s\n", md.get().c_str());
              printf("Pe_magic = %x\n", header_pair.first->OptionalHeader.Magic);
              printf("sizeof(IMAGE_NT_HEADERS32) = %x\n", sizeof(IMAGE_NT_HEADERS32));
              printf("sizeof(image_nt->OptionalHeader) = %d\n", sizeof(header_pair.first->OptionalHeader));
              sections = reinterpret_cast<PIMAGE_SECTION_HEADER>(ImageBase + dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS32));
              sections_count = header_pair.first->FileHeader.NumberOfSections;
              printf("sections =%d \n", header_pair.first->FileHeader.NumberOfSections);
            //  if ((md.detect("6c685a0afecfb85f9189772f0a6c683f"))&&(0x2BD72F == header_pair.first->OptionalHeader.AddressOfEntryPoint))
             // {
              //  printf("detected!\n");
               // getc(stdin);
              //}

            }
            if (header_pair.second)
            {
              printf("machine = %x\n", header_pair.second->FileHeader.Machine);
              printf("Pe_sign = %x\n", header_pair.second->Signature);
              printf("Pe_magic = %x\n", header_pair.second->OptionalHeader.Magic);
              printf("Entry = %X\n", header_pair.second->OptionalHeader.AddressOfEntryPoint);
              printf("ImageBase = %x\n", header_pair.second->OptionalHeader.ImageBase);
              printf("BaseOfCode = %X\n", header_pair.second->OptionalHeader.BaseOfCode);
              unsigned int sz = header_pair.second->OptionalHeader.SizeOfCode;
              printf("size_of_code = %X\n", sz);
            //  md5_on_disk.set_ptr(header_pair.second->OptionalHeader.BaseOfCode,sz);
            //  std::string on_disk = md5_on_disk.get();
            //  md5_generator  md(reinterpret_cast<BYTE *>(ImageBase)+ header_pair.second->OptionalHeader.BaseOfCode, sz);
            //  printf("disk_md5 = %s\n", on_disk.c_str());
            //  printf("mem_md5 = %s\n", md.get().c_str());
              printf("sizeof(IMAGE_NT_HEADERS64) = %x\n", sizeof(IMAGE_NT_HEADERS64));
              printf("sizeof(image_nt->OptionalHeader) = %d\n", sizeof(header_pair.second->OptionalHeader));
              sections = reinterpret_cast<PIMAGE_SECTION_HEADER>(ImageBase + dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS64));
                 printf("sections =%d \n", header_pair.second->FileHeader.NumberOfSections);
              sections_count = header_pair.second->FileHeader.NumberOfSections;
             // if (md.detect("6c685a0afecfb85f9189772f0a6c683f"))
             // {
             //   printf("detected!\n");
             //   getc(stdin);
             // }

            }*/

         //   for (size_t count = 0; count < sections_count; ++count)
         //   {
         //     printf("section:%s\n", sections->Name);
         //     ++sections;
         //   }
         //   delete ptr;
			}
		}
	}
  _Process 
   get(void)
  {
	  if (_process.commandline.length()>200)
	  {
		  __asm {nop}
		  printf("command = %s\n", _process.commandline.c_str());
	  }
    return (_process);
  }
};

struct GetProcessSeq
{
  typedef ::std::vector<_Process> PList;
  PList PrcLst;
  ULONG TotalOffset;
  ::std::vector<char> Buffer_;
  DWORD Buffer_Size;
  DWORD NeedBufferSize;
  NTSTATUS status;
	GetProcessSeq()
	{
	  reinit();
	}


	void 
	get(PList &processes)
	{
		processes.clear();
		EmptyFunctor< _Process > fn;
		::std::transform(PrcLst.begin(), PrcLst.end(), std::back_inserter(processes), fn);
	}

	void reinit()
	{
		Buffer_Size = 224096;
		NeedBufferSize = 0;
		TotalOffset = 0;
		char *pBuffer = NULL;
		Buffer_.resize(Buffer_Size + 100);
		HMODULE hmod = LoadLibrary("ntdll.dll");
		NtQuerySystemInformation = reinterpret_cast<PNTQUERYSYSTEMINFORMATION>(GetProcAddress(hmod, "NtQuerySystemInformation"));
		pBuffer = &Buffer_[0];
		status = NtQuerySystemInformation(SystemProcessesInformation, pBuffer, Buffer_Size, &NeedBufferSize);
		if (status == STATUS_INFO_LENGTH_MISMATCH)
		{
			Buffer_Size = (Buffer_Size * 2);
			Buffer_.resize(Buffer_Size);
			pBuffer = &Buffer_[0];
			NeedBufferSize = 0;
			status = NtQuerySystemInformation(SystemProcessesInformation, pBuffer, Buffer_Size, &NeedBufferSize);
			PSYSTEM_PROCESS_INFORMATION _p_info = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
			printf("Buffer length mismatch...\r\n");
			printf("need length = %d\n", NeedBufferSize);
		}

		if (STATUS_SUCCESS == status)
		{
			PSYSTEM_PROCESS_INFORMATION _p_info = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
			while (_p_info)
			{

				char tmp[MAX_PATH];
				memset(tmp, 0, MAX_PATH);
				WideCharToMultiByte(CP_ACP, 0, _p_info->ProcessName.Buffer,
					_p_info->ProcessName.Length, tmp, sizeof(tmp), NULL, NULL);
        _Process prc;

				prc.Uid = _p_info->ProcessId;
				prc.ProcessName = tmp;

				if (0 == prc.Uid)
				{
					prc.ProcessName = "idle";
				}
				memset(tmp, 0, MAX_PATH);
				sprintf_s(tmp , "%d", prc.Uid);
				std::string _tmp_str = std::string(tmp) + prc.ProcessName;				
			  for (size_t i = 0; i < _p_info->ThreadCount; ++i)
					printf("kernel_time = %d\n", _p_info->Threads[i].KernelTime.u.LowPart);

				for (size_t i = 0; i < _p_info->ThreadCount; ++i)
				{
        /*  typedef struct _VM_COUNTERS {
            SIZE_T PeakVirtualSize;
            SIZE_T VirtualSize;
            ULONG PageFaultCount;
            SIZE_T PeakWorkingSetSize;
            SIZE_T WorkingSetSize;
            SIZE_T QuotaPeakPagedPoolUsage;
            SIZE_T QuotaPagedPoolUsage;
            SIZE_T QuotaPeakNonPagedPoolUsage;
            SIZE_T QuotaNonPagedPoolUsage;
            SIZE_T PagefileUsage;
            SIZE_T PeakPagefileUsage;
          } VM_COUNTERS;*/
         // printf("PeakWorkingSet = %x\n", _p_info->VmCounters.PeakWorkingSetSize);
          //prc.counter_obj.PageFaultCount_p_info->VmCounters.PageFaultCount;
          Counters counters_obj(_p_info->VmCounters.PeakVirtualSize,
            _p_info->VmCounters.VirtualSize,
            _p_info->VmCounters.PageFaultCount,
            _p_info->VmCounters.PeakWorkingSetSize,
            _p_info->VmCounters.WorkingSetSize,
            _p_info->VmCounters.QuotaPeakPagedPoolUsage,
            _p_info->VmCounters.QuotaPagedPoolUsage,
            _p_info->VmCounters.QuotaPeakNonPagedPoolUsage,
            _p_info->VmCounters.QuotaNonPagedPoolUsage,
            _p_info->VmCounters.PagefileUsage,
            _p_info->VmCounters.PeakPagefileUsage
          );
          prc.counter_obj = counters_obj;
					prc.Thread_Seq.push_back(_Thread(_p_info->Threads[i].ClientId.UniqueThread,
						                             _p_info->Threads[i].KernelTime, 
						                             _p_info->Threads[i].UserTime, 
						                             _p_info->Threads[i].CreateTime,
						                             _p_info->Threads[i].State,
						                             _p_info->Threads[i].BasePriority, 
						                             _p_info->Threads[i].Priority
						                             ));
				} 
				PrcLst.push_back(prc);
				TotalOffset += _p_info->NextEntryDelta;
				pBuffer = &Buffer_[TotalOffset];
				_p_info = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(pBuffer);
				if (!_p_info->NextEntryDelta)
					break;
			}
		}
    __asm {nop}
	}



};
/*
class Tokenizer
{
	HANDLE hDebug;
	bool enabled;
public:
	Tokenizer():hDebug(NULL),
		enabled(false)
	{
		create();
	}

	bool 
	create()
	{
	  HANDLE hProcessToken = NULL;
	  BOOL  res = OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE, &hProcessToken);

	  if (!res)
	  {
		  return (false);
	  }

	  HANDLE hToken = NULL;
	  res = DuplicateTokenEx(
		  hProcessToken,
		  TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		  NULL,
		  SecurityImpersonation,
		  TokenImpersonation,
		  &hToken);

	  if (!res)
	  {
		CloseHandle(hProcessToken);
	    return false;
	  }

	  TOKEN_PRIVILEGES tp;
	  tp.PrivilegeCount = 1;
	  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	  res = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

	  if (!res)
	  {
		  CloseHandle(hToken);
		  CloseHandle(hProcessToken);
		  return (false);
	  }

	  res = AdjustTokenPrivileges(
		  hToken,
		  FALSE,
		  &tp,
		  sizeof(tp),
		  NULL,
		  NULL);

	  if (!res)
	  {
		  CloseHandle(hToken);
		  CloseHandle(hProcessToken);
		  return false;
	  }

	  hDebug = hToken;
	  return (true);
	}

	bool 
	enable()
	{
	  HANDLE SavedToken = GetCurrentThreadToken();
	  BOOL res = SetThreadToken(NULL, hDebug);
	  if (!res)
	  {
		return (res);
	  }
	  enabled = true;
	  return (enabled);
	}

	

};*/

#endif