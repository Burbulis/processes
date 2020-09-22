#include <Windows.h>
//http://blog.rewolf.pl/blog/?p=294
//https://ntopcode.wordpress.com/2018/02/26/anatomy-of-the-process-environment-block-peb-windows-internals/


template <class T>
struct UNICODE_STRING_T
{

	USHORT Length;
	USHORT MaximumLength;
	T _Buffer;
};

typedef UNICODE_STRING_T< PVOID64 > UNICODE_STRING_WOW64;
typedef UNICODE_STRING_T< PVOID64 > UNICODE_STRING_64;


struct _CURDIR
{
	UNICODE_STRING_WOW64 DosPath;
	PVOID64 Handle;
};

struct PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	PVOID64 ConsoleHandle;
	ULONG ConsoleFlags; //: Uint4B
	DWORD64 StandardInput; //: Ptr64 Void
	DWORD64 StandardOutput; //: Ptr64 Void
	DWORD64 StandardError; //: Ptr64 Void
	_CURDIR CurrentDirectory; //: _CURDIR
	UNICODE_STRING_WOW64 DllPath;//: _UNICODE_STRING
	UNICODE_STRING_WOW64 ImagePathName; //: _UNICODE_STRING
	UNICODE_STRING_WOW64 CommandLine;//: _UNICODE_STRING
	PVOID64 Environment;//: Ptr64 Void
	UINT StartingX; //: Uint4B
	UINT StartingY; //: Uint4B
	UINT CountX;//: Uint4B
	UINT CountY;//: Uint4B
	UINT CountCharsX; //: Uint4B
	UINT CountCharsY; //: Uint4B
	UINT FillAttribute; //: Uint4B
	UINT WindowFlags; //: Uint4B
	UINT ShowWindowFlags; //: Uint4B
	UNICODE_STRING_WOW64	WindowTitle; //: _UNICODE_STRING
	UNICODE_STRING_WOW64	DesktopInfo; //: _UNICODE_STRING
	UNICODE_STRING_WOW64	ShellInfo;  //_UNICODE_STRING
	UNICODE_STRING_WOW64	RuntimeData; //: _UNICODE_STRING
										 //	_RTL_DRIVE_LETTER_CURDIR64 CurrentDirectores; //: [32] _RTL_DRIVE_LETTER_CURDIR
										 //	ULONG EnvironmentSize; //: Uint8B
										 //	ULONG EnvironmentVersion; //: Uint8B
										 //	PVOID64 PackageDependencyData; //: Ptr64 Void
										 //	UINT ProcessGroupId; //: Uint4B
										 //	UINT LoaderThreads; //: Uint4B
};

typedef PROCESS_PARAMETERS P64;
typedef P64* PP64;

#pragma pack(push)
#pragma pack(1)

template <class T>
struct LIST_ENTRY_T
{
	T Flink;
	T Blink;
};






template
<
	typename T
>
struct _RTL_DRIVE_LETTER_CURDIR
{
	UINT16	 Flags;//            : Uint2B
	UINT16 Length;// : Uint2B
	UINT TimeStamp;
	T _STRING;
};



typedef _RTL_DRIVE_LETTER_CURDIR< UNICODE_STRING_64 > _RTL_DRIVE_LETTER_CURDIR64;

template <class T, class NGF, int A>
struct _PEB_T
{
	union
	{
		struct
		{
			BYTE InheritedAddressSpace;
			BYTE ReadImageFileExecOptions;
			BYTE BeingDebugged;
			BYTE BitField;
		};
		T dummy01;
	};
	T Mutant;
	T ImageBaseAddress;
	T Ldr;
	T ProcessParameters;
	T SubSystemData;
	T ProcessHeap;
	T FastPebLock;
	T AtlThunkSListPtr;
	T IFEOKey;
	T CrossProcessFlags;
	T UserSharedInfoPtr;
	DWORD SystemReserved;
	DWORD AtlThunkSListPtr32;
	T ApiSetMap;
	T TlsExpansionCounter;
	T TlsBitmap;
	DWORD TlsBitmapBits[2];
	T ReadOnlySharedMemoryBase;
	T HotpatchInformation;
	T ReadOnlyStaticServerData;
	T AnsiCodePageData;
	T OemCodePageData;
	T UnicodeCaseTableData;
	DWORD NumberOfProcessors;
	union
	{
		DWORD NtGlobalFlag;
		NGF dummy02;
	};
	LARGE_INTEGER CriticalSectionTimeout;
	T HeapSegmentReserve;
	T HeapSegmentCommit;
	T HeapDeCommitTotalFreeThreshold;
	T HeapDeCommitFreeBlockThreshold;
	DWORD NumberOfHeaps;
	DWORD MaximumNumberOfHeaps;
	T ProcessHeaps;
	T GdiSharedHandleTable;
	T ProcessStarterHelper;
	T GdiDCAttributeList;
	T LoaderLock;
	DWORD OSMajorVersion;
	DWORD OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	DWORD OSPlatformId;
	DWORD ImageSubsystem;
	DWORD ImageSubsystemMajorVersion;
	T ImageSubsystemMinorVersion;
	T ActiveProcessAffinityMask;
	T GdiHandleBuffer[A];
	T PostProcessInitRoutine;
	T TlsExpansionBitmap;
	DWORD TlsExpansionBitmapBits[32];
	T SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	T pShimData;
	T AppCompatInfo;
	UNICODE_STRING_T<T> CSDVersion;
	T ActivationContextData;
	T ProcessAssemblyStorageMap;
	T SystemDefaultActivationContextData;
	T SystemAssemblyStorageMap;
	T MinimumStackCommit;
	T FlsCallback;
	LIST_ENTRY_T<T> FlsListHead;
	T FlsBitmap;
	DWORD FlsBitmapBits[4];
	T FlsHighIndex;
	T WerRegistrationData;
	T WerShipAssertPtr;
	T pContextData;
	T pImageHeaderHash;
	T TracingFlags;
};

//0x000

typedef _PEB_T<DWORD, DWORD64, 34> PEB32;
typedef _PEB_T<PVOID64, DWORD, 30> PEB64;
typedef PEB32* PPEB32;
typedef PEB64* PPEB64;
//typedef PROCESS_PARAMETERS<DWORD> P32;


#pragma pack(pop)