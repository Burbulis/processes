#ifndef _SQL_TYPE_HELPERS_
#define _SQL_TYPE_HELPERS_
#include <windows.h>
#include <string>
#include <map>
#include <functional>
#include <typeinfo>
#include <sql.h>
#include <sqlext.h>
#include "deffer_excpt.h"
#include "typeinfo_wrapper.h"
namespace sql_base
{

// Получение указателя на неизвестный тип
//
  template
  <
    typename T
  >
  struct TypeToPtr
  {
    enum{IS_POINTER = false};
    typedef T  get_type;
    typedef T* get_type_ptr;
    inline
    static
    const
    get_type& 
    ref(const get_type& t_)
    {
      return ( t_ );
    }
    inline
    static
    get_type_ptr 
    ptr(const T& t_ptr_)
    {
      return ( &t_ptr_ );
    }
  };

  template
  <
    typename T
  >
  struct TypeToPtr<T*>
  {
    enum{IS_POINTER = true};
    typedef T  get_type;
    typedef T* get_type_ptr;
    static
    inline
    const get_type& 
    ref(get_type_ptr t_ptr_)
    {
      return ( (*t_ptr_) );
    }
    inline
    get_type_ptr 
    ptr(get_type_ptr t_ptr_)
    {
      return (t_ptr_);
    }
  };
//
// для будующей реализации обёрток для типов.
//
  template 
    <
      typename type_ , 
      size_t t_delimiter
    >
  struct type_wrapper
  {
    type_wrapper():sql_type_(NULL){};
    typedef type_ get_type; 
    type_ sql_type_;
    enum{ MINOR_TYPE_NUM = t_delimiter };
  };
//
// Wrappers (обёртки для типов BINARY_SQL,LONGVARCHAR_SQL и т.д.)
//
  typedef 
    type_wrapper<SQLCHAR * , 0> 
  SQLCHAR_SQL_TYPE_WRAPPER;

  typedef 
    type_wrapper<char * , 0> 
  STANDARDCHAR_SQL_TYPE_WRAPPER;

  typedef 
    type_wrapper<SQLCHAR * , 1> 
  LONGVARCHAR_SQL_TYPE_WRAPPER;


//
// MAIN WRAPPER TEMPLATE
//
  template <
    typename T
  >
  struct is_wrapper 
  { 
    typedef T* get_type; 
    T *operator() (T *junk_prm)
    {
      return ( junk_prm );
    }
  };
  //SPECIALISACIONS OF IS_WRAPPER TEMPLATE
  template<
  >
  struct is_wrapper<SQLCHAR_SQL_TYPE_WRAPPER> 
  {
    typedef SQLCHAR_SQL_TYPE_WRAPPER::get_type get_type; 
    get_type
    operator()(SQLCHAR_SQL_TYPE_WRAPPER *junk_prm)
    {
      return ( junk_prm->sql_type_ );
    }
  };
 
  template<
  >
  struct is_wrapper<LONGVARCHAR_SQL_TYPE_WRAPPER> 
  {
    typedef LONGVARCHAR_SQL_TYPE_WRAPPER::get_type get_type; 
    get_type
    operator()(LONGVARCHAR_SQL_TYPE_WRAPPER *junk_prm)
    {
      return ( junk_prm->sql_type_ );
    }
  };

 template<
  >
  struct is_wrapper<STANDARDCHAR_SQL_TYPE_WRAPPER> 
  {
    typedef STANDARDCHAR_SQL_TYPE_WRAPPER::get_type get_type; 
    get_type
    operator()(STANDARDCHAR_SQL_TYPE_WRAPPER *junk_prm)
    {
      return ( junk_prm->sql_type_ );
    }
  };


