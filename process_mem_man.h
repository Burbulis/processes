#pragma once
#include <windows.h> 
#include <Psapi.h>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <typeinfo.h>
#include <mutex>
#include "auto_sync.h"
namespace mem
{

  struct  Universal_Type
  {
    Universal_Type() {};
    Universal_Type(PVOID _Ptr, size_t sz, size_t hash_code_) :Ptr(_Ptr), size_(sz),
      hash_code_type(hash_code_) {};
 
    
    PVOID get_ptr(void)
      const
    {
      return (Ptr);
    }

    size_t get_sz(void)
      const
    {
      return (size_);
    }

  private:
    PVOID Ptr;
    size_t hash_code_type;
    size_t size_;
  };



  struct _handle
  {
    _handle(DWORD _Access, DWORD Pid):_pid(Pid),_access(_Access)
    {  
      init();
    };

    void init()
    {
      if (NULL != handle)
      {
        return;
      }
      handle = ::OpenProcess(_access, true, _pid);
    }

    _handle()
    {};

    ~_handle()
    {
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
    HANDLE get()
    {
      return (handle);
    }
  private:
    DWORD _pid;
    DWORD _access;
    static HANDLE handle;
  };
  
  class process_mem_man
  {
  public:
    process_mem_man();
  protected:
    virtual ULONG _rd() =0;
    virtual ~process_mem_man() {};
  };


   struct protector:
     public process_mem_man, protected _handle
  {
    protector(const Universal_Type& Ut):_handle()
    {
      this->Ut = Ut;
      _process = get();
      _old_protect = 0;
      VirtualProtectEx(get() , Ut.get_ptr(), Ut.get_sz(), PAGE_EXECUTE_READ, &_old_protect);
    }


    template
      <
      typename T
      >
     ULONG
     read(T *_buffer)
    {
 
    //  AUTO_MUTEX(this);
      size_t readed = _rd();
      //buffer.resize(readed);
      RtlCopyMemory(_buffer, &buffer[0], readed);
      return (readed);
    }
    ~protector()
    {
    }

  private:
  ULONG _rd()
    {
    //  AUTO_MUTEX(this);
      ULONG Readed = 0;
      buffer.resize(Ut.get_sz());
      ReadProcessMemory(get(), Ut.get_ptr(), &buffer[0], Ut.get_sz(), &Readed);
      reset();
      return (Readed);
    }

  protected:
    void reset()
    {
      DWORD _protect = 0;
      VirtualProtectEx(get(), Ut.get_ptr() /*_Address*/, Ut.get_sz(), _old_protect, &_protect);
    }
    mem::Universal_Type Ut;
    //T *_Address;
    std::vector<char> buffer;
    HANDLE _process;
    DWORD _old_protect;
    size_t _sz;
  };

  template
    <
    typename T,
    typename storage = mem::protector//<T>
    >
  struct PreObjectList
  {
    //virtual storage* Create(T *Address) = 0;
    virtual storage* Create(const mem::Universal_Type& ut) = 0;
  };

 
  template
    <
    typename T,
    typename storage = mem::protector
    >
    struct ObjectList :public PreObjectList<T>
  {
    std::mutex obj_mutex;
    typedef storage* storage_ptr;
    //typedef mem::protector<T> storage;
    typedef mem::protector storage;

    ObjectList()
    {

    }

    ~ObjectList()
    {
      // AUTO_MUTEX(this);

      clear();
    }


    storage* Create(const mem::Universal_Type& ut)
    {
      std::lock_guard<std::mutex> lock(obj_mutex);
      storage* object = new storage(ut);
      storage_seq.push_back(object);
      return (object);
    }

    void
      clear()
    {
      std::lock_guard<std::mutex> lock(obj_mutex);
      std::list<storage *>::iterator first = storage_seq.begin();
      std::list<storage *>::iterator last = storage_seq.end();
      while (first != last)
      {
        storage *obj = (*first);
        delete obj;
        ++first;
      }
      storage_seq.clear();
    }
  private:
    std::list<protector *> storage_seq;
  };

  template
    <
      typename T
    >
  mem::Universal_Type
  CreatorUniversalType(T *Addr_src)
  {
    mem::Universal_Type ut(reinterpret_cast<PVOID>(Addr_src), sizeof(T), typeid(T).hash_code());
    return (ut);
  }

  template
    <
    typename T
    >
   mem::Universal_Type
  CreatorUniversalType(T *Addr_src,size_t sz_)
  {
    mem::Universal_Type ut(reinterpret_cast<PVOID>(Addr_src), sz_, typeid(T).hash_code());
    return (ut);
  }

/*
  template
  <
      typename T,
	  typename U,
      typename lister  = mem::ObjectList<U>,
      typename binder_ = mem::_handle
  >
  ULONG read(lister *list,T *Addr_src,U *Addr_dst)
  {
    lister::storage *object = list->Create(CreatorUniversalType(Addr_src));
    return (object->read(Addr_dst));
  }


    Overload for Buffered functions



  template
    <
    typename T,
    typename lister = mem::ObjectList<T>,
    typename binder_ = mem::_handle
    >
/*    ULONG read(lister *list, T *Addr_src, T *Addr_dst,size_t sz_)
  {
    lister::storage *object = list->Create(CreatorUniversalType(Addr_src,sz_));
    return (object->read(Addr_dst));
  }
  */
  template
  <
     typename T,
	 typename lister  = mem::ObjectList<mem::Universal_Type<T,UINT64>>,
     typename binder_ = mem::_handle
  >
  struct read_x64
  {
	typedef T t_;
	typedef lister lister_t;
	typedef binder_ binder_t;
	  
	size_t
	rx(lister *list,T Addr_src,T Addr_dst)
	{
			
	  object = list->Create(CreatorUniversalType(Addr_src));
	  readed = object->read( Addr_dst );
	  return (readed);
	}
	 private:
	typename  lister::storage *object;
	size_t readed;
  };
/*

    Overload for Buffered functions

*/
  template
  <
    typename T,
    typename lister  = mem::ObjectList<T>,
    typename binder_ = mem::_handle
  >
  struct read_x86
  {
	typedef T t_;
	typedef lister lister_t;
	typedef binder_ binder_t;
	
    size_t rx(lister *list, T Addr_src, T Addr_dst)
	{
			//lister::storage 
  	  size_t sz_ = sizeof(size_t);
  	  object = list->Create(CreatorUniversalType(Addr_src,sz_));
	  readed = object->read(Addr_dst);
	  return (0);
	}

		size_t size(void)
		{
			return(readed);
		}

	  private:
		size_t readed;
		typename lister::storage *object;
	};



}