#ifndef __SQL_BASE_SERVICE__
#define __SQL_BASE_SERVICE__
#include "stdafx.h"
#include <exception> 
#include <map>
#include <functional>
#include <vector>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "deffer_excpt.h"
#include "sql_type_hlp.h"
//USELIB("odbc32.lib");

namespace sql_base
{

  #define CHECK_ERROR_RET_VAL(ret_code)          \
       (( ret_code != SQL_SUCCESS ) &&           \
        ( ret_code != SQL_SUCCESS_WITH_INFO ) && \
        ( ret_code != SQL_NEED_DATA ))

  class sql;
//ENUMS
  enum{BIND_COL,BIND_PARAM};

//METHODS
  bool
 // static 
  _chk_ret_val(
     const SQLRETURN &
     ret_code
   );

  void
  sql_init(
      sql_base::sql &sql_obj,
      std::string server_,
	  std::string login_,
	  std::string pass_,
      std::string data_base_,
      long query_timeout_);
//
//  MAIN_CLASS
//
//-----------------------------------------------------------------------------
//
// Method's and helpers for partial specialization type helper teplates
//
//----------------------------------------------------------------------------- 
  
 template
   <
     typename T
   >
  struct get_data_spclz
  {
    void 
    operator()(
      SQLHSTMT sql_hstmt, 
      SQLUSMALLINT coll_,
      T *t_,   
      size_t* size_
    )
    {   
      SQLRETURN     ret_code   = 0;
      if ( !size_ )
        EXCEPT_GENERATOR( ret_code , "INVALID SIZE ARGUMENT POINTER" );  
      SQLLEN in_sz = *size_;
      SQLLEN out_sz = in_sz;
      is_wrapper<T>::get_type t_w = 
        is_wrapper<T>()(t_);        
      bool is_not_wrapper_ = (typeid(t_w) == typeid(t_));
      bool is_sql_c_char = (SQL_C_CHAR == current_type(t_));
      if ((is_not_wrapper_)&& 
          (!is_sql_c_char))
      {
        ret_code = ::SQLGetData( 
            sql_hstmt ,coll_ ,get_type_selector
            <T>() 
            , t_w , in_sz ,
            &out_sz
          );
      }
      else
      {
        ret_code = ::SQLGetData( 
            sql_hstmt ,coll_ ,get_type_selector
            <
            TypeToPtr<T>::get_type_ptr
            >() 
            , t_w , in_sz ,
            &out_sz
          );
      }
      (*size_) =  
        static_cast<size_t>(out_sz);
      if( !_chk_ret_val( ret_code ) )
        EXCEPT_GENERATOR( ret_code ,"get_data_spclz()()" );  
    
    }
  };
  
 

  template <typename T , long work_type_ >
  struct bind_spclz_ptr
  {
//-----------------------------------------------------------------------------
    void
    operator()  (SQLHSTMT handle_ , SQLUSMALLINT coll_ , T &t)
    {
      SQLINTEGER size_in = static_cast<SQLINTEGER>(get_type_size_selector<T>());
      SQLINTEGER size_ =   0;
      SQLRETURN ret_code = 0;
      if (work_type_ == BIND_COL)
      {
        ret_code = ::SQLBindCol(handle_, coll_, get_type_selector<T>()
          , &t, size_in, &size_);
      }
      else if (work_type_ == BIND_PARAM)
      {
        ret_code = ::SQLBindParameter(handle_, coll_, SQL_PARAM_INPUT,
          get_type_selector<T>(), get_sql_type_selector<T>(),
          size_in, 0, &t, 0, &size_
        );
      }
      else
      {
        EXCEPT_GENERATOR( work_type_ , "INVALID work_type_" );
      }
      if( !_chk_ret_val( ret_code ) )
        EXCEPT_GENERATOR( ret_code , "SQLBindCol or SQLBindParameter" );
    }
  };
//-----------------------------------------------------------------------------
  template <typename T , long work_type_ >
  struct bind_spclz_ptr<T* , work_type_>
  {

//----------------------------------------------------------------------------- 
    void
    operator()
    (
      SQLHSTMT handle_ , SQLUSMALLINT coll_ , 
      T *t,size_t  *size_first_ptr , 
      SQLINTEGER size_last_ = 0
    )
    { 
      internal_binder<T,work_type_>
      (
        handle_ , coll_ , t ,
        reinterpret_cast<SQLINTEGER *>
        (size_first_ptr) ,
       size_last_
      );
    }
//----------------------------------------------------------------------------- 

