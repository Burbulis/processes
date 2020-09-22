#include "stdafx.h"
#include "sql_connector.h"
namespace sql_base
{

//-----------------------------------------------------------------------------
  void
  sql_init(
      sql_base::sql &sql_obj,
      std::string server_,
	  std::string login_,
	  std::string pass_,
      std::string data_base_,
      long query_timeout_)
  {
    std::string drv_("driver={MySQL ODBC 8.0 ANSI Driver};server=") ;
    drv_ += server_;
    if (!data_base_.empty())
    {
		drv_ += std::string(";database=") + data_base_ + std::string(";USER=") + login_ + std::string(";PASSWORD=")+pass_;
    }
    drv_ += std::string(";OPTION=3;");
    sql_obj.set_timeout( 60 , 60 , query_timeout_ );
    sql_obj.connect( drv_ );
  }
//-----------------------------------------------------------------------------
  bool
 // static
  _chk_ret_val(
     const SQLRETURN &
     ret_code
   )
   {
     return (
        ( ret_code == SQL_SUCCESS ) ||
        ( ret_code == SQL_SUCCESS_WITH_INFO ) ||
        ( ret_code == SQL_NEED_DATA )
       );
   }
//-----------------------------------------------------------------------------
  sql::
  sql():
    sql_henv  ( SQL_NULL_HANDLE ),
    sql_hdbc  ( SQL_NULL_HDBC   ),
    sql_hstmt ( SQL_NULL_HSTMT  ),
    timeouts_chk_ ( false ),
    connected_chk_( false ),
    timeout_query_(0)
  {
    SQLRETURN  
    ret_code = SQLAllocHandle( SQL_HANDLE_ENV , SQL_NULL_HANDLE , &sql_henv );
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLAllocHandle" );   
    ret_code = SQLSetEnvAttr( sql_henv , SQL_ATTR_ODBC_VERSION, 
         reinterpret_cast<void*>(SQL_OV_ODBC3), 0
       ); 
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLSetEnvAttr" );   
	  ret_code = SQLAllocHandle( SQL_HANDLE_DBC , sql_henv , &sql_hdbc );
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLAllocHandle" );
   }
//-----------------------------------------------------------
  void
  sql::
  bind_reset(void)
  {
    if ( SQL_NULL_HSTMT == sql_hstmt )
       EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , "NULL_POINTER" );
  	SQLFreeStmt( sql_hstmt , SQL_CLOSE  );
    SQLFreeStmt( sql_hstmt , SQL_UNBIND );
	  SQLFreeStmt( sql_hstmt , SQL_RESET_PARAMS );
  }
//-----------------------------------------------------------
  bool 
  sql::
  execute(const std::string &req_string)
  {
    if ( !connected_chk_ )
      EXCEPT_GENERATOR( connected_chk_ , "SQL_NOT_CONNECTED" );
    if ( SQL_NULL_HSTMT == sql_hstmt )
      EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , "NULL_POINTER" );
    if ( req_string.empty() )
      EXCEPT_GENERATOR( req_string.empty() , "EMPTY SQL REQUEST" );
    SQLRETURN
    ret_code = ::SQLExecDirect(sql_hstmt,reinterpret_cast<SQLCHAR *>
        (const_cast<char *>(req_string.c_str())),
         static_cast<SQLINTEGER>(req_string.length())
      );
    
    if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code ,get_diag_message() + 
      std::string( "SQLExecDirect" ));
    return ( ret_code == SQL_NEED_DATA );
  }

  void
  sql::
  sql_bind_param(const char *str_text ,
    SQLUSMALLINT coll_
  ) 
  {
    size_t  sz_ = strlen(str_text);
    sql_bind_param( reinterpret_cast<SQLCHAR *>(
        const_cast<char *>(
        str_text))
        , coll_ , &sz_ 
      );
  }
//-----------------------------------------------------------
  void 
  sql::
  prepare(const std::string &req_string)
  {
    if ( !connected_chk_ )
      EXCEPT_GENERATOR( connected_chk_ , "SQL_NOT_CONNECTED" );
    if ( SQL_NULL_HSTMT == sql_hstmt )
      EXCEPT_GENERATOR( SQL_NULL_HSTMT == sql_hstmt , "NULL_POINTER" );
    if ( req_string.empty() )
      EXCEPT_GENERATOR( req_string.empty() , "EMPTY SQL REQUEST" );
    SQLRETURN
    ret_code = ::SQLPrepare( sql_hstmt , reinterpret_cast<SQLCHAR *>
          (const_cast<char *>(req_string.c_str())) , SQL_NTS 
        ); 
    if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLPrepare" );
  }
//-----------------------------------------------------------
  void 
  sql::   
  fetch( void )
  {
    if ( !connected_chk_ )
      EXCEPT_GENERATOR( connected_chk_ , "SQL_NOT_CONNECTED" );
    if ( SQL_NULL_HSTMT == sql_hstmt )
      EXCEPT_GENERATOR( 0 , "NULL_POINTER" );
    SQLRETURN
    ret_code = SQLFetch ( sql_hstmt );
    if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , get_diag_message() + std::string("::SQLFetch") );
  }