  //  Хранилище соответствий тип - селектор типа
  //  
  //   template<
  //      typename T
  //    >
  //    void
  //    add_type_(SQLSMALLINT type_selector_)
  //
  //  Метод добавляет тип в хранилище типов, пример:
  //  test_type.add_type_<int>(0)
  //  тип int будет добавлен в хранилище под номером 0
  //  
  // template<
  //      typename T
  //    >
  //    SQLSMALLINT
  //    get_selector(void)
  // Метод возвращает селектор принадлежащий типу, пример:
  // printf("selector(int) = %d\n", 
  //  test_type.add_type_<int>get_selector())
  // данный код выведет на экран "0" селектор принадлежащий int
  // инициализированный примером ранее:-)

    struct test_type_
    {
//-----------------------------------------------------------------------------
     template<
        typename T
      >
      void
      add_c_sql_type_(SQLSMALLINT type_selector_c,
      SQLSMALLINT type_selector_sql, size_t size_)
      {
        T t = T();
        type_collector[ typeinfo_wrapper( typeid( T ) ) ]  = 
          type_pair( type_selector_c ,
              type_selector_sql,
              size_
             );
      }
//-----------------------------------------------------------------------------
     template<
        typename T
      >
      SQLSMALLINT
      get_c_selector(void)
      {
        T t = T();
        if ( !type_collector.count( typeid(T) ) )
          except< std::string >( typeid(T).name() , __LINE__ , __FILE__ , 
            std::string("get_c_selector")
          );
        return ( type_collector[ typeinfo_wrapper( typeid( T ) ) ].c_type_ );
      }
//-----------------------------------------------------------------------------
     template<
        typename T
      >
     /* SQLSMALLINT*/
     size_t
      get_c_type_sz(void)
      {
        T t = T();
        if ( !type_collector.count( typeid(T) ) )
          except< std::string >( typeid(T).name() , __LINE__ , __FILE__ , 
            std::string("get_c_type_sz")
          );
       // printf("typename _%s:%d\n" , typeid(T).name(),type_collector[ typeinfo_wrapper( typeid( T ) ) ].size_type_);
        return ( type_collector[ typeinfo_wrapper( typeid( T ) ) ].size_type_ );
      }
//-----------------------------------------------------------------------------
     template<
        typename T
      >
      SQLSMALLINT
      get_sql_selector(void)
      {
        T t = T();
        if ( !type_collector.count( typeid(T) ) )
          except< std::string >( typeid(T).name() , __LINE__ , __FILE__ , 
            std::string("get_sql_selector")
          );
        return ( type_collector[ typeinfo_wrapper( typeid( T ) ) ].sql_type_ );
      }
//-----------------------------------------------------------------------------
    private:
    
    struct type_pair
    {
      type_pair(){};
      type_pair(SQLSMALLINT left_type,
        SQLSMALLINT right_type,
        size_t size_)
        :c_type_(left_type),
         sql_type_(right_type),
         size_type_(size_)
      {};
      SQLSMALLINT c_type_;
      SQLSMALLINT sql_type_;
      size_t size_type_;
    };