  private:
    template 
      <
        typename T,
        long work_type_
      >
    void
    internal_binder
    (
      SQLHSTMT handle_ , SQLUSMALLINT coll_ ,T *t,
      SQLINTEGER *size_first,SQLINTEGER size_last
    )
    {
      is_wrapper<T>::get_type t_ = is_wrapper<T>()(t);
      if ( NULL == size_first )
        EXCEPT_GENERATOR( NULL == size_first , "NULL_POINTER_ERROR" );
      SQLINTEGER size__ =  *size_first ;
      SQLRETURN ret_code =0;
      if (work_type_ == BIND_COL)
      {
        ret_code = ::SQLBindCol( handle_ , coll_ ,
            get_type_selector<T*>()
            , t_ , size__ , size_first
          );
      }
      else if (work_type_ == BIND_PARAM)
      {
        ret_code = ::SQLBindParameter(
             handle_ , coll_ , SQL_PARAM_INPUT ,
             get_type_selector<T*>(), get_sql_type_selector<T*>(),
             size__, 0 , t_ ,  0 , (size_last)? &size_last : size_first
           );
      }
      else {EXCEPT_GENERATOR( work_type_ , "INVALID work_type_" );}
      if(
         ( ret_code != SQL_SUCCESS ) &&
         ( ret_code != SQL_SUCCESS_WITH_INFO ) &&
         ( ret_code != SQL_NEED_DATA )
       )
       {EXCEPT_GENERATOR( ret_code , "SQLBindParameter" );}
      (*size_first) =  size__ ;
    }
  };
//-----------------------------------------------------------------------------
  template <
          typename T,
          long work_type_
  >
  void
  sql_bind_void(T &t , SQLHSTMT handle_ , SQLUSMALLINT coll_)
  {
    bind_spclz_ptr< T , work_type_ >()(handle_,coll_,t);
  }
//-----------------------------------------------------------------------------
  template <
          long work_type_
  >
  void
  sql_bind_void
  (
    SQLCHAR *t , SQLHSTMT handle_ ,
    SQLUSMALLINT coll_ ,
    size_t *size_f , SQLINTEGER size_l = 0
  )
  {
    if ( NULL == t )
      EXCEPT_GENERATOR( NULL == t , "NULL_POINTER_ERROR" );
    if ( NULL == size_f )
      EXCEPT_GENERATOR( NULL == size_f , "NULL_POINTER_ERROR" );
    bind_spclz_ptr<SQLCHAR *, work_type_ >()
      ( handle_ , coll_ , t , size_f , size_l );
  }
//-----------------------------------------------------------------------------
  template <
          long work_type_
  >
  void
  sql_bind_void
  (
    SQLCHAR_SQL_TYPE_WRAPPER *t , SQLHSTMT handle_ ,
    SQLUSMALLINT coll_ ,
    size_t *size_f , SQLINTEGER size_l = 0
   )
  {
    if ( NULL == t )
      EXCEPT_GENERATOR( NULL == t , "NULL_POINTER_ERROR" );
    if ( NULL == size_f )
      EXCEPT_GENERATOR( NULL == size_f , "NULL_POINTER_ERROR" );
    if ( NULL == t->sql_type_ )
      EXCEPT_GENERATOR( NULL == t , "NULL_POINTER_ERROR" );
    bind_spclz_ptr<SQLCHAR_SQL_TYPE_WRAPPER  *, work_type_ >()
      ( handle_ , coll_ , t , size_f , size_l );
  }
//-----------------------------------------------------------------------------
 template <
          long work_type_
  >
  void
  sql_bind_void(
    LONGVARCHAR_SQL_TYPE_WRAPPER *t , SQLHSTMT handle_ , 
    SQLUSMALLINT coll_ , 
    size_t *size_f , SQLINTEGER size_l = 0
  )
  {
    if ( NULL == t )
      EXCEPT_GENERATOR( NULL == t , "NULL_POINTER_ERROR" );
    if ( NULL == size_f )
      EXCEPT_GENERATOR( NULL == size_f , "NULL_POINTER_ERROR" );
    if ( NULL == t->sql_type_ )
      EXCEPT_GENERATOR( NULL == t , "NULL_POINTER_ERROR" );
    bind_spclz_ptr
      <LONGVARCHAR_SQL_TYPE_WRAPPER *, work_type_ >()
      ( handle_ , coll_ , t ,  size_f , size_l );
  }
//-----------------------------------------------------------------------------
 template <
   long work_type_
  >
  void
  sql_bind_void
  (
    STANDARDCHAR_SQL_TYPE_WRAPPER *t , SQLHSTMT handle_ , 
    SQLUSMALLINT coll_ , 
    size_t *size_f , SQLINTEGER size_l = 0
  )
  {
    if ( NULL == t )
      EXCEPT_GENERATOR( NULL == t ,
      "NULL_POINTER_ERROR" 
      );
    if ( NULL == size_f )
      EXCEPT_GENERATOR( NULL == size_f , 
      "NULL_POINTER_ERROR" 
      );
    if ( NULL == t->sql_type_ )
      EXCEPT_GENERATOR( NULL == t , 
       "NULL_POINTER_ERROR" 
      );
    bind_spclz_ptr<
      STANDARDCHAR_SQL_TYPE_WRAPPER *, work_type_ 
      >()( handle_ , coll_ , t , size_f , size_l );
  }
//-----------------------------------------------------------------------------
  class sql
  {
    friend struct auto_transactor;
  public:
    sql();
    virtual ~sql();
	void connect(void);