//-----------------------------------------------------------
  void 
  sql::
  set_timeout(long timeout_connect,
              long timeout_login,
              long timeout_query)
  {
    timeouts_chk_ = ( timeout_connect && timeout_login ); 
    SQLRETURN
    ret_code = SQLSetConnectAttr( sql_hdbc , SQL_ATTR_CONNECTION_TIMEOUT , 
                  reinterpret_cast<SQLINTEGER *>(&timeout_connect) , 0
                  );
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLSetConnectAttr" );
    ret_code = SQLSetConnectAttr( sql_hdbc , SQL_ATTR_LOGIN_TIMEOUT , 
                  reinterpret_cast<SQLINTEGER *>(&timeout_login) , 0
                  );
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLSetConnectAttr" );
    timeout_query_ = timeout_query;
  }
//-----------------------------------------------------------
  void
  sql::
  connect(void)
  {
    SQLRETURN sql_ret =
	  SQLConnect(sql_hdbc,(SQLCHAR*)"data",4, NULL, NULL, 0, NULL);
  }
//-----------------------------------------------------------
  void
  sql::
  connect(const std::string& _sql_str)
  {
    SQLCHAR sql_output_buff[MAX_PATH];
  	SQLSMALLINT		buff_size_needed=0;

    if ( !_chk_sql_timeouts() || !_chk_sql_handls() )
      EXCEPT_GENERATOR( !_chk_sql_timeouts() || !_chk_sql_handls() , 
            "INCORRECT CONDITION" 
      ); 
    SQLRETURN
    ret_code = ::SQLDriverConnect( sql_hdbc , NULL ,
          reinterpret_cast<SQLTCHAR *>( 
          const_cast<char *>(_sql_str.c_str()) ) ,
          static_cast<SQLSMALLINT>( _sql_str.length() ) ,
          sql_output_buff , MAX_PATH ,
          &buff_size_needed , SQL_DRIVER_NOPROMPT
        );
	if (!_chk_ret_val(ret_code))
		EXCEPT_GENERATOR(ret_code,"SQLDriverConnect" );
	
	ret_code = ::SQLAllocHandle( SQL_HANDLE_STMT , sql_hdbc , &sql_hstmt ); 
	
	if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , "SQLAllocHandle" ); 
    
    if ( timeout_query_ )
    {
      ret_code = SQLSetStmtAttr( sql_hstmt , 
                    SQL_ATTR_QUERY_TIMEOUT , 
                    reinterpret_cast<SQLINTEGER *>
                    (&timeout_query_) , 0
                  );
	    if( !_chk_ret_val( ret_code ) )
        EXCEPT_GENERATOR( ret_code , "SQLSetStmtAttr" );
    }
    connected_chk_ = true;
  }
//----------------------------------------------------------- 
  bool
  sql::
  execute(void)
  {
    SQLRETURN
    ret_code = SQLExecute( sql_hstmt ); 
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , get_diag_message() );  
     return ( ret_code == SQL_NEED_DATA );
  }
//----------------------------------------------------------- 
  void 
  sql::
  put_date(
      LONGVARCHAR_SQL_TYPE_WRAPPER data_ , 
      size_t size_
  )
  {
    SQLPOINTER		token_ptr  ;
    SQLRETURN     ret_code   = 0;
    ret_code = ::SQLParamData( sql_hstmt , &token_ptr);
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , get_diag_message() );  
    if ( ret_code )
      ret_code = ::SQLPutData( sql_hstmt , data_.sql_type_ ,
          static_cast<SQLINTEGER>( size_ )
        );
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , get_diag_message() );   
    ret_code = ::SQLParamData( sql_hstmt , &token_ptr);
	  if( !_chk_ret_val( ret_code ) )
      EXCEPT_GENERATOR( ret_code , get_diag_message() );   

  }
//-----------------------------------------------------------  
  std::string
  sql::
  get_diag_message()
  {
    enum{ SIZE_BUFFERS = 1000 };
    SQLCHAR		  diag_state_[SIZE_BUFFERS];
	  SQLINTEGER	native_state;
	  SQLCHAR		  message_txt[SIZE_BUFFERS];
	  SQLSMALLINT	out_num_ = 0;
    memset(diag_state_ , 0 , SIZE_BUFFERS);
    memset(message_txt , 0 , SIZE_BUFFERS);

    ::SQLGetDiagRec( SQL_HANDLE_STMT , sql_hstmt , 1 , 
        (SQLCHAR*)diag_state_ , &native_state ,
        (SQLCHAR*)message_txt , SIZE_BUFFERS , &out_num_
      );
    std::string result_diag_ = std::string (
          reinterpret_cast<char *>(diag_state_)) 
          + std::string (":")+
          std::string (
          reinterpret_cast<char *>(message_txt)
        );
    return (  result_diag_  );
  }
//----------------------------------------------------------- 
  inline
  bool 
  sql::
  _chk_sql_handls(void)
  {
    return ( sql_henv && sql_hdbc ); 
  }
//----------------------------------------------------------- 
  inline
  bool 
  sql::
  _chk_sql_timeouts(void)
  {
    return ( timeouts_chk_ ); 
  }
//----------------------------------------------------------- 
  sql::
  ~sql()
  {
    if ( sql_hstmt )
      SQLFreeHandle( SQL_HANDLE_STMT , sql_henv );
    if ( connected_chk_ )
  		SQLDisconnect( sql_hdbc );
    if ( sql_hdbc  )
      SQLFreeHandle( SQL_HANDLE_DBC , sql_hdbc  ); 
  	if ( sql_henv  )
      SQLFreeHandle( SQL_HANDLE_ENV , sql_henv  );
  }
//----------------------------------------------------------- 

}