    std::map< 
            typeinfo_wrapper , type_pair
    > type_collector;
  };
//-----------------------------------------------------------------------------
// Автоматическая инициализация глобального объекта test_type_ type_tst,
// заполнение хранилища типами и селекторами типов
  struct auto_init_struct
  {
//-----------------------------------------------------------------------------
    auto_init_struct()
    {
      enum{SQL_TYPE_TIMESTAMP_SIZE = 23};
      type_tst.add_c_sql_type_<SQLCHAR *>           
        ( SQL_C_CHAR   , SQL_CHAR , 0         );
      type_tst.add_c_sql_type_<SQLINTEGER>          
        ( SQL_C_SLONG  , SQL_INTEGER  , sizeof(SQLINTEGER)  );  
      type_tst.add_c_sql_type_<SQLUINTEGER>         
        ( SQL_C_ULONG  , SQL_INTEGER  , sizeof(SQLINTEGER)  );  
      type_tst.add_c_sql_type_<SQLCHAR>             
        ( SQL_C_BIT    , SQL_BIT , sizeof(SQLCHAR)          );
      type_tst.add_c_sql_type_<SQLUSMALLINT>        
        ( SQL_C_USHORT , SQL_SMALLINT , sizeof(SQLUSMALLINT));  
      type_tst.add_c_sql_type_<SQLSMALLINT>         
        ( SQL_C_SSHORT , SQL_INTEGER , sizeof(SQLSMALLINT)  ); 
      type_tst.add_c_sql_type_< SQLCHAR_SQL_TYPE_WRAPPER *>           
        ( SQL_C_BINARY , SQL_BINARY , 0          ); 
      type_tst.add_c_sql_type_< LONGVARCHAR_SQL_TYPE_WRAPPER *>           
        ( SQL_C_CHAR , SQL_LONGVARCHAR , 0       ); 
      type_tst.add_c_sql_type_< STANDARDCHAR_SQL_TYPE_WRAPPER * >           
        ( SQL_C_BINARY , SQL_BINARY , 0          ); 
      type_tst.add_c_sql_type_<SQL_DATE_STRUCT>     
        ( SQL_C_TYPE_DATE , SQL_TYPE_DATE , sizeof(SQL_DATE_STRUCT) );
      type_tst.add_c_sql_type_<SQL_TIME_STRUCT>     
        ( SQL_C_TYPE_TIME , SQL_TYPE_TIME , sizeof(SQL_TIME_STRUCT) );
      type_tst.add_c_sql_type_<SQL_TIMESTAMP_STRUCT>
        ( SQL_C_TYPE_TIMESTAMP ,  SQL_TYPE_TIMESTAMP , SQL_TYPE_TIMESTAMP_SIZE ); 
    }
//-----------------------------------------------------------------------------
   template<
        typename T
      >
   inline 
   size_t
   get_t_size_selector(void)
   {
     return (type_tst.get_c_type_sz<T>()
       );
   }
 //-----------------------------------------------------------------------------
   template<
        typename T
      >
   inline 
   SQLSMALLINT
   get_type_selector(void)
   {
     return (type_tst.get_c_selector<T>()
       );
   }
//-----------------------------------------------------------------------------
   template<
        typename T
      >
   inline 
   SQLSMALLINT
   get_sql_type_selector(void)
   {
     return (type_tst.get_sql_selector<T>()
       );
   }
//-----------------------------------------------------------------------------
  private:
    test_type_ type_tst;
  }; 
  extern auto_init_struct AUTO_INITIALIZE_TYPE_COLLECTOR_DO_NOT_REMOVE_THIS;
  //
  // Метод возвращает селектор по заданному типу.
  // Пример:
  // printf("%d\n",get_type_selector<int>());
  template <
    typename T
  >
  SQLSMALLINT
  get_type_selector( void )
  {
    T t = T();
    return (
      AUTO_INITIALIZE_TYPE_COLLECTOR_DO_NOT_REMOVE_THIS.get_type_selector<T>()
      );
  }
 // Метод возвращает SQLtype селектор по заданному типу.
 // Пример:
 // printf("%d\n",get_type_selector<int>());
  template <
    typename T
  >
  SQLSMALLINT
  get_sql_type_selector( void )
  {
    T t = T();
    return (
      AUTO_INITIALIZE_TYPE_COLLECTOR_DO_NOT_REMOVE_THIS.get_sql_type_selector<T>()
      );
  }
//
//
//
  template <
    typename T
  >
  size_t
  get_type_size_selector( void )
  {
    T t = T();
    return (
      AUTO_INITIALIZE_TYPE_COLLECTOR_DO_NOT_REMOVE_THIS.get_t_size_selector<T>()
      );
  }

  template
  <
    typename T
  >
  inline
  SQLSMALLINT
  current_type(T t)
  {
    SQLSMALLINT res_ =0;
    try
    {
      res_ = get_sql_type_selector<T>();
    }
    catch(...)
    {};
    return (res_);
  }

  

};
#endif