    void connect(const std::string&);



    void set_timeout(long timeout_connect,
              long timeout_login,
              long timeout_query);
    void sql_bind_coll(unsigned long coll_ 
      , std::string &ret_val );
    void prepare(const std::string &req_string);
    bool execute(const std::string &req_string);
    bool execute( );
    void put_date( LONGVARCHAR_SQL_TYPE_WRAPPER data_ , 
      size_t size_ );
    void fetch( void );
    void sql_bind_param(const char *str_text,
      SQLUSMALLINT coll_
    ); 
    std::string get_diag_message();
    void bind_reset( void );
//-----------------------------------------------------------
//  TEMPLATE OVERLOADED METHODS
//-----------------------------------------------------------
    template
    <
      typename T
    >
    void 
    get_data(
        T *t,   
        SQLUSMALLINT coll_,
        size_t* size_
      )
    {
      get_data_spclz<T>()( sql_hstmt , coll_ , t , size_ );
    }

   template
    <
      typename T
    >
    void 
    get_data(
        T& t,   
        SQLUSMALLINT coll_
      )
    {
      if (TypeToPtr<T>::IS_POINTER)
        EXCEPT_GENERATOR( connected_chk_ , 
        "sql::get_data() Pointer type in non pointer template specialisation" );
      size_t sz_ = sizeof(T);
      get_data_spclz<T>()( sql_hstmt, coll_ , &t , &sz_ );
    }

    template <
          typename T
    >
    void 
    sql_bind_param(  
      T *t , SQLUSMALLINT coll_ 
      , size_t *size_f , SQLINTEGER size_l = 0
    )
    {     
      if ( !connected_chk_ )
        EXCEPT_GENERATOR( connected_chk_ , 
        "SQL_NOT_CONNECTED" );
      if ( SQL_NULL_HSTMT == sql_hstmt )
        EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt ,
        "NULL HANDLE" );
      if ( NULL == t )
        EXCEPT_GENERATOR( NULL == t , "NULL_POINTER" );
      if ( NULL == size_f )
        EXCEPT_GENERATOR( NULL == size_f , "NULL_SIZE" );   
      sql_bind_void<BIND_PARAM>( t , sql_hstmt , 
          coll_ , size_f , size_l 
        );
    }
//-----------------------------------------------------------
    template <
          typename T
    >
    void 
    sql_bind_param(  T &t , SQLUSMALLINT coll_ )
    {     
      if ( !connected_chk_ )
        EXCEPT_GENERATOR( connected_chk_ , 
        "SQL_NOT_CONNECTED" 
        );
      if ( SQL_NULL_HSTMT == sql_hstmt )
        EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , 
        "NULL_POINTER" 
        );
      sql_bind_void<T,BIND_PARAM>( t , sql_hstmt , 
          coll_ 
        );
    }  
