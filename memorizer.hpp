#pragma once
#include <windows.h> 
#include <Psapi.h>
#include <vector>
#include "md5.h"

typedef LONG(__stdcall *PNTWOW64READVIRTUALMEMORY64)(
  IN HANDLE ProcessHandle,
  IN PVOID64 BaseAddress,
  OUT PVOID Buffer,
  IN ULONG64 Size,
  OUT PULONG64 NumberOfBytesRead);

static PNTWOW64READVIRTUALMEMORY64 NtWow64ReadVirtualMemory64 = NULL;

template
<
  typename type_t
>
struct universal_type
{
  universal_type()
  {
    type_t_buffer_sz = 0;
    t_ptr = NULL;
  }

  ~universal_type()
  {
    delete t_ptr;
  }


  void
    set(std::vector<char>& _buffer)
  {
    type_t_buffer_sz = (_buffer.size() / sizeof(type_t));
    t_ptr = new type_t[type_t_buffer_sz];
    RtlCopyMemory(
      t_ptr, reinterpret_cast<void *>(&_buffer[0]), _buffer.size()
    );
  }

  size_t
    count(void)
  {
    return (type_t_buffer_sz);
  }

  type_t*
    get(void)
  {
    return(t_ptr);
  }

  typedef type_t* type_t_ptr;
  size_t  type_t_buffer_sz;
  std::vector<char> buffer;
  type_t_ptr t_ptr;
};

struct md5_generator
{
  md5_generator(std::string ProcessName,size_t sz)
  {
    MD5 md;
    HANDLE hFile = CreateFile(ProcessName.c_str(), GENERIC_READ,
      /*FILE_SHARE_WRITE |*/ FILE_SHARE_READ, NULL,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    CloseHandle(hFile);
    ProcessBuffer = reinterpret_cast<BYTE *>(MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0));
    md.update(ProcessBuffer,sz);
    md.finalize();
    result_ = md.hexdigest();
    CloseHandle(hMapFile);
  }

  void
  set_ptr(size_t ptr, unsigned int length_)
  {
    MD5 md;
    md.update(ProcessBuffer + ptr, length_);
    md.finalize();
  //  result_ = md.hexdigest();//result_ "fa6c268a5b5bda067a901764d203d433"
  //  std::vector<BYTE> Buffer;
  //  Buffer.resize(length_);
  //  memcpy(&Buffer[0], ProcessBuffer + ptr, length_);
  //  FILE *f = fopen("d:\\arc\\saved0.bin", "wb+");
  //  fwrite(&Buffer[0], 1, length_, f);
  //  fclose(f);

  }

  md5_generator(BYTE *ProcessBuffer_, unsigned int length_)
  {
    MD5 md;
    md.update(ProcessBuffer_, length_);
    md.finalize();
    std::vector<BYTE> Buffer;
    Buffer.resize(length_);
    memcpy(&Buffer[0], ProcessBuffer_ , length_);
    FILE *f = fopen("d:\\arc\\saved1.bin", "wb+");
    fwrite(&Buffer[0], 1, length_, f);
    fclose(f);
    result_ = md.hexdigest();
  }

  bool
  detect(std::string md5_str)
  {
    return (result_.find(md5_str, 0) != std::string::npos);
  }


  std::string
    get(void)
  {
    return (result_);
  }
private:
  std::string result_;
  BYTE *ProcessBuffer;
};


template
<
  typename t_handle_ = _handle
>
struct memoriser
  :public t_handle_
{
  memoriser(DWORD _access, DWORD _pid)
    :handle_x(_access, _pid)
  {
    InitAll();
  }

  /*

  */
  template
    <
    typename t_ptr,
    typename t_size,
    typename t_out
    >
    t_size
    read(t_ptr ptr_, t_size sz, t_out& ptr_out_)
  {
    //t_out out_;
    HANDLE test_ = get();
    t_size Readed = 0;
    Buffer.resize(sz);
    RtlZeroMemory(&Buffer[0], Buffer.size());
    if (!IsWin32Mode())
    {
     
      NtWow64ReadVirtualMemory64(test_, ptr_, &Buffer[0], sz, &Readed);
    }
    else
    {
      SIZE_T _readed = 0;
      ::ReadProcessMemory(get(), ptr_, &Buffer[0], sz, &_readed);
      Readed = _readed;
    }
    RtlCopyMemory(reinterpret_cast<void*>(&ptr_out_), &Buffer[0], Buffer.size());

    return (Readed);
  }

  
  template
    <
    typename t_ptr,
    typename t_size,
    typename t_out
    >
    t_size
    read(t_ptr ptr_, t_size sz, universal_type<t_out>& ptr_out_)
  {
    t_size Readed = 0;
  //  if (sz == 7008762595795752284)
  //  {
  //    _asm {nop}
  //    return;
  //  }
    Buffer.resize(sz * sizeof(t_out));
    RtlZeroMemory(&Buffer[0], Buffer.size());
    if (!IsWin32Mode())
    {
      NtWow64ReadVirtualMemory64(get(), ptr_, &Buffer[0], sz, &Readed);
    }
    else
    {
      SIZE_T _readed = 0;
      ::ReadProcessMemory(get(), ptr_, &Buffer[0], sz, &_readed);
      Readed = _readed;
    }
    ptr_out_.set(Buffer);
    return (Readed);
  }

  BOOL IsWin32Mode(void)
  {
    return (t_handle_::IsWin32Mode());
  }

  BOOL IsWin32()
  {
    return (t_handle_::isWin32());
  }


  HANDLE get()
  {
    return (t_handle_::get());
  }

  t_handle_ handle_x;
private:

  std::vector<char> Buffer;

};

class mem_mapper
{
  HANDLE hMap;
  BYTE *Buffer;
public:
  mem_mapper(HANDLE hF)
  {
    hMap = CreateFileMapping(hF, NULL, PAGE_READONLY, 0, 0, NULL);
    Buffer = reinterpret_cast<BYTE *>(MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0));
  }

  BYTE *Get(void)
  {
    return (Buffer);
  }

};

template
<
  typename Generator
>
class copy_to
{
  HANDLE hF;
  std::auto_ptr<Generator> _generator;

public:
  copy_to(std::string ProcessName)
  {
    //GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    //hF = CreateFile(/*Name.c_str()*/"c:\\windows\\system32\\sihost.exe", GENERIC_READ,
    //  FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
    //  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
 
   // HANDLE hFile = CreateFile(ProcessName.c_str(), GENERIC_READ,
   //   FILE_SHARE_WRITE | FILE_SHARE_READ, NULL,
   //   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    FILE *f = fopen(ProcessName.c_str(), "r");
    /*
    if (INVALID_HANDLE_VALUE == hFile)
    {
      DWORD error_ = GetLastError();
      printf("ERROR OPEN FILE %s ERROR_CODE=%x", ProcessName.c_str(), error_);
      //    getc(stdin);
      return;

    }
     _generator = std::auto_ptr<Generator>(new Generator(hF));*/
    __asm {nop}
  }

};


  
//static void InitAll(void);
static
void InitAll(void)
{
  if (!NtWow64ReadVirtualMemory64)
    NtWow64ReadVirtualMemory64 = reinterpret_cast<PNTWOW64READVIRTUALMEMORY64>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64ReadVirtualMemory64"));
  }







