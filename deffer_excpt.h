#ifndef __DEFERRED_EXCEPTION__
#define __DEFERRED_EXCEPTION__
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <exception>
#include <string>
#include <sstream>
namespace sql_base
{
  class sql_exceptions
  {
  public:
//---------------------------------------------------------
   // template <
   // typename type_code_
   // >
   // sql_exceptions( std::string&  text,
   // type_code_ code):text_( text ),code_(0)
   // {
   // }

//---------------------------------------------------------
    sql_exceptions( std::string  text,
       std::string code):
    text_( text )//,
    //code_(code)
    {
    }
//---------------------------------------------------------
    sql_exceptions( std::string  text,
      bool code):
    text_( text ),
    code_(code)
    {
    }
//---------------------------------------------------------
    sql_exceptions( std::string  text,
       int code):
    text_( text ),
    code_(code)
    {
    }
//---------------------------------------------------------
   sql_exceptions( std::string  text,
       short code):
    text_( text ),
    code_(code)
    {
    }
//---------------------------------------------------------
    virtual
    const char
    *what(void) const
    {
      return (
        text_.c_str()
        );
    }
//---------------------------------------------------------
    sql_exceptions(sql_exceptions& excpt_)
    {
     text_ = excpt_.text_;
     code_ = excpt_.code_;
    }
//---------------------------------------------------------  
    long 
    code()
    {
      return (code_);
    }
//---------------------------------------------------------  

    sql_exceptions& operator = (sql_exceptions& excpt_)
    {
      text_ = excpt_.text_;
      code_ = excpt_.code_;
      return (*this);
    }
//---------------------------------------------------------
    virtual ~sql_exceptions(){};
  private:
    std::string text_;
    long code_;
  };

  void
  except( long line_ , const std::string &file_ ,
         const std::string &func_
      );
  template <
        typename return_code_type
  >
  void
  except(
          return_code_type code_,
          unsigned long line_,
          const std::string &file_,
          const std::string &func_
        )
  {
    std::ostringstream stream_;
    stream_ << "excpt throw [file:" << file_ << " func: " << func_ << " line: "
       << line_ << " with code:" << code_ << "]" << std::endl;
    throw (sql_exceptions( stream_.str() ,code_ )); 
  }

  template <
    typename return_code_type
  >
  struct deferred_excpt
  {
//-----------------------------------------------------------------------------
    deferred_excpt():excpt_chk(false)
    {}
//-----------------------------------------------------------------------------
    void
    except(return_code_type code_,unsigned long line_,
      const std::string &file_,const std::string &func_
      )
    {
      if (!excpt_chk)
      {
        excpt_chk = true;
        this->code_ = code_;
        this->line_ = line_;
        this->file_ = file_;
        this->func_ = func_;
      }
    }
//-----------------------------------------------------------------------------   
    void
    excpt_throw()
    {
      std::ostringstream stream_;
      stream_ << "excpt throw file:" << file_ << " func: " << func_ << " line: "
        << line_ << "with code:" << code_ << std::endl;
      throw (std::logic_error(stream_)); 
    }
//-----------------------------------------------------------------------------
  private:
    deferred_excpt&
    operator = (deferred_excpt&);  
    deferred_excpt(deferred_excpt &);

    bool excpt_chk;
    return_code_type code_;
    unsigned long line_;
    std::string file_;
    std::string func_;
  };
  typedef deferred_excpt<SQLRETURN> def_excpt_type;
  #define EXCEPT_GENERATOR( _CODE_ , _FNAME_ ) \
        except( (_CODE_) , __LINE__ , __FILE__ , (_FNAME_) )
};

#define EXCEPT_GENERATOR_I( _FNAME_ ) \
      sql_base::except( __LINE__ , __FILE__ , (_FNAME_) )

template<
  typename T
>
void
dbg_message(T& t,std::string file_,size_t prj_line,
            std::string& meth_name
            )
{
  std::ostringstream dbg_msg;           
  dbg_msg << file_
          << ":"
          << meth_name 
          << ":" 
          << prj_line 
          << ":" 
          << t.what(); 
  MessageBox(NULL,dbg_msg.str().c_str(),
    "DEBUG_MESSAGE",NULL);
}

//для ингорирования кода 100 - (запись отсутствует в базе данных)
//перегружаем шаблон обычным методом.
void
dbg_message(sql_base::sql_exceptions t,
            std::string file_,
            size_t prj_line,
            std::string& meth_name
            );




#define LOGIC_ERROR_CATCH( METHOD_NAME )           \
     }                                             \
     catch(std::logic_error& le_error)             \
{dbg_message(le_error,__FILE__,__LINE__,std::string(METHOD_NAME));} \
     catch(sql_base::sql_exceptions& se_)          \
{dbg_message(se_,__FILE__,__LINE__,std::string(METHOD_NAME));} 

#define TRY_LOGIC_ERROR()                    \
     try                                     \
     {	

#endif