//-----------------------------------------------------------
    template <
          typename T
    >
    void 
    sql_bind_coll(  T &t , SQLUSMALLINT coll_ )
    {
      if ( !connected_chk_ )
        EXCEPT_GENERATOR( connected_chk_ , "SQL_NOT_CONNECTED" );
      if ( SQL_NULL_HSTMT == sql_hstmt )
        EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , "NULL_POINTER" );
      sql_bind_void<T,BIND_COL>( t , sql_hstmt , coll_ );
    }
//----------------------------------------------------------- 
    template <
          typename T
    >
    void 
    sql_bind_coll(  T *t , SQLUSMALLINT coll_ , size_t *size_ )
    {
      if ( !connected_chk_ )
        EXCEPT_GENERATOR( connected_chk_ , "SQL_NOT_CONNECTED" );
      if ( SQL_NULL_HSTMT == sql_hstmt )
        EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , "NULL_POINTER" );
      if ( NULL == t )
        EXCEPT_GENERATOR( NULL == t , "NULL_POINTER" );
      if ( NULL == size_ )
        EXCEPT_GENERATOR( NULL == size_ , "NULL_POINTER" );
      sql_bind_void<BIND_COL>( t , sql_hstmt , coll_ , size_ );
    }
//----------------------------------------------------------- 
  private:
//private helper methods
    bool _chk_sql_handls(void);
    bool _chk_sql_timeouts(void);
//variables
    bool         timeouts_chk_ ;
    bool         connected_chk_;
    long         timeout_query_;
    SQLHENV      sql_henv;
    SQLHDBC      sql_hdbc;
    SQLHSTMT     sql_hstmt;
  };
//HELPERS
//
  struct auto_transactor
  {
//-----------------------------------------------------------------------------
    auto_transactor(def_excpt_type *dexcpt,sql *sql_ )
    {
     if (!dexcpt || !sql_)
        except<bool>(!dexcpt || !sql_ , __LINE__ , __FILE__ , 
        std::string("NULL_POINTER"));     
      this->completion_type = SQL_ROLLBACK;
      this->sql_            = sql_; 
      this->dexcpt          = dexcpt;
      SQLRETURN
	    ret_code = SQLSetConnectAttr( this->sql_->sql_hdbc , SQL_ATTR_AUTOCOMMIT , 
            reinterpret_cast<void *>(SQL_AUTOCOMMIT_OFF) , 0 
          );
      if (!_chk_ret_val(ret_code))
        except<SQLRETURN>(ret_code , __LINE__ , __FILE__ , std::string("SQLSetConnectAttr"));
    }
//-----------------------------------------------------------------------------   
    void commit(void)
    {
      this->completion_type = SQL_COMMIT;
    }
//-----------------------------------------------------------------------------   
    ~auto_transactor()
    {
      SQLRETURN
        ret_code=SQLEndTran(SQL_HANDLE_DBC,sql_->sql_hdbc ,completion_type);
      if (_chk_ret_val(ret_code))
        ret_code=SQLSetConnectAttr(sql_->sql_hdbc, SQL_ATTR_AUTOCOMMIT, 
        reinterpret_cast<void *>(SQL_AUTOCOMMIT_ON), 0);
      if (!_chk_ret_val(ret_code))
        dexcpt->except(ret_code , __LINE__ , __FILE__ , std::string("SQLSetConnectAttr"));
    }

  private:
    def_excpt_type *dexcpt;
    sql *sql_;
    SQLSMALLINT completion_type;
//-----------------------------------------------------------------------------
  };

};


#endif